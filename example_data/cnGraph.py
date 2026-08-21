import numpy as np
import matplotlib.pyplot as plt
import sys

dirName = sys.argv[1]
outputPath = dirName + "/" + dirName + ".csv"
data = np.loadtxt(outputPath, delimiter=',')

totX = np.array([])
totY = np.array([])
totZ = np.array([])

pRadius = 16.9 - 0.25
zCap = 18.1 - 0.25
nPerY = np.array([11, 19, 23, 27, 29, 31, 35, 39, 39, 41, 41, 43, 45, 47, 47, 49, 49, 51, 51, 51, 51, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 51, 51, 51, 51, 49, 49, 47, 47, 45, 43, 41, 41, 39, 39, 35, 31, 29, 27, 23, 19, 11])
cumOffsetTop = np.empty(53)
cumOffsetBot = np.empty(53)
cumOffsetTop[0] = 7200 
for i in range(53): 
		cumOffsetTop[i] = cumOffsetTop[i-1] + nPerY[i-1];

for i in range(53):
		cumOffsetBot[i] = cumOffsetTop[i] + 2221;

rI = np.array([])
aI = np.array([])

for hit in data:
    cn = hit[1]
    # Handle barrel graph
    if (cn < 7200):
        ringIndex = cn / 120
        angIndex = cn % 120
        
        x = pRadius * np.cos(angIndex * 3)
        y = pRadius * np.sin(angIndex * 3)
        z = -17.8 + ringIndex * 0.6

        totX = np.append(totX, x)
        totY = np.append(totY, y)
        totZ = np.append(totZ, z)
        rI = np.append(rI, ringIndex)
        aI = np.append(aI, angIndex)
    # Handle top cap graph

plt.scatter(aI, rI, alpha=0.1)
plt.axis('equal')
plt.show()
