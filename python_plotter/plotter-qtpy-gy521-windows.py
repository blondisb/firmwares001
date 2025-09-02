from threading import Thread
from collections import deque
import serial, matplotlib.pyplot as plt
import re, sys
import time

# === CONFIGURACIÓN SERIAL ===
PORT = "COM5"  # Ajusta tu puerto
BAUD = 57600

# === CAMPOS A GRAFICAR ===
CAMPOS = ["roll_rate", "pitch_rate", "yaw_rate"]

# === BUFFERS ===
buffers = {campo: deque(maxlen=200) for campo in CAMPOS}
time_buffer = deque(maxlen=200)  # Para eje X (tiempo)

# === HILO DE LECTURA SERIAL ===
def serial_thread():
    pattern = re.compile(r"roll_rate\s+([-0-9.]+)\s+pitch_rate\s+([-0-9.]+)\s+yaw_rate\s+([-0-9.]+)")
    while True:
        try:
            line = ser.readline().decode("utf-8", errors="ignore").strip()
            match = pattern.search(line)
            if match:
                t = time.time() - start_time  # tiempo relativo en segundos
                time_buffer.append(t)
                buffers["roll_rate"].append(float(match.group(1)))
                buffers["pitch_rate"].append(float(match.group(2)))
                buffers["yaw_rate"].append(float(match.group(3)))
        except Exception as e:
            print("Error en lectura:", e)
            break

# === INICIO SERIAL ===
try:
    ser = serial.Serial(PORT, BAUD, timeout=0.1)
except Exception as e:
    print(f"❌ No se pudo abrir el puerto {PORT}: {e}")
    sys.exit(1)

# Tiempo inicial
start_time = time.time()

# === LANZAR HILO ===
t = Thread(target=serial_thread, daemon=True)
t.start()

# === CONFIGURACIÓN GRÁFICA ===
plt.ion()
fig, ax = plt.subplots(figsize=(10, 6))

# Colores distintos
colors = {"roll_rate": "r", "pitch_rate": "g", "yaw_rate": "b"}
lines = {}
for campo in CAMPOS:
    lines[campo], = ax.plot([], [], label=campo, color=colors[campo])

ax.set_xlabel("Tiempo (s)")
ax.set_ylabel("Velocidad Angular (º/s)")
ax.set_title("Velocidad Angular vs Tiempo")
ax.legend()
ax.grid(True)

plt.tight_layout()

# === LOOP PRINCIPAL DE PLOTTING ===
while True:
    if len(time_buffer) > 0:
        for campo in CAMPOS:
            lines[campo].set_xdata(list(time_buffer))
            lines[campo].set_ydata(list(buffers[campo]))
        ax.set_xlim(min(time_buffer), max(time_buffer))
        # Ajustar Y según rango actual de todos los datos
        all_data = [v for campo in CAMPOS for v in buffers[campo]]
        if all_data:
            ymin, ymax = min(all_data), max(all_data)
            padding = (ymax - ymin) * 0.1 or 1
            ax.set_ylim(ymin - padding, ymax + padding)
    plt.pause(0.01)
