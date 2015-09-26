import distutils
from distutils.core import setup, Extension, Command
import os
import sys
import numpy
import glob
import shutil
import platform

if platform.system()=='Darwin':
    extra_compile_args=['-arch','x86_64']
    extra_link_args=['-arch','x86_64']
else:
    extra_compile_args=[]
    extra_link_args=[]
    

sources = ["bootstrap/_bootstrap.c"]

include_dirs=[numpy.get_include()]
ext=Extension("bootstrap._bootstrap", 
              sources,
              extra_compile_args=extra_compile_args, 
              extra_link_args=extra_link_args,
              include_dirs=include_dirs)

description = ("Bootstrap some data")

long_description=open(os.path.join(os.path.dirname(__file__), "README.md")).read()

try:
    from distutils.command.build_py import build_py_2to3 as build_py
except ImportError:
    from distutils.command.build_py import build_py

setup(name="bootstrap", 
      version="0.1",
      description=description,
      long_description=long_description,
      license = "GPL",
      url="https://github.com/esheldon/bootstrap",
      author="Erin Scott Sheldon",
      author_email="erin.sheldon@gmail.com",
      install_requires=['numpy'],
      packages=['bootstrap'],
      ext_modules=[ext],
      cmdclass = {"build_py":build_py})



