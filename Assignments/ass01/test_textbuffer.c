////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT
//
// 2018-11-30	Andrew Wong <z5206677@unsw.edu.au>

#include <assert.h>
#include <stdio.h>

#include "textbuffer.h"

#define O() printf(" [OK]\n");
#define T(m) printf("\n"); printf(m); printf(" ::\n");
#define A(m) printf("  "); printf(m);

void test_create__emptyTextbuffer(void);
void test_create__singleLineTextbuffer(void);
void test_create__multiLineTextbuffer(void);
void test_create__multipleNewlineTextbuffer(void);

void test_swap__swapHeadBody(void);
void test_swap__swapHeadBody_adjacent(void);
void test_swap__swapBodyTail(void);
void test_swap__swapBodyTail_adjacent(void);
void test_swap__swapHeadTail(void);
void test_swap__swapBodyBody(void);
void test_swap__swapBodyBody_adjacent(void);

void test_swap_reverse__swapHeadBody(void);
void test_swap_reverse__swapHeadBody_adjacent(void);
void test_swap_reverse__swapBodyTail(void);
void test_swap_reverse__swapBodyTail_adjacent(void);
void test_swap_reverse__swapHeadTail(void);
void test_swap_reverse__swapBodyBody(void);
void test_swap_reverse__swapBodyBody_adjacent(void);

void test_swap_self__swapHeadHead(void);
void test_swap_self__swapTailTail(void);
void test_swap_self__swapBodyBody(void);

void test_copy__one(void);
void test_copy__many(void);
void test_copy__all(void);

void test_cut__one(void);
void test_cut__many(void);
void test_cut__all(void);

void test_delete__one(void);
void test_delete__many(void);
void test_delete__all(void);

void test_insert__insertHead(void);
void test_insert__insertBody(void);
void test_insert__insertTail(void);
void test_insert__insertBodyTwice(void);

void test_paste__pasteHead(void);
void test_paste__pasteBody(void);
void test_paste__pasteTail(void);
void test_paste__pasteBodyTwice(void);

void test_search__match(void);
void test_search__nomatch(void);
void test_search__empty(void);

void test_search_reverse__match(void);
void test_search_reverse__nomatch(void);
void test_search_reverse__empty(void);

void test_replace__withSameLength(void);
void test_replace__withLesserLength(void);
void test_replace__withGreaterLength(void);
void test_replace__withNothing(void);
void test_replace__nothing(void);

///

void test_create(void);
void test_swap(void);
void test_swap_reverse(void);
void test_swap_self(void);
void test_copy(void);
void test_cut(void);
void test_delete(void);
void test_insert(void);
void test_paste(void);
void test_search(void);
void test_search_reverse(void);
void test_replace(void);

///

void test_history(void);

void test_diff(void);
char* _diff_applyEdit(Textbuffer tb, char* script);

///

int main(void) {
    // White-box tests
    white_box_tests();

    // Standard tests
    test_create();
    test_swap();
    test_swap_reverse();
    test_swap_self();
    test_copy();
    test_cut();
    test_delete();
    test_insert();
    test_paste();
    test_search();
    test_search_reverse();
    test_replace();

    // Challenge tests
    test_history();
    test_diff();

    printf("\nAll tests passed!\n");
    return EXIT_SUCCESS;
}

///

void test_create__emptyTextbuffer(void) {
    T("zero length textbuffer");
    A("new"); Textbuffer tb = textbuffer_new(NULL); O();
    A("lines"); assert(textbuffer_lines(tb) == 0); O();
    A("bytes"); assert(textbuffer_bytes(tb) == 0); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(result == NULL); O();
    A("drop"); textbuffer_drop(tb); O();
}

