////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT
//
// 2018-11-30	Andrew Wong <z5206677@unsw.edu.au>

#include "textbuffer.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

// Structure definitions

typedef struct textbuffer_line *TextbufferLine;
struct textbuffer_line {
    size_t size;            // number of characters in .data; equivalent to strlen(3)
    char *data;             // line data
    TextbufferLine prev;    // previous line
    TextbufferLine next;    // next line
};

typedef struct textbuffer *Textbuffer;
struct textbuffer {
    size_t size;            // number of lines
    TextbufferLine head;    // first line
};

/// Helper function prototypes

static TextbufferLine textbuffer_line_new(const char *text);
static TextbufferLine textbuffer_get_line(Textbuffer tb, size_t lineNo);
static Textbuffer textbuffer_clone(Textbuffer tb);
static void textbuffer_line_drop(TextbufferLine line);
static void textbuffer_line_replace(TextbufferLine line, const char *match, const char *replace);

static ssize_t _textbuffer_search(Textbuffer tb, char *match, bool rev, bool exact);
static void _textbuffer_delete(Textbuffer tb, size_t from, size_t to, bool touchHistory);
static Textbuffer _textbuffer_cut(Textbuffer tb, size_t from, size_t to, bool touchHistory);
static void _textbuffer_insert(Textbuffer tb1, size_t pos, Textbuffer tb2, bool touchHistory);

/// Challenge: History structure definitions, function prototypes

#define HISTORY_SIZE 10
struct _historyContainer;
struct _historyKey;
typedef struct _historyKey *_HistoryKey;
static void history_initHistory(void);
static void history_addUndo(Textbuffer tb, const char *string, bool clearRedo);
static char *history_getUndo(Textbuffer tb, bool *status);
static _HistoryKey history_getKey(Textbuffer tb);
static void history_dropKey(Textbuffer tb);
static void history_clearRedo_by_key(_HistoryKey key);
static char *history_getRedo(Textbuffer tb, bool *status);
static void history_addRedo(Textbuffer tb, const char *string);
static void history_saveTextbufferState(Textbuffer tb);

/// Challenge: Difference

static void textbuffer_diff_addDiff(char **string, bool diffIsAdd, size_t lineNo, TextbufferLine tb);
static size_t _nDigits(size_t number);


/// White box test and helper function prototypes

void white_box_tests(void);
static void _verifyLinks(Textbuffer tb);

///

/*
 * textbuffer_line_new
 *
 * Creates a line node which contains `text`
 */
static TextbufferLine textbuffer_line_new(const char *text) {
    // Return NULL if the input was empty
    if (!text) return NULL;

    // Allocate memory and populate the textbuffer line node
    TextbufferLine line = malloc(sizeof(*line));
    (*line) = (struct textbuffer_line) {
            .size = strlen(text),
            .data = strdup(text),
            .prev = NULL,
            .next = NULL
    };

    return line;
}

/**
 * Allocate a new textbuffer, whose contents is initialised with the
 * text given in the array.  The lines in the input array are all
 * terminated by a '\n'. The whole text is terminated by a '\0'.
 */
Textbuffer textbuffer_new(const char *text) {
    // Note: Passing in a null pointer will return an empty textbuffer, rather than a NULL.

    size_t size = 0;
    TextbufferLine head = NULL;

    if (text != NULL) {
        TextbufferLine prev = NULL;

        // Clone the input, and set up variables for iterating over tokens
        char *input = strdup(text);
        char *inputCursor = input;
        char *line;

        // For each line, append a new instance of a TextbufferLine to the textbuffer
        while ((line = strsep(&inputCursor, "\n")) != NULL && inputCursor) {
            // Create a line node
            TextbufferLine tb_line = textbuffer_line_new(line);

            // Update links
            if (head == NULL) head = tb_line;
            if (prev != NULL) {
                prev->next = tb_line;
                tb_line->prev = prev;
            }
            prev = tb_line;

            // Increase the line count
            size++;
        }

        // Memory management
        free(input);
    }

    // Create the Textbuffer, and attach the line nodes
    Textbuffer tb = malloc(sizeof(*tb));
    (*tb) = (struct textbuffer) {
            .head = head,
            .size = size
    };

    return tb;
}

/*
 * textbuffer_line_drop
 * Drops a line node
 */
static void textbuffer_line_drop(TextbufferLine line) {
    free(line->data);
    free(line);
}

/*
 * textbuffer_drop_lines
 * Drops only the lines of a given textbuffer, but not drop the textbuffer itself
 */
static void textbuffer_drop_lines(Textbuffer tb) {
    TextbufferLine cursor = tb->head;
    while (cursor != NULL) {
        TextbufferLine next = cursor->next;
        textbuffer_line_drop(cursor);
        cursor = next;
    }
    tb->head = NULL;
    tb->size = 0;
}

/**
 * Release any resources used by a given textbuffer.
 * It is an error to access the textbuffer after this.
 */
