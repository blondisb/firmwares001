from threading import Thread
from collections import deque
import serial, matplotlib.pyplot as plt
import time, re
    

# Configura el puerto (cámbialo por el tuyo, ejemplo: /dev/ttyUSB0 o /dev/ttyACM0)
PORT = "/dev/ttyUSB1"
BAUD = 9600


# buffers
data = deque(maxlen=200)

# función que corre en hilo aparte
def serial_thread():
    pattern = re.compile(r"AngX:\s*([-0-9.]+).*")
    while True:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        match = pattern.search(line)
        if match:
            data.append(float(match.group(1)))

# conexión serial
ser = serial.Serial(PORT, BAUD, timeout=0.1)

# lanzar hilo
t = Thread(target=serial_thread, daemon=True)
t.start()

# configuración gráfica
plt.ion()
fig, ax = plt.subplots()
line, = ax.plot([], [])
ax.set_ylim(-90, 90)

while True:
    line.set_ydata(list(data))
    line.set_xdata(range(len(data)))
    ax.set_xlim(0, len(data))
    plt.pause(0.01)
