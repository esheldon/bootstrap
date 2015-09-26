# bootstrap
python code to do bootstraps, wrapping fast C code

Working in C we can avoid large memory allocations for the bootstrap subsets

```python
from bootstrap import bootstrap, Bootstrap

mean = array(  [2.5, -5.6] )
cov  = array([ [1.5,  0.2],
               [0.4,  2.7] ])

np = 10000
rdata = cholesky_sample(mean,cov, np)

nboot=1000

ecov = numpy.cov(rdata.T)
print('expected mean:',mean)
print('expected cov: ')
print(ecov/np)

res=bootstrap(rdata, nboot)

print('boot mean:',res['mean'])
print('boot cov:')
print(res['cov'])


# the result is a dict with the following entries
#    A dictionary with entries
#            'mean': mean over the sample
#            'cov': covariance over the sample
#            'err': sqrt of the diagonal of the covariance matrix
#            'seed': the used seed
#            'nboot': number of bootstrap realizations
 
# You can also use an object

b=Bootstrap(data)
b.go(nboot)
res = b.get_result()

```
