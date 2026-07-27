import numpy as np
import matplotlib.pyplot as plt
import sys

file_path = sys.argv[1]
data = np.loadtxt(file_path, delimiter=',')
theta = np.array([])
z = np.array([])

for hit in data:
    r = np.sqrt(hit[3] ** 2 + hit[4] ** 2)
    if (hit[4] >= 0):
        theta = np.append(theta, np.arccos(hit[3] / r))
    else:
        theta = np.append(theta, (np.arccos(-hit[3] / r) + np.pi))
    z = np.append(z, hit[5])

plt.hist2d(theta, z, bins=100)
plt.title("G4CPDD Run " + file_path[:13])
plt.xlabel("Angle from +x axis (rad)")
plt.ylabel("Z Position (mm)")
plt.show()
