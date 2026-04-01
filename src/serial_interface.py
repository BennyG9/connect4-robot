import serial

#START = 0xAA
#COMMANDS = {
#    "STARTUP": 0x00,
#    "MOVE_TRAY": 0x01,
#    "DROP_TRAY": 0x02,
#    "REGISTER_MOVE": 0x03,
#}

class SerialInterface:

    START = 0xAA
    COMMANDS = {
        "STARTUP": 0x00,
        "MOVE_TRAY": 0x01,
        "DROP_TRAY": 0x02,
        "REGISTER_MOVE": 0x03,
        "CAL_SENSORS": 0x04,
    }

    def __init__(self, port='/dev/serial0', baud=9600):
        self.ser = serial.Serial(port, baud)

    def send_packet(self, packet):
        #print("send_packet")
        self.ser.write(packet)

    def read_packet(self):
        if(self.ser.in_waiting >= 4):
            start = self.ser.read(1)[0]
            if(start == self.START):
                cmd = self.ser.read(1)[0]
                arg = self.ser.read(1)[0]
                checksum = self.ser.read(1)[0]
                if((cmd + arg) % 256 == checksum):
                    return cmd, arg
        return None

    def close(self):
        self.ser.close()

    def make_packet(self, cmd, arg):
        cmd = self.COMMANDS[cmd]
        checksum = (cmd + arg) % 256
        return bytes([self.START,cmd,arg,checksum])

    def decode_packet(self, cmd, arg):
        cmd = [k for k, v in self.COMMANDS.items() if v == cmd]
        return cmd[0], arg

    def read_line_raw(self):
        if(self.ser.in_waiting):
            line_bytes = self.ser.readline()
            if(line_bytes): return line_bytes
        return None
