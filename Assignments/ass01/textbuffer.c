////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT
//
// 2018-11-30	Andrew Wong <z5206677@unsw.edu.au>

#include "textbuffer.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define VERIFY 1
#define doVerify(tb) if (VERIFY) verifyLinks(tb)

typedef struct textbuffer_line *TextbufferLine;
struct textbuffer_line {
    size_t size;
    char *data;
    TextbufferLine prev;
    TextbufferLine next;
};

struct textbuffer {
    size_t size;
    TextbufferLine head;
};

static TextbufferLine textbuffer_line_copy(TextbufferLine line);

static TextbufferLine textbuffer_line_new(const char *text);

static TextbufferLine textbuffer_get_line(Textbuffer tb, size_t lineNo);

static void textbuffer_line_drop(TextbufferLine line);

static void textbuffer_line_replace(TextbufferLine line, const char *match, const char *replace);

static Textbuffer textbuffer_clone(Textbuffer tb);

///

#define HISTORY_SIZE 10
struct _historyKey;
struct _historyContainer;
typedef struct _historyKey *_HistoryKey;


static void history_initHistory(void);

static void history_addHistory(Textbuffer tb, const char *string, bool clearRedo);

static char *history_getHistory(Textbuffer tb);

static _HistoryKey history_getKey(Textbuffer tb);
static void history_dropKey(Textbuffer tb);

static void history_clearRedo_by_key(_HistoryKey key);
static char *history_getRedo(Textbuffer tb);
static void history_addRedo(Textbuffer tb, const char* string);
///

void white_box_tests(void);

void verifyLinks(Textbuffer tb);

///


static TextbufferLine textbuffer_line_copy(TextbufferLine line) {
    if (line == NULL) return NULL;
    return textbuffer_line_new(line->data);
}

