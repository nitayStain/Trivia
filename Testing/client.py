import socket
import struct
import json


class Client:

    def __init__(self, port=4200):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.sock.connect(("127.0.0.1", port))

    def send(self, code: int, msg: dict):

        data = json.dumps(msg).encode()
        packed_code = code.to_bytes(1, byteorder="little")
        packed_length = len(data).to_bytes(4, byteorder="little")

        print(f"Sending {data.decode()} | [Code: {code}, Length: {len(data)}]")

        self.sock.send(packed_code)
        self.sock.send(packed_length)
        self.sock.send(data if len(data) > 0 else None)

        return self._recv()

    def _recv(self):
        packed_code = self.sock.recv(1)
        packed_length = self.sock.recv(4)
        code = int.from_bytes(packed_code, byteorder="little")
        length = int.from_bytes(packed_length, byteorder="little")
        data = self.sock.recv(length)

        return code, json.loads(data.decode())
