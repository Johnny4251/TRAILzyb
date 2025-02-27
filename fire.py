import socket
from datetime import datetime
import time

HOST = '127.0.0.1'
PORT = 7887

MORNING_TIME = "07:00:00"
NIGHT_TIME   = "17:00:00"

def main():
    try:
        addr = (HOST, PORT)
        client_sock = socket.socket()
        client_sock.connect(addr)
    except Exception as e:
        print(e)
        return
    
    while True:
        time_now = datetime.now()
        curr_time = time_now.strftime("%H:%M:%S")

        if curr_time == MORNING_TIME or curr_time == NIGHT_TIME:
            event = "capture picture now"
            client_sock.send(event.encode())
        else: time.sleep(0.4)
    



if __name__ == "__main__":
    main()

