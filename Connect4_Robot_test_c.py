from src.serial_interface import SerialInterface
from src.connect4_game_c import Connect4Game
import sys

def run_command(serial, cmd, arg):
    if(cmd in serial.COMMANDS):
        packet = ser.make_packet(cmd, arg)
        if(DEBUG): print(packet)
        serial.send_packet(packet)
    else:
        print("ERROR: INVALID COMMAND")

DEBUG = 1

DEPTH = 7

ser = SerialInterface()

if __name__ == "__main__":

    if(len(sys.argv) == 1):
        #print("ERROR: MISSING ARGUMENT(S)")
        game = Connect4Game()
        game.new_game(start_depth=DEPTH)
        while(True):
            if(game.turn == 1):
                c = int(input("YOUR MOVE: "))
                game.player_move(c)
            else:
                game.computer_move()
            game.print_game()
            win = game.check_win()
            if(not win): continue
            elif(win > 0):
                print("Player wins")
                break
            else:
                print("Computer wins")
                break
    elif(len(sys.argv) == 2):
        run_command(ser, sys.argv[1], 0x00)
    elif(len(sys.argv) == 3):
        run_command(ser, sys.argv[1], int(sys.argv[2]))
    else:
        print("ERROR: TOO MANY ARGUMENTS")
    ser.close()
