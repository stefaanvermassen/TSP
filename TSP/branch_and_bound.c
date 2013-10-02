//
//  branch_and_bound.c
//  TSP
//
//  Created by Stefaan Vermassen on 2/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "branch_and_bound.h"

void search(int city, int weight, travel *current, int visited, route *min, matrix *weights)
{
    int i,j;
    //printf("visited: %i, city: %i, weight: %i\n", visited, city, weight);
    current->route_points[visited-1] = city;
    if(visited == weights->number_of_cities)
    {
        if(weight+weights->data[city][0] < min->distance)
        {
            min->distance = weight+weights->data[city][0];
            for(j=0; j<=weights->number_of_cities; j++){
                min->route_points[j] = current->route_points[j];
            }
        }
    }else
    {
        current->visited[city] = 1;
        current->route_points[visited-1] = city;
        for(i=1; i<weights->number_of_cities; i++){
            if((current->visited[i] == 0) && weight+weights->data[city][i] + (weights->number_of_cities - visited)*weights->smallest_distance < min->distance){
                search(i, weight+weights->data[city][i],current, visited+1, min, weights);
            }
        }
        current->visited[city]=0;
    }
}

void init_travel(route *min, travel *current, matrix *weights)
{
    min->distance = INT_MAX;
    min->route_points = (int*) calloc(weights->number_of_cities+1, sizeof(int));
    current->visited = (int*) calloc(weights->number_of_cities, sizeof(int));
    current->route_points = (int*) calloc(weights->number_of_cities+1, sizeof(int));
}

void destroy_travel(route *min, travel *current){
    free(current->route_points);
    free(min->route_points);
    free(current->visited);
}