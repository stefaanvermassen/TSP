#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int number_of_cities, smallest_distance, current_min_door;
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
    if(p_total > 1)
    {
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
                MPI_Send(distances.min_door, distances.number_of_cities, MPI_INT, i, TAG_MIN_DOOR, MPI_COMM_WORLD);
                MPI_Send(&(distances.current_min_door), 1, MPI_INT, i, TAG_CURRENT_MIN_DOOR, MPI_COMM_WORLD);
            }
            
        } else
        {
            MPI_Recv(&number_of_cities, 1, MPI_INT, 0, TAG_SIZE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            dist = (matrix*) malloc (sizeof(matrix));
            dist->data = (int **)malloc(number_of_cities*sizeof(int*));
            dist->min_door = (int*)malloc(number_of_cities*sizeof(int));
            for(i=0; i<number_of_cities; i++){
                dist->data[i] = (int *)malloc(number_of_cities*sizeof(int));
            }
            dist->number_of_cities =number_of_cities;
            for(i=0; i<dist->number_of_cities; i++)
            {
                MPI_Recv(dist->data[i], number_of_cities, MPI_INT, 0, TAG_DATA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            MPI_Recv(&smallest_distance, 1, MPI_INT, 0, TAG_SMALLEST, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(dist->min_door, number_of_cities, MPI_INT, 0, TAG_MIN_DOOR, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&current_min_door, 1, MPI_INT, 0, TAG_CURRENT_MIN_DOOR, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            dist->current_min_door=current_min_door;
            dist->smallest_distance = smallest_distance;
            distances.number_of_cities = dist->number_of_cities;
            distances.data = dist->data;
            distances.smallest_distance = dist->smallest_distance;
            distances.min_door = dist->min_door;
            distances.current_min_door = dist->current_min_door;
        }
        
        init_solution(&best, &distances, p_total-1, 1);
        //Use the last process for greedy heuristic
        if(p_id != p_total-1)
        {
            perform_branch_and_bound(&distances, &best, p_id);
        }else
        {
            perform_greedy(&distances, &best, p_id);
        }
        destroy_distance_matrice(dist, p_id);
        
    } else
    {
        read_distances(&distances, argv);
        init_solution(&best, &distances, p_total, 1);
        perform_branch_and_bound(&distances, &best, p_id);
    }
    destroy_solution(&best, &distances);
    destroy_matrix(&distances,p_id);
    MPI_Finalize();
    return (EXIT_SUCCESS);
}

void perform_branch_and_bound(matrix* distances, best_solution* best, int p_id)
{
    search_solution(distances, best, p_id);
}

void perform_greedy(matrix* distances, best_solution* best, int p_id)
{
    int i;
    MPI_Request request;
    //Use greedy algorithm as a base for the simulated annealing algorithm
    //best->greedy_route is the best route found by greedy algorithm
    search_greedy_solution(distances, best, p_id);
    if(best->greedy_distance<best->distance) best->distance = best->greedy_distance;
    for(i=0; i<best->number_of_processes-1; i++)
    {
        MPI_Isend(&best->greedy_distance, 1, MPI_INT, i, TAG_BOUND, MPI_COMM_WORLD, &request);
    }
    simulated_annealing_search(best, distances);
    for(i=0; i<best->number_of_processes-1; i++)
    {
        MPI_Isend(&best->greedy_distance, 1, MPI_INT, i, TAG_BOUND, MPI_COMM_WORLD, &request);
    }
    if(best->greedy_distance<best->distance) best->distance = best->greedy_distance;
    tabu_search(best, distances);
}

void destroy_distance_matrice(matrix* distances, int p_id)
{
    if(p_id != 0)
    {
        free(distances);
    }
}