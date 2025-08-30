import serial
import re
import matplotlib.pyplot as plt
from collections import deque

# Configura el puerto (cámbialo por el tuyo, ejemplo: /dev/ttyUSB0 o /dev/ttyACM0)
PORT = "/dev/ttyUSB1"
BAUD = 9600

# Conexión al Arduino
ser = serial.Serial(PORT, BAUD, timeout=1)

# Buffers para graficar (últimos 200 puntos)
N = 200
angx_data = deque(maxlen=N)
angy_data = deque(maxlen=N)
angz_data = deque(maxlen=N)
pwm_izq_data = deque(maxlen=N)
pwm_der_data = deque(maxlen=N)

# Configurar gráfica
plt.ion()
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 6))

line_angx, = ax1.plot([], [], label="AngX")
line_angy, = ax1.plot([], [], label="AngY")
line_angz, = ax1.plot([], [], label="AngZ")
ax1.set_ylim(-90, 90)
ax1.set_title("Ángulos (grados)")
ax1.legend()
ax1.grid()

line_pwm_izq, = ax2.plot([], [], label="PWM Izq")
line_pwm_der, = ax2.plot([], [], label="PWM Der")
ax2.set_ylim(0, 255)
ax2.set_title("Señales PWM")
ax2.legend()
ax2.grid()

plt.tight_layout()

# Expresión regular para parsear la línea
pattern = re.compile(
    r"AngX:\s*([-0-9.]+).*PWM_Izq:\s*([0-9]+).*PWM_Der:\s*([0-9]+).*AngY:\s*([-0-9.]+).*AngZ:\s*([-0-9.]+)"
)

try:
    while True:
        line = ser.readline().decode("utf-8", errors="ignore").strip()
        if not line:
            continue

        match = pattern.search(line)
        if match:
            angx = float(match.group(1))
            pwm_izq = int(match.group(2))
            pwm_der = int(match.group(3))
            angy = float(match.group(4))
            angz = float(match.group(5))

            # Guardar datos en buffer
            angx_data.append(angx)
            angy_data.append(angy)
            angz_data.append(angz)
            pwm_izq_data.append(pwm_izq)
            pwm_der_data.append(pwm_der)

            # Actualizar gráficas
            line_angx.set_ydata(list(angx_data))
            line_angx.set_xdata(range(len(angx_data)))

            line_angy.set_ydata(list(angy_data))
            line_angy.set_xdata(range(len(angy_data)))

            line_angz.set_ydata(list(angz_data))
            line_angz.set_xdata(range(len(angz_data)))

            line_pwm_izq.set_ydata(list(pwm_izq_data))
            line_pwm_izq.set_xdata(range(len(pwm_izq_data)))

            line_pwm_der.set_ydata(list(pwm_der_data))
            line_pwm_der.set_xdata(range(len(pwm_der_data)))

            ax1.set_xlim(0, len(angx_data))
            ax2.set_xlim(0, len(pwm_izq_data))

            plt.pause(0.01)

except KeyboardInterrupt:
    print("Cerrando programa...")
    ser.close()


plt.ioff()   # desactivar modo interactivo
plt.show()   # mostrar la ventana y no cerrarla