# bootstrap
python code to do bootstraps, wrapping fast C code

```python
import bootstrap
import numpy
from numpy import sqrt

np = 10000
data = numpy.zeros( (np, 2) )
data[:,0] = numpy.random.normal(loc=2.5, scale=1.0, size=np)
data[:,1] = numpy.random.normal(loc=-5.7, scale=0.2, size=np)

nboot=100
seed=3847

print('expected ')
print(data.mean(axis=0))
print(data.std(axis=0)/sqrt(np))

res=bootstrap.bootstrap(data, nboot, seed=seed)

print(res)
```