void test_create__singleLineTextbuffer(void) {
    char *input = "Hey guys!\n";
    T("textbuffer with single line input");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_lines"); assert(textbuffer_lines(tb) == 1); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(input)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_create__multiLineTextbuffer(void) {
    char *input = "Hey guys!\nMy name is...\n(wait for it...)\n...\n...\nAndrew!...\n";
    T("textbuffer with multiple line input");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_lines"); assert(textbuffer_lines(tb) == 6); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(input)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_create__multipleNewlineTextbuffer(void) {
    char *input = "\nAlot\n\nof\n\n\n\nli\nn\n\n\n\ne\n\n\n\n\n\nbreaks...\n";
    T("textbuffer with consecutive line breaks");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_lines"); assert(textbuffer_lines(tb) == 19); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(input)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

///

void test_swap__swapHeadBody(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "3\n2\n1\n4\n";
    T("textbuffer swap head and body");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 0, 2); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swap__swapHeadBody_adjacent(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "2\n1\n3\n4\n";
    T("textbuffer swap head and body | adjacent swap");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 0, 1); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}


void test_swap__swapBodyTail(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "1\n4\n3\n2\n";
    T("textbuffer swap body and tail");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 1, 3); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swap__swapBodyTail_adjacent(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "1\n2\n4\n3\n";
    T("textbuffer swap body and tail | adjacent");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 2, 3); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swap__swapHeadTail(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "4\n2\n3\n1\n";
    T("textbuffer swap head and tail");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 0, 3); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swap__swapBodyBody(void) {
    char *input = "1\n2\n3\n4\n5\n6\n";
    char *expectedOutput = "1\n5\n3\n4\n2\n6\n";
    T("textbuffer swap body and body");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 1, 4); O();
    A("_lines"); assert(textbuffer_lines(tb) == 6); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swap__swapBodyBody_adjacent(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "1\n3\n2\n4\n";
    T("textbuffer swap body and body | adjacent swap");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 1, 2); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

///

void test_swap_self__swapHeadHead(void) {
    char *input = "1\n2\n3\n4\n";
    T("textbuffer swap head and head");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 0, 0); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(input)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swap_self__swapBodyBody(void) {
    char *input = "1\n2\n3\n4\n";
    T("textbuffer swap body and body | self with self");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 2, 2); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(input)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swap_self__swapTailTail(void) {
    char *input = "1\n2\n3\n4\n";
    T("textbuffer swap tail and tail");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 3, 3); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(input)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

///

