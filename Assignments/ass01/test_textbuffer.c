////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT
//
// 2018-11-30	Andrew Wong <z5206677@unsw.edu.au>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "textbuffer.local.h"
#include "textbuffer.h"

#define O() printf(" [OK] ...\n");
#define T(m) printf("\n"); printf(m); printf(" ::\n");
#define A(m) printf("  "); printf(m);

void test_emptyTextbuffer(void);
void test_singleLineTextbuffer(void);
void test_multiLineTextbuffer(void);
void test_multipleNewlineTextbuffer(void);

void test_swapHeadBody(void);
void test_swapHeadBody_adjacent(void);
void test_swapBodyTail(void);
void test_swapBodyTail_adjacent(void);
void test_swapHeadTail(void);

void test_swapHeadHead(void);
void test_swapTailTail(void);
void test_swapBodyBody(void);
void test_swapBodyBody_adjacent(void);
void test_swapBodyBody_self(void);

void test_swapHeadBody_reverse(void);
void test_swapHeadBody_adjacent_reverse(void);
void test_swapBodyTail_reverse(void);
void test_swapBodyTail_adjacent_reverse(void);
void test_swapHeadTail_reverse(void);

void test_swapBodyBody_reverse(void);
void test_swapBodyBody_adjacent_reverse(void);
/*
    Textbuffer textbuffer_new(const char *text);
    void textbuffer_drop(Textbuffer tb);
    size_t textbuffer_lines(const Textbuffer tb);
    size_t textbuffer_bytes(const Textbuffer tb);
    char *textbuffer_to_str(const Textbuffer tb);

    void textbuffer_swap(Textbuffer tb, size_t pos1, size_t pos2);
    void textbuffer_insert(Textbuffer tb1, size_t pos, Textbuffer tb2);
    void textbuffer_paste(Textbuffer tb1, size_t pos, const Textbuffer tb2);
    void textbuffer_paste(Textbuffer tb1, size_t pos, const Textbuffer tb2);
    Textbuffer textbuffer_copy(const Textbuffer tb, size_t from, size_t to);
    void textbuffer_delete(Textbuffer tb, size_t from, size_t to);
    ssize_t textbuffer_search(const Textbuffer tb, const char *match, bool rev);
    void textbuffer_replace(Textbuffer tb, const char *match, const char *replace);
*/

void test_emptyTextbuffer(void) {
    T("zero length textbuffer");
    A("new"); Textbuffer tb = textbuffer_new(NULL); O();
    A("lines"); assert(textbuffer_lines(tb) == 0); O();
    A("bytes"); assert(textbuffer_bytes(tb) == 0); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(result == NULL); free(result); O();
    A("drop"); textbuffer_drop(tb); O();
}

void test_singleLineTextbuffer(void) {
    char *input = "Hey guys!\n";
    T("textbuffer with single line input");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_lines"); assert(textbuffer_lines(tb) == 1); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 10); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_multiLineTextbuffer(void) {
    char *input = "Hey guys!\nMy name is...\n(wait for it...)\n...\n...\nAndrew!...\n";
    T("textbuffer with multiple line input");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_lines"); assert(textbuffer_lines(tb) == 6); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 60); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_multipleNewlineTextbuffer(void) {
    char *input = "\nAlot\n\nof\n\n\n\nli\nn\n\n\n\ne\n\n\n\n\n\nbreaks...\n";
    T("textbuffer with consecutive line breaks");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_lines"); assert(textbuffer_lines(tb) == 19); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 38); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swapHeadBody(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "3\n2\n1\n4\n";
    T("textbuffer swap head and body");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 0, 2); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swapHeadBody_adjacent(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "2\n1\n3\n4\n";
    T("textbuffer swap head and body | adjacent swap");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 0, 1); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}


void test_swapBodyTail(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "1\n4\n3\n2\n";
    T("textbuffer swap body and tail");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 1, 3); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swapBodyTail_adjacent(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "1\n2\n4\n3\n";
    T("textbuffer swap body and tail | adjacent");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 2, 3); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swapHeadTail(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "4\n2\n3\n1\n";
    T("textbuffer swap head and tail");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 0, 3); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swapHeadHead(void) {
    char *input = "1\n2\n3\n4\n";
    T("textbuffer swap head and head");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 0, 0); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swapBodyBody_self(void) {
    char *input = "1\n2\n3\n4\n";
    T("textbuffer swap body and body | self with self");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 2, 2); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swapTailTail(void) {
    char *input = "1\n2\n3\n4\n";
    T("textbuffer swap tail and tail");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 3, 3); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swapBodyBody(void) {
    char *input = "1\n2\n3\n4\n5\n6\n";
    char *expectedOutput = "1\n5\n3\n4\n2\n6\n";
    T("textbuffer swap body and body");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 1, 4); O();
    A("_lines"); assert(textbuffer_lines(tb) == 6); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 12); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swapBodyBody_adjacent(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "1\n3\n2\n4\n";
    T("textbuffer swap body and body | adjacent swap");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 1, 2); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}



///

void test_swapHeadBody_reverse(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "3\n2\n1\n4\n";
    T("textbuffer swap head and body (reverse)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 2, 0); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swapHeadBody_adjacent_reverse(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "2\n1\n3\n4\n";
    T("textbuffer swap head and body | adjacent swap (reverse)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 1, 0); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}


void test_swapBodyTail_reverse(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "1\n4\n3\n2\n";
    T("textbuffer swap body and tail (reverse)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 3, 1); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swapBodyTail_adjacent_reverse(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "1\n2\n4\n3\n";
    T("textbuffer swap body and tail | adjacent (reverse)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 3, 2); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swapHeadTail_reverse(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "4\n2\n3\n1\n";
    T("textbuffer swap head and tail (reverse)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 3, 0); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swapBodyBody_reverse(void) {
    char *input = "1\n2\n3\n4\n5\n6\n";
    char *expectedOutput = "1\n5\n3\n4\n2\n6\n";
    T("textbuffer swap body and body (reverse)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 4, 1); O();
    A("_lines"); assert(textbuffer_lines(tb) == 6); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 12); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swapBodyBody_adjacent_reverse(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "1\n3\n2\n4\n";
    T("textbuffer swap body and body | adjacent swap (reverse)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 2, 1); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == 8); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}


///













int main(void) {
    test_emptyTextbuffer();
    test_singleLineTextbuffer();
    test_multiLineTextbuffer();
    test_multipleNewlineTextbuffer();

    //
    test_swapHeadBody();
    test_swapHeadBody_adjacent();

    test_swapBodyTail();
    test_swapBodyTail_adjacent();

    test_swapHeadTail();

    test_swapHeadHead();
    test_swapBodyBody_self();
    test_swapTailTail();

    test_swapBodyBody();
    test_swapBodyBody_adjacent();
    //

    test_swapHeadBody_reverse();
    test_swapHeadBody_adjacent_reverse();
    test_swapBodyTail_reverse();
    test_swapBodyTail_adjacent_reverse();
    test_swapHeadTail_reverse();

    test_swapBodyBody_reverse();
    test_swapBodyBody_adjacent_reverse();

    puts("\nAll tests passed!");
    return EXIT_SUCCESS;
}

