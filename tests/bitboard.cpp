#include <cstdio>
#include <bit>
#include <cstdint>
#include <cinttypes>
using namespace std;

int ROWS;
int COLS;

void print_board(uint64_t player, uint64_t opponent){
    for(int r = ROWS; r >= 0; r--){
        for(int c = 0; c < COLS; c++){
            uint64_t bit = ((uint64_t)1) << (c*COLS + r);
//            if(bit & player) printf("%i, %" PRIu64 "\n", c*COLS+r, bit);
            if(bit & player){
                printf("X ");
            }else if(bit & opponent){
                printf("O ");
            }else if(r != 6){
                printf(". ");
            }else{
                printf("_ ");
            }
        }
        printf("\n");
    }
}


int main(){
    ROWS = 6;
    COLS = 7;
    int64_t ROW_MASKS[] = {0,0,0,0,0,0,0};
    int64_t COL_MASKS[] = {0,0,0,0,0,0,0};

    for(int r = 0; r < ROWS+1; r++){
        printf("%" PRIu64 "\n", ROW_MASKS[r]);
        for(int c = 0; c < COLS; c++){
            //printf("%" PRIu64 "\n", ROW_MASKS[r]);
            ROW_MASKS[r] += (((uint64_t)1) << (c*COLS + r));
        }
        COL_MASKS[r] = ((uint64_t)(0b1111111)) << (7*r);
    }


    printf("\n");
    for(int i = 0; i < COLS; i++){
        print_board(ROW_MASKS[i], 0);
        print_board(0, COL_MASKS[i]);
    }

    for(int i = 0; i < COLS; i++) printf("%" PRIu64 "\n", COL_MASKS[i]);
    printf("\n");
    for(int i = 0; i < COLS; i++) printf("%" PRIu64 "\n", ROW_MASKS[i]);


//    printf("NEW: %i, %i\n",0, __builtin_popcount(0));
//    print_board(0,0);
//    uint64_t bit = 1;
//    for(int b = 0; b < 20; b++){
//        uint64_t cur = bit << b;
//        printf("NEW: %i, %i\n", cur, __builtin_popcount(cur));
//        print_board(cur, 0);
//    }

//    printf("%" PRIu64 "\n", ((uint64_t)1) << 31);

    return 1;
}


