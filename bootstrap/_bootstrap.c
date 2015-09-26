/*
 * _bootstrap.c
 *
 * This is a CPython wrapper for bootstrapping code.

  Copyright (C) 2015  Erin Sheldon, BNL.  erin dot sheldon at gmail dot com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Python.h>
#include <stdint.h>
#include <numpy/arrayobject.h> 



#define _U64_MAX 18446744073709551615L

// from http://stackoverflow.com/questions/20175148/64bit-random-number-between-a-range

static uint64_t urand64_full(void) {
    uint64_t hi = lrand48();
    uint64_t md = lrand48();
    uint64_t lo = lrand48();
    return (hi << 42) + (md << 21) + lo;
}

staic int is_power_of_2(uint64_t x) {
    return x == (x & -x);
}

static uint64_t urand64(uint64_t low, uint64_t high) {
  static const uint64_t M = ~(uint64_t)0; 
  uint64_t range = high - low;
  uint64_t to_exclude = is_power_of_2(range) ? 0
                                             : M % range + 1;
  uint64_t res;

  // Eliminate `to_exclude` possible values from consideration.
  while ((res = urand64_full()) < to_exclude) {
  }

  return low + res % range;
}

/*

   Correct input types must be guaranteed at the python level

*/
static PyObject *
PyBootstrap_bootstrap(PyObject* self, PyObject* args) {

    long seed=0, nboot=0;
    long idim1=0, idim2=0, ipt=0, iboot=0;
    npy_intp ndim=0;
    uint64_t npoints=0;
    npy_intp *adims=NULL;
    PyObject* array=NULL;
    PyObject* mean=NULL;
    PyObject* cov=NULL;
    PyObject* scratch=NULL;
    double *ptr=NULL, val=0, fac=0;
    double *sptr=NULL;

    if (!PyArg_ParseTuple(args, (char*)"lOOOlO",
                          &seed, &array, &mean, &cov, &nboot, &scratch)) {
        return NULL;
    }

    srand48(seed);

    ndim = PyArray_SIZE(mean);
    adims = PyArray_DIMS(array);
    npoints= (uint64_t) adims[0];

    // first get the overall mean
    for (ipt=0; ipt<npoints; ipt++) {

        for (idim1=0; idim1<ndim; idim1++) {

            val = *(double *) PyArray_GETPTR2(array, ipt, idim1);
            ptr = (double *) PyArray_GETPTR1(mean, idim1);
            (*ptr) += val;
        }
    }

    for (idim1=0; idim1<ndim; idim1++) {
        ptr = PyArray_GETPTR1(mean, idim1);
        (*ptr) *= (1.0/npoints);

        printf("%lf ", (*ptr));
    }
    printf("\n");

    // assume sptr is contiguous for simplicity
    sptr = PyArray_DATA(scratch);

    for (iboot=0; iboot<nboot; iboot++) {

        memset(sptr, 0, ndim*sizeof(double));

        // draw npoints random indices with replacement
        for (ipt=0; ipt<npoints; ipt++) {

            uint64_t irand = urand64(0, (uint64_t) npoints);
            //printf("irand: %lu\n", irand);

            for (idim1=0; idim1<ndim; idim1++) {

                val = *(double *) PyArray_GETPTR2(array, irand, idim1);

                sptr[idim1] += val;

            } // over dimensions

        } // over random indices

        // now divide by npoints to get the average for this bootstrapp
        // realization

        for (idim1=0; idim1<ndim; idim1++) {
            sptr[idim1] /= npoints;
        }

        // now add to the covariance sums
        for (idim1=0; idim1<ndim; idim1++) {

            double mean1 = *(double *) PyArray_GETPTR1(mean, idim1);
            double diff1=sptr[idim1] - mean1;

            for (idim2=0; idim2<ndim; idim2++) {
                double mean2 = *(double *) PyArray_GETPTR1(mean, idim2);
                double diff2=sptr[idim2] - mean2;

                ptr = PyArray_GETPTR2(cov, idim1, idim2);
                (*ptr) += diff1*diff2;
            }
        }

    } // over bootstrap realizations

    // normalize
    fac = 1.0/(nboot-1.0);
    for (idim1=0; idim1<ndim; idim1++) {
        for (idim2=0; idim2<ndim; idim2++) {
            ptr = PyArray_GETPTR2(cov, idim1, idim2);
            *ptr *= fac;
        }
    }

    Py_RETURN_NONE;
}





static PyMethodDef bootstrap_methods[] = {
    {"bootstrap",      (PyCFunction)PyBootstrap_bootstrap, METH_VARARGS,  "bootstrap the input data"},
    {NULL}  /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3
    static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "_bootstrap",      /* m_name */
        "Defines the bootstrap functions",  /* m_doc */
        -1,                  /* m_size */
        bootstrap_methods,    /* m_methods */
        NULL,                /* m_reload */
        NULL,                /* m_traverse */
        NULL,                /* m_clear */
        NULL,                /* m_free */
    };
#endif


#ifndef PyMODINIT_FUNC  /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
#if PY_MAJOR_VERSION >= 3
PyInit__bootstrap(void) 
#else
init_bootstrap(void) 
#endif
{
    PyObject* m;


#if PY_MAJOR_VERSION >= 3
    m = PyModule_Create(&moduledef);
    if (m==NULL) {
        return NULL;
    }

#else
    m = Py_InitModule3("_bootstrap", bootstrap_methods, "Define bootstrap methods.");
    if (m==NULL) {
        return;
    }
#endif

    import_array();
#if PY_MAJOR_VERSION >= 3
    return m;
#endif
}
