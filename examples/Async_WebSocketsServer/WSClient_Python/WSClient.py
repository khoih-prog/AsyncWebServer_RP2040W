# Run by $ python3.8 WSClient.py
# From websocket-client package. Install by $ pip3 install websocket-client
import websocket
import time

ws = websocket.WebSocket()
#ws.connect("ws://192.168.2.98/ws")
ws.connect("ws://192.168.2.77/ws")
#ws.connect("ws://192.168.2.103/ws")

while True:
    ws.send("Hello, Server")
    result = ws.recv()
    print(result)
    time.sleep(10)

