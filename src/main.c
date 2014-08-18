#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "force-atlas-2.h"
#include "gexfparser.h"
#include "graph.h"
#include "timer.h"
#include "vector.h"
#include "vector-smoothening.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
  srand(time(NULL ));

  // Input parsing.
  const char* inputFile = NULL;
  unsigned int numTicks = 100;

  for (int i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i], "-i"))
    {
      // Input file param.
      inputFile = argv[++i];
    }
    else if (!strcmp(argv[i], "-n"))
    {
      numTicks = atoi(argv[++i]);
    }
    else
    {
      printf("Unrecognized parameter: %s.\n", argv[i]);
      // exit(EXIT_FAILURE);
    }
  }

  // Input checking.
  if (!inputFile)
  {
    printf("No input file specified. Exit.\n");
    exit(EXIT_FAILURE);
  }

  Graph* graph = (Graph*) calloc(1, sizeof(Graph));

  unsigned int numgraphs = 1;

  Vertices* vertices = gexfParseFileVertices(inputFile);
  graph->vertices = vertices;
  size_t edgesLength;
  Edges** edges = gexfParseFileEdgesAtSteps(inputFile, graph, 0, 199,
      &edgesLength);

  float* averageSpeedX = NULL;
  float* averageSpeedY = NULL;

  float* smoothSpeedX = NULL;
  float* smoothSpeedY = NULL;

  // Computing.
  Timer timer;
  startTimer(&timer);
  fa2RunOnGraphInStream(graph->vertices, edges, numgraphs, numTicks,
      &averageSpeedX, &averageSpeedY);
  stopTimer(&timer);
  //printf("time: total.\n");
  //printTimer(&timer);

  unsigned int* smootheningEdges;
  unsigned int* smootheningNumEdges;
  vectorSmootheningPrepareEdges(edges[edgesLength - 1]->edgeTargets,
      edges[edgesLength - 1]->numedges,
      edges[edgesLength - 1]->maxedges * graph->vertices->numvertices,
      graph->vertices->numvertices, &smootheningEdges, &smootheningNumEdges);
  vectorSmootheningPrepareOutput(&smoothSpeedX, &smoothSpeedY,
      graph->vertices->numvertices);
  vectorSmootheningRun(averageSpeedX, averageSpeedY,
      graph->vertices->numvertices, smootheningNumEdges, smootheningEdges, 10,
      0.5, smoothSpeedX, smoothSpeedY);
  vectorSmootheningCleanEdges(smootheningEdges, smootheningNumEdges);

  // Printing
  printGraph(graph);
  freeGraph(graph);
}