static TextbufferLine textbuffer_line_new(const char *text) {
    // Assume that text does not end with a \n character.
    if (text == NULL) return NULL;

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
 * terminated by a `'\\n'`. The whole text is terminated by a `'\\0'`.
 */

Textbuffer textbuffer_new(const char *text) {
    size_t size = 0;
    TextbufferLine head = NULL;

    if (text != NULL) {
        TextbufferLine prev = NULL;

        char *input = strdup(text);
        char *EOL = input + strlen(text);

        char *inputCursor = input;
        char *line;
        while ((line = strsep(&inputCursor, "\n")) != NULL) {
            // Create the single textbuffer instance
            TextbufferLine tb_line = textbuffer_line_new(line);

            // Update cursors
            if (head == NULL) {
                head = tb_line;
            }

            // Link instances
            if (prev != NULL) {
                prev->next = tb_line;
                tb_line->prev = prev;
            }

            prev = tb_line;

            size++;

            // check if we've reached the end
            if (inputCursor == EOL) break;
        }

        free(input);
    }

    Textbuffer tb = malloc(sizeof(*tb));
    (*tb) = (struct textbuffer) {
            .head = head,
            .size = size
    };

    return tb;
}

static void textbuffer_line_drop(TextbufferLine line) {
    free(line->data);
    free(line);
}

static void textbuffer_drop_head(Textbuffer tb) {
    TextbufferLine cursor = tb->head;
    while (cursor != NULL) {
        TextbufferLine next = cursor->next;
        textbuffer_line_drop(cursor);
        cursor = next;
    }
}

/**
 * Release any resources used by a given textbuffer.
 * It is an error to access the textbuffer after this.
 */
void textbuffer_drop(Textbuffer tb) {
    history_dropKey(tb);
    textbuffer_drop_head(tb);
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
    size_t bytes = textbuffer_bytes(tb);

    // If the textbuffer has lines, then add one more byte for the string NULL terminator character
    bytes = bytes ? bytes + 1 : 0;

    if (bytes == 0) return NULL;

    char *string = malloc(bytes + 1);
    if (string == NULL) {
        fprintf(stderr, "Could not allocate memory!\n");
        abort();
    }

    char *stringCursor = string;
    for (TextbufferLine cursor = tb->head; cursor != NULL; cursor = cursor->next) {
        sprintf(stringCursor, "%s\n", cursor->data);
        stringCursor += cursor->size + 1;
    }

    return string;

}

static TextbufferLine textbuffer_get_line(Textbuffer tb, size_t lineNo) {
    //    if (lineNo < 0 || lineNo >= textbuffer_lines(tb)) return NULL;
    // size_t is unsigned
    if (lineNo >= textbuffer_lines(tb)) return NULL;
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
    size_t _from = pos1 < pos2 ? pos1 : pos2;
    size_t _to   = pos1 > pos2 ? pos1 : pos2;

    TextbufferLine line1 = textbuffer_get_line(tb, _from);
    TextbufferLine line2 = textbuffer_get_line(tb, _to);

    if (!line1 || !line2) {
        fprintf(stderr, "Line index out of range!\n");
        abort();
    }

    if (line1 == line2) return; // Silently fail if lines are the same (skip)

    char *history_str = textbuffer_to_str(tb);
    history_addHistory(tb, history_str, true);
    free(history_str);

    TextbufferLine line1_prev, line1_next, line2_prev, line2_next;
    line1_prev = line1->prev;
    line1_next = line1->next;
    line2_prev = line2->prev;
    line2_next = line2->next;

    if (line1_prev == NULL) {
        tb->head = line2;
    } else if (line2_prev == NULL) {
        tb->head = line1;
    }

    if (line1_next == line2) {
//        printf("Adjacent match\n");
    
        line2->next = line1;
        line1->prev = line2;
        
        line2->prev = line1_prev;
        if (line1_prev) line1_prev->next = line2;

        line1->next = line2_next;
        if (line2_next) line2_next->prev = line1;    
    } else {
        line2->prev = line1_prev;
        if (line1_prev) line1_prev->next = line2;

        line2->next = line1_next;
        if (line1_next) line1_next->prev = line2;

        line1->prev = line2_prev;
        if (line2_prev) line2_prev->next = line1;

        line1->next = line2_next;
        if (line2_next) line2_next->prev = line1;
    }




/*


    if (line1_next == line2) {
        printf("ADJACENT\n");
        line1->next = line2_next;
        if (line2_next) line2_next->prev = line1;

        line2->next = line1;
        line1->prev = line2;
    }
    */
    doVerify(tb);
}
//
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
    if (tb1 == tb2 || tb2->head == NULL) return;

    char *history_str = textbuffer_to_str(tb1);
    history_addHistory(tb1, history_str, true);
    free(history_str);

    if (tb1->head == NULL) {
        tb1->head = tb2->head;
    } else {
        TextbufferLine tb2_tail;
        for (tb2_tail = tb2->head; tb2_tail && tb2_tail->next; tb2_tail = tb2_tail->next);

        if (pos > textbuffer_lines(tb1)) {
            fprintf(stderr, "Line index out of range!\n");
            abort();
        }

        if (pos == textbuffer_lines(tb1)) {
            TextbufferLine tb1_tail = textbuffer_get_line(tb1, pos - 1);
            tb1_tail->next = tb2->head;
            tb2->head->prev = tb1_tail;
        } else {

            TextbufferLine tb1_split = textbuffer_get_line(tb1, pos);
            TextbufferLine tb1_split_prev = tb1_split->prev;

            if (tb1_split_prev) {
                tb1_split_prev->next = tb2->head;
            } else {
                tb1->head = tb2->head;
            }

            tb2->head->prev = tb1_split_prev;
            tb2_tail->next = tb1_split;
            tb1_split->prev = tb2_tail;
        }
    }

    tb1->size += tb2->size;
    free(tb2); // free to clear the `size` data of tb2
    doVerify(tb1);
}

static Textbuffer textbuffer_clone(Textbuffer tb) {
    return textbuffer_new(textbuffer_to_str(tb));
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
    Textbuffer tb2_clone = textbuffer_clone(tb2);
    textbuffer_insert(tb1, pos, tb2_clone);
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
    size_t _from = from < to ? from : to;
    size_t _to   = from > to ? from : to;

    TextbufferLine lineFrom = textbuffer_get_line(tb, _from);
    TextbufferLine lineTo = textbuffer_get_line(tb, _to);

    if (!lineFrom || !lineTo) {
        fprintf(stderr, "Line index out of range!\n");
        abort();
    }

    char *history_str = textbuffer_to_str(tb);
    history_addHistory(tb, history_str, true);
    free(history_str);

    if (lineFrom->prev == NULL) {
        tb->head = lineTo->next;
        if (tb->head) tb->head->prev = NULL;
    } else {
        lineFrom->prev->next = lineTo->next;
        if (lineTo->next) lineTo->next->prev = lineFrom->prev;
    }

    lineFrom->prev = NULL;
    lineTo->next = NULL;

    size_t newSize = to - from + 1;

    Textbuffer segment = textbuffer_new(NULL);
    segment->head = lineFrom;
    segment->size = newSize;

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
    Textbuffer tb_clone = textbuffer_clone(tb);
    Textbuffer segment = textbuffer_cut(tb_clone, from, to);
    free(tb_clone); // free-ing just the `struct textbuffer`, and not the elements within
    return segment;
}

