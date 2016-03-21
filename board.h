#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <bitset>
#include <iostream>
#include <vector>
#include <cstring>
#include <assert.h>

#include "global.h"
#include "macros.txt"

//#include "eng_global.h"

typedef uint64_t U64;

class board
{
	friend class game;
	public:
		board();
		bool import_FEN(char *fen, char *side_to_move, char *castling_rights, char *en_passant, char *fifty_move, char *full_moves);
		void print_bit_board(U64 x);
		void print_char_board();
		void clear_all_moves() {move_t.clear();}
	   void clear_moves(int st, int end) {move_t.erase(move_t.begin()+st, move_t.begin()+end);}
	   void clear_move(int index) {move_t.erase(move_t.begin() + index);}
	   char get_move_count() {return move_t.size();}
	   void set_side(bool s) {side = s;}
	   void switch_side() {side = !side;}
	   bool get_side() {return side;}
	   void push_move(int move) {game_t.push_back(move_t[move]);}
	   void pop_move() {game_t.pop_back();}
	   void output_move_t_max_size() {std::cout << "max size: " << move_t.max_size() << std::endl;}
	   bool fifty_move_check() {if(fifty_move_rule >= 50) return true; else return false;}
	   bool integrity();
	   char get_game_mv_cnt() {return game_t.size();}
	   void print_game_moves();
	   void print_generated_moves();
	   void print_specified_move(int i);
	   void print_algebraic(int i);
	private:
	   bool checkmate;
	   bool stalemate;
	   int fifty_move_rule;
	   //char repetition;
	   int full_move_count;
	   bool side;
		U64 pieceBB[14];
		U64 occupied_squares;
		//char16_t battle_field[2][64];
		//char battle_value[6];
		bool can_castle[4];
		bool has_castled[2];
		enum enumPiece
		{
			white_king=0, black_king,
			white_queens, black_queens,
			white_rooks, black_rooks,
			white_knights, black_knights,
			white_bishops, black_bishops,
			white_pawns, black_pawns,
			white_pieces, black_pieces
		};
		enum genPiece {
			king=0, queens=2,rooks=4, knights=6, bishops=8, pawns=10, pieces=12
		};
		enum battle_piece {
		   Bking = 0, Bqueens, Brooks, Bknights, Bbishops, Bpawns
		};
		enum Algebraic_Square
		{
			A1=0, B1, C1, D1, E1, F1, G1, H1,
			A2, B2, C2, D2, E2, F2, G2, H2,
			A3, B3, C3, D3, E3, F3, G3, H3,
			A4, B4, C4, D4, E4, F4, G4, H4,
			A5, B5, C5, D5, E5, F5, G5, H5,
			A6, B6, C6, D6, E6, F6, G6, H6,
			A7, B7, C7, D7, E7, F7, G7, H7,
			A8, B8, C8, D8, E8, F8, G8, H8
		};
	
		struct move_info {
			char move[2];
			bool side;           //LIGHT=0 or DARK=1
			
			char piece;          //king=0, queens=2,rooks=4,bishops=6,pawns=8
			char captured_piece; //king=0, queens=2,rooks=4,bishops=6,pawns=8
			bool capture;        //is this move a capture
		
		   bool canCastle_off[4];
			bool isCastle[4];
			
			bool passant_capture;   //is this move a capture using en passant
			char enPassant;         //if the pawn is pushed two places, set en passant square, along with the square the pawn is on for capture porpoises
			
			bool promotion;
			char promotion_piece;
		};
		move_info move1;
		move_info makeMove;
		std::vector<move_info> game_t;
		std::vector<move_info> move_t;
		char charBoard[64];
};
#endif
