#ifndef GRAPH_H
#define GRAPH_H

// Forward declarations of types
typedef struct edge edge_t;
typedef struct node node_t;
typedef struct graph graph_t;

// Constructors/Destructors
graph_t* GraphNew();
void GraphFree(const graph_t* to_free);

// Functions to fill in graph
node_t* GraphAddNode(graph_t* graph, const char* name);
edge_t* GraphAddEdge(graph_t* graph, node_t* start, node_t* end, unsigned weight);
const char* NodeGetName(node_t* node);

// Functions to do calculations on graph
unsigned GraphCalculateNumberOfRoutesFromNode(node_t* start, unsigned steps);

#endif
