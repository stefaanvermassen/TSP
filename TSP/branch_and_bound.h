//
//  branch_and_bound.h
//  TSP
//
//  Created by Stefaan Vermassen on 2/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#ifndef TSP_branch_and_bound_h
#define TSP_branch_and_bound_h
#include <limits.h>
#include "main.h"
#include "solution.h"

typedef struct {
    int distance;
    int *route_points;
} route;

typedef struct {
    int *visited;
    int *route_points;
} travel;

void init_travel(route *min, travel *current, matrix *weights);
void destroy_travel(route *min, travel *current);
void search_solution(matrix* distances, best_solution* best, int p_id);
void search(int city, int weight, travel *current, int visited, route *min, matrix *weights, best_solution* best, int* b_nr, int p_id);
int above_splitlevel(best_solution* t, int visited);
int on_splitlevel(best_solution* t, int visited);

#endif
