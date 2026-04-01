from src.connect4ai import Connect4AI
import random as rand

class Connect4Game:



    def __init__(self):
        self.gameAI = Connect4AI()
        #self.new_game()
        pass

    def new_game(self, start_turn=0, start_depth=5):
        self.player = 0
        self.opponent = 0
        self.depth = start_depth
        if(start_turn == 0):
            self.turn = (-1) ** rand.randint(0,1)
        else:
            self.turn = start_turn / abs(start_turn)
        pass

    def player_move(self, move):
        c = self.gameAI.get_move(move, self.player, self.opponent)
        print(type(move))
        self.player = self.player | c
        self.turn *= -1
        pass

    def computer_move(self):
        self.update_depth()
        c = int(self.gameAI.minimax(self.opponent, self.player, self.gameAI.W, self.depth)[1])
        self.opponent = self.opponent | self.gameAI.get_move(c[1], self.player, self.opponent)
        self.turn *= -1
        return c[1]

    def update_depth(self):
        pass

    def check_win(self):
        if(self.gameAI.check_win(self.player)):
            return 1
        elif(self.gameAI.check_win(self.opponent)):
            return -1
        return 0

    def print_game(self):
        self.gameAI.print_board(self.player, self.opponent)
        pass










