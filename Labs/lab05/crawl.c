// crawl.c ... build a graph of part of the web
// Written by John Shepherd, September 2015
// Uses the cURL library and functions by Vincent Sanders <vince@kyllikki.org>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <curl/curl.h>
#include "stack.h"
#include "set.h"
#include "directedGraph.h"
#include "html.h"
#include "url_file.h"

#define BUFSIZE 1024

void setFirstURL(char *, char *);

int main(int argc, char **argv) {
    URL_FILE *handle;
    char buffer[BUFSIZE];
    char baseURL[BUFSIZE];
    char firstURL[BUFSIZE];
    char next[BUFSIZE];
    int maxURLs;

    if (argc > 2) {
        strcpy(baseURL, argv[1]);
        setFirstURL(baseURL, firstURL);
        maxURLs = atoi(argv[2]);
        if (maxURLs < 40) maxURLs = 40;
    } else {
        fprintf(stderr, "Usage: %s BaseURL MaxURLs\n", argv[0]);
        exit(1);
    }

    // You need to modify the code below to implement:
    //
    // add firstURL to the ToDo list
    // initialise Graph to hold up to maxURLs
    // initialise set of Seen URLs
    // while (ToDo list not empty and Graph not filled) {
    //    grab Next URL from ToDo list
    //    if (not allowed) continue
    //    foreach line in the opened URL {
    //       foreach URL on that line {
    //          if (Graph not filled or both URLs in Graph)
    //             add an edge from Next to this URL
    //          if (this URL not Seen already) {
    //             add it to the Seen set
    //             add it to the ToDo list
    //          }
    //       }
    //    }
    //    close the opened URL
    //    sleep(1)
    // }

    Graph g = newGraph(maxURLs);


    Stack S = newStack();
    Set s = newSet();

    // Queue stores ints, but we can store string pointers then!
    pushOnto(S, firstURL);

    while (!emptyStack(S) && nElems(s) < maxURLs) {
        char *link = popFrom(S);

//        if (isElem(s, link)) continue;

//		if (not allowed) continue;

        if (!(handle = url_fopen(link, "r"))) {
            fprintf(stderr, "Couldn't open %s\n", next);
            exit(1);
        }
        while (!url_feof(handle)) {
            url_fgets(buffer, sizeof(buffer), handle);
            //fputs(buffer,stdout);
            int pos = 0;
            char result[BUFSIZE];
            memset(result, 0, BUFSIZE);
            while ((pos = GetNextURL(buffer, link, result, pos)) > 0) {

                // Challenge A: Filter out non-html links...
                // Sure.. just check that the end of the string is .html
                // Because that's 'surely' a foolproof method!...
                int isHTML;
                char *suffix = ".html";
                isHTML = strcmp(result + strlen(result) - strlen(suffix), suffix) == 0;

                if (isHTML) {
                    if (nVertices(g) < maxURLs) {//|| (isElem(s, link) && isElem(s, result))) {

                        if (!isConnected(g, link, result)) {
                            addEdge(g, link, result);

                        }
                    }

                    if (!isElem(s, result)) {
                        printf("Found: '%s'\n", result);
                        insertInto(s, result);
                        pushOnto(S, result);
                    }
                }

                memset(result, 0, BUFSIZE);
            }
        }
        url_fclose(handle);
        sleep(1);
    }
    return 0;
}

// setFirstURL(Base,First)
// - sets a "normalised" version of Base as First
// - modifies Base to a "normalised" version of itself
void setFirstURL(char *base, char *first) {
    char *c;
    if ((c = strstr(base, "/index.html")) != NULL) {
        strcpy(first, base);
        *c = '\0';
    } else if (base[strlen(base) - 1] == '/') {
        strcpy(first, base);
        strcat(first, "index.html");
        base[strlen(base) - 1] = '\0';
    } else {
        strcpy(first, base);
        strcat(first, "/index.html");
    }
}
