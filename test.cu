#include <stdio.h>
#include <cuda_runtime.h>

__global__ void cuda_hello(){
    int i = threadIdx.x;
    printf("Hello World from GPU! %i\n", i);
}

int main() {
    cuda_hello<<<2,5>>>(); 
    return 0;
}