#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    argc == 3;

    double start, end; // used for timing

    int n_size, input_iter, iter, i, j;
    n_size = atoi(argv[1]);
    input_iter = atoi(argv[2]);

    printf("N size: %d\n",n_size);
    printf("Iterations: %d\n",input_iter);

    double ***h = (double***)malloc((n_size+1)*sizeof(double**));
    
    for (i = 0; i < n_size+1; i++) {
        h[i] = (double**)malloc((n_size+1)*sizeof(double*));
        for (j = 0; j < n_size+1; j++) {
            h[i][j] = (double*)malloc(2*sizeof(double));
        }
    }

    //initializing
    for(i = 0; i < n_size+1; i++){
        for(j = 0; j < n_size+1; j++){
            h[i][j][0] = 20.0;
            h[i][j][1] = 20.0;
        }
    }

    for(j = 0; j < n_size; j++){
        if(  (j >= (n_size * 0.3)) && (j < (n_size * 0.7))  ){
            h[0][j][0] = 100.0;
            h[0][j][1] = 100.0;
        }
    }

    //algorithm
    int threadcount = 4;
    printf("Thread count: %d\n",threadcount);
    start = omp_get_wtime();

    
    for (iter = 0; iter < input_iter; iter++){
        #pragma omp parallel for shared(h) private(i,j) num_threads(threadcount) collapse(2) schedule(guided)
            for(i = 1; i < n_size-1; i++){
                for(j = 1; j < n_size-1; j++){
                    h[i][j][(iter+1)%2] = 0.25 * (  h[i-1][j][iter%2] + h[i+1][j][iter%2] + h[i][j-1][iter%2] + h[i][j+1][iter%2]  );
                }
            }
    }
        
    end = omp_get_wtime();
    printf("Time of computation: %f seconds\n", end-start);

    // for(i = 0; i < n_size; i++){
    //     for(j = 0; j < n_size; j++){
    //         printf("%f ", h[i][j][(input_iter+1)%2]);
    //     }
    //     printf("\n");
    // }

    FILE *fwrite;
    fwrite = fopen ("heatpar_output.txt", "w");
    for(i = 0; i < n_size; i++){
        for(j = 0; j < n_size; j++){
            fprintf(fwrite, "%f ", h[i][j][(input_iter+1)%2]);
        }
        fprintf(fwrite, "\n");
    }

    return(0);
}