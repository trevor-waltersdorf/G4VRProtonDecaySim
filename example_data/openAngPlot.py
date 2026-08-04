import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('runInfo.csv', delimiter=',')
openAng = data[:,1]

plt.hist(openAng, bins=10)
plt.show()
