#include <vector>
#include <limits>
#include <cstdint>
using namespace std;

#ifndef CONNECT4AI_H
#define CONNECT4AI_H


class Connect4AI {
public:

    static const int ROWS = 6;
    static const int COLS = 7;
    vector<int> W = {100000000,1000,1000,10000,100000,5,10,15,20,15,10,5,5,5,10,10,15,15,-100000000,-1000,-1000,-10000,-100000,-5,-10,-15,-20,-15,-10,-5,-5,-5,-10,-10,-15,-15};

    Connect4AI();

    bool move_available(int c, uint64_t player, uint64_t opponent);
    uint64_t get_move(int c, uint64_t player, uint64_t opponent);
    vector<int> moves_available(uint64_t player, uint64_t opponent);
    bool check_win(uint64_t player);
    int num_wins(uint64_t player);
    uint64_t wins(uint64_t player);
    void print_board(uint64_t player, uint64_t opponent);
    int model_query(uint64_t player, uint64_t opponent, vector<int> weights);
    int user_query();
    vector<int> fill_feat_vec(uint64_t player, uint64_t opponent);
    vector<double> minimax(uint64_t player, uint64_t opponent, vector<int> weights, int depth, double alpha=-numeric_limits<double>::infinity(), double beta=numeric_limits<double>::infinity(), bool root=true);

    uint64_t ROW_MASKS[7] = {0,0,0,0,0,0,0};
    uint64_t COL_MASKS[7] = {0,0,0,0,0,0,0};

};

#endif
