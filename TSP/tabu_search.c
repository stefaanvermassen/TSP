//
//  tabu_search.c
//  TSP
//
//  Created by Stefaan Vermassen on 16/11/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "tabu_search.h"
#define NUMBER_OF_ITERATIONS 1000

void tabu_search(best_solution* best, matrix* weights)
{
    int i,j,index;
    tabu_sol tabu;
    init_tabu(weights, &tabu);
    for(i=0; i<weights->number_of_cities; i++)
    {
        if(best->greedy_route[i]==0) index=i;
    }
    rotate(best->greedy_route, weights->number_of_cities-index, weights);
    best->greedy_route[weights->number_of_cities] = 0;
    for (i=0; i<NUMBER_OF_ITERATIONS; i++)
    {
        get_better_path(best, weights, &tabu);
        if(tabu.temp_distance < best->greedy_distance)
        {
            best->greedy_distance = tabu.temp_distance;
            for(j=0; j<weights->number_of_cities; j++)
            {
                best->greedy_route[j] = tabu.temp_solution[j];
            }
        }
        
    }
    destroy_tabu(weights, &tabu);
}

void decrement_tabu(tabu_sol* tabu, matrix* weights)
{
    int i,j;
    for(i=0; i<weights->number_of_cities; i++)
    {
        for(j=0; j<weights->number_of_cities; j++)
        {
            if(tabu->tabu_list[i][j] >=0) tabu->tabu_list[i][j]-=1;
        }
    }
}

void get_better_path(best_solution* best, matrix* weights, tabu_sol* tabu)
{
    int i,j,city1=0;
    int city2=0;
    int first_neighbor=1;
    int swap_distance = 0;
    tabu->temp_distance = best->greedy_distance;
    for(i=0;i<weights->number_of_cities; i++)
    {
        tabu->temp_solution[i] = best->greedy_route[i];
    }
    for(i=1; i<weights->number_of_cities; i++)
    {
        for(j=2; j<weights->number_of_cities; j++)
        {
            if(i!=j)
            {
                do_swap(i, j, tabu->temp_solution);
                swap_distance = get_route_distance(tabu->temp_solution, weights);
                if((swap_distance > tabu->temp_distance || first_neighbor) && tabu->tabu_list[i][j]==0)
                {
                    first_neighbor = 0;
                    city1=i;
                    city2=j;
                    tabu->temp_distance = swap_distance;
                }else
                {
                    do_swap(i, j, tabu->temp_solution);
                }
            }
        }
    }
    if(city1 != 0)
    {
        decrement_tabu(tabu, weights);
        tabu->tabu_list[city1][city2] += 5;
        tabu->tabu_list[city2][city1] += 5;
    }
    
}

void init_tabu(matrix* weights, tabu_sol* tabu){
    int i;
    tabu->tabu_length=10;
    tabu->temp_solution = (int*) calloc(weights->number_of_cities+1,sizeof(int));
    tabu->tabu_list = (int **)malloc(weights->number_of_cities*sizeof(int*));
    for(i=0; i<weights->number_of_cities; i++){
        tabu->tabu_list[i] = (int *)malloc(weights->number_of_cities*sizeof(int));
    }
    tabu->temp_distance = INT_MAX;
}

void destroy_tabu(matrix* weights, tabu_sol* tabu)
{
    int i;
    free(tabu->temp_solution);
    for(i=0; i<weights->number_of_cities; i++)
    {
        free(tabu->tabu_list[i]);
    }
    free(tabu->tabu_list);
}

void swap(int* path, int pos1, int pos2)
{
    int temp = path[pos1];
    path[pos1] = path[pos2];
    path[pos2] = temp;
}
void reverse(int* path, int start, int end)
{
    while(start < end)
    {
        swap(path, start, end);
        start++;
        end--;
    }
}
void rotate(int* path, int k, matrix* weights)
{
    reverse(path, 0, weights->number_of_cities-1);
    reverse(path, 0, k-1);
    reverse(path, k, weights->number_of_cities-1);
}
int get_route_distance(int *path, matrix* weights)
{
    int i, cost = 0;
    for(i=0; i<weights->number_of_cities; i++){
        cost += weights->data[path[i]][path[i+1]];
    }
    return cost;
}

