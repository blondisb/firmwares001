from threading import Thread
from collections import deque
import serial, matplotlib.pyplot as plt
import time, re
    

# Configura el puerto (cámbialo por el tuyo, ejemplo: /dev/ttyUSB0 o /dev/ttyACM0)
PORT = "/dev/ttyUSB1"
BAUD = 9600


# buffers
dataX = deque(maxlen=200)
dataY = deque(maxlen=200)
dataZ = deque(maxlen=200)

# función que corre en hilo aparte
def serial_thread():
    #pattern = re.compile(r"AngX:\s*([-0-9.]+).*AngY:\s*([-0-9.]+).*AngZ:\s*([-0-9.]+)")
    
    pattern = re.compile(
        r"AngY:\s*([-0-9.]+)\|\s*AngZ:\s*([-0-9.]+)\|\s*AngX:\s*([-0-9.]+)\|\s*PWM_Izq:\s*([0-9]+)\|\s*PWM_Der:\s*([0-9]+)"
    )
    
    while True:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        match = pattern.search(line)
        if match:
            dataX.append(float(match.group(1)))
            dataY.append(float(match.group(2)))
            dataZ.append(float(match.group(3)))

# conexión serial
ser = serial.Serial(PORT, BAUD, timeout=0.1)

# lanzar hilo
t = Thread(target=serial_thread, daemon=True)
t.start()

# configuración gráfica
plt.ion()

fig, (ax1, ax2, ax3) = plt.subplots(nrows=3, ncols=1, figsize=(10, 8))

lineX, = ax1.plot([], [], label="AngX", color='r')
lineY, = ax2.plot([], [], label="AngY", color='g')
lineZ, = ax3.plot([], [], label="AngZ", color='b')

ax1.set_ylim(-180, 180)
ax2.set_ylim(-180, 180)
ax3.set_ylim(-180, 360)

ax1.set_title("AngX")
ax2.set_title("AngY")
ax3.set_title("AngZ")

ax1.legend()
ax2.legend()
ax3.legend()

plt.tight_layout()

# loop principal de plotting
while True:
    #print(dataX)
    #print(dataY)
    #print(dataZ)

    if len(dataX) > 0:
        lineX.set_ydata(list(dataX))
        lineX.set_xdata(range(len(dataX)))
        ax1.set_xlim(0, len(dataX))

        lineY.set_ydata(list(dataY))
        lineY.set_xdata(range(len(dataY)))
        ax2.set_xlim(0, len(dataY))

        lineZ.set_ydata(list(dataZ))
        lineZ.set_xdata(range(len(dataZ)))
        ax3.set_xlim(0, len(dataZ))

        # Mostrar valor actual en la esquina superior
        ax1_text = f"{dataX[-1]:.2f}"
        ax2_text = f"{dataY[-1]:.2f}"
        ax3_text = f"{dataZ[-1]:.2f}"

        # Limpiar texto anterior
        for txt in ax1.texts:
            txt.remove()
        for txt in ax2.texts:
            txt.remove()
        for txt in ax3.texts:
            txt.remove()
        
        # Añadir texto
        ax1.text(0.95, 0.95, ax1_text, transform=ax1.transAxes,
                 horizontalalignment='right', verticalalignment='top',
                 fontsize=12, color='r')
        ax2.text(0.95, 0.95, ax2_text, transform=ax2.transAxes,
                 horizontalalignment='right', verticalalignment='top',
                 fontsize=12, color='g')
        ax3.text(0.95, 0.95, ax3_text, transform=ax3.transAxes,
                 horizontalalignment='right', verticalalignment='top',
                 fontsize=12, color='b')

    plt.pause(0.01)