////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 -- Graphs!
//
// 2016-09-18	Jashank Jeremy <jashankj@cse.unsw.edu.au>
// 2018-12-21	Jashank Jeremy <jashankj@cse.unsw.edu.au>

// This ADT corresponds to the mathematical notion of a graph: a
// collection of vertices and edges.  This ADT aims to be as complete
// as possible.

#include <stdbool.h>
#include <stdlib.h>

#include "item.h"

#ifndef CS2521__GRAPH_H_
#define CS2521__GRAPH_H_

typedef struct graph *Graph;
typedef int vertex;
typedef int weight;
typedef struct edge {
	vertex v;
	vertex w;
	weight n;
} edge;

/**
 * Create a new instance of a Graph.
 *
 * @param max_edges	a hint for the maximum number of edges
 * @param max_vertices a hint for the maximum number of vertices
 * @param is_directed is this graph directed?
 * @param is_weighted is this graph weighted?
 * @returns the new Graph
 */
Graph graph_new (
	size_t max_edges,
	size_t max_vertices,
	bool is_directed,
	bool is_weighted);

/**
 * Deallocate resources in use by a graph.
 *
 * @param g the graph
 */
void graph_drop (Graph g);

/**
 * Return the number of vertices in the graph.
 *
 * @param g the graph
 * @returns the number of vertices in g
 */
size_t graph_num_vertices (Graph g);

/**
 * Return the number of edges in the graph.
 *
 * @param g the graph
 * @returns the number of edges in g.
 */
size_t graph_num_edges (Graph g);

/**
 * Is the graph a directed graph?
 *
 * @param g the graph
 * @returns whether g is a directed graph
 */
bool graph_directed_p (Graph g);


/**
 * Is the graph a weighted graph?
 *
 * @param g the graph
 * @returns whether g is a weighted graph
 */
bool graph_weighted_p (Graph g);


/**
 * Add a vertex with index `v` to the Graph.
 * If the vertex already exists, a no-op returning false.
 *
 * @param g	the graph
 * @param v	the vertex index to add
 * @returns whether `v` was added
 */
bool graph_vertex_add (Graph g, vertex v);

/**
 * Add an edge `e`, from vertex `v` to `w`, to the Graph.
 * If the graph is weighted, added with weight `n`.
 * If the edge already exists, a no-op returning false.
 *
 * @param g	the graph
 * @param e	the edge to add
 * @returns whether `e` was added
 */
bool graph_edge_add (Graph g, edge e);

/**
 * Remove edge `e` between `v` and `w` with weight `n` from the Graph.
 *
 * @param g	the graph
 * @param e	the edge to remove
 */
void graph_edge_remove (Graph g, edge e);

/**
 * Remove vertex `v` from the Graph.
 *
 * @param g	the graph
 * @param v	the vertex to remove
 */
void graph_vertex_remove (Graph g, vertex v);


/**
 * Is there a vertex that matches the vertex specified?
 *
 * @param g the graph
 * @param v the vertex
 * @returns whether v exists in g
 */
bool graph_has_vertex_p (Graph g, vertex v);

/**
 * Return the degree of a vertex.
 *
 * @param g the graph
 * @param v the vertex
 * @returns the number of edges incident with that vertex.
 */
size_t graph_vertex_degree (Graph g, vertex v);

/**
 * Is there an edge that matches the edge specified?
 *
 * In a weighted graph, the weight must also match.  In a directed
 * graph, the vertices must be connected by this edge in the order
 * specified.
 *
 * @param g the graph
 * @param e the edge
 * @returns whether e exists in g
 */
bool graph_has_edge_p (Graph g, edge e);


/**
 * Return the adjacency matrix of the graph.
 *
 * An adjacency matrix is a square matrix that has, in each cell,
 * whether a connection exists and the weight of the connection in a
 * weighted graph.
 *
 * @param g the graph
 * @returns the $VxV$ adjacency matrix
 */
weight **graph_adjacency_matrix (Graph g);

/**
 * Return a list of all edges in the graph.
 *
 * @param	g	the graph
 * @param[out] 	n_edges	the number of edges
 * @returns the edge list.
 */
edge *graph_edge_list (Graph g, size_t *n_edges);


/**
 * Perform a breadth-first search on a graph.
 *
 * A breadth-first search takes the following pattern:
 *
 *     enqueue a starting edge
 *     until the queue is empty:
 *         dequeue an edge to a next vertex;
 *         if we've already seen the vertex, skip;
 *         otherwise, enqueue its edges
 *
 * @param g	the graph to search on
 * @param v	the vertex to start with
 * @param[out] count	the number of vertices traversed
 * @param[out] pre	the pre-order in which vertices were visited
 * @param[out] st	the spanning tree resulting from this search
 */
void graph_bfs (
	Graph g, vertex v,
	size_t *count, vertex pre[], vertex st[]);

/**
 * Perform a depth-first search on a graph.
 *
 * A depth-first search takes the following pattern:
 *
 *     push a starting edge
 *     until the stack is empty:
 *         pop an edge to a next vertex;
 *         if we've already seen the vertex, skip;
 *         otherwise, push its edges
 *
 * @param g	the graph to search on
 * @param v	the vertex to start with
 * @param[out] count	the number of vertices traversed
 * @param[out] pre	the pre-order in which vertices were visited
 * @param[out] st	the spanning tree resulting from this search
 */
void graph_dfs (
	Graph g, vertex v,
	size_t *count, vertex pre[], vertex st[]);

