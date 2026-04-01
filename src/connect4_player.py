import random as rand
import numpy as np

class Connect4AI:

    ROWS = 6
    COLS = 7

    ROW_MASKS = [sum(1 << (c*7 + r) for c in range(7)) for r in range(6+1)]
    COL_MASKS = [0b1111111 << 7*c for c in range(7)]
    NUM_FEATURES = 36
    n = 1/(1.1 * np.array([4, 6, 5, 3, 2, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 3, 6, 5, 4, 2, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7]))
    w = (1/n) * np.array([100000000,1000,1000,10000,100000,5,10,15,20,15,10,5,5,5,10,10,15,15,-100000000,-1000,-1000,-10000,-100000,-5,-10,-15,-20,-15,-10,-5,-5,-5,-10,-10,-15,-15])

    def __init__(self):
        pass

    def move_available(self, c, player_board, opponent_board):
        choice_mask = 0b0100000 << ((self.ROWS+1) * c)
        return (player_board | opponent_board) & choice_mask == 0

    def get_move(self, c, player_board, opponent_board):
        if(not self.move_available(c, player_board, opponent_board)):
            raise ValueError('MOVE UNAVAILABLE')
        all_moves = (player_board | opponent_board)
        move = (all_moves & self.COL_MASKS[c]) + (1 << (self.ROWS+1)*c)
        return move

    def moves_available(self, player_board, opponent_board):
        moves = []
        all_moves = player_board | opponent_board
        top_row = all_moves & self.ROW_MASKS[5]
        for c in rand.sample(range(self.COLS), self.COLS):
            if(top_row & 0b0100000 << ((self.ROWS+1) * c) == 0):
                moves.append(c)
        return moves

    def check_win(self, player):
        cols = player & (player << 1) & (player << 2) & (player << 3)
        rows = player & (player << 7) & (player << 14) & (player << 21)
        u_diag = player & (player << 8) & (player << 16) & (player << 24)
        d_diag = player & (player << 6) & (player << 12) & (player << 18)
        return cols | rows | u_diag | d_diag > 0

    def num_wins(self, player):
        cols = player & (player << 1) & (player << 2) & (player << 3)
        rows = player & (player << 7) & (player << 14) & (player << 21)
        u_diag = player & (player << 8) & (player << 16) & (player << 24)
        d_diag = player & (player << 6) & (player << 12) & (player << 18)
        return cols.bit_count() + rows.bit_count() + u_diag.bit_count() + d_diag.bit_count()

    def wins(self, player):
        cols = player & (player << 1) & (player << 2) & (player << 3)
        rows = player & (player << 7) & (player << 14) & (player << 21)
        u_diag = player & (player << 8) & (player << 16) & (player << 24)
        d_diag = player & (player << 6) & (player << 12) & (player << 18)
        return cols | rows | u_diag | d_diag

    def fill_feat_vec(self, player, opponent):

        x = []

        # wins
        my_wins = self.wins(player).bit_count()
        op_wins = self.wins(opponent).bit_count()

        #3 in a rows
        my_possible_3s = 0
        op_possible_3s = 0
        directions = [1, 7]
        for d in directions:
            my_OOO_ = (player << d) & (player << 2*d) & (player << 3*d)
            my_OO_O = (player << d) & (player << 2*d) & (player >> d)
            my_O_OO = (player << d) & (player >> d) & (player >> 2*d)
            my__OOO = (player >> d) & (player >> 2*d) & (player >> 3*d)

            my_open_seq = (my_OOO_ | my_OO_O | my_O_OO | my__OOO) & (~self.ROW_MASKS[6])
            my_possible_3s = my_possible_3s | my_open_seq


            op_OOO_ = (opponent << d) & (opponent << 2*d) & (opponent << 3*d)
            op_OO_O = (opponent << d) & (opponent << 2*d) & (opponent >> d)
            op_O_OO = (opponent << d) & (opponent >> d) & (opponent >> 2*d)
            op__OOO = (opponent >> d) & (opponent >> 2*d) & (opponent >> 3*d)

            op_open_seq = (op_OOO_ | op_OO_O | op_O_OO | op__OOO) & (~self.ROW_MASKS[6])
            op_possible_3s = op_possible_3s | op_open_seq

        open_spaces = sum(0b0111111 << 7*c for c in range(self.COLS)) & ~(player | opponent)
        immediately_open_spaces = 0
        for c in range(self.COLS):
            if(self.move_available(c, player, opponent)):
                immediately_open_spaces = immediately_open_spaces | self.get_move(c, player, opponent)

        # open 3 in a rows
        my_open_3s = (my_possible_3s & open_spaces).bit_count()
        op_open_3s = (op_possible_3s & open_spaces).bit_count()

        # immediate threats
        my_threats = (my_possible_3s & immediately_open_spaces).bit_count()
        op_threats = (op_possible_3s & immediately_open_spaces).bit_count()

        # open double win spaces
        P = (my_possible_3s & open_spaces)
        O = (op_possible_3s & open_spaces)
        my_open_doubles = 0
        op_open_doubles = 0
        for d in directions:
            my_open_doubles = my_open_doubles | (P & (P >> d))
            op_open_doubles = op_open_doubles | (O & (O >> d))
        my_open_doubles = my_open_doubles.bit_count()
        op_open_doubles = op_open_doubles.bit_count()

        # double threats
        P = (my_possible_3s & immediately_open_spaces)
        O = (op_possible_3s & immediately_open_spaces)
        my_double_threats = 0
        op_double_threats = 0
        for d in directions:
            my_double_threats = my_double_threats | (P & (P << d))
            op_double_threats = op_double_threats | (O & (O << d))
        my_double_threats = my_double_threats.bit_count()
        op_double_threats = op_double_threats.bit_count()

        # rows and cols
        my_cols = []
        op_cols = []
        my_rows = []
        op_rows = []
        for i in range(self.COLS):
            my_cols.append((player & self.COL_MASKS[i]).bit_count())
            op_cols.append((opponent & self.COL_MASKS[i]).bit_count())
            my_rows.append((player & self.ROW_MASKS[i]).bit_count())
            op_rows.append((opponent & self.ROW_MASKS[i]).bit_count())

        x.append(my_wins)
        x.append(my_open_3s)
        x.append(my_threats)
        x.append(my_open_doubles)
        x.append(my_double_threats)
        x.extend(my_cols)
        x.extend(my_rows[:6])

        x.append(op_wins)
        x.append(op_open_3s)
        x.append(op_threats)
        x.append(op_open_doubles)
        x.append(op_double_threats)
        x.extend(op_cols)
        x.extend(op_rows[:6])

        return np.array(x)


    def model_query(self, player, opponent, w):
        x = self.fill_feat_vec(player, opponent)
        x_normalized = x * self.n
        #print(x_normalized)
        return np.dot(w, x_normalized)

    def user_query(self, player, opponent):
        move = int(input("YOUR MOVE (COL INDEX): "))
        if(move >= self.COLS):
            raise ValueError("COLUMN INDEX OUT OF RANGE")
        return move

    def minimax(self, player, opponent, w, depth, alpha=float('-inf'), beta=float('inf'), root=True):
        best_val = float('-inf')
        best_move = None
        moves = self.moves_available(player, opponent)

        for move in moves:
            if(depth == 0):
                value = self.model_query(player | move, opponent, w)
                if(value > best_val):
                    best_val = value
                    best_move = move
            else:
                P = player | self.get_move(move, player, opponent)
                value = None
                if(self.check_win(P)):
                    value = float('inf')
                elif((P | opponent).bit_count() == 49):
                    value = 0
                else:
                    value = -1*(self.minimax(opponent, P, w, depth-1, alpha=(-beta), beta=(-alpha), root=False)[0])

                if(value >= best_val):
                    best_val = value
                    best_move = move

                #alpha-beta pruning
                if(not root):
                    alpha = max(alpha, value)
                    if(alpha >= beta):
                        break
            pass
        return best_val, best_move


    def print_board(self, player, opponent):
        b = ''
        for r in reversed(range(self.ROWS+1)):
            for c in range(self.COLS):
                i = c * self.COLS + r
                bit = 1 << i

                if(bit & player > 0):
                    b += "X "
                elif(bit & opponent > 0):
                    b += "O "
                else:
                    if(r != 6):
                        b += ". "
                    else:
                        b += "_ "
            b += '\n'
        print(b)
        pass

    def play_model(self, model=w, initial_depth=5):
        pl1 = 0
        pl2 = 0
        turn = (-1) ** (rand.randint(0,8))
        while(True):
            legal_moves = self.moves_available(pl1, pl2)
            print("MOVES: " + str(legal_moves))
            depth = initial_depth + int((pl1 | pl2).bit_count()*0.1)
            print("DEPTH: " + str(depth))
            if(turn == 1):
                c = self.user_query(pl1, pl2)
                print(c)
                pl1 += self.get_move(c, pl1, pl2)
            else:
                c = self.minimax(pl2, pl1, model, depth)
                print(c)
                pl2 += self.get_move(c[1], pl2, pl1)

            self.print_board(pl1, pl2)

            #check for pl1 win
            if(turn == 1 and self.check_win(pl1)):
                print("USER WINS")
                return
            #check for pl2 win
            elif(self.check_win(pl2)):
                print("MODEL WINS")
                return
            #check for draw
            elif((pl1 | pl2).bit_count() == self.ROWS*self.COLS):
                print("DRAW")
                return

            turn *= -1
            pass
        pass






























