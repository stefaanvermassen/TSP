//
//  matrix.h
//  TSP
//
//  Created by Stefaan Vermassen on 2/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#ifndef TSP_matrix_h
#define TSP_matrix_h

typedef struct {
    int number_of_cities;
    int ** data;
    int smallest_distance;
    int * min_door;
    int current_min_door;
} matrix;

int read_distances(matrix *m, char **argv);
void init_min_door(matrix *m);
size_t quickfindPartition (char * base, size_t left, size_t right, size_t size, size_t pivotIndex);
void quickfindFirstK (char * base, size_t left, size_t right, size_t size, size_t k);
void destroy_matrix(matrix *m, int p_id);

#endif
