# bootstrap
python code to do bootstraps, wrapping fast C code

Working in C we can avoid large memory allocations for the bootstrap subsets

```python
np = 10000

mean = array(  [2.5, -5.6] )
cov  = array([ [1.5,  0.2],
               [0.4,  2.7] ])

rdata = cholesky_sample(mean,cov, np)

nboot=1000
seed=3847

ecov = numpy.cov(rdata.T)
print('expected mean:',mean)
print('expected cov: ')
print(ecov/np)

res=bootstrap.bootstrap(rdata, nboot, seed=seed)

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

b=bootstrap.Bootstrap(data, seed=seed)
b.go(nboot)
res = b.get_result()

```