void textbuffer_drop(Textbuffer tb) {
    // Fail silently if tb == NULL
    if (!tb) return;

    // Drop data related to the textbuffer
    history_dropKey(tb);
    textbuffer_drop_lines(tb);
    free(tb);
}

/**
 * Return the number of lines of text stored in the given textbuffer.
 */
size_t textbuffer_lines(const Textbuffer tb) {
    return tb->size;
}

/**
 * Return the number of bytes of text stored in the given textbuffer,
 * counting the newlines that would be needed.
 */
size_t textbuffer_bytes(const Textbuffer tb) {
    // Sum the size elements in the line nodes, adding an additional byte for the newline character
    size_t bytes = 0;
    for (TextbufferLine cursor = tb->head; cursor != NULL; cursor = cursor->next) {
        bytes += cursor->size + 1;
    }

    return bytes;
}

/**
 * Return an array containing the text in the given textbuffer.  Each
 * individual line of the textbuffer needs to be terminated by a `'\\n'`,
 * including the last line.  If there are no lines in the textbuffer,
 * return NULL;
 *
 * It is the caller's responsibility to free the returned array.
 */
char *textbuffer_to_str(const Textbuffer tb) {
    // Check the number of bytes to allocate for the string
    // Return NULL if the textbuffer is empty
    size_t bytes = textbuffer_bytes(tb);
    if (bytes == 0) return NULL;

    // Allocate n+1 bytes (one extra byte for the null terminator)
    char *string = malloc(bytes + 1);
    if (string == NULL) {
        fprintf(stderr, "Could not allocate memory!\n");
        abort();
    }

    // Concatenate the lines together
    char *stringCursor = string;
    for (TextbufferLine cursor = tb->head; cursor != NULL; cursor = cursor->next) {
        sprintf(stringCursor, "%s\n", cursor->data);
        stringCursor += cursor->size + 1;
    }

    return string;
}

/*
 * textbuffer_get_line
 * Returns line number `lineNo` (0-based) of the textbuffer `tb`
 */
static TextbufferLine textbuffer_get_line(Textbuffer tb, size_t lineNo) {
    if (lineNo >= textbuffer_lines(tb)) return NULL;

    // Step through the textbuffer until lineNo is reached
    TextbufferLine curr = tb->head;
    for (size_t i = 0; i < lineNo; i++) curr = curr->next;

    return curr;
}

/**
 * Swap the two given lines in the textbuffer.
 *
 * If line `pos1` or line `pos2` are out of range, the program should
 * `abort()` with an error message.  Lines are zero-indexed; that is,
 * the first line of a textbuffer is at position 0.
 *
 * Your implementation should not swap lines by value; instead, you
 * should manipulate your linked data structure.
 */
void textbuffer_swap(Textbuffer tb, size_t pos1, size_t pos2) {
    // Sort pos1 and pos2 so that pos1 <= pos2
    // Makes it simpler for adjacent line detection
    size_t _from = pos1 < pos2 ? pos1 : pos2;
    size_t _to = pos1 > pos2 ? pos1 : pos2;

    // Get the textbuffer lines to swap
    TextbufferLine line1 = textbuffer_get_line(tb, _from);
    TextbufferLine line2 = textbuffer_get_line(tb, _to);
    if (!line1 || !line2) {
        fprintf(stderr, "Line index out of range!\n");
        abort();
    }

    // Skip if the lines are the same
    if (line1 == line2) return;

    // Save the current textbuffer content
    history_saveTextbufferState(tb);

    // Assign variables to hold the line nodes
    TextbufferLine line1_prev, line1_next, line2_prev, line2_next;
    line1_prev = line1->prev;
    line1_next = line1->next;
    line2_prev = line2->prev;
    line2_next = line2->next;

    // Reassign head if line1 was the head
    if (line1_prev == NULL) tb->head = line2;

    // Pointer reassignment magic
    line2->prev = line1_prev;
    line1->next = line2_next;

    if (line1_prev) line1_prev->next = line2;
    if (line2_next) line2_next->prev = line1;

    if (line1_next == line2) {
        // If the lines are adjacent, make them point to each other, rather than their prev/next nodes
        line2->next = line1;
        line1->prev = line2;
    } else {
        line2->next = line1_next;
        if (line1_next) line1_next->prev = line2;
        line1->prev = line2_prev;
        if (line2_prev) line2_prev->next = line1;
    }
}

/**
 * Merge `tb2` into `tb1` at line `pos`.
 *
 * Afterwards, line 0 of `tb2` will be line `pos` of `tb1`, and the old
 * line `pos` of `tb1` will follow after the last line of `tb2`.  If
 * line `pos` is equal to the number of lines in `tb1`, `tb2` is
 * inserted at the end.  If line `pos` is out of range, the program
 * should `abort()` with an error message.
 *
 * After this operation, `tb2` can no longer be used, as if we had used
 * `textbuffer_drop()` on it.
 */
