#ifndef GAME_H
#define GAME_H
#include <assert.h>
#include "board.h"
#include "global.h"

class game
{
public:
    game();
    ~game();
    game(board *b);
    int search_max(char depth, int start);
    int search_min(char depth, int start);
    int alpha_beta_max(int alpha, int beta, char depth, int start);
    int alpha_beta_min(int alpha, int beta, char depth, int start);
    int evaluate();
    int evaluate_new();
    void generate_moves();
    bool generate_attack_BBs();
    unsigned msb_scan(U64 b);
    unsigned lsb_scan(U64 x);
    char *serialize(U64 x);
    void push_moves(char *dest, char sq, char piece);
    void push_single_move(char dest, char sq, char piece);
    void push_promotion(char sq, int shift);
    void print_moves();
    void print_move(const int i);
        //void print_game_moves();
    int check_move(char x, char y);
    bool make_move(const int i);
    void unmake_move();
    bool mate_check();
        //bool king_safe(char index, bool side);
    U64 northEastAttack(char square);
    U64 eastAttack(char square);
    U64 southEastAttack(char square);
    U64 southAttack(char square);
    U64 southWestAttack(char square);
    U64 westAttack(char square);
    U64 northWestAttack(char square);
    U64 northAttack(char square);
    int get_engine_move() {return engine_move;}
    unsigned long get_perft() {return perft;}
    unsigned long get_perft_total() {return perft_total;}
    int popcount(U64 b);
private:
    unsigned long perft;
    unsigned long perft_total;
    char *tz;
    char *dest_sqs;
    board *position;
    int engine_move;
    int move_count;
    int score;
};
#endif
