import numpy as np
a = np.random.randint(1, 10, (4, 5))
b = np.random.randint(1, 10, (5, 4))
print("the first  matrix is:\n ",a)
print("the second matrix is:\n ",b)

mul=np.matmul(a,b)
print("the multiplication of the two matrices is:\n")
print(mul)   
print("the determinant of the multiplication matrix is: ")
deter=np.linalg.det(mul)

print(deter)