void test_swap_reverse__swapHeadBody(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "3\n2\n1\n4\n";
    T("textbuffer swap head and body (reverse)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 2, 0); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swap_reverse__swapHeadBody_adjacent(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "2\n1\n3\n4\n";
    T("textbuffer swap head and body | adjacent swap (reverse)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 1, 0); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swap_reverse__swapBodyTail(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "1\n4\n3\n2\n";
    T("textbuffer swap body and tail (reverse)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 3, 1); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swap_reverse__swapBodyTail_adjacent(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "1\n2\n4\n3\n";
    T("textbuffer swap body and tail | adjacent (reverse)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 3, 2); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swap_reverse__swapHeadTail(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "4\n2\n3\n1\n";
    T("textbuffer swap head and tail (reverse)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 3, 0); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swap_reverse__swapBodyBody(void) {
    char *input = "1\n2\n3\n4\n5\n6\n";
    char *expectedOutput = "1\n5\n3\n4\n2\n6\n";
    T("textbuffer swap body and body (reverse)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 4, 1); O();
    A("_lines"); assert(textbuffer_lines(tb) == 6); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_swap_reverse__swapBodyBody_adjacent(void) {
    char *input = "1\n2\n3\n4\n";
    char *expectedOutput = "1\n3\n2\n4\n";
    T("textbuffer swap body and body | adjacent swap (reverse)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_swap"); textbuffer_swap(tb, 2, 1); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

///

void test_copy__one(void) {
    char *input = "1\n2\n3\n4\n5\n";
    char *expectedOutput = "1\n";
    T("textbuffer copy one line");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_copy"); Textbuffer tb_segment = textbuffer_copy(tb, 0, 0); O();
    A("_lines (input)"); assert(textbuffer_lines(tb) == 5); O();
    A("_lines (segment)"); assert(textbuffer_lines(tb_segment) == 1); O();
    A("_to_str"); char *result = textbuffer_to_str(tb_segment); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); textbuffer_drop(tb_segment); O();
}

void test_copy__many(void) {
    char *input = "1\n2\n3\n4\n5\n";
    char *expectedOutput = "2\n3\n4\n";
    T("textbuffer copy many lines");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_copy"); Textbuffer tb_segment = textbuffer_copy(tb, 1, 3); O();
    A("_lines (input)"); assert(textbuffer_lines(tb) == 5); O();
    A("_lines (segment)"); assert(textbuffer_lines(tb_segment) == 3); O();
    A("_to_str"); char *result = textbuffer_to_str(tb_segment); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); textbuffer_drop(tb_segment); O();
}

void test_copy__all(void) {
    char *input = "1\n2\n3\n4\n5\n";
    T("textbuffer copy all lines");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_copy"); Textbuffer tb_segment = textbuffer_copy(tb, 0, 4); O();
    A("_lines (input)"); assert(textbuffer_lines(tb) == 5); O();
    A("_lines (segment)"); assert(textbuffer_lines(tb_segment) == 5); O();
    A("_to_str"); char *result = textbuffer_to_str(tb_segment); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); textbuffer_drop(tb_segment); O();
}

///

void test_cut__one(void) {
    char *input = "1\n2\n3\n4\n5\n";
    char *expectedOutput = "1\n";
    T("textbuffer cut one line");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_cut"); Textbuffer tb_segment = textbuffer_cut(tb, 0, 0); O();
    A("_lines (input)"); assert(textbuffer_lines(tb) == 4); O();
    A("_lines (segment)"); assert(textbuffer_lines(tb_segment) == 1); O();
    A("_to_str"); char *result = textbuffer_to_str(tb_segment); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); textbuffer_drop(tb_segment); O();
}

void test_cut__many(void) {
    char *input = "1\n2\n3\n4\n5\n";
    char *expectedOutput = "2\n3\n4\n";
    T("textbuffer cut many lines");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_cut"); Textbuffer tb_segment = textbuffer_cut(tb, 1, 3); O();
    A("_lines (input)"); assert(textbuffer_lines(tb) == 2); O();
    A("_lines (segment)"); assert(textbuffer_lines(tb_segment) == 3); O();
    A("_to_str"); char *result = textbuffer_to_str(tb_segment); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); textbuffer_drop(tb_segment); O();
}

void test_cut__all(void) {
    char *input = "1\n2\n3\n4\n5\n";
    T("textbuffer cut all lines");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_cut"); Textbuffer tb_segment = textbuffer_cut(tb, 0, 4); O();
    A("_lines (input)"); assert(textbuffer_lines(tb) == 0); O();
    A("_lines (segment)"); assert(textbuffer_lines(tb_segment) == 5); O();
    A("_to_str"); char *result = textbuffer_to_str(tb_segment); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); textbuffer_drop(tb_segment); O();
}

///

void test_delete__one(void) {
    char *input = "1\n2\n3\n4\n5\n";
    char *expectedOutput = "2\n3\n4\n5\n";
    T("textbuffer delete one line");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_delete"); textbuffer_delete(tb, 0, 0); O();
    A("_lines"); assert(textbuffer_lines(tb) == 4); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_delete__many(void) {
    char *input = "1\n2\n3\n4\n5\n";
    char *expectedOutput = "1\n5\n";
    T("textbuffer delete many lines");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_delete"); textbuffer_delete(tb, 1, 3); O();
    A("_lines"); assert(textbuffer_lines(tb) == 2); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_delete__all(void) {
    char *input = "1\n2\n3\n4\n5\n";
    T("textbuffer delete all lines");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_delete"); textbuffer_delete(tb, 0, 4); O();
    A("_lines"); assert(textbuffer_lines(tb) == 0); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(result == NULL); O();
    A("_drop"); textbuffer_drop(tb); O();
}

///

void test_insert__insertHead(void) {
    char *input = "2\n3\n4\n5\n";
    char *insert = "1\n";
    char *expectedOutput = "1\n2\n3\n4\n5\n";
    T("textbuffer insert into head");
    A("_new (input)"); Textbuffer tb = textbuffer_new(input); O();
    A("_new (insert)"); Textbuffer tb_insert = textbuffer_new(insert); O();
    A("_insert"); textbuffer_insert(tb, 0, tb_insert); O();
    A("_lines"); assert(textbuffer_lines(tb) == 5); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_insert__insertBody(void) {
    char *input = "1\n3\n4\n5\n";
    char *insert = "2\n";
    char *expectedOutput = "1\n2\n3\n4\n5\n";
    T("textbuffer insert into body");
    A("_new (input)"); Textbuffer tb = textbuffer_new(input); O();
    A("_new (insert)"); Textbuffer tb_insert = textbuffer_new(insert); O();
    A("_insert"); textbuffer_insert(tb, 1, tb_insert); O();
    A("_lines"); assert(textbuffer_lines(tb) == 5); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_insert__insertTail(void) {
    char *input = "1\n2\n3\n4\n";
    char *insert = "5\n";
    char *expectedOutput = "1\n2\n3\n4\n5\n";
    T("textbuffer insert into tail");
    A("_new (input)"); Textbuffer tb = textbuffer_new(input); O();
    A("_new (insert)"); Textbuffer tb_insert = textbuffer_new(insert); O();
    A("_insert"); textbuffer_insert(tb, 4, tb_insert); O();
    A("_lines"); assert(textbuffer_lines(tb) == 5); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_insert__insertBodyTwice(void) {
    char *input = "1\n4\n5\n";
    char *insert1 = "3\n";
    char *insert2 = "2\n";
    char *expectedOutput = "1\n2\n3\n4\n5\n";
    T("textbuffer insert into body twice");
    A("_new (input)"); Textbuffer tb = textbuffer_new(input); O();
    A("_new (insert 1)"); Textbuffer tb_insert1 = textbuffer_new(insert1); O();
    A("_new (insert 2)"); Textbuffer tb_insert2 = textbuffer_new(insert2); O();
    A("_insert 1"); textbuffer_insert(tb, 1, tb_insert1); O();
    A("_insert 2"); textbuffer_insert(tb, 1, tb_insert2); O();
    A("_lines"); assert(textbuffer_lines(tb) == 5); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

///

void test_paste__pasteHead(void) {
    char *input = "2\n3\n4\n5\n";
    char *paste = "1\n";
    char *expectedOutput = "1\n2\n3\n4\n5\n";
    T("textbuffer paste into head");
    A("_new (input)"); Textbuffer tb = textbuffer_new(input); O();
    A("_new (paste)"); Textbuffer tb_paste = textbuffer_new(paste); O();
    A("_paste"); textbuffer_paste(tb, 0, tb_paste); O();
    A("_lines (input)"); assert(textbuffer_lines(tb) == 5); O();
    A("_lines (paste)"); assert(textbuffer_lines(tb_paste) == 1); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); textbuffer_drop(tb_paste); O();
}

void test_paste__pasteBody(void) {
    char *input = "1\n3\n4\n5\n";
    char *paste = "2\n";
    char *expectedOutput = "1\n2\n3\n4\n5\n";
    T("textbuffer paste into body");
    A("_new (input)"); Textbuffer tb = textbuffer_new(input); O();
    A("_new (paste)"); Textbuffer tb_paste = textbuffer_new(paste); O();
    A("_paste"); textbuffer_paste(tb, 1, tb_paste); O();
    A("_lines (input)"); assert(textbuffer_lines(tb) == 5); O();
    A("_lines (paste)"); assert(textbuffer_lines(tb_paste) == 1); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); textbuffer_drop(tb_paste); O();
}

void test_paste__pasteTail(void) {
    char *input = "1\n2\n3\n4\n";
    char *paste = "5\n";
    char *expectedOutput = "1\n2\n3\n4\n5\n";
    T("textbuffer paste into tail");
    A("_new (input)"); Textbuffer tb = textbuffer_new(input); O();
    A("_new (paste)"); Textbuffer tb_paste = textbuffer_new(paste); O();
    A("_paste"); textbuffer_paste(tb, 4, tb_paste); O();
    A("_lines (input)"); assert(textbuffer_lines(tb) == 5); O();
    A("_lines (paste)"); assert(textbuffer_lines(tb_paste) == 1); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); textbuffer_drop(tb_paste); O();
}

void test_paste__pasteBodyTwice(void) {
    char *input = "5\n2\n0\n6\n6\n";
    char *paste = "7\n";
    char *expectedOutput = "5\n2\n0\n6\n6\n7\n7\n";
    T("textbuffer paste into body twice");
    A("_new (input)"); Textbuffer tb = textbuffer_new(input); O();
    A("_new (paste)"); Textbuffer tb_paste = textbuffer_new(paste); O();
    A("_paste 1"); textbuffer_paste(tb, 5, tb_paste); O();
    A("_paste 2"); textbuffer_paste(tb, 6, tb_paste); O();
    A("_lines (input)"); assert(textbuffer_lines(tb) == 7); O();
    A("_lines (paste)"); assert(textbuffer_lines(tb_paste) == 1); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); textbuffer_drop(tb_paste); O();
}

///

void test_search__match(void) {
    char *input = "one\nmississippi\ntwo\nmississippi\nthree\nmississippi\nfour\n";
    T("textbuffer check for search match");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_search"); assert(textbuffer_search(tb, "mississippi", false) == 1); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_search__nomatch(void) {
    char *input = "one\nmississippi\ntwo\nmississippi\nthree\nmississippi\nfour\n";
    T("textbuffer check for no search match");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_search"); assert(textbuffer_search(tb, "Mississippi", false) == -1); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_search__empty(void) {
    char *input = "one\nmississippi\ntwo\nmississippi\nthree\nmississippi\nfour\n";
    T("textbuffer check for empty search");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_search"); assert(textbuffer_search(tb, "", false) == -1); O();
    A("_drop"); textbuffer_drop(tb); O();
}

///

void test_search_reverse__match(void) {
    char *input = "one\nmississippi\ntwo\nmississippi\nthree\nmississippi\nfour\n";
    T("textbuffer check for search match (reversed)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_search"); assert(textbuffer_search(tb, "mississippi", true) == 5); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_search_reverse__nomatch(void) {
    char *input = "one\nmississippi\ntwo\nmississippi\nthree\nmississippi\nfour\n";
    T("textbuffer check for no search match (reversed)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_search"); assert(textbuffer_search(tb, "Mississippi", true) == -1); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_search_reverse__empty(void) {
    char *input = "one\nmississippi\ntwo\nmississippi\nthree\nmississippi\nfour\n";
    T("textbuffer check for empty search (reversed)");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_search"); assert(textbuffer_search(tb, "Mississippi", true) == -1); O();
    A("_drop"); textbuffer_drop(tb); O();
}

///

void test_replace__withSameLength(void) {
    char *input = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n";
    char *expectedOutput = "Lorem_ipsum_dolor_sit_amet,_consectetur_adipiscing_elit,_sed_do_eiusmod_tempor_incididunt_ut_labore_et_dolore_magna_aliqua.\nUt_enim_ad_minim_veniam,_quis_nostrud_exercitation_ullamco_laboris_nisi_ut_aliquip_ex_ea_commodo_consequat.\nDuis_aute_irure_dolor_in_reprehenderit_in_voluptate_velit_esse_cillum_dolore_eu_fugiat_nulla_pariatur.\nExcepteur_sint_occaecat_cupidatat_non_proident,_sunt_in_culpa_qui_officia_deserunt_mollit_anim_id_est_laborum.\n";
    T("textbuffer replace with same length");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_replace"); textbuffer_replace(tb, " ", "_"); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_replace__withLesserLength(void) {
    char *input = "phishing for attention\nphotos are phake.\n";
    char *expectedOutput = "fishing for attention\nfotos are fake.\n";
    T("textbuffer replace with shorter length");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_replace"); textbuffer_replace(tb, "ph", "f"); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_replace__withGreaterLength(void) {
    char *input = "rock\nyou are a rock\n\ngrey\nyou are grey\n\nlike a rock\nwhich you are\n\nrock\n";
    char *expectedOutput = "stone\nyou are a stone\n\ngrey\nyou are grey\n\nlike a stone\nwhich you are\n\nstone\n";
    T("textbuffer replace with greater length");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_replace"); textbuffer_replace(tb, "rock", "stone"); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_replace__withNothing(void) {
    char *input = "buffalo buffalo buffalo buffalo buffalo buffalo buffalo buffalo\n";
    char *expectedOutput = "\n";
    T("textbuffer replace with nothing");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_replace (1)"); textbuffer_replace(tb, "buffalo", ""); O();
    A("_replace (2)"); textbuffer_replace(tb, " ", ""); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(expectedOutput)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, expectedOutput) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

void test_replace__nothing(void) {
    char *input = "buffalo buffalo buffalo buffalo buffalo buffalo buffalo buffalo\n";
    T("textbuffer replace nothing");
    A("_new"); Textbuffer tb = textbuffer_new(input); O();
    A("_replace"); textbuffer_replace(tb, "", ""); O();
    A("_bytes"); assert(textbuffer_bytes(tb) == strlen(input)); O();
    A("_to_str"); char *result = textbuffer_to_str(tb); assert(strcmp(result, input) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

///

void test_create(void) {
    test_create__emptyTextbuffer();
    test_create__singleLineTextbuffer();
    test_create__multiLineTextbuffer();
    test_create__multipleNewlineTextbuffer();
}

void test_swap(void) {
    test_swap__swapHeadBody();
    test_swap__swapHeadBody_adjacent();
    test_swap__swapBodyTail();
    test_swap__swapBodyTail_adjacent();
    test_swap__swapHeadTail();
    test_swap__swapBodyBody();
    test_swap__swapBodyBody_adjacent();
}

void test_swap_reverse(void) {
    test_swap_reverse__swapHeadBody();
    test_swap_reverse__swapHeadBody_adjacent();
    test_swap_reverse__swapBodyTail();
    test_swap_reverse__swapBodyTail_adjacent();
    test_swap_reverse__swapHeadTail();
    test_swap_reverse__swapBodyBody();
    test_swap_reverse__swapBodyBody_adjacent();
}

void test_swap_self(void) {
    test_swap_self__swapHeadHead();
    test_swap_self__swapBodyBody();
    test_swap_self__swapTailTail();
}

void test_copy(void) {
    test_copy__one();
    test_copy__many();
    test_copy__all();
}

void test_cut(void) {
    test_cut__one();
    test_cut__many();
    test_cut__all();
}
void test_delete(void) {
    test_delete__one();
    test_delete__many();
    test_delete__all();
}
void test_insert(void) {
    test_insert__insertHead();
    test_insert__insertBody();
    test_insert__insertTail();
    test_insert__insertBodyTwice();
}

void test_paste(void) {
    test_paste__pasteHead();
    test_paste__pasteBody();
    test_paste__pasteTail();
    test_paste__pasteBodyTwice();
}

void test_search(void) {
    test_search__match();
    test_search__nomatch();
    test_search__empty();
}

void test_search_reverse(void) {
    test_search_reverse__match();
    test_search_reverse__nomatch();
    test_search_reverse__empty();
}

void test_replace(void) {
    test_replace__withSameLength();
    test_replace__withLesserLength();
    test_replace__withGreaterLength();
    test_replace__withNothing();
    test_replace__nothing();
}

///

void test_history(void) {
    char *input = "Hey\nthere\n";
    /* insert */ char *insert1 = "Andrew\n"; char *expected1 = "Hey\nthere\nAndrew\n";
    /* insert */ char *insert2 = "Wong\n"; char *expected2 = "Hey\nthere\nAndrew\nWong\n";
    /*  undo  */ char *expected3 = expected1;
    /*  undo  */ char *expected4 = input;
    /*  redo  */ char *expected5 = expected1;
    /*  redo  */ char *expected6 = expected2;
    char *result;
    T("textbuffer check history undo/redo");
    A("_new (input)"); Textbuffer tb = textbuffer_new(input); O();
    A("_new (insert 1)"); Textbuffer tb_insert1 = textbuffer_new(insert1); O();
    A("_new (insert 2)"); Textbuffer tb_insert2 = textbuffer_new(insert2); O();
    A("_insert (1)"); textbuffer_insert(tb, 2, tb_insert1); O();
    A("_to_str"); result = textbuffer_to_str(tb); assert(strcmp(result, expected1) == 0); free(result); O();
    A("_insert (2)"); textbuffer_insert(tb, 3, tb_insert2); O();
    A("_to_str"); result = textbuffer_to_str(tb); assert(strcmp(result, expected2) == 0); free(result); O();
    A("_undo"); textbuffer_undo(tb); O();
    A("_to_str"); result = textbuffer_to_str(tb); assert(strcmp(result, expected3) == 0); free(result); O();
    A("_undo"); textbuffer_undo(tb); O();
    A("_to_str"); result = textbuffer_to_str(tb); assert(strcmp(result, expected4) == 0); free(result); O();
    A("_redo"); textbuffer_redo(tb); O();
    A("_to_str"); result = textbuffer_to_str(tb); assert(strcmp(result, expected5) == 0); free(result); O();
    A("_redo"); textbuffer_redo(tb); O();
    A("_to_str"); result = textbuffer_to_str(tb); assert(strcmp(result, expected6) == 0); free(result); O();
    A("_drop"); textbuffer_drop(tb); O();
}

///

/*
 * _diff_applyEdit
 * Performs edit operations on `tb`, given instructions from `script`
 */
char* _diff_applyEdit(Textbuffer tb, char* script) {
    if (!script) return NULL;

    // Clone the function parameters
    char* src = textbuffer_to_str(tb); tb = textbuffer_new(src); free(src);
    script = strdup(script);

    char *scriptPtr = script;
    char* editLine;

    while((editLine = strsep(&scriptPtr, "\n")) != NULL && scriptPtr) {
        char* editLinePtr = editLine;

        // Get metadata of the line (line number, add/delete)
        char* metadata = strsep(&editLinePtr, "\t");
        assert(metadata != NULL);

        // Extract action ('+'/'-')
        char *actionPtr = &metadata[strlen(metadata)-1];
        char action = *actionPtr;
        *actionPtr = '\0';

        // Extract line number
        size_t line = (size_t) strtol(metadata, NULL, 10);

        if (action == '+') {
            // Get line content
            char* editData = editLinePtr;
            assert(editData);

            // Append '\n' to the end of the line
            char *insertData = malloc(strlen(editData) + 1 + 1);
            sprintf(insertData, "%s\n", editData);

            // Insert the edit line
            textbuffer_insert(tb, line, textbuffer_new(insertData));
            free(insertData);
        } else {
            // Delete the line
            textbuffer_delete(tb, line, line);
        }
    }

    char *result = textbuffer_to_str(tb); textbuffer_drop(tb); free(script);
    return result;
}

void test_diff(void) {
    {
        char *source = "\nDont\nreally\n\nknow\nhow\n2\ntest this\n";
        char *output = "I\nDon't\nreally\n\nknow\nhow\ntwo\ntest\nthis\n";

        T("textbuffer difference check");
        A("_new (source)"); Textbuffer tb_source = textbuffer_new(source); O();
        A("_new (output)"); Textbuffer tb_output = textbuffer_new(output); O();

        A("_diff"); char* diff = textbuffer_diff(tb_source, tb_output); O();
        A("[edit]"); char* edit = _diff_applyEdit(tb_source, diff); assert(edit && strcmp(output, edit) == 0); O();

        A("_drop"); {
            free(diff);
            free(edit);
            textbuffer_drop(tb_source);
            textbuffer_drop(tb_output);
        }; O();
    }
}
