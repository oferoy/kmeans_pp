# define PY_SSIZE_T_CLEAN
# include <Python.h>
# include "kmeans_arrays.h"


static PyObject* kmeansP(PyObject *self, PyObject *args)
{
    /*int pointsDimension;*/
    int clusters;
    double epsilon;
    int iterations;
    PyObject *pointsArray;
    PyObject *initialCentroids;
    PyObject *line;
    PyObject *item;
    int i,j;
    int numOfPoints, dimension;
    double **cPointsArray;
    double **cCentArr;
    double **resCentroids;
    PyObject *PyResult;
    PyObject *centroid;
   


    if(!PyArg_ParseTuple(args, "iiidiOO", &numOfPoints, &dimension, &clusters, &epsilon, &iterations, &pointsArray, &initialCentroids))
    {
        return NULL;
    }
    
    cPointsArray = (double **)malloc(numOfPoints * sizeof(double *));
    if(!cPointsArray){
        printf("An Error Has Occurred\n");
        return NULL;
    }
    for(i=0; i<numOfPoints; i++)
    {
        cPointsArray[i] = (double *)malloc(dimension * sizeof(double));
        if(!cPointsArray){
            for (j = 0; j < i; j++) {
                free(cPointsArray[j]);
            }
            free(cPointsArray);
            printf("An Error Has Occurred\n");
            return NULL;
        }
        line = PyList_GetItem(pointsArray, i);
        for(j=0; j<dimension; j++)
        {
            item = PyList_GetItem(line, j);
            cPointsArray[i][j] = PyFloat_AsDouble(item);
        }
    }
    cCentArr = (double **)malloc(clusters * sizeof(double *));
    if(!cCentArr){
        for (j = 0; j < numOfPoints; j++) {
            free(cPointsArray[j]);
        }
        free(cPointsArray);
        printf("An Error Has Occurred\n");
        return NULL;
    }
    for(i=0; i<clusters; i++)
    {
        cCentArr[i] = (double *)malloc(dimension * sizeof(double));
        if(!cCentArr[i]){
        for (j = 0; j < numOfPoints; j++) {
            free(cPointsArray[j]);
        }
        for (j = 0; j < i; j++) {
            free(cCentArr[j]);
        }
        free(cPointsArray);
        free(cCentArr);
        printf("An Error Has Occurred\n");
        return NULL;
    }
        line = PyList_GetItem(initialCentroids, i);
        for(j=0; j<dimension; j++)
        {
            item = PyList_GetItem(line, j);
            cCentArr[i][j] = PyFloat_AsDouble(item);
        }
    }

    resCentroids = kmeans(numOfPoints, dimension, clusters, epsilon, iterations, cPointsArray, cCentArr);
    PyResult = PyList_New(clusters); 
    for (i = 0; i < clusters; i++)
    {
        centroid = PyList_New(dimension);  
        for (j = 0; j < dimension; j++)
        {
            PyList_SetItem(centroid, j, PyFloat_FromDouble(resCentroids[i][j]));
        }
        PyList_SetItem(PyResult, i, centroid);  
    }

    for (i = 0; i < numOfPoints; i++)
    {
        free(cPointsArray[i]);
    }
    free(cPointsArray);

    for (i = 0; i < clusters; i++)
    {
        free(cCentArr[i]);
    }
    free(cCentArr);

    return PyResult;
}

static PyMethodDef kmeansMethods[] = {
    {"kmeansP",                   /* the Python method name that will be used */
      (PyCFunction) kmeansP, /* the C-function that implements the Python function and returns static PyObject*  */
      METH_VARARGS,           /* flags indicating parameters accepted for this function */
      PyDoc_STR("this do kmeans amen grade 100")}, /*  The docstring for the function */
    {NULL, NULL, 0, NULL}     /* The last entry must be all NULL as shown to act as sentinel. Python looks for this entry to know that all of the functions for the module have been defined. */
};

static struct PyModuleDef kmeansmodule = {
    PyModuleDef_HEAD_INIT,
    "kmeans_capi", /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,  /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    kmeansMethods /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_kmeans_capi(void)
{
    PyObject *m;
    m = PyModule_Create(&kmeansmodule);
    if (!m) {
        return NULL;
    }
    return m;
}