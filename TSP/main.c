#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char *argv[])
{
    matrix distances;
    if (argc != 2){
        fprintf(stderr, "Wrong arguments!");
        return 1;
    }
    
    if(read_distances(&distances, argv) == 0){
        route min;
        min.distance = 999;
        int visited[distances.number_of_cities];
        memset(visited, 0, distances.number_of_cities*sizeof(int));
        search(0, 0, 1, visited, &min, &distances);
        printf("Beste rondreis heeft lengte %i\n", min.distance);
        /*Clean up*/
        destroy_matrix(&distances);
    } else
    {
        return 1;
    }
    
    return (EXIT_SUCCESS);
}