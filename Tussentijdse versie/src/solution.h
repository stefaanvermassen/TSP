//
//  solution.h
//  TSP
//
//  Created by Stefaan Vermassen on 23/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#ifndef TSP_solution_h
#define TSP_solution_h
//#include "main.h"
#include "matrix.h"

typedef struct{
    int distance;
    int greedy_distance;
    int splitlevel;
    int number_of_processes;
    int *route_points;
    int *greedy_route;
} best_solution;

void init_solution(best_solution* best, matrix* distances, int number_of_processes, int splitlevel);
void destroy_solution(best_solution* best, matrix* distances);

#endif
