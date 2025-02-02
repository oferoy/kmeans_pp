from setuptools import Extension, setup

module = Extension("kmeans_capi", sources=['kmeans_arrays.c','kmeansmodule.c'])
setup(name='kmeans_capi',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])