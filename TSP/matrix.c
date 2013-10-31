//
//  matrix.c
//  TSP
//
//  Created by Stefaan Vermassen on 2/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "matrix.h"
#define SWAP(a, b, size) \
do { \
register size_t __size = (size); \
register char *__a = (a), *__b = (b); \
if (__a != __b) do { \
char __tmp = *__a; \
*__a++ = *__b; \
*__b++ = __tmp; \
} while (--__size > 0); \
} while (0)

int read_distances(matrix *m, char **argv)
{
    int c;
    FILE *f = NULL;
    m->number_of_cities = 0;
    m->smallest_distance = INFINITY;
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
            //save smallest distance
            if(c<m->smallest_distance && i != j){
                m->smallest_distance = c;
            }
            i++;
            
        }
        fclose(f);
        init_min_door(m);
        //        for (i=0; i<m->number_of_cities; i++){
        //            for(j=0; j<m->number_of_cities; j++){
        //                printf("data[%i][%i]=%d\n", i,j,m->data[i][j]);
        //            }
        //        }
        
    }
    return 0;
    
}



size_t quickfindPartition (char * base, size_t left, size_t right, size_t size, size_t pivotIndex)
{
    size_t i;
    size_t storeIndex;
    SWAP (base+(size*pivotIndex), base+(size*right), size);
    storeIndex = left;
    for (i=left; i<=right-1; ++i)
    {
        if (*(int*)(base+(size*i)) - *(int*)(base+(size*right)) <= 0)
        {
            SWAP (base+(size*storeIndex), base+(size*i), size);
            ++storeIndex;
        }
    }
    SWAP (base+(size*right), base+(size*storeIndex), size);
    return storeIndex;
}

void quickfindFirstK (char * base, size_t left, size_t right, size_t size, size_t k)
{
    if (right > left)
    {
        size_t pivotIndex = left + (right-left)/2u;
        size_t pivotNewIndex = quickfindPartition (base, left, right, size, pivotIndex);
        if (pivotNewIndex > left + k)
            quickfindFirstK (base, left, pivotNewIndex-1, size, k);
        else if (pivotNewIndex < left + k)
            quickfindFirstK (base, pivotNewIndex+1, right, size, k+left-pivotNewIndex-1);
    }
}

void init_min_door(matrix *m)
{
    int i,j;
    m->min_door = (int*) malloc(m->number_of_cities * sizeof(int));
    m->current_min_door=0;
    for(i=0; i<m->number_of_cities; i++)
    {
        int minima[m->number_of_cities];
        for(j=0; j< m->number_of_cities; j++)
        {
            if(i==j)
            {
                minima[j] = INT_MAX;
            }else
            {
                minima[j] = m->data[i][j];
            }
        }
        quickfindFirstK((void*) minima, 0, m->number_of_cities-1, sizeof(int), 2);
        m->min_door[i] = minima[0] + minima[1];
        m->current_min_door += m->min_door[i];
    }
}

void destroy_matrix(matrix *m, int p_id)
{
    int i;
    for (i=0; i< m->number_of_cities; i++){
        free(m->data[i]);
    }
    free(m->data);
    free(m->min_door);
}