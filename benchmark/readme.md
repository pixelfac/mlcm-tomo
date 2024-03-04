# Benchmark Guide

## Tigre
The timing analysis of Tigre is done in the cpp file that invokes the cuda kernel.
 - Timing is started after the cudaMallocs
 - Timing is ended before the cudaFrees
 - Timing includes the cudaMemCpy since that happens in the loop of running execution.

## Astra
The timing analysis of Astra is done when you call the "astra_mex_algorithm()" function with the 'run' parameter.