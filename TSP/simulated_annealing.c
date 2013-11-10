//
//  simulated_annealing.c
//  TSP
//
//  Created by Stefaan Vermassen on 30/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "simulated_annealing.h"
#define SATISFIED 100000

void simulated_annealing_search(best_solution* best, matrix * weights)
{
    int rand_index1, rand_index2, swap_distance, dst, attempt=0;
    double temp = weights->smallest_distance/40.0;
    dst = best->greedy_distance;
    
    while(attempt < SATISFIED)
    {
        rand_index1 = (rand() % (weights->number_of_cities -1))+1;
        rand_index2 = rand_index1+(rand() % (weights->number_of_cities-rand_index1));
        swap_distance = get_swap_distance(dst,rand_index1,rand_index2,best->greedy_route,weights);
        if(accept(swap_distance, dst, temp))
        {
            do_swap(rand_index1, rand_index2, best->greedy_route);
            dst = swap_distance;
            if(dst < best->greedy_distance)
            {
                best->greedy_distance = dst;
            }
            attempt = 0;
        } else
        {
            attempt++;
        }
        if(temp> 0.01){
            temp = temp*0.9999;
        }
    }    
}

int get_swap_distance(int dst, int i, int j, int* path, matrix* weights)
{
    int dist;
    if(j==weights->number_of_cities-1)
    {
        dist = dst - (weights->data[path[i-1]][path[i]] + weights->data[path[j]][path[0]]);
        dist += weights->data[path[i-1]][path[j]] + weights->data[path[i]][path[0]];
    }else
    {
        dist = dst - (weights->data[path[i-1]][path[i]] + weights->data[path[j]][path[j+1]]);
        dist += weights->data[path[i-1]][path[j]] + weights->data[path[i]][path[j+1]];
    }
    return dist;
}

int accept(int new_distance, int old_distance, double temp)
{
    double prob, q;
    if(new_distance == old_distance) return 0;
    prob = exp((old_distance-new_distance)/temp);
    q = rand () / (double) RAND_MAX;
    return (q<prob)?1:0;
}

void do_swap(int i, int j, int* path)
{
    while(i<j)
    {
        int temp;
        temp = path[i];
        path[i]=path[j];
        path[j] = temp;
        i++;
        j--;
    }
}

