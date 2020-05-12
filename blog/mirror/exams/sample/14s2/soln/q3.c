// Graph.c ... implementation of Graph ADT

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"

// type for small +ve int values
typedef unsigned char bool;

// graph representation (adjacency matrix)
typedef struct _graph_rep {
	int    nV;    // #vertices
	int    nE;    // #edges
	bool **edges; // matrix of booleans
} GraphRep;

// validV ... check validity of Vertex
int validV(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// mkEdge ... create an Edge value
Edge mkEdge(Graph g, Vertex v, Vertex w)
{
	assert(validV(g,v) && validV(g,w));
	Edge e; e.v = v; e.w = w;
	return e;
}

// newGraph ... create an empty graph
Graph newGraph(int nV)
{
	assert(nV > 0);
	int i, j;
	bool **e = malloc(nV * sizeof(bool *));
	assert(e != NULL);
	for (i = 0; i < nV; i++) {
		e[i] = malloc(nV * sizeof(bool));
		assert(e[i] != NULL);
		for (j = 0; j < nV; j++)
			e[i][j] = 0;
	}
	Graph g = malloc(sizeof(GraphRep));
	assert(g != NULL);
	g->nV = nV;  g->nE = 0;  g->edges = e;
	return g;
}

// readGraph ... read graph contents from file
static void readError()
{
	fprintf(stderr,"Bad graph data file\n");
	exit(1);
}
Graph readGraph(FILE *in)
{
	Graph g;
	char line[100];
	// get #vertices and create graph
	int nV = 0;
	if (fgets(line,100,in) == NULL) readError();
	if (sscanf(line,"%d",&nV) != 1) readError();
	if (nV < 2) readError();
	g = newGraph(nV);
	// read edge data and add edges
	Vertex v, w;
	while (fgets(line,100,in) != NULL) {
		sscanf(line,"%d-%d",&v,&w);
		insertE(g, mkEdge(g,v,w));
	}
	return g;
}

// showGraph ... display a graph
void showGraph(Graph g)
{
	assert(g != NULL);
	printf("# vertices = %d, # edges = %d\n\n",g->nV,g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf("%d is connected to",v);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf(" %d",w);
			}
		}
		printf("\n");
	}
}

// insertE ... add a new edge
void  insertE(Graph g, Edge e)
{
	assert(g != NULL);
	assert(validV(g,e.v) && validV(g,e.w));
	if (g->edges[e.v][e.w]) return;
	g->edges[e.v][e.w] = 1;
	g->edges[e.w][e.v] = 1;
	g->nE++;
}

// removeE ... delete an edge
void  removeE(Graph g, Edge e)
{
	assert(g != NULL);
	assert(validV(g,e.v) && validV(g,e.w));
	if (!g->edges[e.v][e.w]) return;
	g->edges[e.v][e.w] = 0;
	g->edges[e.w][e.v] = 0;
	g->nE--;
}

// compare two Connects pairs
static int cmp(Connects a, Connects b)
{
	if (a.nconn < b.nconn)
		return -1;
	else if (a.nconn > b.nconn)
		return 1;
	else { // (a.conn == b.nconn)
		if (a.vertx < b.vertx)
			return 1;
		else if (a.vertx > b.vertx)
			return -1;
	}
	return 0; // never happens in this context
}

// wellConnected ... list of vertices
// - ordered on #connections, most connected first
Connects *wellConnected(Graph g, int *nconns)
{
	assert(g != NULL && nconns != NULL);
	Connects *nc = malloc(g->nV*sizeof(Connects));
	assert(nc != NULL);
	int i, j;
	for (i = 0; i < g->nV; i++) {
		nc[i].vertx = i;
		nc[i].nconn = 0;
	}
	// count connections for each vertex
	for (i = 0; i < g->nV; i++) {
		for (j = 0; j < g->nV; j++) {
			if (g->edges[i][j]) nc[i].nconn++;
		}
	}
	// sort the nc array (cheap-and-nasty bubblesort)
	int nswaps;
	for (i = 0; i < g->nV; i++) {
		nswaps = 0;
		for (j = 0; j < g->nV; j++) {
			if (cmp(nc[j],nc[j-1]) > 0) {
				Connects tmp;
				tmp = nc[j];
				nc[j] = nc[j-1];
				nc[j-1] = tmp;
				nswaps++;
			}
		}
		if (nswaps == 0) break;
	}
	// work out size of result array
	int n = 0;
	for (i = 0; i < g->nV; i++)
		if (nc[i].nconn > 1) n++;
	// set up returns
	*nconns = n;
	return nc;
}
