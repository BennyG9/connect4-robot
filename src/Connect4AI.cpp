#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <bit>
#include <cstdint>
#include <limits>
#include <algorithm>
using namespace std;

#include "Connect4AI.h"


Connect4AI::Connect4AI(){
    for(int r = 0; r < ROWS+1; r++){
        for(int c = 0; c < COLS; c++){
            ROW_MASKS[r] += (1 << (c*COLS + r)); //fill ROW_MASKS
        }
        COL_MASKS[r] = 0b1111111 << (7*r); //fill COL_MASKS
    }
} //constructor


bool Connect4AI::move_available(int c, int player, int opponent){
    int choice_mask = 0b0100000 << ((ROWS+1)*c);
    print_board(choice_mask, 0);
    return ((player | opponent) & choice_mask) == 0;
}


int Connect4AI::get_move(int c, int player, int opponent){
    if(!move_available(c, player, opponent)){
        return -1;
    }
    int all_moves = player | opponent;
    return (all_moves & COL_MASKS[c]) + (1 << (ROWS+1)*c);
}


vector<int> Connect4AI::moves_available(int player, int opponent){
    vector<int> moves;
    int all_moves = player | opponent;
    int top_row = all_moves & ROW_MASKS[5];
    for(int c = 0; c < COLS; c++){
        if((top_row & 0b0100000 << ((ROWS+1)*c)) == 0){
            moves.push_back(c);
        }
    }
    return moves;
}


bool Connect4AI::check_win(int player){
    int cols = player & (player << 1) & (player << 2) & (player << 3);
    int rows = player & (player << 7) & (player << 14) & (player << 21);
    int u_diag = player & (player << 8) & (player << 16) & (player << 24);
    int d_diag = player & (player << 6) & (player << 12) & (player << 18);
    return (cols | rows | u_diag | d_diag) > 0;
}


int Connect4AI::num_wins(int player){
    int cols = player & (player << 1) & (player << 2) & (player << 3);
    int rows = player & (player << 7) & (player << 14) & (player << 21);
    int u_diag = player & (player << 8) & (player << 16) & (player << 24);
    int d_diag = player & (player << 6) & (player << 12) & (player << 18);
    return __builtin_popcount(cols) + __builtin_popcount(rows) + __builtin_popcount(u_diag) + __builtin_popcount(d_diag);
}


int Connect4AI::wins(int player){
    int cols = player & (player << 1) & (player << 2) & (player << 3);
    int rows = player & (player << 7) & (player << 14) & (player << 21);
    int u_diag = player & (player << 8) & (player << 16) & (player << 24);
    int d_diag = player & (player << 6) & (player << 12) & (player << 18);
    return cols | rows | u_diag | d_diag;
}


void Connect4AI::print_board(int player, int opponent){
    for(int r = ROWS; r >= 0; r--){
        for(int c = 0; c < COLS; c++){
            int bit = 1 << (c*COLS + r);
            if(bit & player) printf("X ");
            else if(bit & opponent) printf("O ");
            else if(r != 6) printf(". ");
            else printf("_ ");
        }
        printf("\n");
    }
}


int Connect4AI::model_query(int player, int opponent, vector<int> weights){
    vector<int> x = fill_feat_vec(player, opponent);
    int dot_product = 0;
    for(int i = 0; i < (int)x.size(); i++){
        dot_product += x.at(i) * weights.at(i);
    }
    return dot_product;
}


int Connect4AI::user_query(){
    string query;
    printf("YOUR MOVE (COL INDEX): ");
    cin >> query;
    return stoi(query);
}


