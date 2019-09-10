#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

/**
 * Create an empty graph with n vertices
 */
graph_t *createGraph(int n) {
  graph_t *graph = (graph_t *) malloc(sizeof(graph_t));
  if(!graph){
      fprintf(stderr, "Unable to allocate memory.\n");
      exit(-1);
  }
  graph->nb_vertices = n;

  /* Create an array of adjacency lists*/
  graph->adjacency_list_array = (adjacency_list_t *) malloc(n * sizeof(adjacency_list_t));
  if(!graph->adjacency_list_array){
      fprintf(stderr, "Unable to allocate memory.\n");
      exit(-1);
  }

  for(int i = 0; i < n; i++) {
    graph->adjacency_list_array[i].head = NULL;
    graph->adjacency_list_array[i].nb_members = 0;
  }

  return graph;
}


/**
 * Destroy the graph
 */
void freeGraph(graph_t *graph) {
  if (graph != NULL) {
    if (graph->adjacency_list_array != NULL) {
      // Free the adjacency list array
      for (int v = 0; v < graph->nb_vertices; v++) {
        adjacency_list_node_t *item = graph->adjacency_list_array[v].head;

        while (item != NULL) {
          adjacency_list_node_t *tmp = item->next;
          free(item);
          item = tmp;
        }
      }
    }
    // Free the graph
    free(graph->adjacency_list_array);
    free(graph);
  }
}


/**
 * Get the nodes in a graph.
 */
int *getNodes(graph_t const *graph) {
  int *nodes = malloc(sizeof(int) * graph->nb_vertices);
  if(!nodes){
    fprintf(stderr, "Unable to allocate memory.\n");
    exit(-1);
  }
  for (int i = 0; i < graph->nb_vertices; i++) {
    nodes[i] = i;
  }

  return nodes;
}


/**
 * Add an edge to a graph.
 */
void addEdge(graph_t *graph, int src, int dest, double weight) {
  // Create a new adjacency list node
  adjacency_list_node_t *newNode = (adjacency_list_node_t *) malloc(sizeof(adjacency_list_node_t));
    if(!newNode){
        fprintf(stderr, "Unable to allocate memory.\n");
        exit(-1);
    }
  newNode->vertex = dest;
  newNode->weight = weight;

  // Add it in place of the head of the list
  newNode->next = graph->adjacency_list_array[src].head;

  graph->adjacency_list_array[src].head = newNode;
  graph->adjacency_list_array[src].nb_members++;
}


/**
 * Pretty print
 */
void printNode(graph_t const *graph, int n) {    
  adjacency_list_node_t *adjacency_list_item = graph->adjacency_list_array[n].head;
  printf("\n%d: ", n);
  while (adjacency_list_item != NULL) {
    printf("%d (%f) -> ", adjacency_list_item->vertex, adjacency_list_item->weight);
    adjacency_list_item = adjacency_list_item->next;
  }
  printf("NULL\n");
}
void printGraph(graph_t const *graph) {
  for (int i = 0; i < graph->nb_vertices; i++) {
    printNode(graph, i);
  }
}


/**
 * Reverse a graph
 */
graph_t *reverseGraph(graph_t const *graph) {
  graph_t *reversed_graph = createGraph(graph->nb_vertices);

  for (int i = 0; i < graph->nb_vertices; i++) {
    adjacency_list_node_t *adjacency_list_item = graph->adjacency_list_array[i].head;
    while (adjacency_list_item != NULL) {
      addEdge(reversed_graph, adjacency_list_item->vertex, i, adjacency_list_item->weight);
      adjacency_list_item = adjacency_list_item->next;
    }
  }

  return reversed_graph;
}


/**
 * Do a DFS starting from specified node.
 *
 * Note: nodes param is here to specify the order of iteration in the main loop.
 */
