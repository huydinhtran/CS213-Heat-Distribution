#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int round_out(float n)
{
    // Smaller multiple
    int a = (int)(n / 10) * 10;
     
    // Larger multiple
    int b = (int)(a + 10);
 
    // Return of closest of two
    return (n - a > b - n)? b : a;
}

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
            h[i][j][0] = 0.0;
            h[i][j][1] = 0.0;
        }
    }

    for(i = 0; i < n_size+1; i++){
        h[i][0][0] = 20.0;
        h[i][0][1] = 20.0;
        h[n_size][i][0] = 20.0;
        h[n_size][i][1] = 20.0;
        h[0][i][0] = 20.0;
        h[0][i][1] = 20.0;
        h[i][n_size][0] = 20.0;
        h[i][n_size][1] = 20.0;
    }

    for(j = 0; j < n_size+1; j++){
        if(  (j >= (n_size * 0.3)) && (j < (n_size * 0.7))  ){
            h[0][j][0] = 100.0;
            h[0][j][1] = 100.0;
        }
    }

    //algorithm
    int threadcount = 16;
    printf("Thread count: %d\n",threadcount);
    start = omp_get_wtime();

    for (iter = 0; iter < input_iter; iter++){
        #pragma omp parallel for shared(h) private(i,j) num_threads(threadcount) collapse(2) schedule(guided)
            for(i = 1; i < n_size; i++){
                for(j = 1; j < n_size; j++){
                    h[i][j][(iter+1)%2] = 0.25 * (  h[i-1][j][iter%2] + h[i+1][j][iter%2] + h[i][j-1][iter%2] + h[i][j+1][iter%2]  );
                }
            }
    }
    
    end = omp_get_wtime();
    printf("Time of computation: %f seconds\n", end-start);

    int p = (int)(n_size / 8);
    for (i = 0; i < n_size; i += p){
        for (j = 0; j < n_size; j += p){
            printf("%lf ", h[i][j][input_iter%2]);
        }
        printf("\n");
    }

    // // Print whole matrix
    // for (i = 0; i < n_size+1; i ++){
    //     for (j = 0; j < n_size+1; j ++){
    //         printf("%lf ", h[i][j][input_iter%2]);
    //     }
    //     printf("\n");
    // }

    // Write rounded output into text file for graphing
    FILE *fwrite;
    fwrite = fopen ("heatpar_output.txt", "w");
    for(i = 0; i < n_size+1; i++){
        for(j = 0; j < n_size+1; j++){
            fprintf(fwrite, "%d ", round_out(h[i][j][(input_iter+1)%2]));
        }
        fprintf(fwrite, "\n");
    }

    return(0);
}