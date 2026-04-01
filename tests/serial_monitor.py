from src.serial_interface import SerialInterface

ser = SerialInterface()

while True:
    try:
        line = ser.read_line_raw()
        if(line): print(line)
    except KeyboardInterrupt:
        ser.close()
        break