static void dfsVisit(graph_t const *graph, int u, states_t *states, int *date, int *d, int *pi, associative_array_t *f) {
  markNode(states, u, VISITING);
  (*date)++;

  d[u] = *date;

  adjacency_list_node_t *v = graph->adjacency_list_array[u].head;
  while (v != NULL) {
    if (isState(states, v->vertex, NOT_VISITED)) {
      pi[v->vertex] = u;
      dfsVisit(graph, v->vertex, states, date, d, pi, f);
    }
    v = v->next;
  }

  markNode(states, u, VISITED);
  (*date)++;
  f[u].value = *date;
  f[u].key = u;
}
void dfs (graph_t const *graph, int *d, int *pi, associative_array_t *f, int const *nodes) {
  int date = 0;
  states_t *states = initStates(graph);  // Set all nodes to not visited

  for (int u = 0; u < graph->nb_vertices; u++) {
    pi[u] = -1;
  }

  for (int u = 0; u < graph->nb_vertices; u++) {
    if (isState(states, nodes[u], NOT_VISITED)) {
      dfsVisit(graph, nodes[u], states, &date, d, pi, f);
    }
  }

  freeStates(states);
}


/**
 * Check whether the graph is connected or not.
 */
static int compare_int_desc(void const *a, void const *b) {
   associative_array_t const *pa = a;
   associative_array_t const *pb = b;

   return pb->value - pa->value;
}
static int count(int *array, int nb_items, int value) {
  int nb = 0;

  for (int i = 0; i < nb_items; i++) {
    if (array[i] == value) {
      nb++;
    }
  }

  return nb;
}
bool isConnected(graph_t const *graph) {
  int *d = malloc(sizeof(int) * graph->nb_vertices);
  int *pi = malloc(sizeof(int) * graph->nb_vertices);
  if(!d || !pi){
      fprintf(stderr, "Unable to allocate memory.\n");
      exit(-1);
  }
  associative_array_t *f = malloc(sizeof(associative_array_t) * graph->nb_vertices);
  if(!f){
      fprintf(stderr, "Unable to allocate memory.\n");
      exit(-1);
  
  }
  int *nodes = getNodes(graph);

  dfs(graph, d, pi, f, nodes);
  free(pi);
  free(d);
  free(nodes);

  graph_t *reversed_graph = reverseGraph(graph);

  associative_array_t *fR = malloc(sizeof(associative_array_t) * graph->nb_vertices);
  if(!fR){
      fprintf(stderr, "Unable to allocate memory.\n");
      exit(-1);
  }
  d = malloc(sizeof(int) * graph->nb_vertices);
  if(!d){
        fprintf(stderr, "Unable to allocate memory.\n");
        exit(-1);
  }
  pi = malloc(sizeof(int) * graph->nb_vertices);
  if(!pi){
      fprintf(stderr, "Unable to allocate memory.\n");
      exit(-1);
  }
  qsort(f, graph->nb_vertices, sizeof(associative_array_t), compare_int_desc);

  nodes = malloc(sizeof(int) * graph->nb_vertices);
  if(!nodes){
      fprintf(stderr, "Unable to allocate memory.\n");
      exit(-1);
  }
  for (int i = 0; i < graph->nb_vertices; i++) {
    nodes[i] = f[i].key;
  }
  dfs(reversed_graph, d, pi, fR, nodes);

  freeGraph(reversed_graph);
  free(f);
  free(fR);
  free(d);
  free(nodes);

  int nb = count(pi, graph->nb_vertices, -1);
  free(pi);

  return nb == 1;
}


/**
 * Init states array.
 */
states_t *initStates(graph_t const *graph) {
    states_t *states = (states_t *) malloc(sizeof(states_t));

    if (states == NULL) {
        fprintf(stderr, "Unable to allocate memory to store states.\n");
        exit(EXIT_FAILURE);
    }

    states->nb_members = graph->nb_vertices;
    states->states = malloc(sizeof(state_t) * states->nb_members);

    if (states->states == NULL) {
        fprintf(stderr, "Unable to allocate memory to store states.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < states->nb_members; i++) {
        states->states[i] = NOT_VISITED;
    }

    return states;
}


/**
 * Free the states array.
 */
void freeStates(states_t *states) {
    free(states->states);
    free(states);
}


/**
 * Mark a node with a given state.
 */
void markNode(states_t *states, int v, state_t state) {
    states->states[v] = state;
}


/**
 * Check if the node `v` is already of given state.
 */
int isState(states_t const *states, int v, state_t state) {
    return (states->states[v] == state);
}


/**
 * Check if *a* node is already of given state.
 */
int someIsState(states_t const *states, state_t state) {
    for (int i = 0; i < states->nb_members; i++) {
        if (states->states[i] == state) {
            return true;
        }
    }
    return false;
}
