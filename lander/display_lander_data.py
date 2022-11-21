
import numpy as np
import matplotlib.pyplot as plt

results = np.loadtxt('lander_data.txt')
fig, axs = plt.subplots(2, sharex = True)
axs[0].plot(results[:, 0], results[:, 1], label='x (m)')
axs[1].plot(results[:, 0], results[:, 2], label='v (m/s)')
axs.
plt.legend()
plt.show()