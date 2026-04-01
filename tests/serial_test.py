from src.serial_interface import SerialInterface

ser = SerialInterface()

while True:
    try:
        line = ser.read_line_raw()
        if(line): print(line)
        packet = ser.make_packet("STARTUP", 0x00)
        ser.send_packet(packet)
    except KeyboardInterrupt:
        ser.close()
        break