void textbuffer_insert(Textbuffer tb1, size_t pos, Textbuffer tb2) {
    _textbuffer_insert(tb1, pos, tb2, true);
}

void _textbuffer_insert(Textbuffer tb1, size_t pos, Textbuffer tb2, bool touchHistory) {
    // Save the current textbuffer content
    // NOTE: Perform this first before checking for fails
    // TODO: Pending forum post: #2714052
    if (touchHistory) history_saveTextbufferState(tb1);

    /*
     * Q: if I call delete or insert or swap with no effect on a textbuffer, then i call undo,
     *    should a same textbuffer be returned since operation with no effect is still an operation?
     * Jashank: Yes, operations that do nothing are valid to undo.
     */

    // Fail silently if the textbuffers are the same, or if `tb2` is empty
    if (tb1 == tb2) {

        return;
    } else if (tb2->head == NULL) {

    } else if (tb1->head == NULL) {
        // If `tb1` is empty, change its head to point to `tb2` head
        tb1->head = tb2->head;
    } else {
        // Find tail line node of `tb2`
        TextbufferLine tb2_tail;
        for (tb2_tail = tb2->head; tb2_tail && tb2_tail->next; tb2_tail = tb2_tail->next);

        // Check that pos <= lines
        if (pos > textbuffer_lines(tb1)) {
            fprintf(stderr, "Line index out of range!\n");
            abort();
        }

        if (pos == textbuffer_lines(tb1)) {
            // Append `tb2` to the end of `tb1`
            TextbufferLine tb1_tail = textbuffer_get_line(tb1, pos - 1);
            tb1_tail->next = tb2->head;
            tb2->head->prev = tb1_tail;
        } else {
            // Insert `tb2` into `tb1` and update links

            // Find lines to insert between
            TextbufferLine tb1_split = textbuffer_get_line(tb1, pos);
            TextbufferLine tb1_split_prev = tb1_split->prev;

            // Update links for the start of `tb2`
            if (tb1_split_prev) {
                tb1_split_prev->next = tb2->head;
            } else {
                tb1->head = tb2->head;
            }

            // Update links for the end of `tb2`
            tb2->head->prev = tb1_split_prev;
            tb2_tail->next = tb1_split;
            tb1_split->prev = tb2_tail;
        }
    }

    // Update the line count of `tb1`
    tb1->size += tb2->size;

    // Memory management
    history_dropKey(tb2);
    free(tb2);
}

/*
 * textbuffer_clone
 * Create a deep copy of a textbuffer
 *
 * Instead of actually stepping through each line node and malloc'ing, strdup'ing, etc
 * This function creates a new Textbuffer from the textbuffer_to_str function
 */
static Textbuffer textbuffer_clone(Textbuffer tb) {
    char *string = textbuffer_to_str(tb);

    Textbuffer tb_new = textbuffer_new(string);

    free(string);
    return tb_new;
}

/**
 * Copy `tb2` into `tb1` at line `pos`.
 *
 * Afterwards, line 0 of `tb2` will be line `pos` of `tb1`, and the old
 * line `pos` of `tb1` will follow after the last line of `tb2`.  If
 * line `pos` is equal to the number of lines in `tb1`, `tb2` is
 * inserted at the end.  If line `pos` is out of range, the program
 * should `abort()` with an error message.
 *
 * `tb2` is unmodified and remains usable independent of `tb1`.
 */
void textbuffer_paste(Textbuffer tb1, size_t pos, const Textbuffer tb2) {
    // Clone tb2 so that tb2 can still be used
    Textbuffer tb2_clone = textbuffer_clone(tb2);

    // Insert the clone into tb1
    _textbuffer_insert(tb1, pos, tb2_clone, true);
}


/**
 * Cut lines `from` through `to` inclusive out of the textbuffer `tb`,
 * into a new textbuffer.  The cut lines should be deleted from `tb`,
 * and should no longer be associated with it.
 *
 * If lines `from` or `to` are out of range, the program should
 * `abort()` with an error message.
 */
Textbuffer textbuffer_cut(Textbuffer tb, size_t from, size_t to) {
    return _textbuffer_cut(tb, from, to, true);
}

Textbuffer _textbuffer_cut(Textbuffer tb, size_t from, size_t to, bool touchHistory) {
    // Fail silently if from > to
    // TODO: Pending forum post: #2714052
    if (from > to) return NULL;

    // Get the textbuffer lines
    TextbufferLine lineFrom = textbuffer_get_line(tb, from);
    TextbufferLine lineTo = textbuffer_get_line(tb, to);

    if (!lineFrom || !lineTo) {
        fprintf(stderr, "Line index out of range!\n");
        abort();
    }

    // Save the current textbuffer content
    if (touchHistory) {
        history_saveTextbufferState(tb);
    }

    // Update links of the textbuffer
    if (lineFrom->prev == NULL) {
        tb->head = lineTo->next;
        if (tb->head) tb->head->prev = NULL;
    } else {
        lineFrom->prev->next = lineTo->next;
        if (lineTo->next) lineTo->next->prev = lineFrom->prev;
    }

    // Unlink the extracted lines from adjacent line nodes
    lineFrom->prev = NULL;
    lineTo->next = NULL;

    // Calculate the number of extracted lines
    size_t newSize = to - from + 1;

    // Create and populate the new textbuffer
    Textbuffer segment = textbuffer_new(NULL);
    segment->head = lineFrom;
    segment->size = newSize;

    // Decrease the line count of the old textbuffer
    tb->size -= newSize;

    return segment;
}