/**
 * Remove lines `from` through `to` inclusive from textbuffer `tb`.
 *
 * If lines `from` or `to` are out of range, the program should
 * `abort()` with an error message.
 */
void textbuffer_delete(Textbuffer tb, size_t from, size_t to) {
    textbuffer_drop(textbuffer_cut(tb, from, to));
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
    if (strlen(match) == 0) return -1;
    if (!rev) {
        // Go from HEAD to TAIL
        size_t lineCount = 0;
        for (TextbufferLine cursor = tb->head; cursor != NULL; cursor = cursor->next, lineCount++) {
            if (strstr(cursor->data, match)) {
                return (ssize_t) lineCount;
            }
        }
    } else {
        // Go from TAIL to HEAD
        TextbufferLine cursor;
        for (cursor = tb->head; cursor && cursor->next; cursor = cursor->next);
        size_t lineCount = tb->size - 1;
        for (; cursor != NULL; cursor = cursor->prev, lineCount--) {
            if (strstr(cursor->data, match)) {
                return (ssize_t) lineCount;
            }
        }
    }

    return -1;
}


static void textbuffer_line_replace(TextbufferLine line, const char *match, const char *replace) {
    if (strlen(match) == 0) return;

    size_t sizeMatch = strlen(match);
    size_t sizeReplace = strlen(replace);

    int sizeDifference = (int) (sizeReplace - sizeMatch);

    size_t newLine_len = line->size + 1;
    char *newLine = malloc(newLine_len);
    char *newLineCursor = newLine;

    char *lineCursor = line->data;

    char *token;
    while ((token = strstr(lineCursor, match)) != NULL) {
        size_t offset = (size_t)(token - lineCursor);
        if (sizeDifference != 0) {
            int cursorOffset = newLineCursor - newLine;
            newLine = realloc(newLine, (newLine_len = (size_t)((int) newLine_len + sizeDifference)));
            newLineCursor = newLine + cursorOffset;
        }

        // Copy everything before the matched string
        strncpy(newLineCursor, lineCursor, offset);

        // Copy the replacement string, growing/shrinking memory for newLine if necessary.
        strncpy(newLineCursor + offset, replace, sizeReplace);

        lineCursor += offset + sizeMatch;
        newLineCursor += offset + sizeReplace;
    }

    strcpy(newLineCursor, lineCursor);

    line->size = newLine_len - 1;

    free(line->data);
    line->data = newLine;
}

/**
 * Search every line of `tb` for occurrences of `match`, and replace
 * them all with `replace`.
 */
void textbuffer_replace(Textbuffer tb, char *match, char *replace) {
    for (TextbufferLine cursor = tb->head; cursor != NULL; cursor = cursor->next) {
        textbuffer_line_replace(cursor, match, replace);
    }
}

/* Challenge Part */

struct _historyKey {
    Textbuffer key;
    char *records[HISTORY_SIZE];
    size_t _cursor;
    
    char *redoItems[HISTORY_SIZE]; 
    size_t _redoItems_count;

    _HistoryKey next;
};
struct _historyContainer {
    _HistoryKey items;
    size_t size;
};
static struct _historyContainer *History;


static void history_initHistory() {
    History = malloc(sizeof(*History));
    (*History) = (struct _historyContainer) {
            .items = NULL,
            .size = 0
    };
}

static void history_addHistory(Textbuffer tb, const char *string, bool clearRedo) {
    if (History == NULL) history_initHistory();
    _HistoryKey key = history_getKey(tb);

    char **record = &key->records[key->_cursor];
    if (*record) free(*record);
    *record = strdup(string);
    key->_cursor = (key->_cursor + 1) % HISTORY_SIZE;
    
    if (clearRedo) history_clearRedo_by_key(key);
}

