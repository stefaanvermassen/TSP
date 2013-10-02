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
        greedy_route greedy_min;
        travel current;
        int i;
        init_travel(&min, &current, &distances);
        search(0, 0, &current, 1, &min, &distances);
        printf("%i\n", min.distance);
        for (i=0; i<=distances.number_of_cities; i++){
            printf("%i ", min.route_points[i]);
        }
        printf("\n");
        printf("Greedy approach:\n");
        init_greedy_route(&greedy_min, &distances);
        greedy_search(&greedy_min, &distances);
        printf("%i\n", greedy_min.distance);
        for (i=0; i<=distances.number_of_cities; i++){
            printf("%i ", greedy_min.route_points[i]);
        }
        /*Clean up*/
        destroy_matrix(&distances);
        destroy_travel(&min, &current);
    } else
    {
        return 1;
    }
    
    return (EXIT_SUCCESS);
}