#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

int main(int argc, char *argv[])
{
    matrix distances;
    if (argc != 2){
        fprintf(stderr, "Wrong arguments");
        return 1;
    }
    
    if(read_distances(&distances, argv) != 0){
        
        
        /*Clean up*/
        destroy_matrix(&distances);
    } else
    {
        return 1;
    }
    
    return (EXIT_SUCCESS);
}