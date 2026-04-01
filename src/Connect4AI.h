#include <vector>
#include <limits>
using namespace std;

#ifndef CONNECT4AI_H
#define CONNECT4AI_H


class Connect4AI {
public:

    static const int ROWS = 6;
    static const int COLS = 7;
    int ROW_MASKS[] = {0,0,0,0,0,0,0};
    int COL_MASKS[] = {0,0,0,0,0,0,0};
    vector<int> W = {100000000,1000,1000,10000,100000,5,10,15,20,15,10,5,5,5,10,10,15,15,-100000000,-1000,-1000,-10000,-100000,-5,-10,-15,-20,-15,-10,-5,-5,-5,-10,-10,-15,-15};

    Connect4AI();

    bool move_available(int c, int player, int opponent);
    int get_move(int c, int player, int opponent);
    vector<int> moves_available(int player, int opponent);
    bool check_win(int player);
    int num_wins(int player);
    int wins(int player);
    void print_board(int player, int opponent);
    int model_query(int player, int opponent, vector<int> weights);
    int user_query();
    vector<int> fill_feat_vec(int player, int opponent);
    vector<double> minimax(int player, int opponent, vector<int> weights, int depth, double alpha=-numeric_limits<double>::infinity(), double beta=numeric_limits<double>::infinity(), bool root=true);

};

#endif
