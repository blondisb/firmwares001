import matplotlib.pyplot as plt
import time

# Crear gráfica
plt.ion()
fig, ax = plt.subplots()
ydata = []
xdata = []
line, = ax.plot([], [])
ax.set_xlim(0, 100)
ax.set_ylim(0, 100)

for i in range(100):
    xdata.append(i)
    ydata.append(i)  # línea recta
    line.set_xdata(xdata)
    line.set_ydata(ydata)
    ax.set_xlim(0, max(10, i))
    ax.set_ylim(0, max(10, i))
    plt.pause(0.1)

plt.ioff()
plt.show()
