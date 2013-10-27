#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char *argv[])
{
    matrix distances;
    if (argc != 2){
        fprintf(stderr, "Wrong arguments!\n");
        return 1;
    }
    
    if(read_distances(&distances, argv) == 0){
        int p_id, p_total;
        MPI_Init (&argc, &argv);
        MPI_Comm_rank (MPI_COMM_WORLD, &p_id);
        MPI_Comm_size(MPI_COMM_WORLD, &p_total);
        /*Clean up*/
        perform_branch_and_bound(&distances, p_id, p_total);
        //perform_greedy(&distances, p_id);
        
        destroy_matrix(&distances);
        MPI_Finalize();
    } else
    {
        return 1;
    }
    
    return (EXIT_SUCCESS);
}

void perform_branch_and_bound(matrix* distances, int p_id, int p_total)
{
    best_solution best;
    search_solution(distances, &best, p_id, p_total);
}

void perform_greedy(matrix* distances, int p_id)
{
    search_greedy_solution(distances, p_id);
}