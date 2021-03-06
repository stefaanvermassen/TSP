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
#define min(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a > _b ? _b : _a; })

int mpi_rec_value;
int mpi_test_value;

void search(int city, int weight, travel *current, int visited, route *min, matrix *weights, best_solution* best, int* b_nr, int p_id, MPI_Request bound_request)
{
    int i,j,z;
    
    current->route_points[visited-1] = city;
    if(visited == weights->number_of_cities)
    {
        if(weight+weights->data[city][0] < min->distance)
        {
            min->distance = weight+weights->data[city][0];
            for(j=0; j<=weights->number_of_cities; j++){
                min->route_points[j] = current->route_points[j];
            }
            if(min->distance<=mpi_rec_value)
            {
                for(z=0; z<best->number_of_processes; z++)
                {
                    MPI_Request send_request;
                    int dist = min(min->distance, best->distance);
                    MPI_Isend(&dist, 1, MPI_INT, z, TAG_BOUND, MPI_COMM_WORLD, &send_request);
                }
            }
        }
    }else
    {
        current->visited[city] = 1;
        weights->current_min_door -= weights->min_door[city];
        current->route_points[visited-1] = city;
        check_for_better_bound(best, visited, bound_request);
        if(mpi_test_value)
        {
            MPI_Request new_bound_request;
            MPI_Irecv(&mpi_rec_value, 1, MPI_INT, MPI_ANY_SOURCE, TAG_BOUND, MPI_COMM_WORLD, &new_bound_request);
            bound_request = new_bound_request;
        }
        if(above_splitlevel(best, visited) || weight + ((weights->current_min_door+1)/2) < min(best->distance, min->distance))
        {
            for(i=1; i<weights->number_of_cities; i++){
                if(!current->visited[i]){
                    if(!on_splitlevel(best, visited) || p_id == (*b_nr%best->number_of_processes))
                    {
                        check_for_better_bound(best, visited, bound_request);
                        if(mpi_test_value)
                        {
                            MPI_Request new_bound_request;
                            MPI_Irecv(&mpi_rec_value, 1, MPI_INT, MPI_ANY_SOURCE, TAG_BOUND, MPI_COMM_WORLD, &new_bound_request);
                            bound_request = new_bound_request;
                        }
                        if(weight+weights->data[city][i]+(weights->number_of_cities - visited)*weights->smallest_distance<=min(best->distance,min->distance)){
                            
                            search(i, weight+weights->data[city][i],current, visited+1, min, weights, best, b_nr,p_id, bound_request);
                            
                        } else {
                            //printf("2bound %i<=min(%i,%i)p_id=%i\n", weight+weights->data[city][i]+(weights->number_of_cities - visited)*weights->smallest_distance, best->distance, min->distance, p_id);
                        }
                        
                    }
                    if(on_splitlevel(best, visited))
                    {
                        (*b_nr)++;
                    }
                }
                
            }
        }
        current->visited[city]=0;
        weights->current_min_door += weights->min_door[city];
    }
}

void check_for_better_bound(best_solution * best, int visited, MPI_Request bound_request){
    mpi_rec_value=INT_MAX;
    mpi_test_value=0;
    if(!above_splitlevel(best, visited))
    {
        MPI_Status status;
        MPI_Test(&bound_request, &mpi_test_value, &status);
        if(mpi_test_value && mpi_rec_value<best->distance){
            best->distance = mpi_rec_value;
            //printf("received%i\n", mpi_rec_value);
        }
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

void search_solution(matrix* distances, best_solution* best, int p_id)
{
    int i,smallest_dist;
    route min;
    travel current;
    MPI_Status status_distance;
    MPI_Status status_route;
    MPI_Request bound_request;
    MPI_Request request_distance;
    MPI_Request request_route;
    int** all_routes;
    MPI_Irecv(&mpi_rec_value, 1, MPI_INT, MPI_ANY_SOURCE, TAG_BOUND, MPI_COMM_WORLD, &bound_request);
    init_travel(&min, &current, distances);
    int b_nr=0;
    int index_smallest_distance = 0;
    int* received_size = (int*)malloc(sizeof(int));
    best->distance=INT_MAX;
    search(0, 0, &current, 1, &min, distances, best, &b_nr, p_id, bound_request);
    /*printf("p_id:%i, distance:%i\n", p_id, min.distance);
     for(i=0; i<distances->number_of_cities; i++)
     {
     printf("%i",min.route_points[i]);
     }
     printf("\n");*/
    if(best->number_of_processes>1)
    {
        best->distance = min.distance;
        for(i=0;i<distances->number_of_cities; i++)
        {
            best->route_points[i] = min.route_points[i];
        }
        if(p_id != 0)
        {
            //send solution to process 0
            MPI_Isend(&(best->distance), 1, MPI_INT, 0, TAG_DISTANCE, MPI_COMM_WORLD, &request_distance);
            MPI_Isend((best->route_points), distances->number_of_cities, MPI_INT, 0, TAG_ROUTE, MPI_COMM_WORLD, &request_route);
            MPI_Wait(&request_distance, &status_distance);
            MPI_Wait(&request_route, &status_route);
        } else
        {
            MPI_Status status_dist;
            MPI_Status status_ids;
            int all_distances[best->number_of_processes];
            all_routes = (int**) malloc(best->number_of_processes * sizeof (int*));
            all_distances[0] = min.distance;
            all_routes[0] = min.route_points;
            for(i=0; i<best->number_of_processes-1; i++)
            {
                MPI_Probe(MPI_ANY_SOURCE, TAG_DISTANCE, MPI_COMM_WORLD, &status_dist);
                MPI_Recv(&all_distances[status_dist.MPI_SOURCE], 1, MPI_INT, MPI_ANY_SOURCE, TAG_DISTANCE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Probe(MPI_ANY_SOURCE, TAG_ROUTE, MPI_COMM_WORLD, &status_ids);
                MPI_Get_count(&status_ids, MPI_INT, received_size);
                if (*received_size == distances->number_of_cities) {
                    all_routes[status_ids.MPI_SOURCE] = (int*) malloc(distances->number_of_cities * sizeof (int));
                    MPI_Recv(all_routes[status_ids.MPI_SOURCE], distances->number_of_cities, MPI_INT, MPI_ANY_SOURCE, TAG_ROUTE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                } else {
                    printf("Received a route of the wrong size!\n");
                }
            }
            smallest_dist=all_distances[0];
            for(i=1; i<best->number_of_processes; i++)
            {
                if(all_distances[i]<smallest_dist)
                {
                    index_smallest_distance = i;
                    smallest_dist = all_distances[i];
                }
            }
            best->distance=smallest_dist;
            for(i=0; i<distances->number_of_cities; i++)
            {
                best->route_points[i]=all_routes[index_smallest_distance][i];
            }
            printf("%i\n",best->distance);
            for(i=0; i<=distances->number_of_cities; i++)
            {
                printf("%i ",best->route_points[i]);
            }
            printf("\n");
            for(i=1; i<best->number_of_processes; i++)
            {
                free(all_routes[i]);
            }
            free(&all_routes[0]);
        }
        
    } else
    {
        printf("%i\n",min.distance);
        for(i=0; i<=distances->number_of_cities; i++)
        {
            printf("%i ",min.route_points[i]);
        }
        printf("\n");
    }
    free(received_size);
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