//
//  greedy.h
//  TSP
//
//  Created by Stefaan Vermassen on 2/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#ifndef TSP_greedy_h
#define TSP_greedy_h
#include "matrix.h"

typedef struct {
    int distance;
    int *route_points;
} greedy_route;

void greedy_search(greedy_route *route, matrix *weights);
void init_greedy_route(greedy_route *route, matrix *weights);
void destroy_greedy_route(greedy_route *route);
#endif
