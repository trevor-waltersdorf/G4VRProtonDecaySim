import numpy as np
import matplotlib.pyplot as plt
import random

x = np.array([])
y = np.array([])
z = np.array([])

for i in range(100):
    phi = random.random() * 2. * np.pi
    cosTheta = (random.random() - 0.5) * 2.
    theta = np.arccos(cosTheta)
    
    nx = np.cos(phi) * np.sin(theta)
    x = np.append(x, nx)
    ny = np.sin(phi) * np.sin(theta)
    y = np.append(y, ny)
    z = np.append(z, cosTheta)

fig = plt.figure()
ax = fig.add_subplot(projection='3d')
scatter = ax.scatter(x, y, z)
plt.show()