static void history_clearRedo_by_key(_HistoryKey key) {
    while (key->_redoItems_count) {
        free(key->redoItems[--key->_redoItems_count]);
    };
    assert(key->_redoItems_count == 0);
}

static void history_addRedo(Textbuffer tb, const char* string) {
    if (History == NULL) history_initHistory();
    _HistoryKey key = history_getKey(tb);
    
    // assert(key->_redoItems_count < 10);
    key->redoItems[key->_redoItems_count++] = strdup(string);
}

static char *history_getRedo(Textbuffer tb) {
    if (History == NULL) history_initHistory();
    _HistoryKey key = history_getKey(tb);
    
    if (key->_redoItems_count) {
        char **data = &key->redoItems[--key->_redoItems_count];
        char *res = strdup(*data);
        free(*data);
        
        return res;
    }
    
    return NULL;
}

static char *history_getHistory(Textbuffer tb) {
    if (History == NULL) history_initHistory();
    _HistoryKey key = history_getKey(tb);

    key->_cursor = (key->_cursor - 1 % HISTORY_SIZE + HISTORY_SIZE) % HISTORY_SIZE;

    char **data = &key->records[key->_cursor % HISTORY_SIZE];
    if (*data) {
        char *res = strdup(*data);
        free(*data);
        *data = NULL;

        return res;
    }
    return NULL;
}


static _HistoryKey history_getKey(Textbuffer tb) {
    if (History == NULL) history_initHistory();

    _HistoryKey *item = &History->items;
    while (*item) {
        if ((*item)->key == tb) return *item;
        if ((*item)->next == NULL) break;
        *item = (*item)->next;
    }

    *item = malloc(sizeof(struct _historyKey));

    (**item) = (struct _historyKey) {
            .key = tb,
            .records = {NULL},
            ._cursor = 0,
            .next = NULL,
            
            .redoItems = {NULL},
            ._redoItems_count = 0
    };

    return *item;
}


static void history_dropKey(Textbuffer tb) {
    if (History == NULL) history_initHistory();

    _HistoryKey prev = NULL;
    for (_HistoryKey item = History->items; item; item = item->next) {
        if (item->key == tb) {
            if (prev) prev->next = item->next;
            if (History->items == item) {
                History->items = item->next;
            }

            char **record = item->records;
            for (int i = 0; i < HISTORY_SIZE; i++) {
                if (record[i]) free(record[i]);
            }

            history_clearRedo_by_key(item);
            
            free(item);
            return;
        }
        prev = item;
    }
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
    char *lastVal = history_getHistory(tb);
    if (lastVal) {
    
        char* currVal = textbuffer_to_str(tb);
        history_addRedo(tb, currVal);
        free(currVal);
        
        Textbuffer oldTB = textbuffer_new(lastVal);
        free(lastVal);
        textbuffer_drop_head(tb);
        tb->head = oldTB->head;
        tb->size = oldTB->size;
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
    char *lastVal = history_getRedo(tb);
    if (lastVal) {
        char* currVal = textbuffer_to_str(tb);
        history_addHistory(tb, currVal, false);
        free(currVal);
    
        Textbuffer oldTB = textbuffer_new(lastVal);
        free(lastVal);
        textbuffer_drop_head(tb);
        tb->head = oldTB->head;
        tb->size = oldTB->size;
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
    if (tb1 && tb2) 0;
}

/* White box testing */
void verifyLinks(Textbuffer tb) {
//    printf("  Looking at tb: %p\n", tb);
    TextbufferLine curr = tb->head;
    for (unsigned int i = 0; i < tb->size; curr = curr->next, i++) {
//        printf("%10p <- %10p -> %10p\n", curr->prev, curr, curr->next);
        if (i == 0) {
            assert(curr->prev == NULL);
        } else {
            assert(curr->prev);
            assert(curr == curr->prev->next);
        }

        if (i == (tb->size - 1)) {
            assert(curr->next == NULL);
        } else {
            assert(curr->next);
            assert(curr == curr->next->prev);
        }
    }
//    puts("");
//    for (TextbufferLine curr = tb->head; curr; curr = curr->next) {
//        if (curr->prev) assert(curr == curr->prev->next);
//        if (curr->next) assert(curr == curr->next->prev);
//    }
}


void white_box_tests(void) {
}
