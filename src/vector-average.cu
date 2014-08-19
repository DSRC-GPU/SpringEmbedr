
#include "vector-average.h"
#include "util.h"

float* vectorAverageNewVectorArray(unsigned int numelements)
{
  float* array = NULL;
  cudaMalloc(&array, 2 * numelements * sizeof(float));
  return array;
}

void vectorAverageFreeVectorArray(float* averageArray)
{
  cudaFree(averageArray);
}

float** vectorAverageNewWindow()
{
  return (float**) calloc(WINDOW_SIZE, sizeof(float*));
}

void vectorAverageFreeWindow(float** window)
{
  for (size_t i = 0; i < WINDOW_SIZE; i++)
  {
    cudaFree(window[i]);
  }
  free(window);
}

void vectorAverageShiftAndAdd(float** window, float* newEntry)
{
  cudaFree(window[0]);
  for (size_t i = 1; i < WINDOW_SIZE; i++)
  {
    window[i - 1] = window[i];
  }
  window[WINDOW_SIZE - 1] = newEntry;
}

void vectorAverageComputeAverage(float** window, unsigned int numelements,
    float* average)
{
  utilVectorSetByScalar(average, 0, numelements); 
  for (size_t i = 0; i < WINDOW_SIZE; i++)
  {
    utilVectorAdd(average, window[i], numelements);
  }
  utilVectorDevideByScalar(average, WINDOW_SIZE, numelements);
}