/**
 * Copy lines `from` through `to` inclusive from the textbuffer `tb`,
 * into a new textbuffer, leaving `tb` unmodified.
 *
 * If lines `from` or `to` are out of range, the program should
 * `abort()` with an error message.
 */
Textbuffer textbuffer_copy(const Textbuffer tb, size_t from, size_t to) {
    // Clone `tb`  so that it can still be used
    Textbuffer tb_clone = textbuffer_clone(tb);

    // Extract lines
    Textbuffer segment = textbuffer_cut(tb_clone, from, to);

    textbuffer_drop(tb_clone);
    return segment;
}

/**
 * Remove lines `from` through `to` inclusive from textbuffer `tb`.
 *
 * If lines `from` or `to` are out of range, the program should
 * `abort()` with an error message.
 */
void textbuffer_delete(Textbuffer tb, size_t from, size_t to) {
    // textbuffer_cut removes lines from-to from tb, and returns a new textbuffer with the extracted lines
    // These extracted lines are then dropped, as they are not needed
    _textbuffer_delete(tb, from, to, true);
}

static void _textbuffer_delete(Textbuffer tb, size_t from, size_t to, bool touchHistory) {
    // textbuffer_cut removes lines from-to from tb, and returns a new textbuffer with the extracted lines
    // These extracted lines are then dropped, as they are not needed
    textbuffer_drop(_textbuffer_cut(tb, from, to, touchHistory));
}

/**
 * From the beginning/end of the textbuffer, search for the first
 * occurrence of the string `match`.  Return the number of the line
 * containing the match.
 *
 * @param tb	the Textbuffer to search in
 * @param match	the string to match on
 * @param rev	search for the previous matching term.
 */
ssize_t textbuffer_search(Textbuffer tb, char *match, bool rev) {
    // Function wrapper to perform a loose search
    return _textbuffer_search(tb, match, rev, false);
}

/*
 * _textbuffer_search
 * From the beginning/end of the textbuffer, search for the first line that matches the string `match`.
 * If `exact` is true, then the line must match completely
 * Return the number of the line containing the match.
 */
static ssize_t _textbuffer_search(Textbuffer tb, char *match, bool rev, bool exact) {
    // cursor->data could be NULL, when altered in the diff check

    // Fail if there is no search term
    if (strlen(match) == 0 && !exact) return -1;

    if (!rev) {
        // reverse = false ... search from HEAD to TAIL

        // For each textbuffer line, check if that line contains the search term
        size_t lineCount = 0;
        for (TextbufferLine cursor = tb->head; cursor != NULL; cursor = cursor->next, lineCount++) {
            if (cursor->data && (exact ? (strcmp(cursor->data, match) == 0) : (strstr(cursor->data, match) != NULL))) {
                // A match was found, return the current line number
                return (ssize_t) lineCount;
            }
        }
    } else {
        // reverse = true ... search from TAIL to HEAD

        // Get the last line of the textbuffer
        TextbufferLine cursor;
        for (cursor = tb->head; cursor && cursor->next; cursor = cursor->next);

        // For each textbuffer line, check if that line contains the search term
        size_t lineCount = tb->size - 1;
        for (; cursor; cursor = cursor->prev, lineCount--) {
            if (cursor->data && (exact ? (strcmp(cursor->data, match) == 0) : (strstr(cursor->data, match) != NULL))) {
                // A match was found, return the current line number
                return (ssize_t) lineCount;
            }
        }
    }

    // No match found, fail
    return -1;
}


/*
 * textbuffer_line_replace
 * Replaces instances of `match` by copying characters up to and after the `match` position
 */
