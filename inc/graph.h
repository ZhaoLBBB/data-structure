#ifndef _GRAPH_H_
#define _GRAPH_H_
#if 0
#include <stdbool.h>
#include "utils.h"

typedef struct states states_t;

typedef struct associative_array {
  int key;
  int value;
} associative_array_t;

typedef struct adjacency_list_node {  // Represents edges between nodes
  int vertex;  // Other end of the edge
  double weight;  // Weight of the edge
  struct adjacency_list_node *next;  // Pointer to next item in the adjacency linked list
} adjacency_list_node_t;

typedef struct adjacency_list {  // Represents a node
  int nb_members;  // Number of members in the adjacency list
  adjacency_list_node_t *head;  // Head of the adjacency linked list
} adjacency_list_t;

typedef struct graph {
  int nb_vertices;  // Number of vertices
  adjacency_list_t *adjacency_list_array;  // An array of adjacency lists
} graph_t;

graph_t *createGraph(int n);
void freeGraph(graph_t *graph);
int *getNodes(graph_t const *graph);
void addEdge(graph_t *graph, int src, int dest, double weight);
void printNode(graph_t const *graph, int n);
void printGraph(graph_t const *graph);

graph_t *reverseGraph(graph_t const *graph);
void dfs (graph_t const *graph, int *d, int *pi, associative_array_t *f, int const *nodes);
bool isConnected(graph_t const *graph);


typedef enum state {
    NOT_VISITED,
    VISITING,
    VISITED
} state_t;

typedef struct states {
    int nb_members;
    state_t *states;
} states_t;


states_t *initStates(graph_t const *graph);
void freeStates(states_t *states);
void markNode(states_t *states, int v, state_t state);
int isState(states_t const *states, int v, state_t state);
int someIsState(states_t const *states, state_t state);
#endif

#endif  // _GRAPH_H_
