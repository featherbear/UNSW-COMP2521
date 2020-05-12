////////////////////////////////////////////////////////////////////////
// COMP2521 19x1 ... Assignment 1: A Simple Text Buffer
//
// 2018-11-29	v1.0	Team Textbuffer <cs2521@cse.unsw.edu.au>
//      Initial release onto unsuspecting students.
//
// 2018-11-30	v1.0.1	Team Textbuffer <cs2521@cse.unsw.edu.au>
//      Tweak the prototype of `textbuffer_search`; clarify its comments
//      to define its behaviour when no match is found and the meaning
//      of its third argument.
//
// 2018-12-02	v1.0.2	Team Textbuffer <cs2521@cse.unsw.edu.au>
//      Remove `const` qualifiers from `Textbuffer`.
//
// 2018-12-07	v1.0.7	Team Textbuffer <cs2521@cse.unsw.edu.au>
//      Add a prototype to enable white-box testing.
//
// // // // // // // DO NOT MODIFY THIS FILE! // // // // // // // // //

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef CS2521__TEXTBUFFER_H_
#define CS2521__TEXTBUFFER_H_

/**
 * A handle to an instance of a textbuffer.
 *
 * A textbuffer is an ordered collection of strings, where each string
 * represents one line of a text file.  Such a data structure may, for
 * example, be used as part of a text editor.
 *
 * Your implementation must keep the lines of a textbuffer in a linked
 * data structure (such as a linked list, or similar).  Each line must
 * be represented as a dynamically-allocated character array.  Adding,
 * deleting, or moving of lines requires manipulation of the linked
 * structure.
 */
typedef struct textbuffer *Textbuffer;

/**
 * Allocate a new textbuffer, whose contents is initialised with the
 * text given in the array.  The lines in the input array are all
 * terminated by a `'\\n'`. The whole text is terminated by a `'\\0'`.
 */
Textbuffer textbuffer_new (const char *text);

/**
 * Release any resources used by a given textbuffer.
 * It is an error to access the textbuffer after this.
 */
void textbuffer_drop (Textbuffer tb);


/**
 * Return the number of lines of text stored in the given textbuffer.
 */
size_t textbuffer_lines (Textbuffer tb);

/**
 * Return the number of bytes of text stored in the given textbuffer,
 * counting the newlines that would be needed.
 */
size_t textbuffer_bytes (Textbuffer tb);

/**
 * Return an array containing the text in the given textbuffer.  Each
 * individual line of the textbuffer needs to be terminated by a `'\\n'`,
 * including the last line.  If there are no lines in the textbuffer,
 * return the empty string.
 *
 * It is the caller's responsibility to free the returned array.
 */
char *textbuffer_to_str (Textbuffer tb);


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
void textbuffer_swap (Textbuffer tb, size_t pos1, size_t pos2);

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
void textbuffer_insert (Textbuffer tb1, size_t pos, Textbuffer tb2);

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
void textbuffer_paste (
	Textbuffer tb1, size_t pos, Textbuffer tb2);

/**
 * Cut lines `from` through `to` inclusive out of the textbuffer `tb`,
 * into a new textbuffer.  The cut lines should be deleted from `tb`,
 * and should no longer be associated with it.
 *
 * If lines `from` or `to` are out of range, the program should
 * `abort()` with an error message.
 */
Textbuffer textbuffer_cut (Textbuffer tb, size_t from, size_t to);

/**
 * Copy lines `from` through `to` inclusive from the textbuffer `tb`,
 * into a new textbuffer, leaving `tb` unmodified.
 *
 * If lines `from` or `to` are out of range, the program should
 * `abort()` with an error message.
 */
Textbuffer textbuffer_copy (
	Textbuffer tb, size_t from, size_t to);

/**
 * Remove lines `from` through `to` inclusive from textbuffer `tb`.
 *
 * If lines `from` or `to` are out of range, the program should
 * `abort()` with an error message.
 */
void textbuffer_delete (Textbuffer tb, size_t from, size_t to);

/**
 * From the beginning/end of the textbuffer, search for the first
 * occurrence of the string `match`.  Return the number of the line
 * containing the match, or -1 if there was no match.
 *
 * @param tb	the Textbuffer to search in
 * @param match	the string to match on
 * @param rev	if true, search from the last line backwards;
 *		otherwise, search forwards from the first line.
 */
ssize_t textbuffer_search (
	Textbuffer tb, char *match, bool rev);

/**
 * Search every line of `tb` for occurrences of `match`, and replace
 * them all with `replace`.
 */
void textbuffer_replace (
	Textbuffer tb, char *match, char *replace);


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
void textbuffer_undo (Textbuffer tb);

/**
 * CHALLENGE:
 *
 * Redo an operation that has been undone by `textbuffer_undo`.  This
 * function should redo one operation per call; when a new operation is
 * called on `tb`, previously-undone operations cannot be redone.
 */
void textbuffer_redo (Textbuffer tb);


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
char *textbuffer_diff (Textbuffer tb1, Textbuffer tb2);


/**
 * Run white-box tests for this particular implementation.
 */
void white_box_tests (void);

#endif // !defined (CS2521__TEXTBUFFER_H_)
