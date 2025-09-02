from threading import Thread
from collections import deque
import serial, matplotlib.pyplot as plt
import re, sys
import time

# === CONFIGURACIÓN SERIAL ===
PORT = "COM5"   # Ajusta tu puerto
BAUD = 57600

# === BUFFERS ===
buffers = {}          # Se crean dinámicamente según las señales recibidas
time_buffer = deque(maxlen=200)  # Para eje X (tiempo)

# === HILO DE LECTURA SERIAL ===
def serial_thread():
    global buffers, lines

    # expresión regular genérica: signalX= valor
    pattern = re.compile(r"(signal\d+)\s*=\s*([-0-9.]+)")
    
    while True:
        try:
            line = ser.readline().decode("utf-8", errors="ignore").strip()
            matches = pattern.findall(line)

            if matches:
                t = time.time() - start_time
                time_buffer.append(t)

                for name, value in matches:
                    if name not in buffers:
                        # Crear buffer nuevo si aparece una señal nueva
                        buffers[name] = deque(maxlen=200)
                        color = next(color_cycle)
                        lines[name], = ax.plot([], [], label=name, color=color)
                        ax.legend()

                    buffers[name].append(float(value))

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

# === CONFIGURACIÓN GRÁFICA ===
plt.ion()
fig, ax = plt.subplots(figsize=(10, 6))

# Generador de colores (cicla automáticamente)
import itertools
color_cycle = itertools.cycle(["r", "g", "b", "c", "m", "y", "k"])

lines = {}

ax.set_xlabel("Tiempo (s)")
ax.set_ylabel("Valor")
ax.set_title("Señales en tiempo real")
ax.grid(True)
plt.tight_layout()

# === LANZAR HILO ===
t = Thread(target=serial_thread, daemon=True)
t.start()

# === LOOP PRINCIPAL DE PLOTTING ===
while True:
    if len(time_buffer) > 0 and buffers:
        for name in buffers:
            lines[name].set_xdata(list(time_buffer))
            lines[name].set_ydata(list(buffers[name]))

        ax.set_xlim(min(time_buffer), max(time_buffer))

        # Ajustar Y según rango actual de todos los datos
        all_data = [v for buf in buffers.values() for v in buf]
        if all_data:
            ymin, ymax = min(all_data), max(all_data)
            padding = (ymax - ymin) * 0.1 or 1
            ax.set_ylim(ymin - padding, ymax + padding)

    plt.pause(0.01)
