from threading import Thread
from collections import deque
import serial, matplotlib.pyplot as plt
import time, re

# Configura el puerto
PORT = "/dev/ttyUSB1"
BAUD = 9600

# buffers
dataAccX = deque(maxlen=200)
dataRoll = deque(maxlen=200)
dataPitch = deque(maxlen=200)

# función que corre en hilo aparte
def serial_thread():
    pattern = re.compile(
        r"Acc X:\s*([-0-9]+)\s*Y:\s*([-0-9]+)\s*Z:\s*([-0-9]+)\s*\|"
        r"\s*Gyro X:\s*([-0-9]+)\s*Y:\s*([-0-9]+)\s*Z:\s*([-0-9]+)\s*\|"
        r"\s*Roll:\s*([-0-9.]+)\s*Pitch:\s*([-0-9.]+|nan)"
    )
    
    while True:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        match = pattern.search(line)
        if match:
            dataAccX.append(float(match.group(1)))
            dataRoll.append(float(match.group(7)))
            pitch_val = match.group(8)
            dataPitch.append(float(pitch_val) if pitch_val != 'nan' else 0)

# conexión serial
ser = serial.Serial(PORT, BAUD, timeout=0.1)

# lanzar hilo
t = Thread(target=serial_thread, daemon=True)
t.start()

# configuración gráfica
plt.ion()

fig, (ax1, ax2, ax3) = plt.subplots(nrows=3, ncols=1, figsize=(10, 8))

lineAccX, = ax1.plot([], [], label="Acc X", color='r')
lineRoll, = ax2.plot([], [], label="Roll", color='g')
linePitch, = ax3
