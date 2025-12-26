import socket
import matplotlib.pyplot as plt
from collections import deque

UDP_IP = "0.0.0.0"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

# Plot buffer
buf = deque(maxlen=200)

plt.ion()
fig, ax = plt.subplots()
line, = ax.plot([], [])
ax.set_ylim(0, 3.3)     # ESP32 voltage range
ax.set_xlim(0, 200)

while True:
    data, _ = sock.recvfrom(1024)
    v = float(data.decode())
    buf.append(v)

    line.set_xdata(range(len(buf)))
    line.set_ydata(list(buf))

    ax.relim()
    ax.autoscale_view()

    plt.pause(0.001)
