#include <iostream>
#include <string>
#include <climits>

#include "macros.txt"
#include "eng_global.h"

#include "game.h"
#include "board.h"

using namespace std;

bool convert_algebraic(string x, string y, int &xx, int &yy);
bool get_user_color();
void get_depth();
void set_debug_mode();
int get_user_move(int &x, int &y, game &play, string command);
void unmake_move(game &play, board &board);
int get_engine_move(game &play, board &board);
void make_move(int move_index, game &play, board &board);
void manage_arguments(board board, int arcg, char *argv[]);
void print_depth() {cout << "Depth: " << static_cast<int>(DEPTH) << endl;}
void print_debug_mode() {cout << "Debug mode: " << DEBUG_MODE << endl;}

int main(int argc, char *argv[]) {
    string   command;
    string   from;
    string   dest;
    string   input;
    int      x, y;
    int      move_index;
    unsigned char move_count;
    bool     valid;
    bool     eng_v_eng = false;
    board    board;
    game     play(&board);
    bool     user_color, eng_color;
    
    assert(board.integrity());
    
    DEPTH = 5;
    DEBUG_MODE = false;
    
    manage_arguments(board, argc, argv);
    
    print_depth();
    print_debug_mode();
    
    cout << "\n\n\t\tWelcome to Tony's Chess Engine!\n\n\n";
    board.print_char_board();
    play.generate_moves();
    
    user_color = get_user_color();
    eng_color = !user_color;
    
    
    while(true) {
        
        if((board.get_side() == user_color) && (!eng_v_eng)) {
            
            if(board.get_move_count()) {
                
                while(true) {
                    std::cout << "\n\t\tEnter a command: ";
                    std::cin >> command;
                    if(command.length() == 2) {
                        move_index = get_user_move(x, y, play, command);
                        break;
                    }
                    else if(command.compare("undo") == 0) {
                        unmake_move(play, board);
                    }
                    else if(command.compare("help") == 0) {
                        std::cout << "\n\t\t:::::HELP MENU:::::\n\tType any of the following commands to learn more.";
                        std::cout << "\n\t'move'\n\t'undo'\n\t'set_depth'\n\t'debug'\n\t'observe'\n\t'quit'";
                    }
                    else if(command.compare("observe") == 0) {
                        eng_v_eng = true;
                        break;
                    }
                    else if(command.compare("set_depth") == 0) { get_depth(); }
                    else if(command.compare("debug") == 0) { set_debug_mode(); }
                    else if(command.compare("quit") == 0) {
                        board.print_game_moves();
                        exit(1);
                    }
                    else {
                        std::cout << "\n\t\tInvalid command. Enter 'help' for options.";
                    }
                }
            }
            else {
                std::cout << "\n\t\tCheck Mate!";
                exit(1);
            }
        }
        
        //Engine's turn
        if((board.get_side() != user_color) || eng_v_eng) {
            move_index = get_engine_move(play, board);
        }
        
        make_move(move_index, play, board);
    }
}


////////////////////////////////////////////////////
//Get User Input & Set Values Accordingly
///////////////////////////////////////////////////

void manage_arguments(board board, int argc, char *argv[]) {
    for(int i=0; i < argc; i++) {
        if(strcmp(argv[i], "-fen") == 0) {
            assert(board.import_FEN(argv[i+1], argv[i+2], argv[i+3], argv[i+4], argv[i+5], argv[i+6]));
            assert(board.integrity());
            i += 6;
        }
        else if((strcmp(argv[i], "-depth") == 0) || (strcmp(argv[i], "-d") == 0)) {
            DEPTH = atoi(argv[i+1]);
            i++;
        }
        else if((strcmp(argv[i],"-debug") == 0) || (strcmp(argv[i],"-db") == 0)) {
            DEBUG_MODE = true;
        }
    }
}

