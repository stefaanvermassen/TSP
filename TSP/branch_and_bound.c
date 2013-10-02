//
//  branch_and_bound.c
//  TSP
//
//  Created by Stefaan Vermassen on 2/10/13.
//  Copyright (c) 2013 Stefaan Vermassen. All rights reserved.
//

#include <stdio.h>
#include "branch_and_bound.h"

void search(int city, int weight, int number_of_visited, int visited[], route *min, matrix *weights)
{
    int i;
    if(number_of_visited == weights->number_of_cities)
    {
        if(weight+weights->data[city][0] < min->distance)
        {
            min->distance = weight+weights->data[city][0];
        }
    }else
    {
        visited[city] = 1;
        for(i=1; i<=weights->number_of_cities; i++){
            if(visited[i] == 0){
                search(i, weight+weights->data[city][i],number_of_visited+1, visited, min, weights);
            }
        }
        visited[city]=0;
    }
}