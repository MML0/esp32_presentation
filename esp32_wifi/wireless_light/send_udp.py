import socket
import random

UDP_IP = "192.168.43.54"  # ESP32 IP
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Example: send 1000 bytes in one go
data_bytes = [random.randint(0, 255) for _ in range(1000)]
data_bytes = [0x01]*20000
data_bytes = [0x02]
data_bytes = [0x01]
sock.sendto(bytes(data_bytes), (UDP_IP, UDP_PORT))