static void textbuffer_line_replace(TextbufferLine line, const char *match, const char *replace) {
    // Fail if there is no search term
    if (strlen(match) == 0) return;

    // Store the string lengths of the search and replacement strings
    size_t sizeMatch = strlen(match);
    size_t sizeReplace = strlen(replace);

    // Calculate the length difference between the search and replacement strings (to efficiently allocate memory)
    int sizeDifference = (int) (sizeReplace - sizeMatch);

    // Allocate memory and create pointers to the output line
    size_t newLine_len = line->size + 1;
    char *newLine = malloc(newLine_len);
    char *newLineCursor = newLine;

    // For each match, perform a replacement
    char *lineCursor = line->data;
    char *token;
    while ((token = strstr(lineCursor, match)) != NULL) {
        // Get position of the matched string
        size_t offset = (size_t)(token - lineCursor);

        // If the length of the search and replacement strings differ, expand/truncate memory to fit
        if (sizeDifference != 0) {
            int cursorOffset = newLineCursor - newLine;
            newLine = realloc(newLine, (newLine_len = (size_t)((int) newLine_len + sizeDifference)));
            newLineCursor = newLine + cursorOffset;
        }

        // Copy everything before the matched string
        strncpy(newLineCursor, lineCursor, offset);

        // Copy the replacement string, growing/shrinking memory for newLine if necessary.
        strncpy(newLineCursor + offset, replace, sizeReplace);

        // Move cursors
        lineCursor += offset + sizeMatch;
        newLineCursor += offset + sizeReplace;
    }

    // Copy the remaining characters of the input line
    // This additional call is needed due to the nature of strsep(3)
    strcpy(newLineCursor, lineCursor);

    // Remove the null terminating character from the character count
    line->size = newLine_len - 1;

    // Memory management
    free(line->data);
    line->data = newLine;
}

/**
 * Search every line of `tb` for occurrences of `match`, and replace
 * them all with `replace`.
 */
void textbuffer_replace(Textbuffer tb, char *match, char *replace) {
    // history_saveTextbufferState(tb);
    // TODO pending for forum post: #2714115

    // For each line, check for matches and replace
    for (TextbufferLine cursor = tb->head; cursor != NULL; cursor = cursor->next) {
        textbuffer_line_replace(cursor, match, replace);
    }
}

/* Challenge Part */

struct _historyKey {
    Textbuffer key;                  // key
    char *records[HISTORY_SIZE];     // array of history records (for undo)
    size_t _records_count;           // number of items in .records
    size_t _cursor;                  // current position in .records

    char *redoItems[HISTORY_SIZE];   // array of history records (undone items)
    size_t _redoItems_count;         // number of items in .redoItems

    _HistoryKey next;                // next key record
};

struct _historyContainer {
    _HistoryKey items;
};

/*
 * Aaaah! A global variable!
 *
 * I could have achieved a history management system without using a global variable.
 * BUT - It's not good practice to store every bit of metadata in the same struct.
 *     - A database engineer would lose their job if they didn't use a one to many approach here...
 * ...
 * I got Jashank's approval though, so it's all gucci
 */
static struct _historyContainer *History;

/*
 * history_initHistory
 * Initialise the history container
 */
static void history_initHistory() {
    // Abort if _initHistory is called when it is already initialised
    assert(History == NULL);

    // Allocate memory for the history container
    History = malloc(sizeof(*History));
    History->items = NULL;
}

/*
 * history_addUndo
 * Add `string` to the history record for `tb`, overriding the oldest values
 */
static void history_addUndo(Textbuffer tb, const char *string, bool clearRedo) {
    // Initialise history if not initialised
    if (History == NULL) history_initHistory();

    // Get the history key record for `tb`
    _HistoryKey key = history_getKey(tb);

    // Create a pointer to the location of the history record in the array
    // (pointer needed so the value in the array can be changed)
    // (actually just because I didn't want to keep typing **key->records[key->_cursor]**)
    char **record = &key->records[key->_cursor];

    // Free the old value
    if (*record) free(*record);

    // Store the new value
    *record = string ? strdup(string) : NULL;

    key->_cursor = (key->_cursor + 1) % HISTORY_SIZE;

    if (++key->_records_count > HISTORY_SIZE) key->_records_count = HISTORY_SIZE;

    // Clear the redo history array if `clearRedo` is true
    if (clearRedo) history_clearRedo_by_key(key);
}

/*
 * history_clearRedo_by_key
 * Clears the redo array
 */
static void history_clearRedo_by_key(_HistoryKey key) {
    // Free all of the strings in the .redoItems array
    while (key->_redoItems_count) free(key->redoItems[--key->_redoItems_count]);
    assert(key->_redoItems_count == 0);
}

/*
 * history_addRedo
 * Add a new entry into the redo array of the record for `tb`
 */
static void history_addRedo(Textbuffer tb, const char *string) {
    if (History == NULL) history_initHistory();
    _HistoryKey key = history_getKey(tb);

    // Add a new entry into .redoItems
    key->redoItems[key->_redoItems_count++] = strdup(string);
}

/*
 * history_getRedo
 * Gets the value before the last undo
 *
 * NOTE: No need to set the pointer to NULL for redo, as the ._redoItems_count element dictates which pointers are valid
 */
static char *history_getRedo(Textbuffer tb, bool *status) {
    // Initialise history if not initialised
    if (History == NULL) history_initHistory();

    // Get the history key record for `tb`
    _HistoryKey key = history_getKey(tb);

    if (key->_redoItems_count == 0) {
        *status = false;
        return NULL;
    } else *status = true;

    // Get the latest value
    char **data = &key->redoItems[--key->_redoItems_count];

    // Duplicate the value
    char *res = strdup(*data);
    free(*data);

    return res;
}

