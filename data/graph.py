import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('output.csv', delimiter=',')

fig, axs = plt.subplots(2, 3, figsize=(8,8))
axs[0, 0].set_title("XPos")
axs[0, 0].set_xlabel("Y")
axs[0, 0].set_ylabel("Z")
axs[1, 0].set_title("XNeg")
axs[1, 0].set_xlabel("Y")
axs[1, 0].set_ylabel("Z")
axs[0, 1].set_title("YPos")
axs[0, 1].set_xlabel("X")
axs[0, 1].set_ylabel("Z")
axs[1, 1].set_title("YNeg")
axs[1, 1].set_xlabel("X")
axs[1, 1].set_ylabel("Z")
axs[0, 2].set_title("ZPos")
axs[0, 2].set_xlabel("X")
axs[0, 2].set_ylabel("Y")
axs[1, 2].set_title("ZNeg")
axs[1, 2].set_xlabel("X")
axs[1, 2].set_ylabel("Y")

for hit in data:
    copyNo = hit[1]
    if (copyNo == 0):
        axs[0, 0].scatter(hit[4], hit[5], color='red', alpha=0.25)
    if (copyNo == 1):
        axs[1, 0].scatter(hit[4], hit[5], color='red', alpha=0.25)
    if (copyNo == 2):
        axs[0, 1].scatter(hit[3], hit[5], color='red', alpha=0.25)
    if (copyNo == 3):
        axs[1, 1].scatter(hit[3], hit[5], color='red', alpha=0.25)
    if (copyNo == 4):
        axs[0, 2].scatter(hit[3], hit[4], color='red', alpha=0.25)
    if (copyNo == 5):
        axs[1, 2].scatter(hit[3], hit[4], color='red', alpha=0.25)

plt.tight_layout()
plt.show()
