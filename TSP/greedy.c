//
//  greedy.c
//  TSP
//
//  Created by Stefaan Vermassen on 2/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "greedy.h"

void greedy_search(greedy_route *route, matrix *weights)
{
    int *visited = (int *) calloc(weights->number_of_cities+1, sizeof(int));
    int number_of_visited = 1;
    int smallest_distance = INT_MAX;
    int i,nearest_ciy = 0;
    int city=route->start_city;
   // printf("%i\n", visited[5]);
    while (number_of_visited <= weights->number_of_cities)
    {
        for(i=0; i<weights->number_of_cities; i++)
        {
            if (city != i && weights->data[city][i] < smallest_distance){
                if(!visited[i])
                {
                    nearest_ciy = i;
                    smallest_distance = weights->data[city][i];
                }
            }
        }
        route->route_points[number_of_visited-1] = city;
        if (number_of_visited != weights->number_of_cities) route->distance += smallest_distance;
        visited[city] = 1;
        city = nearest_ciy;
        smallest_distance = INT_MAX;
        number_of_visited++;
        
    }
    route->route_points[number_of_visited-1] = route->route_points[0];
    route->distance += weights->data[route->route_points[number_of_visited-2]][route->route_points[number_of_visited-1]];
    free(visited);
}

void search_greedy_solution(matrix* weights, int p_id)
{
    int i;
    greedy_route greedy_min;
    init_greedy_route(&greedy_min, weights);
    greedy_min.start_city=3;
    greedy_search(&greedy_min, weights);
    printf("%i\n", greedy_min.distance);
    for(i=0; i<weights->number_of_cities; i++)
    {
        printf("%i", greedy_min.route_points[i]);
    }
    printf("\n");
    destroy_greedy_route(&greedy_min);
}

void init_greedy_route(greedy_route *route, matrix *weights)
{
    route->distance = 0;
    route->route_points = (int *) calloc(weights->number_of_cities+1, sizeof(int));
}

void destroy_greedy_route(greedy_route *route){
    free(route->route_points);
}
