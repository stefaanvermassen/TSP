//
//  greedy.c
//  TSP
//
//  Created by Stefaan Vermassen on 2/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "greedy.h"

void greedy_search(greedy_route *route, matrix *weights)
{
    int *visited = (int *) calloc(weights->number_of_cities+1, sizeof(int));
    int number_of_visited = 1;
    int smallest_distance = 9999;
    int i, city, nearest_ciy = 0;
   // printf("%i\n", visited[5]);
    while (number_of_visited <= weights->number_of_cities)
    {
        for(i=0; i<weights->number_of_cities; i++)
        {
            if (city != i && weights->data[city][i] < smallest_distance){
                if(visited[i]==0)
                {
                    nearest_ciy = i;
                    smallest_distance = weights->data[city][i];
                }
            }
        }
        route->route_points[number_of_visited-1] = city;
        visited[city] = 1;
        city = nearest_ciy;
        smallest_distance = 9999;
        number_of_visited++;
        
    }
    route->route_points[number_of_visited-1] = 0;
}

void init_greedy_route(greedy_route *route, matrix *weights)
{
    route->route_points = (int *) calloc(weights->number_of_cities, sizeof(int));
}
