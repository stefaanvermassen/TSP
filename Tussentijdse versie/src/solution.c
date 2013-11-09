//
//  solution.c
//  TSP
//
//  Created by Stefaan Vermassen on 23/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "solution.h"

void init_solution(best_solution* best, matrix* distances, int number_of_processes, int splitlevel)
{
    best->route_points = (int*) calloc(distances->number_of_cities+1, sizeof(int));
    best->greedy_route = (int*) calloc(distances->number_of_cities+1, sizeof(int));
    best->splitlevel = splitlevel;
    best->distance = INT_MAX;
    best->greedy_distance = INT_MAX;
    best->number_of_processes = number_of_processes;
}

void destroy_solution(best_solution* best, matrix* distances)
{
    free(best->route_points);
    free(best->greedy_route);
}