/*
 * history_getUndo
 * Gets the value before the latest action
 */
static char *history_getUndo(Textbuffer tb, bool *status) {
    // Get the history key record for `tb`
    _HistoryKey key = history_getKey(tb);

    if (key->_records_count == 0) {
        *status = false;
        return NULL;
    } else *status = true;

    // Decrement the cursors
    key->_records_count--;
    key->_cursor = (key->_cursor - 1 % HISTORY_SIZE + HISTORY_SIZE) % HISTORY_SIZE;

    // Get the last value
    char **data = &key->records[key->_cursor % HISTORY_SIZE];
    if (*data) {
        char *res = strdup(*data);

        // Create a copy of the value, and unlink from the history record
        free(*data);
        *data = NULL;

        return res;
    }

    return NULL;
}


/*
 * history_getKey
 * Return the key record for `tb`
 * Creates a record and returns it if there is no previous record for `tb`
 */
static _HistoryKey history_getKey(Textbuffer tb) {
    // Initialise history if not initialised
    if (History == NULL) history_initHistory();

    // Pointer to the pointer to the head of History->items
    _HistoryKey *item = &History->items;

    // Search through the current history records
    while (*item) {
        if ((*item)->key == tb) return *item;
        item = &((*item)->next);
    }

    // No matching record was found, create a new record
    *item = malloc(sizeof(struct _historyKey));
    (**item) = (struct _historyKey) {
            .key = tb,
            .records = {NULL},
            ._records_count = 0,
            ._cursor = 0,
            .next = NULL,

            .redoItems = {NULL},
            ._redoItems_count = 0
    };

    return *item;
}

/*
 * history_dropKey
 * Drop all records for `tb`
 */
static void history_dropKey(Textbuffer tb) {
    // If History has not been initialised, fail silently
    if (!History) return;

    // Search for the record for `tb`, remove and update links
    _HistoryKey prev = NULL;
    for (_HistoryKey item = History->items; item; prev = item, item = item->next) {
        // Skip if the current record does not match
        if (item->key != tb) continue;

        // Update links from previous record to the record after the matching record
        if (prev) prev->next = item->next;
        if (History->items == item) History->items = item->next;

        // Free all values in the record
        char **record = item->records;
        for (int i = 0; i < HISTORY_SIZE; i++) {
            if (record[i]) free(record[i]);
        }
        history_clearRedo_by_key(item);
        free(item);

        break;
    }

    // Destroy the History container if there are no more items in the container
    if (!History->items) {
        free(History);
        History = NULL;
    }
}

/*
 * history_saveTextbufferState
 * Store the lines of a textbuffer to history
 */
static void history_saveTextbufferState(Textbuffer tb) {
    char *history_str = textbuffer_to_str(tb);
    history_addUndo(tb, history_str, true);
    free(history_str);
}

/**
 * CHALLENGE:
 *
 * Revert (up to) the ten most recently called operations on `tb`.
 * Each time `textbuffer_undo` is called, one operation is reversed on
 * `tb`; when the maximum number of reversible operations is reached,
 * nothing is done.
 *
 * Applicable operations are:
 *  - `textbuffer_swap`,
 *  - `textbuffer_delete`,
 *  - `textbuffer_insert`,
 *  - `textbuffer_paste`, and
 *  - `textbuffer_cut`.
 */
void textbuffer_undo(Textbuffer tb) {
    // Try get the last value before an operation
    bool status;
    char *lastVal = history_getUndo(tb, &status);
    if (status) {
        // Add current value to the redo array
        char *currVal = textbuffer_to_str(tb);
        history_addRedo(tb, currVal);
        free(currVal);

        // Free the current lines in `tb`
        textbuffer_drop_lines(tb);

        // Create a textbuffer with the last value
        Textbuffer oldTB = textbuffer_new(lastVal);

        // Relink
        tb->head = oldTB->head;
        tb->size = oldTB->size;

        // Memory management
        free(lastVal);
        free(oldTB);
    }
}

/**
 * CHALLENGE:
 *
 * Redo an operation that has been undone by `textbuffer_undo`.  This
 * function should redo one operation per call; when a new operation is
 * called on `tb`, previously-undone operations cannot be redone.
 */
void textbuffer_redo(Textbuffer tb) {
    // Try get the last value before an undo
    bool status;
    char *lastVal = history_getRedo(tb, &status);
    if (status) {
        // Add current value to the undo array
        char *currVal = textbuffer_to_str(tb);
        history_addUndo(tb, currVal, false);
        free(currVal);

        // Free the current lines in `tb`
        textbuffer_drop_lines(tb);

        // Create a textbuffer with the last value
        Textbuffer oldTB = textbuffer_new(lastVal);

        // Relink
        tb->head = oldTB->head;
        tb->size = oldTB->size;

        // Memory management
        free(lastVal);
        free(oldTB);
    }
}


