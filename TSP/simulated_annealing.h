//
//  simulated_annealing.h
//  TSP
//
//  Created by Stefaan Vermassen on 30/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#ifndef TSP_simulated_annealing_h
#define TSP_simulated_annealing_h
#include "solution.h"
#include "main.h"

void simulated_annealing_search(best_solution* best, matrix* weights);
int get_swap_distance(int dst, int i, int j, int* path, matrix* weights);
int accept(int new_distance, int old_distance, double temp);
void do_swap(int i, int j, int* path);

#endif
