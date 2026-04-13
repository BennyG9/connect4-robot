from src.connect4ai import Connect4AI
import random as rand
import time

class Connect4Game:



    def __init__(self):
        self.gameAI = Connect4AI()
        self.runtime = []
        #self.new_game()
        pass

    def new_game(self, start_turn=0, start_depth=5):
        self.runtime = []
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
        print(c)
        self.player = self.player | c
        self.turn *= -1
        pass

    def computer_move(self):
        t = time.time()

        val, c = (self.gameAI.minimax_root(self.opponent, self.player, self.gameAI.get_def_weights(), self.depth))
        c = int(c)

        #self.print_board_data()

        print(c, val)

        self.opponent = self.opponent | self.gameAI.get_move(c, self.player, self.opponent)
        self.turn *= -1

        t = time.time() - t
        self.runtime.append(t)
        self.update_depth()
        return c

    def update_depth(self):
        board = self.player | self.opponent
        num_moves = 2 * len(self.runtime)
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

    def print_board_data(self):
        f = self.gameAI.fill_feat_vec(self.player, self.opponent)
        q1, q2 = self.gameAI.model_query(self.player, self.opponent, self.gameAI.get_def_weights()), self.gameAI.model_query(self.opponent, self.player, self.gameAI.get_def_weights())
        print()
        print(f"p wins:\t{f[0]}")
        print(f"p 3s:\t{f[1]}")
        print(f"p thr:\t{f[2]}")
        print(f"p doub:\t{f[3]}")
        print(f"p 2thr:\t{f[4]}")
        #print(f"p col0:\t{f[5]}")
        #print(f"p col1:\t{f[6]}")
        #print(f"p col2:\t{f[7]}")
        #print(f"p col3:\t{f[8]}")
        #print(f"p col4:\t{f[9]}")
        #print(f"p col5:\t{f[10]}")
        #print(f"p col6:\t{f[11]}")
        #print(f"p row0:\t{f[12]}")
        #print(f"p row1:\t{f[13]}")
        #print(f"p row2:\t{f[14]}")
        #print(f"p row3:\t{f[15]}")
        #print(f"p row4:\t{f[16]}")
        #print(f"p row5:\t{f[17]}")

        print()

        print(f"o wins:\t{f[18]}")
        print(f"o 3s:\t{f[19]}")
        print(f"o thr:\t{f[20]}")
        print(f"o doub:\t{f[21]}")
        print(f"o 2thr:\t{f[22]}")
        #print(f"o col0:\t{f[23]}")
        #print(f"o col1:\t{f[24]}")
        #print(f"o col2:\t{f[25]}")
        #print(f"o col3:\t{f[26]}")
        #print(f"o col4:\t{f[27]}")
        #print(f"o col5:\t{f[28]}")
        #print(f"o col6:\t{f[29]}")
        #print(f"o row0:\t{f[30]}")
        #print(f"o row1:\t{f[31]}")
        #print(f"o row2:\t{f[32]}")
        #print(f"o row3:\t{f[33]}")
        #print(f"o row4:\t{f[34]}")
        #print(f"o row5:\t{f[35]}")

        print()

        print(q1, q2)

        print()

        player = self.player
        opponent = self.opponent

        COLS = 7
        ROWS = 6

        ROW_MASKS = [sum(1 << (c*COLS + r) for c in range(COLS)) for r in range(ROWS+1)]
        COL_MASKS = [0b1111111 << 7*c for c in range(COLS)]

        my_possible_3s = 0
        op_possible_3s = 0
        directions = [1, 7]
        for d in directions:
            my_OOO_ = (player << d) & (player << 2*d) & (player << 3*d)
            my_OO_O = (player << d) & (player << 2*d) & (player >> d)
            my_O_OO = (player << d) & (player >> d) & (player >> 2*d)
            my__OOO = (player >> d) & (player >> 2*d) & (player >> 3*d)

            my_open_seq = (my_OOO_ | my_OO_O | my_O_OO | my__OOO) & (~ROW_MASKS[6])
            my_possible_3s = my_possible_3s | my_open_seq


            op_OOO_ = (opponent << d) & (opponent << 2*d) & (opponent << 3*d)
            op_OO_O = (opponent << d) & (opponent << 2*d) & (opponent >> d)
            op_O_OO = (opponent << d) & (opponent >> d) & (opponent >> 2*d)
            op__OOO = (opponent >> d) & (opponent >> 2*d) & (opponent >> 3*d)

            op_open_seq = (op_OOO_ | op_OO_O | op_O_OO | op__OOO) & (~ROW_MASKS[6])
            op_possible_3s = op_possible_3s | op_open_seq

        open_spaces = sum(0b0111111 << 7*c for c in range(COLS)) & ~(player | opponent)
        immediately_open_spaces = 0
        for c in range(COLS):
            immediately_open_spaces = immediately_open_spaces | self.gameAI.get_move(c, player, opponent)

        self.gameAI.print_board(my_possible_3s & open_spaces, 0)
        print()
        self.gameAI.print_board(0, op_possible_3s & open_spaces)

        pass








