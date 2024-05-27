#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void isprime(int input){
    int i;
    for(i=2;i<input/2;i++){
        if(input%i==0)
            return;
    }
    printf("%d is prime\n",input);
}
int main(){
    int max = 9999999;
    int i;
    #pragma omp parallel
    {
        for(i=omp_get_thread_num(); i<max; i+=omp_get_num_threads()){
            isprime(i);
        }
        #pragma omp barrier
        printf("Thread %d has finished calculations\n",omp_get_thread_num());
    }
    return 0;
}
