#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int number_of_cities, smallest_distance;
int main(int argc, char *argv[])
{
    int i, j, p_id, p_total;
    best_solution best;
    matrix distances;
    matrix * dist = NULL;
    if (argc != 2){
        fprintf(stderr, "Wrong arguments!\n");
        return 1;
    }
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &p_id);
    MPI_Comm_size(MPI_COMM_WORLD, &p_total);
    //Root process has to read the input data...
    if(p_id == 0)
    {
        read_distances(&distances, argv);
        for(i=1; i<p_total; i++)
        {
            MPI_Send(&(distances.number_of_cities), 1, MPI_INT, i, TAG_SIZE, MPI_COMM_WORLD);
            for(j=0; j<distances.number_of_cities; j++)
            {
                MPI_Send(distances.data[j], distances.number_of_cities, MPI_INT, i, TAG_DATA, MPI_COMM_WORLD);
            }
            
            MPI_Send(&distances.smallest_distance, 1, MPI_INT, i, TAG_SMALLEST, MPI_COMM_WORLD);
        }
        
    } else
    {
        MPI_Recv(&number_of_cities, 1, MPI_INT, 0, TAG_SIZE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        dist = (matrix*) malloc (sizeof(matrix));
        dist->data = (int **)malloc(number_of_cities*sizeof(int*));
        for(i=0; i<number_of_cities; i++){
            dist->data[i] = (int *)malloc(number_of_cities*sizeof(int));
        }
        dist->number_of_cities =number_of_cities;
        for(i=0; i<dist->number_of_cities; i++)
        {
            MPI_Recv(dist->data[i], number_of_cities, MPI_INT, 0, TAG_DATA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        MPI_Recv(&smallest_distance, 1, MPI_INT, 0, TAG_SMALLEST, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        dist->smallest_distance = smallest_distance;
        distances.number_of_cities = dist->number_of_cities;
        distances.data = dist->data;
        distances.smallest_distance = dist->smallest_distance;
    }
    
    /*init_solution(&best, &distances, p_total-1, 2);
    if(p_id != p_total-1)
    {
        perform_branch_and_bound(&distances, &best, p_id);
    }else
    {
        perform_greedy(&distances, &best, p_id);
    }*/
    init_solution(&best, &distances, p_total-1, 2);
    //Use the last process for greedy heuristic
    if(p_id != p_total-1)
    {
        perform_branch_and_bound(&distances, &best, p_id);
    }else
    {
        perform_greedy(&distances, &best, p_id);
    }
    //perform_branch_and_bound(&distances, &best, p_id);
    //perform_greedy(&distances, &best, p_id);
    destroy_solution(&best, &distances);
    destroy_matrix(&distances);
    destroy_distance_matrice(dist, p_id);
    MPI_Finalize();
    return (EXIT_SUCCESS);
}

void perform_branch_and_bound(matrix* distances, best_solution* best, int p_id)
{
    search_solution(distances, best, p_id);
}

void perform_greedy(matrix* distances, best_solution* best, int p_id)
{
    int i, sol;
    MPI_Request request;
    int best_greedy_solution = search_greedy_solution(distances, best, p_id, 0);
    for(i=1; i<distances->number_of_cities; i++)
    {
        sol = search_greedy_solution(distances, best, p_id, i);
        if(sol<best_greedy_solution) best_greedy_solution = sol;
    }
    if(sol<best->distance) best->distance = sol;
    for(i=0; i<best->number_of_processes-1; i++)
    {
        MPI_Isend(&best_greedy_solution, 1, MPI_INT, i, TAG_BOUND, MPI_COMM_WORLD, &request);
    }
}

void destroy_distance_matrice(matrix* distances, int p_id)
{
    if(p_id != 0)
    {
        free(distances);
    }
}