void make_move(int move_index, game &play, board &board) {
    int move_count = board.get_move_count();
    
    if(move_index != NOT_MOVE) {                 //is human players move in the generated valid move list?
        if(play.make_move(move_index)) {          //make move fxn checks for king safety, and legal castling (not checked for in move generator)
            board.clear_moves(0, move_count);             //clear move list, don't clear the generated dark moves since those are
            
            board.print_char_board();                     //print the new valid position
            std::cout << "\tLast Move:";
            board.print_algebraic(board.get_game_mv_cnt()-1);
            cout << "\n\tscore: " << play.evaluate() << "\n\n";
            
        }
        else {
            std::cout << "\n\n\t\tInvalid Move.\n\t\tLeave's King in Check or castles illegally.\n\n";
        }
    }
    else {
        std::cout << "\n\n\t\tInvalid Move.\n\n";
    }
}

int get_engine_move(game &play, board &board) {
    if(board.get_side() == LIGHT) {
        if(!DEBUG_MODE)
            play.alpha_beta_max(INT_MIN, INT_MAX, DEPTH, 0);
        else
            play.search_max(DEPTH, 0);
    }
    else {
        if(!DEBUG_MODE)
            play.alpha_beta_min(INT_MIN, INT_MAX, DEPTH, 0);
        else
            play.search_min(DEPTH, 0);
    }
    
    if(DEBUG_MODE)
        std::cout << "\n\tperft_total:\t" << play.get_perft_total() << std::endl;
    
    std::cout << "engine move: " << static_cast<int>(play.get_engine_move()) << std::endl;
    
    return play.get_engine_move();
}

void unmake_move(game &play, board &board) {
    if(board.get_game_mv_cnt() > 2) {
        play.unmake_move();
        play.unmake_move();
        board.clear_all_moves();
        play.generate_moves();
        board.print_char_board();
    }
}

int get_user_move(int &x, int &y, game &play, string command) {
    string from, dest;
    int move_index;
    
    from = command;
    std::cin >> dest;
    while(true) {
        if(from.length() == 2 && dest.length() == 2) {
            convert_algebraic(from, dest, x, y);
            std::cout << "\n\t\tconversion result: " << x << " " << y << std::endl;
            move_index = play.check_move(x, y);
            break;
        }
        else {
            std::cout << "\n\t\tInvalid Move Entry.";
        }
    }
    return move_index;
}

void set_debug_mode() {
    if(DEBUG_MODE) {
        DEBUG_MODE = false;
        std::cout << "\n\t\tDebug mode is now off.";
    }
    else {
        DEBUG_MODE = true;
        std::cout << "\n\t\tDebug mode is now on.";
    }
}

void get_depth() {
    int depth;
    std:cout << "\n\t\tSet depth: ";
    std::cin >> depth;
    DEPTH = depth;
    std::cout << "\n\t\tDepth set.";
}

bool get_user_color() {
    bool user_color;
    string input;
    
    while(true) {
        std::cout << "\n\t\tWould you like to play as light or dark? ";
        std::cin >> input;
        if(input.compare("light") == 0) {
            user_color = LIGHT;
            break;
        }
        else if(input.compare("dark") == 0) {
            user_color = DARK;
            break;
        }
        else
            std::cout << "\n\t\tInvalid Entry.";
    }
    return user_color;
}

////////////////////////////////////////////////////
//convert user input from algebriac to index
///////////////////////////////////////////////////
bool convert_algebraic(string x, string y, int &xx, int &yy) {
    
    switch (x[0]) {
        case 'a':
            xx = 0;
            break;
        case 'b':
            xx = 1;
            break;
        case 'c':
            xx = 2;
            break;
        case 'd':
            xx = 3;
            break;
        case 'e':
            xx = 4;
            break;
        case 'f':
            xx = 5;
            break;
        case 'g':
            xx = 6;
            break;
        case 'h':
            xx = 7;
            break;
    }
    xx = xx + (static_cast<int>(x[1])-49)*8;
    
    switch (y[0]) {
        case 'a':
            yy = 0;
            break;
        case 'b':
            yy = 1;
            break;
        case 'c':
            yy = 2;
            break;
        case 'd':
            yy = 3;
            break;
        case 'e':
            yy = 4;
            break;
        case 'f':
            yy = 5;
            break;
        case 'g':
            yy = 6;
            break;
        case 'h':
            yy = 7;
            break;
    }
    yy = yy + (static_cast<int>(y[1])-49)*8;
    
    return true;
}
