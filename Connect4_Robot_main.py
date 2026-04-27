from src.serial_interface import SerialInterface
from src.connect4_game_c import Connect4Game

import time

def cleanup(serial=None):
    print("Cleaning up...")
    if(serial):
        reset_tray_packet = serial.make_packet("MOVE_TRAY", 0)
        serial.send_packet(reset_tray_packet)
        serial.close()
    print("Done")

def run_robot(serial, game):

    game.new_game(start_turn=1, start_depth=7)

    game.print_game()

    if(game.turn == 1): print("Player's turn...")
    else: print("Computer's turn...")

    while(True):
        try:

            #check for packet
            packet = ser.read_packet()

            #if packet exists
            if(packet):

                #get packet information
                cmd, arg = serial.decode_packet(packet[0], packet[1])
                print(cmd, arg)

                #STARTUP cmd 0x00
                if(cmd == "STARTUP"):
                    print("WARNING: ARDUINO RESTART DETECTED")

                #MOVE_TRAY cmd 0x01
                elif(cmd == "MOVE_TRAY"):
                    print("WARNING: COMMAND MOVE_TRAY NOT EXECUTABLE")

                #DROP_TRAY cmd 0x02
                elif(cmd == "DROP_TRAY"):
                    print("WARNING: COMMAND DROP_TRAY NOT EXECUTABLE")

                #REGISTER_MOVE cmd 0x03
                elif(cmd == "REGISTER_MOVE"):

                    #player's move detected
                    if(game.turn == 1):
                        game.player_move(arg) #input player's move
                        game.print_game()
                        if(game.check_win() > 0):
                            print("Player wins!")
                            break;
                        print("Computer's move...")

                    #robot's move detected
                    else:
                        print("Robot move detected")

            #no packet

            #player's turn
            if(game.turn == 1):
                pass #wait for player's move packet

            #computer's turn
            else:
                comp_move = game.computer_move() #get computer's move
                move_packet = serial.make_packet("MOVE_TRAY", comp_move) #make move packet
                drop_packet = serial.make_packet("DROP_TRAY", 0x00) #make drop piece packet
                cal_packet = serial.make_packet("CAL_SENSORS", 0x00) #make calibrate sensors packet
                #print(move_packet)
                #print(drop_packet)
                #print(cal_packet)
                game.print_game()
                serial.send_packet(move_packet) #run command on Arduino
                serial.send_packet(drop_packet) #run command on Arduino
                serial.send_packet(cal_packet) #run command on Arduino
                if(game.check_win() < 0):
                    print("Computer wins!")
                    break
                print("Player's move...")
                pass


        except KeyboardInterrupt:
            #cleanup(serial=ser)
            print("KeyboardInterrupt: Stopping Game")
            break
    pass



ser = SerialInterface()
gm = Connect4Game()

run_robot(ser, gm)

cleanup(serial=ser)


