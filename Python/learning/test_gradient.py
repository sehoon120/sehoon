import numpy as np
import matplotlib as plt

def diff (f,x):
    h = 1e-4
    return (f(x+h) - f(x-h)) / (2*h)

def func(x):
    return (x[0] ** 2 + x[1] ** 2)

'''
def f_tmp(x):
    return x*x+ 4**2

x = [3,4]

print(f_tmp(3))
'''

def num_grad(f,x):
    h = 1e-4
    grad = np.zeros_like(x)

    for i in range(x.size):
        t = x[i]
        x[i] = t+h
        a = f(x)
        x[i] = t-h
        b = f(x)
        grad[i] = (a-b)/(2*h)
        x[i] = t

    return grad

def dec(f,ix,ir=0.01,step=100):
    x=ix
    for i in range(step):
        grad = num_grad(f,x)
        x -= ir*grad
    return x


#x = np.array([-3.,4.])

#print(num_grad(func, x))

#print(dec(func,x,0.1))

print(np.random.randn(2,3))