/**
 * CHALLENGE:
 *
 * Given two text files, we sometimes want to know what changes are made
 * from one file to another file.  The function `textbuffer_diff` works
 * out which lines of text have been added, removed, or modified from
 * `tb1` to get `tb2`.  The returned string of the function is an edit
 * script consisting of a series of add and delete commands.  Applying
 * such commands on `tb1` in sequence should result in `tb2`.
 *
 * An edit solution should have one command per line to either add or
 * delete a line of text at a specific line number.  An example is given
 * below --
 *
 *     2+	add this line please
 *     3-
 *     12+	add this line as well please
 *
 * The first command adds a line of text 'add this line please' at line
 * 2 of the textbuffer (counting from 0).  The existing line 2 is moved
 * to line 3, and so on.  The second command deletes the line 3 of the
 * textbuffer.  The last command adds the specified text at line 12 of
 * the textbuffer.  The command and text are separated by a literal tab
 * character.
 *
 * A mark is given if your solution is
 *
 * - Correct: applying your edit solution on `tb1` results in `tb2`; and
 * - Compact: the size of your edit solution (i.e. number of lines) is
 *   smaller than or equal to the size of our model solution (to avoid
 *   trivial solutions that delete all lines in `tb1` then add all lines
 *   of `tb2`)
 */
char *textbuffer_diff(Textbuffer tb1, Textbuffer tb2) {
    size_t lineCounter = 0;
    char *result = NULL;

    if (!tb1 || !tb1->head) {
        for (TextbufferLine tb2_curr = tb2->head; tb2_curr; tb2_curr = tb2_curr->next, lineCounter++) {
            textbuffer_diff_addDiff(&result, true, lineCounter, tb2_curr);
        }
        return result;
    }

    if (!tb2 || !tb2->head) {
        for (TextbufferLine tb1_curr = tb1->head; tb1_curr; tb1_curr = tb1_curr->next, lineCounter++) {
            // Always remove line 0
            textbuffer_diff_addDiff(&result, false, 0, NULL);
        }
        return result;
    }

    // Make a copy of tb1
    tb1 = textbuffer_clone(tb1);

    // For each line in tb2, check if it exists in tb1, and make edits
    for (TextbufferLine tb2_curr = tb2->head; tb2_curr; tb2_curr = tb2_curr->next, lineCounter++) {
        ssize_t line;

        // Check if tb1 contains the line in tb2
        if ((line = _textbuffer_search(tb1, tb2_curr->data, false, true)) > -1) {

            // Check if the matched line is in the same position as the search line
            if ((size_t) line > lineCounter) {

                // Remove the previous lines
                for (size_t i = 0; i < ((size_t) line - lineCounter); i++) {
                    textbuffer_diff_addDiff(&result, false, lineCounter, NULL);
                    _textbuffer_delete(tb1, lineCounter, lineCounter, false);
                }
            }

            // Disable the matched lines (for duplicate lines)
            TextbufferLine curr = textbuffer_get_line(tb1, lineCounter);
            curr->size = 0;
            free(curr->data);
            curr->data = NULL;
        } else {
            textbuffer_diff_addDiff(&result, true, lineCounter, tb2_curr);

            // Create a 'disabled' line node
            Textbuffer nullLine = textbuffer_new(NULL);
            nullLine->head = malloc(sizeof(struct textbuffer_line));
            (*nullLine->head) = (struct textbuffer_line) {
                    .data = NULL, .size = 0, .next = NULL, .prev = NULL
            };
            nullLine->size = 1;

            // Attach the disabled line
            _textbuffer_insert(tb1, lineCounter, nullLine, false);
        }
    }

    // Remove items that have shifted down
    while (textbuffer_get_line(tb1, lineCounter)) {
        textbuffer_diff_addDiff(&result, false, lineCounter, NULL);
        _textbuffer_delete(tb1, lineCounter, lineCounter, false);
    }

    // Memory management
    textbuffer_drop(tb1);
    return result;
}

/**
 * textbuffer_diff_addDiff
 * Appends difference edit message to string
 * If @param diffIsAdd is true --> add data from @param tb
 * If @param diffIsAdd is true --> delete
 */
static void textbuffer_diff_addDiff(char **string, bool diffIsAdd, size_t lineNo, TextbufferLine tb) {
    // Check current size (length) of the string
    size_t currentLength = *string ? strlen(*string) : 0;

    // Count the number of characters needed to hold the line number
    size_t lineDigits = _nDigits(lineNo);
    if (!lineDigits) lineDigits++;

    // A line consists of: [number]TAB{data}\n.
    size_t lineLength = lineDigits + 1 + 1;
    if (diffIsAdd) lineLength += 1 + tb->size;

    // malloc or realloc space for the string
    *string = realloc(*string, currentLength + lineLength + 1);

    // Append to the string
    if (diffIsAdd) {
        sprintf(*string + currentLength, "%d+\t%s\n", lineNo, tb->data);
    } else {
        sprintf(*string + currentLength, "%d-\n", lineNo);
    }
}

