import numpy as np
import matplotlib.pyplot as plt
import sys

dirName = sys.argv[1]
outputPath = dirName + "/" + dirName + ".csv"
data = np.loadtxt(outputPath, delimiter=',')
theta = np.array([])
z = np.array([])

for hit in data:
    hitTheta = (hit[1] % 100) * 6
    hitZ = int(hit[0] / 100) * 110
    theta = np.append(theta, hitTheta)
    z = np.append(z, hitZ)

plt.scatter(theta, z, alpha=0.05)
plt.title("G4CPDD Run " + dirName)
plt.xlabel("Angle from +x axis (deg)")
plt.ylabel("Z Position (cm)")
plt.savefig(dirName + "/copyNum_plot_" + dirName + ".png")
