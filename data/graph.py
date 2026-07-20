
import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('output.csv', delimiter=',')
y = data[:,3]
z = data[:,4]

plt.scatter(y, z)
plt.show()
