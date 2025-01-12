# cd sp/kmeans_pp
# python3 setup.py build_ext --inplace 

import kmeans_capi as km
import sys
import math
import pandas as pd
import numpy as np

MAX_ITERS_NUM = 1000
DEFAULT_ITERS_NUM = 300



argc = len(sys.argv)
if argc == 6:
    try:
        iter = int(sys.argv[2])
    except:
        print("Invalid maximum iteration!")
        exit()
    try:
        EPSILON = float(sys.argv[3])
    except:
        print("Invalid epsilon!")
        exit()
    if EPSILON < 0:
        print("Invalid epsilon!")
        exit()
    pointsFile1 = sys.argv[4]
    pointsFile2 = sys.argv[5]

elif argc == 5:
    iter = DEFAULT_ITERS_NUM
    try:
        EPSILON = float(sys.argv[2])
    except:
        print("Invalid epsilon!")
        exit()
    if EPSILON < 0:
        print("Invalid epsilon!")
        exit()
    pointsFile1 = sys.argv[3]
    pointsFile2 = sys.argv[4]
else:
    print("An Error Has Occurred")
    exit()


try:
    k = int(sys.argv[1])
except:
    print("Invalid number of clusters!")
    exit()



if iter <= 1 or iter >= MAX_ITERS_NUM:
    print("Invalid maximum iteration!")
    exit()

df1 = pd.read_csv(pointsFile1, header=None)
df2 = pd.read_csv(pointsFile2, header=None)
dfMerged = pd.merge(df1, df2, on=0)
dfSorted = dfMerged.sort_values(by=0)

numpyPointsArr = np.array(dfSorted.iloc[:,1:].to_numpy())
numOfPoints = len(numpyPointsArr)
dimension = len(numpyPointsArr[0])
pointsArr = numpyPointsArr.tolist()



def dist(p1, p2):
    dist = 0.0
    for i in range(dimension):
        dist += math.pow(p1[i] - p2[i], 2)
    return math.sqrt(dist)

def D(p1, centroids):
    minDist = dist(p1,centroids[0])
    for c in centroids:
        curDist = dist(p1,c)
        if curDist < minDist:
            minDist = curDist
    return minDist

def sumD(points, centroids):
    sum = 0
    for point in points:
        sum += D(point, centroids)
    return sum

def probabilityFunc(p1, points, centroids):
    return D(p1,centroids) / sumD(points, centroids)

def centroidInit():
    centroidsArr = []
    np.random.seed(1234)
    chosenIndex = np.random.choice(numpyPointsArr.shape[0])
    centroidsArr.append(numpyPointsArr[chosenIndex].tolist())
    centroidIndices = []
    centroidIndices.append(chosenIndex)

    for i in range(k-1):
        probs = []
        for p in numpyPointsArr:
            probs.append(probabilityFunc(p,numpyPointsArr,centroidsArr))

        nextCentroidIdx = np.random.choice(numpyPointsArr.shape[0], p=probs)
        centroidsArr.append(numpyPointsArr[nextCentroidIdx].tolist())
        centroidIndices.append(nextCentroidIdx)

    print(centroidIndices)
    return centroidsArr


    

    
finalCentroids = []
initialCentroids = centroidInit()
finalCentroids = km.kmeansP(numOfPoints, dimension, k, EPSILON, iter, pointsArr, initialCentroids)
print(finalCentroids)
