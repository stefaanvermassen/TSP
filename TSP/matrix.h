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
} matrix;

int read_distances(matrix *m, char **argv);
void destroy_matrix(matrix *m);
#endif
