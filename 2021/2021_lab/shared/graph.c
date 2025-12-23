#include "graph.h"
#include "vector.h"

// C/C++ headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
//////////                          NODE                              ////////// 
////////////////////////////////////////////////////////////////////////////////
typedef struct node {
  const char* name;
  vector_t* edges;
} node;

static node_t* NodeNew(const char* name)
{
  node_t* ret = (node_t*) malloc(sizeof(node_t));
  ret->name = strdup(name);
  ret->edges = vector_new();

  return ret;
}

const char* NodeGetName(node_t* node)
{
  return node->name;
}

static void NodeFree(const node_t* to_free)
{
  free((void*)to_free->name);
  free((void*)to_free);
}

static void NodeAddEdge(node_t* node, edge_t* edge)
{
  vector_push_back(node->edges, edge);
}

////////////////////////////////////////////////////////////////////////////////
//////////                          EDGE                              ////////// 
////////////////////////////////////////////////////////////////////////////////
typedef struct edge {
  node_t* start;
  node_t *end;
  unsigned weight;
} edge;

static edge_t* EdgeNew(node_t* start, node_t* end, unsigned weight)
{
  edge_t* ret = (edge_t*) malloc(sizeof(edge_t));
  ret->weight = weight;
  NodeAddEdge(start, ret);
  ret->start = start;
  NodeAddEdge(end, ret);
  ret->end = end;

  return ret;
}

static void EdgeFree(const edge_t* to_free)
{
  free((void*)to_free);
}

////////////////////////////////////////////////////////////////////////////////
//////////                          GRAPH                             ////////// 
////////////////////////////////////////////////////////////////////////////////
typedef struct graph {
  vector_t* edges;
  vector_t* nodes;
} graph;

graph_t* GraphNew()
{
  graph_t* ret = (graph_t*) malloc(sizeof(graph_t));
  ret->edges = vector_new();
  ret->nodes = vector_new();
  return ret;
}

void GraphFree(const graph_t* to_free)
{
  for (size_t iii = 0; iii < vector_size(to_free->edges); iii++)
  {
    edge_t* edge = (edge_t*)vector_get(to_free->edges, iii);
    EdgeFree(edge);
  }

  for (size_t iii = 0; iii < vector_size(to_free->nodes); iii++)
  {
    node_t* node = (node_t*)vector_get(to_free->nodes, iii);
    NodeFree(node);
  }

  free((void*)to_free);
}

node_t* GraphAddNode(graph_t* graph, const char* name)
{
  node_t* node = NodeNew(name);
  vector_push_back(graph->nodes, node);
  return node;
}

edge_t* GraphAddEdge(graph_t* graph, node_t* start, node_t* end, unsigned weight)
{
  edge_t* edge = EdgeNew(start, end, weight);
  vector_push_back(graph->edges, edge);
  return edge;
}

////////////////////////////////////////////////////////////////////////////////
//////////                          ALGOS                             ////////// 
////////////////////////////////////////////////////////////////////////////////
unsigned GraphCalculateNumberOfRoutesFromNode(node_t* start, unsigned steps)
{
  if (steps == 0)
    return 1;

  unsigned ret = 0;
  for (size_t iii = 0; iii < vector_size(start->edges); iii++)
  {
    edge_t* edge = (edge_t*)vector_get(start->edges, iii);
    // Get the node on the other end of the edge
    node_t* end = (edge->start == start) ? edge->end : edge->start;

    // Calculate the number of routes from that node, multiply by number of routes *to* node
    ret += edge->weight * GraphCalculateNumberOfRoutesFromNode(end, steps -1);
  }
  return ret;
}
