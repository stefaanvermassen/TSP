//
//  branch_and_bound.h
//  TSP
//
//  Created by Stefaan Vermassen on 2/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#ifndef TSP_branch_and_bound_h
#define TSP_branch_and_bound_h
#include "matrix.h"

typedef struct {
    int distance;
} route;

typedef struct {
    int number_of_visited;
    int visited[];
} city;


void search(int city, int weight, int number_of_visited, int visited[], route *min, matrix *weights);


#endif
