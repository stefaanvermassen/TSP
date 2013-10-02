//
//  matrix.c
//  TSP
//
//  Created by Stefaan Vermassen on 2/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int read_distances(matrix *m, char **argv)
{
    int c;
    FILE *f = NULL;
    m->number_of_cities = 0;
    int i,j=0;
    f = fopen(argv[1],"r");
    if(!f){
        perror ("Failed to read inputfile");
        return 1;
    }
    if (fscanf(f, "%d", &m->number_of_cities) == 1) {
        /*Make matrix*/
        m->data = (int **)malloc(m->number_of_cities*sizeof(int*));
        for(i=0; i<m->number_of_cities; i++){
            m->data[i] = (int *)malloc(m->number_of_cities*sizeof(int));
        }
        i=0;
        while(fscanf(f, "%d", &c) == 1){
            if(i==m->number_of_cities){
                i=0;
                j++;
            }
            m->data[i][j] = c;
            i++;
            
        }
        fclose(f);
        for (i=0; i<m->number_of_cities; i++){
            for(j=0; j<m->number_of_cities; j++){
                printf("data[%i][%i]=%d\n", i,j,m->data[i][j]);
            }
        }
        
    }
    return 0;
    
}

void destroy_matrix(matrix *m)
{
    int i;
    for (i=0; i< m->number_of_cities; i++){
        free(m->data[i]);
    }
    free(m->data);
}