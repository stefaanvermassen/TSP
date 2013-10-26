//
//  branch_and_bound.c
//  TSP
//
//  Created by Stefaan Vermassen on 2/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "branch_and_bound.h"

void search(int city, int weight, travel *current, int visited, route *min, matrix *weights, best_solution* best, int* b_nr, int p_id)
{
    int i,j, mpi_test_value;
    int mpi_rec_value = best->distance;
    MPI_Status status;
    MPI_Request request;
    MPI_Irecv(&mpi_rec_value, best->number_of_processes, MPI_INT, MPI_ANY_SOURCE, 50, MPI_COMM_WORLD, &request);
    MPI_Test(&request, &mpi_test_value, &status);
    current->route_points[visited-1] = city;
    if(visited == weights->number_of_cities)
    {
        if(weight+weights->data[city][0] < min->distance)
        {
            min->distance = weight+weights->data[city][0];
            for(j=0; j<=weights->number_of_cities; j++){
                min->route_points[j] = current->route_points[j];
            }
            if(mpi_test_value && mpi_rec_value > min->distance)
            {
                MPI_Bcast((void*) &best->distance, 1, MPI_INT, p_id, MPI_COMM_WORLD);
            }
        }
    }else
    {
        current->visited[city] = 1;
        current->route_points[visited-1] = city;
        if(above_splitlevel(best, visited) || weight + (weights->number_of_cities - visited)*weights->smallest_distance < min->distance)
        {
            for(i=1; i<weights->number_of_cities; i++){
                if(!current->visited[i]){
                    if(!on_splitlevel(best, visited) || p_id==(*b_nr%best->number_of_processes))
                    {
                     
                        if(mpi_rec_value != 0)
                        {
                            printf("Received%i\n", mpi_rec_value);
                            if(mpi_rec_value>min->distance)
                            {
                                search(i, weight+weights->data[city][i],current, visited+1, min, weights,best, b_nr,p_id);
                            }
                        } else
                        {
                            search(i, weight+weights->data[city][i],current, visited+1, min, weights, best, b_nr,p_id);
                        }
                    }
                }
                if(on_splitlevel(best, visited))
                {
                    (*b_nr)++;
                }
            }
        }
        current->visited[city]=0;
    }
}

void init_travel(route *min, travel *current, matrix *weights)
{
    min->distance = INT_MAX;
    min->route_points = (int*) calloc(weights->number_of_cities+1, sizeof(int));
    current->visited = (int*) calloc(weights->number_of_cities, sizeof(int));
    current->route_points = (int*) calloc(weights->number_of_cities+1, sizeof(int));
}

void destroy_travel(route *min, travel *current){
    free(current->route_points);
    free(min->route_points);
    free(current->visited);
}

void search_solution(matrix* distances, best_solution* best, int p_id, int p_total)
{
    int i;
    route min;
    travel current;
    init_travel(&min, &current, distances);
    int b_nr=-1;
    best->number_of_processes = p_total;
    best->splitlevel=2;
    search(0, 0, &current, 1, &min, distances, best, &b_nr, p_id);
    printf("p_id:%i, distance:%i\n", p_id, min.distance);
    for(i=0; i<distances->number_of_cities; i++)
    {
        printf("%i",min.route_points[i]);
    }
    printf("\n");
    destroy_travel(&min, &current);
    
}

int above_splitlevel(best_solution* t, int visited)
{
    return visited < t->splitlevel;
}
int on_splitlevel(best_solution* t, int visited)
{
    //printf("%i,%i\n", visited, t->splitlevel);
    return visited == t->splitlevel;
}
int distance_from_splitlevel(best_solution* t, int visited)
{
    return t->splitlevel - visited;
}