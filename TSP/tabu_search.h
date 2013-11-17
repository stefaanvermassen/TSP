//
//  tabu_search.h
//  TSP
//
//  Created by Stefaan Vermassen on 16/11/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#ifndef TSP_tabu_search_h
#define TSP_tabu_search_h
#include "solution.h"
#include "main.h"

typedef struct{
    int** tabu_list;
    int tabu_length;
    int* temp_solution;
    int temp_distance;
} tabu_sol;

void init_tabu(matrix* weights, tabu_sol* tabu);
void tabu_search(best_solution* best, matrix* weights);
void get_better_path(best_solution* best, matrix* weights, tabu_sol* tabu);
int get_route_distance(int *path, matrix* weights);
void decrement_tabu(tabu_sol* tabu, matrix* weights);
void swap(int* path, int pos1, int pos2);
void reverse(int* path, int start, int end);
void rotate(int* path, int k, matrix* weights);
void destroy_tabu(matrix* weights, tabu_sol* tabu);

#endif
