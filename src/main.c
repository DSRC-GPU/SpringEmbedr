
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "force-atlas-2.h"
#include "gexfparser.h"
#include "graph.h"
#include "vector.h"

int main(int argc, char* argv[])
{
  srand(time(NULL));

  // Input parsing.
  const char* inputFile = NULL;
  unsigned int numTicks = 100;
  int runForever = 0;

  for (int i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i], "-i"))
    {
      // Input file param.
      inputFile = argv[++i];
    }
    else if (!strcmp(argv[i], "-I"))
    {
      runForever = 1;
    }
    else if (!strcmp(argv[i], "-n"))
    {
      numTicks = atoi(argv[++i]);
    }
    else
    {
      printf("Unrecognized parameter: %s.\n", argv[i]);
   //   exit(EXIT_FAILURE);
    }
  }

  // Input checking.
  if (!inputFile)
  {
    printf("No input file specified. Exit.\n");
    exit(EXIT_FAILURE);
  }

  // Feedback to user.
  Graph* g = (Graph*) calloc(1, sizeof(Graph));
  gexfParseFile(g, inputFile);

  // Printing
  //printGraph(g);

  // Computing.
  fa2RunOnGraph(g, numTicks);

  // Printing
  //printGraph(g);

  free(g);
}

