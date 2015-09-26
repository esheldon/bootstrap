"""
bootstrap

methods
-------
res = bootstrap.bootstrap(data, nboot, seed=None)

classes
-------
b = bootstrap.Bootstrap(data, seed=None)
b.go(nboot)
res = b.get_result()
"""
from . import bootlib

from .bootlib import Bootstrap, bootstrap
