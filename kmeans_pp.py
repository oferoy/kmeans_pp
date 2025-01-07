# cd sp/kmeans_pp
# python3 setup.py build_ext --inplace 

import kmeans_capi as km
import sys
import math

EPSILON = 0.001
MAX_ITERS_NUM = 1000
DEFAULT_ITERS_NUM = 200



argc = len(sys.argv)
if argc == 4:
    try:
        iter = int(sys.argv[2])
    except:
        print("Invalid maximum iteration!")
        exit()
    pointsFile = sys.argv[3]
elif argc == 3:
    iter = DEFAULT_ITERS_NUM
    pointsFile = sys.argv[2]
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


pointsArr = []
with open(pointsFile, "r") as file:
    ctr = 1
    for line in file:
        pointsArr.append([float(val) for val in line.strip().split(",")])

numOfPoints = len(pointsArr)
dimension = len(pointsArr[0])

if(k <= 1 or k >= numOfPoints):
    print("Invalid number of clusters!")
    exit()


centroidsArr = []
for i in range(k):
    centroidsArr.append(pointsArr[i])

centroidsArr = km.kmeansP(numOfPoints, dimension, k, EPSILON, 200, pointsArr, centroidsArr)

for c in range(k):
    for d in range(dimension - 1):
        formatted = "%.4f"%centroidsArr[c][d]
        print(f"{formatted},", end="")
    formatted = "%.4f"%centroidsArr[c][dimension - 1]
    print(formatted)

  

        