/**
 * Execute Kruskal's algorithm to reduce a graph to its minimum
 * spanning tree; we return the resultant tree as a Graph.
 *
 * Kruskal's algorithm is:
 *
 *     create a new MST
 *     take a list of g's edges, el
 *     sort el by weight, ascending
 *     while the edge count of MST is less than the vertex count in g:
 *         add the next edge in el to MST
 *         if there now exists a cycle:
 *             remove the edge we just added
 *
 * @param g the Graph to reduce
 * @returns a new Graph, containing the MST
 * @see graph_mst_prim
 * @see graph_mst_sedgewick
 */
Graph graph_mst_kruskal (Graph g);

/**
 * Execute the Prim's (aka Jarnik's, Dijkstra's) algorithm
 * to reduce a graph to its minimum spanning tree.
 * We return the resultant tree as a Graph.
 *
 * @param g the Graph to reduce
 * @returns a new Graph, containing the MST
 * @see graph_mst_kruskal
 * @see graph_mst_sedgewick
 */
Graph graph_mst_prim (Graph g);

/**
 * Execute Sedgewick's MST algorithm to reduce a graph to its minimum
 * spanning tree; we return the resultant tree as a Graph.
 *
 * @param g the Graph to reduce
 * @returns a new Graph, containing the MST
 * @see graph_mst_kruskal
 * @see graph_mst_prim
 */
Graph graph_mst_sedgewick (Graph g);

/**
 * Perform a shortest-path search from a particular vertex.
 * On an unweighted graph, uses the number of vertices as the priority.
 *
 * A single-source shortest-path search takes the following pattern:
 *
 *     enqueue a starting edge
 *     until the pqueue is empty:
 *         dequeue an edge to a next vertex;
 *         if we've already seen the vertex,
 *             and the known distance to that vertex
 *             is less than that via this edge, skip
 *         otherwise, push its edges
 *
 * @param g	the graph to search on
 * @param v	the vertex to start with
 * @param[out] count	the number of vertices traversed
 * @param[out] st	the spanning tree resulting from this search
 * @param[out] dist	the total weight to each vertex
 */
void graph_sssp_dijkstra (
	Graph g, vertex v,
	size_t *count, vertex st[], weight dist[]);


/**
 * Is this graph simple -- that is, undirected and without self-loops
 * and parallel edges?
 */
bool graph_simple_p (Graph g);

/**
 * Is this graph complete -- that is, does every vertex connect to every
 * other vertex?
 */
bool graph_complete_p (Graph);

/**
 * Is this graph connected -- that is, does every vertex have a path to
 * every other vertex?
 */
bool graph_connected_p (Graph);

/**
 * Does this graph have an Euler path -- that is, given two vertices, is
 * there a path that visits every edge exactly once?  (A graph has an
 * Euler path if and only if it is connected and has exactly two
 * vertices of odd degree.)
 */
bool graph_euler_path_p (Graph);

/**
 * Is this path an Euler path?
 *
 * @param g	the graph to search on
 * @param n_edges	the number of edges in the list
 * @param edges	a list of edges that may form an Euler path
 * @returns true if the edge list forms an Euler path; false otherwise.
 */
bool graph_path_is_euler_path_p (Graph g, size_t n_edges, edge edges[]);

/**
 * Does this graph have an Euler tour -- that is, an Euler path that
 * returns to its start?  (A graph has an Euler tour if and only if it
 * is connected and all its vertices are of even degree.)
 */
bool graph_euler_tour_p (Graph);

/**
 * Is this path an Euler tour?
 *
 * @param g	the graph to search on
 * @param n_edges	the number of edges in the list
 * @param edges	a list of edges that may form an Euler tour
 * @returns true if the edge list forms an Euler tour; false otherwise.
 */
bool graph_path_is_euler_tour_p (Graph g, size_t n_edges, edge edges[]);

/**
 * Does this graph have a Hamilton path -- that is, given two vertices,
 * is there an acyclic path that visits every vertex exactly once?
 */
bool graph_hamilton_path_p (Graph);

/**
 * Is this path a Hamilton path?
 *
 * @param g	the graph to search on
 * @param n_vertices	the number of vertices in the list
 * @param vertices	a list of vertices that may form a Hamilton path
 * @returns true if the vertex list forms an Hamilton path;
 *     false otherwise.
 */
bool graph_path_is_hamilton_path_p (
	Graph g, size_t n_vertices, vertex vertices[]);

/**
 * Does this graph have a Hamilton tour -- that is, a Hamilton path
 * which returns to its start?  In practice, this is also an exponential
 * problem, but Dirac's and Ore's theorems can provide 'fast paths' for
 * certain well-defined subsets of the problem.
 */
bool graph_hamilton_tour_p (Graph);

/**
 * Is this path a Hamilton tour?
 *
 * @param g	the graph to search on
 * @param n_vertices	the number of vertices in the list
 * @param vertices	a list of vertices that may form a Hamilton tour
 * @returns true if the vertex list forms an Hamilton tour;
 *     false otherwise.
 */
bool graph_path_is_hamilton_tour_p (
	Graph g, size_t n_vertices, vertex vertices[]);

/**
 * Is this graph planar -- that is, can it be drawn without intersecting
 * edges?  (Kuratowski's theorem and Tarjan's algorithm may be useful.)
 */
bool graph_planar_p (Graph);

/**
 * Is graph `g1` isomorphic to graph `g2` -- that is, can I take a graph
 * and relabel its vertices and edges to produce the second graph?
 *
 * In practice, this is impossible in the general case.  Contradictions
 * on certain simple cases, such as those where invariant properties of
 * the graphs do not hold, can provide some 'fast paths'.
 */
bool graph_isomorphic_p (Graph g1, Graph g2);

/**
 * Is graph `g1` equal to graph `g2` -- that is, are all its vertices
 * and edges equal?
 */
bool graph_eq (Graph, Graph);

#endif /* !defined (CS2521__GRAPH_H_) */