/*
 * _nDigits
 * Calculates the number of digits of `number`
 */
static size_t _nDigits(size_t number) {
    if (number == 0) return 0;
    return 1 + _nDigits(number / 10);
}


/* White box testing */

/**
 * _verifyLinks
 * Checks that each line node has correct prev/next pointer values
 */
static void _verifyLinks(Textbuffer tb) {
    TextbufferLine curr = tb->head;
    for (size_t i = 0; i < tb->size; curr = curr->next, i++) {
        if (i == 0) {
            // Check that the head item has no previous node assigned
            assert(curr->prev == NULL);
        } else {
            // Check that the previous and current node point to each other
            assert(curr->prev);
            assert(curr == curr->prev->next);
        }

        if (i == (tb->size - 1)) {
            // Check that the tail item has no next node assigned
            assert(curr->next == NULL);
        } else {
            // Check that the current and next node point to each other
            assert(curr->next);
            assert(curr == curr->next->prev);
        }
    }
}

/*
 * _verifySize
 * Checks that tb->size is equal to the number of lines in `tb`
 */
static void _verifySize(Textbuffer tb) {
    size_t count = 0;
    for (TextbufferLine curr = tb->head; curr; curr = curr->next) count++;
    assert(count == tb->size);
}

void white_box_tests(void) {
    Textbuffer tb;
    Textbuffer tb_insert;

    tb = textbuffer_new("0\n");
    {
        _verifyLinks(tb);
        _verifySize(tb);
        assert(tb->size == 1);
        assert(tb->head != NULL);
        assert(strcmp(tb->head->data, "0") == 0);
        assert(tb->head->size == strlen(tb->head->data));
    }

    tb_insert = textbuffer_new("1\n2\n3\n4\n5\n6\n");
    {
        _verifyLinks(tb_insert);
        _verifySize(tb_insert);
        assert(tb_insert->size == 6);
        assert(tb_insert->head != NULL);
        assert(strcmp(tb_insert->head->data, "1") == 0);
    }

    // Test history, and paste
    assert(History == NULL);
    textbuffer_paste(tb, 1, tb_insert);
    {
        assert(History != NULL);
        assert(History->items->key == tb);
        assert(History->items->key == tb);
        assert(History->items->next == NULL);
        assert(History->items->_cursor == 1);
        assert(History->items->records[0] != NULL);
    }
    {
        _verifyLinks(tb);
        _verifySize(tb);
        assert(tb->size == 7);
    }


    // Test multiple swaps
    textbuffer_swap(tb, 0, 5);
    {
        _verifyLinks(tb);
        _verifySize(tb);
        assert(strcmp(tb->head->data, "5") == 0);
        assert(strcmp(tb->head->next->next->next->next->next->data, "0") == 0);
    }

    textbuffer_swap(tb, 5, 6);
    {
        _verifyLinks(tb);
        _verifySize(tb);
        assert(strcmp(tb->head->next->next->next->next->next->data, "6") == 0);
        assert(strcmp(tb->head->next->next->next->next->next->next->data, "0") == 0);
    }

    textbuffer_swap(tb, 0, 6);
    {
        _verifyLinks(tb);
        _verifySize(tb);
        assert(strcmp(tb->head->data, "0") == 0);
        assert(strcmp(tb->head->next->next->next->next->next->next->data, "5") == 0);
    }

    textbuffer_swap(tb, 5, 6);
    {
        _verifyLinks(tb);
        _verifySize(tb);
        assert(strcmp(tb->head->next->next->next->next->next->data, "5") == 0);
        assert(strcmp(tb->head->next->next->next->next->next->next->data, "6") == 0);
    }

    // Test textbuffer_get_line
    assert(textbuffer_get_line(tb, 0) == tb->head);
    assert(textbuffer_get_line(tb, 1) == tb->head->next);
    assert(textbuffer_get_line(tb, tb->size) == NULL);

    // Test textbuffer_to_str
    {
        Textbuffer tb_clone = textbuffer_clone(tb);
        char *tb_str = textbuffer_to_str(tb);
        char *tb_clone_str = textbuffer_to_str(tb_clone);
        assert(strcmp(tb_str, tb_clone_str) == 0);
        free(tb_str);
        free(tb_clone_str);
        textbuffer_drop(tb_clone);
    }

    // Delete first line
    textbuffer_delete(tb, 0, 0);
    {
        assert(tb->size == 6);
        assert(strcmp(tb->head->data, "1") == 0);
    }

    // Clear textbuffer
    textbuffer_drop_lines(tb);
    {
        assert(tb->size == 0);
        assert(tb->head == NULL);
    }

    // Test adding into an empty textbuffer
    textbuffer_insert(tb, 0, tb_insert);
    {
        _verifyLinks(tb);
        _verifySize(tb);
        assert(tb->size == 6);
    }

    textbuffer_drop(tb);
}
