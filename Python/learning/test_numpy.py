import numpy as np



A = np.array([[1,1,0],[0,1,1],[1,1,1],[1,1,2]])

print(A)



B = np.array([[2],[2],[3],[4.1]])

print(B)




X,resid, rank, s=np.linalg.lstsq(A,B,0)

print(X)

print(resid)

print(rank)

print(s) 