vector<int> Connect4AI::fill_feat_vec(int player, int opponent){
    vector<int> x;

    int my_wins = __builtin_popcount(wins(player));
    int op_wins = __builtin_popcount(wins(opponent));

    int my_possible_3s = 0;
    int op_possible_3s = 0;
    for(int d = 1; d <= 7; d += 6){
         int my_OOO_ = (player << d) & (player << 2*d) & (player << 3*d);
         int my_OO_O = (player << d) & (player << 2*d) & (player >> d);
         int my_O_OO = (player << d) & (player >> d) & (player >> 2*d);
         int my__OOO = (player >> d) & (player >> 2*d) & (player >> 3*d);

         int my_open_seq = (my_OOO_ | my_OO_O | my_O_OO | my__OOO) & (~ROW_MASKS[6]);
         int my_possible_3s = my_possible_3s | my_open_seq;

         int op_OOO_ = (opponent << d) & (opponent << 2*d) & (opponent << 3*d);
         int op_OO_O = (opponent << d) & (opponent << 2*d) & (opponent >> d);
         int op_O_OO = (opponent << d) & (opponent >> d) & (opponent >> 2*d);
         int op__OOO = (opponent >> d) & (opponent >> 2*d) & (opponent >> 3*d);

         int op_open_seq = (op_OOO_ | op_OO_O | op_O_OO | op__OOO) & (~ROW_MASKS[6]);
         int op_possible_3s = op_possible_3s | op_open_seq;
    }

    int open_spaces = ~(player | opponent);
    int immediately_open_spaces = 0;
    for(int c = 0; c < COLS; c++){
        int spot = get_move(c, player, opponent);
        if(spot >= 0) immediately_open_spaces = immediately_open_spaces | spot;
    }

    int my_open_3s = __builtin_popcount(my_possible_3s & open_spaces);
    int op_open_3s = __builtin_popcount(op_possible_3s & open_spaces);

    int my_threats = __builtin_popcount(my_possible_3s & immediately_open_spaces);
    int op_threats = __builtin_popcount(op_possible_3s & immediately_open_spaces);

    int my_open = my_possible_3s & open_spaces;
    int op_open = op_possible_3s & open_spaces;
    int my_im_open = my_possible_3s & immediately_open_spaces;
    int op_im_open = op_possible_3s & immediately_open_spaces;
    int my_open_doubles = 0;
    int op_open_doubles = 0;
    int my_double_threats = 0;
    int op_double_threats = 0;
    for(int d = 1; d <= 7; d += 6){
        my_open_doubles = my_open_doubles | (my_open & (my_open >> d));
        op_open_doubles = op_open_doubles | (op_open & (op_open >> d));
        my_double_threats = my_double_threats | (my_im_open & (my_im_open << d));
        op_double_threats = op_double_threats | (op_im_open & (op_im_open << d));
    }
    my_open_doubles = __builtin_popcount(my_open_doubles);
    op_open_doubles = __builtin_popcount(op_open_doubles);
    my_double_threats = __builtin_popcount(my_double_threats);
    op_double_threats = __builtin_popcount(op_double_threats);

    x.push_back(my_wins);
    x.push_back(my_open_3s);
    x.push_back(my_threats);
    x.push_back(my_open_doubles);
    x.push_back(my_double_threats);

    for(int i = 0; i < COLS; i++){
        x.push_back(__builtin_popcount(player & COL_MASKS[i]));
        if(i < COLS - 1) x.push_back(__builtin_popcount(player & ROW_MASKS[i]));
    }

    x.push_back(op_wins);
    x.push_back(op_open_3s);
    x.push_back(op_threats);
    x.push_back(op_open_doubles);
    x.push_back(op_double_threats);

    for(int i = 0; i < COLS; i++){
        x.push_back(__builtin_popcount(opponent & COL_MASKS[i]));
        if(i < COLS - 1) x.push_back(__builtin_popcount(opponent & ROW_MASKS[i]));
    }

    return x;
}


vector<double> Connect4AI::minimax(int player, int opponent, vector<int> weights, int depth, double alpha, double beta, bool root){
    double best_val = -numeric_limits<double>::infinity();
    int best_move;
    vector<int> moves = moves_available(player, opponent);

    int move;
    double value;
    for(int i = 0; i < (int)moves.size(); i++){
        move = moves.at(i);
        if(depth == 0){
            value = model_query(player | move, opponent, W);
            if(value > best_val){
                best_val = value;
                best_move = move;
            }
        }else{
            int P = player | get_move(move, player, opponent);
            if(check_win(P)) value = numeric_limits<double>::infinity();
            else if((P | opponent) == 49) value = 0.;
            else value = (int)(-1*(minimax(opponent, P, W, depth-1, (-beta), (-alpha), (false)).at(0)));

            if(value >= best_val){
                best_val = value;
                best_move = move;
            }

            if(!root){
                alpha = max(alpha, value);
                if(alpha >= beta) break;
            }
        }
    }

    vector<double> out = {best_val, (double)best_move};
    return out;
}








int main(){
    return 1;
}















