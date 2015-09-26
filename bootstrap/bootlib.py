from __future__ import print_function
import numpy
from numpy import sqrt, diag, newaxis
from . import _bootstrap
from copy import deepcopy

def bootstrap(data, nboot, seed=None):
    """
    bootstrap the input data

    parameters
    ----------
    data: array
        [Npoints, Ndim] array or [Npoints] array
    nboot: integer
        Number of bootstrap realizations

    seed: integer, optional
        optional seed

    returns
    -------
    A dictionary with entries
            'mean': mean over the sample
            'cov': covariance over the sample
            'err': sqrt of the diagonal of the covariance matrix
            'seed': the used seed
            'nboot': number of bootstrap realziations
    """
    b=Bootstrap(data, seed=seed)
    b.go(nboot)

    return b.get_result()

class Bootstrap(object):
    """
    bootstrap the input data

    parameters
    ----------
    data: array
        [Npoints, Ndim] array or [Npoints] array

    seed: integer, optional
        optional seed
    """
    def __init__(self, data, seed=None):
        self.set_data(data)
        self.set_seed(seed=seed)

    def get_result(self):
        """
        get the result dict

        The result is a dict with parameters 
            'mean': mean over the sample
            'cov': covariance over the sample
            'err': sqrt of the diagonal of the covariance matrix
            'seed': the used seed
            'nboot': number of bootstrap realziations
        """
        if not hasattr(self,'_result'):
            raise RuntimeError("run go() first")
        return deepcopy(self._result)

    def go(self, nboot):
        """
        run the bootstrapping

        parameters
        ----------
        nboot: integer
            Number of bootstrap realizations
        """
        mean = numpy.zeros(self.ndim)
        cov  = numpy.zeros( (self.ndim, self.ndim) )

        scratch = numpy.zeros(self.ndim)

        _bootstrap.bootstrap(self.seed, self.data, mean, cov, nboot, scratch)


        err=sqrt( diag( cov ) )
        if self.is_scalar:
            mean=mean[0]
            cov=cov[0,0]
            err=err[0]

        self._result={'mean':mean,
                      'cov':cov,
                      'err':err,
                      'seed':self.seed,
                      'nboot':nboot}

    def set_seed(self, seed=None):
        if seed is None:
            seed = numpy.random.randint(0,2**31)
        else:
            seed=int(seed)

        self.seed=seed

    def set_data(self, data):
        if hasattr(self, '_result'):
            del self._result

        data = numpy.array(data, dtype='f8', ndmin=1)
        
        if len(data.shape) == 1:
            self.is_scalar=True
            self.ndim=1
            data = data[:,newaxis]
        elif len(data.shape)==2:
            self.is_scalar=False
            self.ndim=data.shape[1]
        else:
            raise ValueError("expected data shape of [N] or "
                             "[N,Ndim], got %d" % (data.shape))

        self.data=data
