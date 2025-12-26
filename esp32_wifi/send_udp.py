import socket

# ESP32 IP and port
ESP32_IP = "192.168.4.1"  # replace with your ESP32 IP
ESP32_PORT = 12345         # must match ESP32 UDP server port

# Create UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Data to send
message = "Hello ESP32 via UDP!"

# data = bytes([0xFF])
# sock.sendto(data, (ESP32_IP, ESP32_PORT))
# print("Sent 0xFF to ESP32")


try:
    # Send data
    sock.sendto(message.encode(), (ESP32_IP, ESP32_PORT))
    print(f"Sent: {message}")

    # Optional: receive reply
    data, addr = sock.recvfrom(1024)
    print(f"Received from ESP32: {data.decode()}")

finally:
    sock.close()
