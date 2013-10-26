//
//  main.h
//  TSP
//
//  Created by Stefaan Vermassen on 2/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#ifndef TSP_main_h
#define TSP_main_h
#include <mpi.h>
#include "matrix.h"
#include "branch_and_bound.h"
#include "greedy.h"
#include "solution.h"

void perform_branch_and_bound(matrix* distances, int p_id, int p_total);
void perform_greedy(matrix* distances, int p_id);

#endif
