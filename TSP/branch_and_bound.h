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
#include "matrix.h"

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
void search(int city, int weight, travel *current, int visited, route *min, matrix *weights);


#endif
