/*
gcc -ansi -Wall -Wextra -Werror -pedantic-errors kmeans_arrays.c -o kmeans_pp -lm
./kmeans 15 100 < input_3.txt
valgrind --leak-check=full --track-origins=yes ./kmeans 15 < input_3.txt
*/

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "kmeans_arrays.h"


#define DEFAULT_ITERS_NUM (300)
#define MAX_ITERS_NUM (1000)

double dist(double *p1, double *p2, int dim){  
    double distance = 0.0;
    int i;
    for(i = 0; i < dim; i++){
        distance += pow(p1[i]- p2[i], 2);
    }
    return sqrt(distance);
}

double** kmeans(int numPoints, int pointsDimension, int clusters, double epsilon, int iterations, double **pointsArray, double **initialCentroids)
{
    
    int dimension = pointsDimension;
    int clustersNum = clusters;
    double const EPSILON = epsilon;
    int itersNum = iterations;
    double **pointsArr = pointsArray;
    double **centroidsArr = initialCentroids;
    double **newCentroidsArr;
    int *countArr;
    int i, j, k;
    int numOfPoints = numPoints;
    int closestCentIndex = 0;
    double minDist = 0;
    double curDist = 0;
    int smallerThanEps = 0;


    newCentroidsArr = (double **)malloc(clustersNum * sizeof(double *));
    if(!newCentroidsArr){
        printf("An Error Has Occurred\n");
        return NULL;
    }

    for (i = 0; i < clustersNum; i++)
    {
        printf("i=%d\n", i);
        printf("dim = %d\n",dimension);
        printf("k = %d\n",clustersNum);
        newCentroidsArr[i] = (double *)malloc(dimension * sizeof(double));
 
        if(!newCentroidsArr[i]){
            for (j = 0; j < i; j++) {
                free(newCentroidsArr[j]);
            }
            free(newCentroidsArr);
            printf("An Error Has Occurred2\n");
            return NULL;
        }
    }


    /*initialize newCentroidsArr*/
    for(i = 0; i < clustersNum; i++){
        for(j = 0; j < dimension; j++){
            newCentroidsArr[i][j] = 0.0;
        }
    }


    /*initialize countArr*/
    countArr = malloc(sizeof(int) * clustersNum);
    if(!countArr){
        for (i = 0; i < clustersNum; i++) {
        free(newCentroidsArr[i]);
        }
        free(newCentroidsArr);
    }



 /*main algorithm*/
    for(i = 0; i < itersNum; i++){
        /*reset newCentroidsArr to all zeros*/
        for(j = 0; j < clustersNum; j++){
            for(k = 0; k < dimension; k++){
                newCentroidsArr[j][k] = 0.0;
            }
        }

        /*reset countArr to zeros*/
        for(j = 0; j < clustersNum; j++){
            countArr[j] = 0;
        }
        
        /*loop through the points and assign each one to closest centroid*/
        for(j = 0; j < numOfPoints; j++)
        {
            closestCentIndex = 0;
            minDist = dist(pointsArr[j], centroidsArr[0], dimension); 
            for(k = 1; k < clustersNum; k++)
            {
                curDist = dist(pointsArr[j], centroidsArr[k], dimension);
                if(curDist < minDist)
                {
                    minDist = curDist;
                    closestCentIndex = k;
                }
            }
            
            countArr[closestCentIndex]++;
            for(k = 0; k < dimension; k++)
            {
                newCentroidsArr[closestCentIndex][k] += pointsArr[j][k];
            }
        }
    

        /*divide the sum of the values of newCentroidsArr by the number of points assigned to the centroid*/
        for(j = 0; j < clustersNum; j++)
        {
            for(k = 0; k < dimension; k++)
            {
                newCentroidsArr[j][k] /= countArr[j];
            }
        }
        

        smallerThanEps = 1;
        for(j = 0; j < clustersNum; j++)
        {
            if(fabs(dist(newCentroidsArr[j], centroidsArr[j], dimension)) >= EPSILON)
            {
                smallerThanEps =0;
            }
        }
        if(smallerThanEps == 1)
        {
            break;
        }



        for (j = 0; j < clustersNum; j++){
            for (k = 0; k < dimension; k++){
                centroidsArr[j][k] = newCentroidsArr[j][k];
            }
        }

        
    }
    

    free(countArr);
    for(i=0; i<clustersNum; i++){
        free(newCentroidsArr[i]);
    }
    free(newCentroidsArr);
  
    return centroidsArr;



}