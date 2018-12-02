////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT
//
// 2018-11-30	Andrew Wong <z5206677@unsw.edu.au>

#include "textbuffer.h"
#include <stdio.h>

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

/**
 * Release any resources used by a given textbuffer.
 * It is an error to access the textbuffer after this.
 */
void textbuffer_drop(Textbuffer tb) {
    TextbufferLine cursor = tb->head;
    while (cursor != NULL) {
        TextbufferLine next = cursor->next;
        textbuffer_line_drop(cursor);
        cursor = next;
    }
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
    TextbufferLine line1 = textbuffer_get_line(tb, pos1);
    TextbufferLine line2 = textbuffer_get_line(tb, pos2);

    if (!line1 || !line2) {
        fprintf(stderr, "Line index out of range!\n");
        abort();
    }

    if (line1 == line2) return; // Silently fail if lines are the same (skip)

    TextbufferLine line1_prev, line1_next, line2_prev, line2_next;
    line1_prev = line1->prev;
    line1_next = line1->next;
    line2_prev = line2->prev;
    line2_next = line2->next;

    printf("\n\nFROM\n");
    printf("LINE1 %10p <- %10p -> %10p\n", line1->prev, line1, line1->next);
    printf("LINE2 %10p <- %10p -> %10p\n", line2->prev, line2, line2->next);
    printf("ORDER -> [%p]", tb->head);
    for (TextbufferLine curr = tb->head->next; curr; curr = curr->next) printf(" -> %p", curr);

    if (line1_prev == NULL) {
        printf("\n--> Head reassigned\n");
        tb->head = line2;
    } else if (line2_prev == NULL) {
        printf("\n--> Head reassigned\n");
        tb->head = line1;
    }

    line2->prev = line1_prev;
    if (line1_prev) line1_prev->next = line2;

    line2->next = line1_next;
    if (line1_next) line1_next->prev = line2;

    line1->prev = line2_prev;
    if (line2_prev) line2_prev->next = line1;

    line1->next = line2_next;
    if (line2_next) line2_next->prev = line1;

    if (line1_next == line2) {
        printf("--> Adjacent elements\n");
        line1->next = line2_next;
        if (line2_next) line2_next->prev = line1;

        line2->next = line1;
        line1->prev = line2;
    } else if (line2_next == line1) {
        printf("--> Adjacent elements\n");
        line2->next = line1_next;
        if (line1_next) line1_next->prev = line2;

        line1->next = line2;
        line2->prev = line1;
    }
//    line1->next = line2_next;
//    if (line2_next) line2_next->prev = line1;
//
//    line2->prev = line1_prev;
//    if (line1_prev) line1_prev->next = line2;
////
//
//    if (line2_prev == line1) {
//        printf("-ADJ-");
//        line1->prev = line2;
//        line2->next = line1;
//    } else if (line1_prev == line2) {
//        printf("-RADJ-");
//        line1->next = line2;
//        line2->prev = line1;
//    } else {
//        line1->prev = line2_prev;
//        if (line2_prev) line2_prev->next = line1;
//
//    }
////
//    if (line1_next == line2) {
//        printf("-ADJ-");
//        line2->next = line1;
//        line1->prev = line2;
//    } else {
//        line2->next = line1_next;
//        if (line1_next) line1_next->prev = line2;
//    }


    printf("\n\nTO\n");
    printf("LINE1 %10p <- %10p -> %10p\n", line1->prev, line1, line1->next);
    printf("LINE2 %10p <- %10p -> %10p\n\n", line2->prev, line2, line2->next);
    printf("ORDER -> [%p]", tb->head);
    for (TextbufferLine curr = tb->head->next; curr; curr = curr->next) {
        printf(" -> %p", curr);
        if (curr->next == curr) {
            printf("\nGOT LOOP\n");
            abort();
        }
    }
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
    if (tb1 == tb2) return;

    if (pos > 0) {
        TextbufferLine tempTail = textbuffer_get_line(tb1, pos);
        TextbufferLine tempTail_next = tempTail->next;

        tempTail->next = tb2->head;
        tempTail->next->prev = tempTail;

        TextbufferLine tb2_tail;
        for (tb2_tail = tb2->head; tb2_tail && tb2_tail->next; tb2_tail = tb2_tail->next);
        tb2_tail->next = tempTail_next;
        tempTail_next->prev = tb2_tail;
    } else {
        tb1->head = tb2->head;
    }

    tb1->size += tb2->size;
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
    free(tb2); // free-ing just the `struct textbuffer`, and not the elements within
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
    TextbufferLine lineFrom = textbuffer_get_line(tb, from);
    TextbufferLine lineTo = textbuffer_get_line(tb, to);

    if (!lineFrom || !lineTo) {
        fprintf(stderr, "Line index out of range!\n");
        abort();
    }

    if (lineFrom->prev == NULL) {
        tb->head = lineTo->next;
        if (lineTo->next != NULL) {
            lineTo->next->prev = NULL;
        }
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
    Textbuffer segment = textbuffer_cut(tb, from, to);
    free(tb_clone); // free-ing just the `struct textbuffer`, and not the elements within
    return segment;
}
//Textbuffer textbuffer_copy(const Textbuffer tb, size_t from, size_t to) {
//    TextbufferLine lineFrom = textbuffer_get_line(tb, from);
//    TextbufferLine lineTo = textbuffer_get_line(tb, to);
//    if (!lineFrom || !lineTo) {
//        fprintf(stderr, "Line index out of range!\n");
//        abort();
//    }
//
//    size_t newSize = to - from + 1;
//    TextbufferLine head = NULL, prev = NULL;
//    TextbufferLine cursor = lineFrom;
//    for (unsigned int count = 0; count < newSize; count++) {
//        TextbufferLine line = textbuffer_line_copy(cursor);
//        if (head == NULL) {
//            head = line;
//        }
//
//        if (prev != NULL) {
//            prev->next = line;
//        }
//
//        prev = line;
//        cursor = cursor->next;
//    }
//
//    Textbuffer segment = textbuffer_new(NULL);
//    segment->head = head;
//    segment->size = newSize;
//
//    return segment;
//}

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
ssize_t textbuffer_search(const Textbuffer tb, const char *match, bool rev) {
    if (!rev) {
        // Go from HEAD to TAIL
        size_t lineCount = 0;
        for (TextbufferLine cursor = tb->head; cursor != NULL; cursor = cursor->next) {
            if (strstr(cursor->data, match)) {
                return (ssize_t) lineCount;
            }
        }
    } else {
        // Go from TAIL to HEAD
        TextbufferLine cursor;
        for (cursor = tb->head; cursor && cursor->next; cursor = cursor->next);
        size_t lineCount = tb->size - 1;
        for (; cursor != NULL; cursor = cursor->prev) {
            if (strstr(cursor->data, match)) {
                return (ssize_t) lineCount;
            }
        }
    }

    return -1;
}


static void textbuffer_line_replace(TextbufferLine line, const char *match, const char *replace) {
    size_t sizeReplace = strlen(replace);
    size_t sizeMatch = strlen(match);

    int sizeDifference = (int) (sizeReplace - sizeMatch);

    size_t newLine_len = line->size + 1;
    char *newLine = malloc(newLine_len);
    char *newLineCursor = newLine;

    char *lineCursor = line->data;

    char *token;
    while ((token = strstr(lineCursor, match)) != NULL) {
        if (sizeDifference != 0) {
            int cursorOffset = newLineCursor - newLine;
            newLine = realloc(newLine, (newLine_len = (size_t)((int) newLine_len + sizeDifference)));
            newLineCursor = newLine + cursorOffset;
        }

        // Copy everything before the matched string
        size_t offset = (size_t)(token - lineCursor);
        strncpy(newLineCursor, lineCursor, offset);

        // Copy the replacement string, growing/shrinking memory for newLine if necessary.
        strncpy(newLineCursor + offset, replace, sizeReplace);

        lineCursor += sizeMatch;
    }

    line->size = newLine_len - 1;
}

/**
 * Search every line of `tb` for occurrences of `match`, and replace
 * them all with `replace`.
 */
void textbuffer_replace(Textbuffer tb, const char *match, const char *replace) {
    for (TextbufferLine cursor = tb->head; cursor != NULL; cursor = cursor->next) {
        textbuffer_line_replace(cursor, match, replace);
    }
}

/* Challenge Part */

/* White box testing */
void white_box_tests();

void white_box_tests() {

//    static bool test_textbuffer_empty() {
//        puts("test_textbuffer_empty ::");
//        printf("  create textbuffer"); Textbuffer tb = textbuffer_new(NULL); printOK();
//        printf("  check tb->head == NULL"); assert(tb->head == NULL); printOK();
//        printf("  check tb->size == 0"); assert(tb->size == 0); printOK();
//        return true;
//    }
}
