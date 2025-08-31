import serial
import re
from threading import Thread
from collections import deque
import matplotlib.pyplot as plt

# Configuración del puerto
PORT = "/dev/ttyUSB1"
BAUD = 9600

# Buffers
N = 200
angx_data = deque(maxlen=N)
angy_data = deque(maxlen=N)
angz_data = deque(maxlen=N)
pwm_izq_data = deque(maxlen=N)
pwm_der_data = deque(maxlen=N)

# Regex para parsear línea del Arduino
pattern = re.compile(
    r"AngX:\s*([-0-9.]+).*PWM_Izq:\s*([0-9]+).*PWM_Der:\s*([0-9]+).*AngY:\s*([-0-9.]+).*AngZ:\s*([-0-9.]+)"
)

# Función que corre en un hilo para leer el serial
def serial_thread():
    ser = serial.Serial(PORT, BAUD, timeout=0.1)
    while True:
        line = ser.readline().decode("utf-8", errors="ignore").strip()
        if not line:
            continue
        match = pattern.search(line)
        if match:
            angx_data.append(float(match.group(1)))
            pwm_izq_data.append(int(match.group(2)))
            pwm_der_data.append(int(match.group(3)))
            angy_data.append(float(match.group(4)))
            angz_data.append(float(match.group(5)))

# Lanzar hilo
t = Thread(target=serial_thread, daemon=True)
t.start()

# Configurar figura
plt.ion()
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 6))

# Líneas para ángulos
line_angx, = ax1.plot([], [], label="AngX")
line_angy, = ax1.plot([], [], label="AngY")
line_angz, = ax1.plot([], [], label="AngZ")
ax1.set_ylim(-90, 90)
ax1.set_title("Ángulos (grados)")
ax1.legend()
ax1.grid()

# Líneas para PWM
line_pwm_izq, = ax2.plot([], [], label="PWM Izq")
line_pwm_der, = ax2.plot([], [], label="PWM Der")
ax2.set_ylim(0, 255)
ax2.set_title("Señales PWM")
ax2.legend()
ax2.grid()

plt.tight_layout()

# Loop principal de plotting
try:
    while True:
        if len(angx_data) > 0:
            line_angx.set_ydata(list(angx_data))
            line_angx.set_xdata(range(len(angx_data)))

            line_angy.set_ydata(list(angy_data))
            line_angy.set_xdata(range(len(angy_data)))

            line_angz.set_ydata(list(angz_data))
            line_angz.set_xdata(range(len(angz_data)))

            ax1.set_xlim(0, len(angx_data))
            ax1.set_ylim(min(list(angx_data + angy_data + angz_data)) - 5,
                         max(list(angx_data + angy_data + angz_data)) + 5)

        if len(pwm_izq_data) > 0:
            line_pwm_izq.set_ydata(list(pwm_izq_data))
            line_pwm_izq.set_xdata(range(len(pwm_izq_data)))

            line_pwm_der.set_ydata(list(pwm_der_data))
            line_pwm_der.set_xdata(range(len(pwm_der_data)))

            ax2.set_xlim(0, len(pwm_izq_data))
            ax2.set_ylim(0, 260)

        plt.pause(0.01)


except KeyboardInterrupt:
    print("Cerrando programa...")
    
