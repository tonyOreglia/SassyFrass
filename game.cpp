#include <iostream>
#include "game.h"
#include "global.h"

game::game()
{
}

game::game(board *b) {
    position = b;
    score = 0;
    tz = new char[64];
    dest_sqs = new char[64];
    perft = 0;
    perft_total = 0;
}

game::~game() {
    delete [] tz;
    delete [] dest_sqs;
}

int game::evaluate(){
   if(DEBUG_MODE)
       perft++;
    int b_piece_cnt = 0;
    int w_piece_cnt = 0;
    char w_king_pos;
    char b_king_pos;
    int pos = 0;
    short unsigned int w_bishop_count, w_knight_count;
    short unsigned int b_bishop_count, b_knight_count;
    short unsigned int bishop_pos, knight_pos, pawn_pos;

    U64 white_king = position->pieceBB[position->white_king];
    U64 black_king = position->pieceBB[position->black_king];
    U64 white_bishops = position->pieceBB[position->white_bishops];
    U64 black_bishops = position->pieceBB[position->black_bishops];
    U64 white_knights = position->pieceBB[position->white_knights];
    U64 black_knights = position->pieceBB[position->black_knights];
    U64 white_rooks = position->pieceBB[position->white_rooks];
    U64 black_rooks= position->pieceBB[position->black_rooks];
    U64 white_pawns = position->pieceBB[position->white_pawns];
    U64 black_pawns = position->pieceBB[position->black_pawns];
    U64 white_queens = position->pieceBB[position->white_queens];
    U64 black_queens = position->pieceBB[position->black_queens];
    U64 white_pieces = position->pieceBB[position->white_pieces];
    U64 black_pieces = position->pieceBB[position->black_pieces];
    pos += 20000 * (popcount(white_king) - popcount(black_king));
    pos += king_bonus_w[lsb_scan(white_king)] - king_bonus_b[lsb_scan(black_king)];
    w_bishop_count = popcount(white_bishops);
    b_bishop_count = popcount(black_bishops);
    w_piece_cnt = popcount(white_pieces);
    b_piece_cnt = popcount(black_pieces);
    pos += 510 * (popcount(white_rooks) - popcount(black_rooks));
    pos += 320 * (popcount(white_knights) - popcount(black_knights));
    pos += 330 * (popcount(white_bishops) - popcount(black_bishops));
    pos += 100 * (popcount(white_pawns) - popcount(black_pawns));
    pos += 890 * (popcount(white_queens) - popcount(black_queens));

    while(white_bishops) {
        bishop_pos = lsb_scan(white_bishops);
        pos += bishop_bonus_w[bishop_pos];
        white_bishops ^= (1ULL << bishop_pos);
    }
    while(black_bishops) {
        bishop_pos = lsb_scan(black_bishops);
        pos -= bishop_bonus_b[bishop_pos];
        black_bishops ^= (1ULL << bishop_pos);
    }
    while(white_knights) {
        knight_pos = lsb_scan(white_knights);
        pos += knight_bonus[knight_pos];
        white_knights ^= (1ULL << knight_pos);
    }
    while(black_bishops) {
        knight_pos = lsb_scan(black_knights);
        pos -= knight_bonus[knight_pos];
        black_knights ^= (1ULL << knight_pos);
    }
    while(white_pawns) {
        pawn_pos = lsb_scan(white_pawns);
        pos += pawn_bonus_w[pawn_pos];
        white_pawns ^= (1ULL << pawn_pos);
    }
    while(white_pawns) {
        pawn_pos = lsb_scan(black_pawns);
        pos -= pawn_bonus_b[pawn_pos];
        black_pawns ^= (1ULL << pawn_pos);
    }
    if((b_piece_cnt + w_piece_cnt) < 16) {
        pos += king_endgame_bonus_w[lsb_scan(white_king)];
        pos -= king_endgame_bonus_b[lsb_scan(black_king)];
    }
    else {
        pos += king_bonus_b[lsb_scan(white_king)];
        pos -= king_bonus_b[lsb_scan(black_king)];
    }
    if(w_bishop_count >= 2) {
        pos += 15;
    }
    if(b_bishop_count >= 2) {
        pos -= 15;
    }
    return pos;
}


int game::search_max(char depth, int start) {
    if( depth == 0) return evaluate();
    int max = -30000;
    bool root = depth == DEPTH;
    int total = position->move_t.size();
    if(root) {
        move_count = position->move_t.size();
    }
    if(total == start) {
        return -3000;
    }
    for(int j = start; j < total; j++) {
        if(make_move(j)) {
            if(root) {
                position->print_algebraic(position->game_t.size()-1);
                std::cout << "\t\t";
            }
            score = search_min(depth - 1, total);
            if(root) {
                std::cout << perft << std::endl;
                perft_total += perft;
                perft = 0;
            }
            unmake_move();
            position->clear_moves(total, position->move_t.size());
            if(score > max) {
                max = score;
                if(root) engine_move = j;
            }
        }
    }
    return max;
}

int game::search_min(char depth, int start) {
    if( depth == 0) return evaluate();
    int min = 30000;
    bool root = depth == DEPTH;
    int total = position->move_t.size();
    if(root) {
        move_count = position->move_t.size();
    }
    for(int j=start; j < total; j++) {
        if(make_move(j)) {
            if(root) {
                position->print_algebraic(position->game_t.size()-1);
                std::cout << "\t\t";
            }
            score = search_max(depth - 1, total);
            if(root) {
                std::cout << perft << std::endl;
                perft_total += perft;
                perft = 0;
            }
            unmake_move();
            position->clear_moves(total, position->move_t.size());
            if(score < min) {
                min = score;
                if(root) engine_move = j;
            }
        }
    }
    return min;
}

int game::alpha_beta_max(int alpha, int beta, char depth, int start) {
    bool no_moves = true;
    if(depth == 0) return evaluate(); //even: light. odd: dark
    bool root = depth == DEPTH;
    int total = position->move_t.size();
    if(root) {
        move_count = position->move_t.size();
    }
    for(int j = start; j < total; j++) {
        if(make_move(j)) {
            no_moves = false;
            score = alpha_beta_min(alpha, beta, depth - 1, total);
            unmake_move();
            position->clear_moves(total, position->move_t.size());
            if(score >= beta)
                return beta;
            if(score > alpha) {
                alpha = score;
                if(root) engine_move = j;
            }
        }
    }
    if(no_moves) {
        if(mate_check()) {
            return (-25000 - depth);
        }
    }
    return alpha;
}

int game::alpha_beta_min(int alpha, int beta, char depth, int start) {
    bool no_moves = true;
    if( depth == 0) return evaluate(); //even: dark. odd: light
    bool root = depth == DEPTH;
    int total = position->move_t.size();
    if(root) {
        move_count = position->move_t.size();
    }
    for(int j=start; j < total; j++) {
        if(make_move(j)) {
            no_moves = false;
            score = alpha_beta_max(alpha, beta, depth - 1, total);
            unmake_move();
            position->clear_moves(total, position->move_t.size());
            if(score <= alpha)
                return alpha;
            if(score < beta) {
                beta = score;
                if(root) engine_move = j;
            }
        }
    }
    if(no_moves) {
        if(mate_check()) {
            return (25000 + depth);
        }
            //return 1000 + depth;
    }
    return beta;
}

bool game::mate_check() {
    int start = position->move_t.size();
    position->switch_side();
    generate_moves();
    position->switch_side();
    int total = position->move_t.size();
        //std::cout << "# moves: " << position->move_t.size();

    for(int i = start; i < total; i++) {
            //std::cout << "i: " << i << std::endl;
            //std::cout << position->king+position->side << " == ";
            //std::cout << " " << static_cast<int>(position->charBoard[position->move_t[i].move[1]]) << std::endl;
        if(position->charBoard[position->move_t[i].move[1]] == (position->king + position->side)) {
            position->clear_moves(start, total);
                //std::cout << "\n\t\tCheckmate\n";
                //position->print_char_board();
            return true;
        }
    }
    position->clear_moves(start, total);
    return false;
}

void game::generate_moves() {
    if(DEBUG_MODE)
        assert(position->integrity());
    U64 valid_attack_set, temp;
    char sigBit, sq;

    //////////////////////////////////////
    //BISHOP MOVES
    /////////////////////////////////////
    temp = position->pieceBB[position->bishops+position->side];
    while(temp) {
        sq = lsb_scan(temp);
        temp ^= sqBB[sq];

        valid_attack_set = northEastAttack(sq);
        valid_attack_set |= northWestAttack(sq);
        valid_attack_set |= southEastAttack(sq);
        valid_attack_set |= southWestAttack(sq);
        valid_attack_set &= (~position->pieceBB[position->pieces+position->side]);



        if(valid_attack_set) {
            dest_sqs = serialize(valid_attack_set);
            push_moves(dest_sqs, sq, position->bishops+position->side);
        }
    }

    //////////////////////////////////////
    //ROOK MOVES
    /////////////////////////////////////
    char j=0;
    temp = position->pieceBB[position->rooks+position->side];
    while(temp) {
        sq = lsb_scan(temp);
        temp ^= sqBB[sq];

        valid_attack_set = northAttack(sq);
        valid_attack_set |= southAttack(sq);
        valid_attack_set |= eastAttack(sq);
        valid_attack_set |= westAttack(sq);
        valid_attack_set &= (~position->pieceBB[position->pieces+position->side]);

        ///need to turn off castling for rook moves! ?????/////
        while(valid_attack_set) {
            sigBit = lsb_scan(valid_attack_set);
            valid_attack_set ^= sqBB[sigBit];
            push_single_move(sigBit, sq, position->rooks+position->side);

                ///note: the if statements below are necessary so that on the unmake move fxn
                //the castle ability can be re established with confidence
            if(position->can_castle[position->king+position->side] && (sq == 7 || sq == 63))
                position->move_t.back().canCastle_off[position->king+position->side] = true;
            if(position->can_castle[position->queens+position->side] && (sq == 0 || sq == 56))
                position->move_t.back().canCastle_off[position->queens+position->side] = true;
        }
    }
    //////////////////////////////////////
    //QUEEN MOVES
    /////////////////////////////////////
    temp = position->pieceBB[position->queens+position->side];

    while(temp) {
        sq = lsb_scan(temp);
        temp ^= sqBB[sq];

        valid_attack_set = northEastAttack(sq);
        valid_attack_set |= northWestAttack(sq);
        valid_attack_set |= southEastAttack(sq);
        valid_attack_set |= southWestAttack(sq);

        valid_attack_set |= northAttack(sq);
        valid_attack_set |= southAttack(sq);
        valid_attack_set |= eastAttack(sq);
        valid_attack_set |= westAttack(sq);

        valid_attack_set &= (~position->pieceBB[position->pieces+position->side]);


        dest_sqs = serialize(valid_attack_set);
        push_moves(dest_sqs, sq, position->queens+position->side);
    }

    ////////////////////////////////
    //KNIGHT MOVES
    ////////////////////////////////
    temp = position->pieceBB[position->knights+position->side];
    while(temp) {
        sq = lsb_scan(temp);
        temp ^= sqBB[sq];

        valid_attack_set = (knightAtt[sq] & (~position->pieceBB[position->pieces+position->side]));

        dest_sqs = serialize(valid_attack_set);
        push_moves(dest_sqs, sq, position->knights+position->side);
    }

    /////////////////////////////////////////////////////
    //KING MOVES//
    ////////////////////////////////////////////////////
    char king = position->king+position->side;
    temp = position->pieceBB[king];
    U64 b_bad_pawn = sqBB[11] | sqBB[13];
    U64 w_bad_pawn = sqBB[51] | sqBB[53];

    sq = lsb_scan(temp);
    valid_attack_set = (kingAtt[sq] & (~position->pieceBB[position->pieces+position->side]));

	   bool kingside = !((sqBB[sq+1] | sqBB[sq+2]) & position->occupied_squares); //true if the castling squares are open.
    bool queenside = !((sqBB[sq-1] | sqBB[sq-2] | sqBB[sq-3]) & position->occupied_squares); //same

    if((position->side == LIGHT) && (b_bad_pawn & position->pieceBB[position->black_pawns]))//((position->charBoard[11] == (position->black_pawns)) | (position->charBoard[13] == position->black_pawns))) {
    {kingside = false; queenside = false; }
    if((position->side == DARK) && (w_bad_pawn & position->pieceBB[position->white_pawns]))//((position->charBoard[51] == (position->white_pawns)) | (position->charBoard[53] == position->white_pawns))){
    {kingside = false; queenside = false;}


    while(valid_attack_set) {
        sigBit = lsb_scan(valid_attack_set);
        valid_attack_set ^= sqBB[sigBit];
        push_single_move(sigBit, sq, king);
            ////the castle code below can be condensed and made more efficient! Do it!
        if(position->can_castle[king])
            position->move_t.back().canCastle_off[king] = true;
        if(position->can_castle[position->queens + position->side])
            position->move_t.back().canCastle_off[position->queens+position->side] = true;
    }

        //castle kingside
    if(position->can_castle[king] && kingside) {
        push_single_move(sq+2, sq, king);
        position->move_t.back().isCastle[king] = true;
        position->move_t.back().canCastle_off[king] = true;
        if(position->can_castle[position->queens+position->side])
            position->move_t.back().canCastle_off[position->queens+position->side] = true;
   	}
        //castle queenside
   	if(position->can_castle[position->queens+position->side] && queenside) {
        push_single_move(sq-2, sq, king);
        position->move_t.back().isCastle[position->queens+position->side] = true;
        if(position->can_castle[king])
            position->move_t.back().canCastle_off[king] = true;
        position->move_t.back().canCastle_off[position->queens+position->side] = true;
   	}

    //////////////////////////////////////////
    //PAWN MOVES///
    //////////////////////////////////////////
    if(position->side==LIGHT) {
        temp = position->pieceBB[position->white_pawns];

        valid_attack_set = position->pieceBB[position->white_pawns] << 8; //single push
        valid_attack_set &= (~position->pieceBB[position->white_pieces]);
        valid_attack_set &= (~position->pieceBB[position->black_pieces]);

        temp = valid_attack_set;
        while(temp) {
            sq = lsb_scan(temp);
            temp ^= sqBB[sq];
            push_single_move(sq, sq-8, position->white_pawns);
            if(sq >= 56) {push_promotion(sq, -8); }
        }

        valid_attack_set = valid_attack_set << 8; //double push
        valid_attack_set &= (~position->pieceBB[position->white_pieces]);
        valid_attack_set &= (~position->pieceBB[position->black_pieces]);
        valid_attack_set &= row4;

        temp = valid_attack_set;
        while(temp) {
            sq = lsb_scan(temp);
            temp ^= sqBB[sq];
            push_single_move(sq, sq-16, position->white_pawns);
            position->move_t.back().enPassant= sq-8;
        }

        valid_attack_set = position->pieceBB[position->white_pawns]; //attack right
        valid_attack_set = valid_attack_set << 9;
        valid_attack_set &= notAfile;
        valid_attack_set &= (position->pieceBB[position->black_pieces] | sqBB[position->game_t.back().enPassant]);   //position->enPassant[0]);

        while(valid_attack_set) {
            sq = lsb_scan(valid_attack_set);
            valid_attack_set ^= sqBB[sq];
            push_single_move(sq, sq-9, position->white_pawns);
            if(sq == position->game_t.back().enPassant) {position->move_t.back().passant_capture = true;}
            if(sq >= 56) {push_promotion(sq, -9);}
        }

        valid_attack_set = position->pieceBB[position->white_pawns]; //attack left
        valid_attack_set = valid_attack_set << 7;
        valid_attack_set &= notHfile;
        valid_attack_set &= (position->pieceBB[position->black_pieces] | sqBB[position->game_t.back().enPassant]);

        while(valid_attack_set) {
            sq = lsb_scan(valid_attack_set);
            valid_attack_set ^= sqBB[sq];
            push_single_move(sq, sq-7, position->white_pawns);
            if(sq == position->game_t.back().enPassant) {position->move_t.back().passant_capture = true;}
            if(sq >= 56) {push_promotion(sq, -7);}
        }

    }
    //DARK//
    else {
        temp = position->pieceBB[position->black_pawns];

        valid_attack_set = position->pieceBB[position->black_pawns] >> 8; //single push
        valid_attack_set &= (~position->pieceBB[position->black_pieces]);
        valid_attack_set &= (~position->pieceBB[position->white_pieces]);

        temp = valid_attack_set;
        while(temp) {
            sq = lsb_scan(temp);
            temp ^= sqBB[sq];
            push_single_move(sq, sq+8, position->black_pawns);
            if(sq <= 7) {push_promotion(sq, 8);}
        }

        valid_attack_set = valid_attack_set >> 8; //double push
        valid_attack_set &= (~position->pieceBB[position->black_pieces]);
        valid_attack_set &= (~position->pieceBB[position->white_pieces]);
        valid_attack_set &= row5;

        temp = valid_attack_set;
        while(temp) {
            sq = lsb_scan(temp);
            temp ^= sqBB[sq];
            push_single_move(sq, sq+16, position->black_pawns);
            position->move_t.back().enPassant = sq+8;
        }

        valid_attack_set = position->pieceBB[position->black_pawns]; //attack down and left
        valid_attack_set = valid_attack_set >> 9;
        valid_attack_set &= notHfile;
        valid_attack_set &= (position->pieceBB[position->white_pieces] | sqBB[position->game_t.back().enPassant]);

        while(valid_attack_set) {
            sq = lsb_scan(valid_attack_set);
            valid_attack_set ^= sqBB[sq];
            push_single_move(sq, sq+9, position->black_pawns);
            if(sq == position->game_t.back().enPassant) {position->move_t.back().passant_capture = true;}
            if(sq <= 7) {push_promotion(sq , 9);}
        }


        valid_attack_set = position->pieceBB[position->black_pawns]; //attack down and right
        valid_attack_set = valid_attack_set >> 7;
        valid_attack_set &= notAfile;
        valid_attack_set &= (position->pieceBB[position->white_pieces] | sqBB[position->game_t.back().enPassant]);

        while(valid_attack_set) {
            sq = lsb_scan(valid_attack_set);
            valid_attack_set ^= sqBB[sq];
            push_single_move(sq, sq+7, position->black_pawns);
            if(sq == position->game_t.back().enPassant) {position->move_t.back().passant_capture = true;}
            if(sq <= 7) {push_promotion(sq, 7);}
        }
    }
    if(DEBUG_MODE)
        assert(position->integrity());
}

U64 game::northEastAttack(char square) {
    U64 temp = position->occupied_squares & northEast[square];
    char sigBit=0;
    if(temp){
        sigBit = lsb_scan(temp);
        return (northEast[square] ^ northEast[sigBit]);
    }
    else
        return northEast[square];
}
U64 game::eastAttack(char square) {
    U64 temp = position->occupied_squares & east[square];
    char sigBit=0;
    if(temp){
        sigBit = lsb_scan(temp);
        return (east[square] ^ east[sigBit]);
    }
    else
        return east[square];

}
U64 game::southEastAttack(char square) {
    U64 temp = position->occupied_squares & southEast[square];
    char sigBit=0;
    if(temp){
        sigBit = msb_scan(temp);
        return (southEast[square] ^ southEast[sigBit]);
    }
    else
        return southEast[square];

}
U64 game::southAttack(char square) {
    U64 temp = position->occupied_squares & south[square];
    char sigBit=0;
    if(temp){
        sigBit = msb_scan(temp);
        return (south[square] ^ south[sigBit]);
    }
    else
        return south[square];

}
U64 game::southWestAttack(char square) {
    U64 temp = position->occupied_squares & southWest[square];
    char sigBit=0;
    if(temp){
        sigBit = msb_scan(temp);
        return (southWest[square] ^ southWest[sigBit]);
    }
    else
        return southWest[square];

}
U64 game::westAttack(char square) {
    U64 temp = position->occupied_squares & west[square];
    char sigBit=0;
    if(temp){
        sigBit = msb_scan(temp);
        return (west[square] ^ west[sigBit]);
    }
    else
        return west[square];

}
U64 game::northWestAttack(char square) {
    U64 temp = position->occupied_squares & northWest[square];
    char sigBit=0;
    if(temp){
        sigBit = lsb_scan(temp);
        return (northWest[square] ^ northWest[sigBit]);
    }
    else
        return northWest[square];

}
U64 game::northAttack(char square) {
    U64 temp = position->occupied_squares & north[square];
    char sigBit=0;
    if(temp){
        sigBit = lsb_scan(temp);
        return (north[square] ^ north[sigBit]);
    }
    else
        return north[square];
}

void game::push_promotion(char sq, int shift) {
    if(DEBUG_MODE)
        assert(position->integrity());
    bool side = position->side;
    char origin = sq + shift;

    position->move_t.back().promotion = true;
    position->move_t.back().promotion_piece = position->queens + side;

    push_single_move(sq, origin, position->pawns + side);
    position->move_t.back().promotion = true;
    position->move_t.back().promotion_piece = position->knights + side;

    push_single_move(sq, origin, position->pawns + side);
    position->move_t.back().promotion = true;
    position->move_t.back().promotion_piece = position->bishops + side;

    push_single_move(sq, origin, position->pawns + side);
    position->move_t.back().promotion = true;
    position->move_t.back().promotion_piece = position->rooks + side;

    if(DEBUG_MODE)
        assert(position->integrity());
}

void game::push_moves(char *dest, char sq, char piece) {
    char i=0;

    while(dest[i] != NOT_MOVE) {
        push_single_move(dest[i], sq, piece);
        i++;
    }
}

void game::push_single_move(char dest, char sq, char piece) {
    if(DEBUG_MODE)
        assert(position->integrity());
    position->move1.move[0] = sq;
    position->move1.move[1] = dest;
    position->move1.side = position->side;
    position->move1.piece = piece;
    position->move1.capture = sqBB[dest] & position->pieceBB[position->pieces+(!position->side)];
    if(position->move1.capture) {
        position->move1.captured_piece = position->charBoard[dest];
    }
    position->move1.enPassant = NOT_MOVE;
    position->move1.passant_capture = false;
    position->move_t.push_back(position->move1);

    if(position->move_t.back().capture) {
        if(position->move_t.back().captured_piece == (position->rooks + (!position->move1.side))) {
            if((position->move1.move[1] == 7) && (position->can_castle[position->white_king]))
                position->move_t.back().canCastle_off[position->white_king] = true;
            else if((position->move1.move[1] == 63) && (position->can_castle[position->black_king]))
                position->move_t.back().canCastle_off[position->black_king] = true;
            else if((position->move1.move[1] == 0) && (position->can_castle[position->white_queens]))
                position->move_t.back().canCastle_off[position->white_queens] = true;
            else if((position->move1.move[1] == 56) && (position->can_castle[position->black_queens]))
                position->move_t.back().canCastle_off[position->black_queens] = true;
        }
    }
    if(DEBUG_MODE)
        assert(position->integrity());
}

int game::check_move(char x, char y) {
    for(int i=0;i < position->move_t.size(); i++) {
        if(x==position->move_t[i].move[0] && y==position->move_t[i].move[1])
            return i;
    }
    return NOT_MOVE;
}

//////////////////////////////////////////////////////////////////
//name:unmake_move
//returns: none
//parameters: index of move to undo in move_t vector
//function:updates all board information, changes side to move.
///////////////////////////////////////////////////////////////////
void game::unmake_move() {
    if(DEBUG_MODE)
        assert(position->integrity());

    position->makeMove = position->game_t.back();                    //safe the indexed moved into the more convenient/expedient makeMove.

    position->game_t.pop_back();

    bool side = position->makeMove.side;                      //save the current side into the more convenient/readable side variable
    bool otherSide = !side;                            //save opposite side into the more conv/readable otherSide var.
    char piece = position->makeMove.piece;                    //save the piece in the current move object into the more conv/readable piece var
                                                              //fromToBB holds the location of the pieces origin and destination

    if(!position->makeMove.capture && (position->makeMove.piece != (position->pawns + side))) {
        position->fifty_move_rule--;
    }

    U64 fromToBB = sqBB[position->makeMove.move[0]] ^ sqBB[position->makeMove.move[1]];
    position->pieceBB[piece] ^= fromToBB;                     //reinstate moving pieces BitBoard
    position->pieceBB[position->pieces + side] ^= fromToBB;          //reinstate moving sides pieces BitBoard
    position->charBoard[position->makeMove.move[1]] = EMPTY;         //reinstate charBoard destination back to empty
    position->charBoard[position->makeMove.move[0]] = piece;         //reinsate charBoard origin back to the moving piece

    /*
     for non captures
     */
    if(!position->makeMove.capture){                          //if this is NOT a capture
        position->occupied_squares ^= fromToBB;                //then last thing to do is update the occupied square BitBoard
    }
    /*
     for captures
     */
    else {                                             //if this is a capture
        U64 fromBB = sqBB[position->makeMove.move[0]];         //hold origin Bibboard
        U64 toBB = sqBB[position->makeMove.move[1]];           //holds destinations BitBoard
        position->pieceBB[position->makeMove.captured_piece] ^= toBB;       //reinstate piece taken BitBoard with the destination
        position->pieceBB[position->pieces + (otherSide)] ^= toBB;          //reinstate the pieces bitboard for the opposing side with the destination
        position->occupied_squares ^= fromBB;                        //reinstate occupied BitBoard with only the origin, bc the dest is still occupied
                                                                     //reinstate charBoard, destination should now show the captured piece replaced.
        position->charBoard[position->makeMove.move[1]] = position->makeMove.captured_piece;
    }
    /*
     //note: en passant is not considered a capture, so non capture code above is used
     */
    if(position->makeMove.passant_capture) {                                       //if this move was an enPassant capture
        position->occupied_squares ^= sqBB[position->game_t.back().move[1]];               //add pawn back to occupied squares BB
        position->pieceBB[position->pawns + otherSide] ^= sqBB[position->game_t.back().move[1]];  //add attacked pawn back to appropriate pawns BitBoard
        position->pieceBB[position->pieces + otherSide] ^= sqBB[position->game_t.back().move[1]]; //add attacked pawn back to appropriate pieces BitBoard
                                                                                                  //add attacked pawn back to charBoard.
        position->charBoard[position->game_t.back().move[1]] = position->pawns + otherSide;
    }


    if(position->makeMove.canCastle_off[position->king + side])            //if this move took away castling ability on the king side
        position->can_castle[position->king+side] = true;                   //then reinstate it
    if(position->makeMove.canCastle_off[position->queens + side])          //if this move took away castling ability on the queen side
        position->can_castle[position->queens + side] = true;               //then reinsate it
    if(position->makeMove.canCastle_off[position->king + otherSide])            //if this move took away castling ability on the king side
        position->can_castle[position->king + otherSide] = true;                   //then reinstate it
    if(position->makeMove.canCastle_off[position->queens + otherSide])          //if this move took away castling ability on the queen side
        position->can_castle[position->queens + otherSide] = true;               //then reinsate it


    /*
     CASTLE KING SIDE
     */
    if(position->makeMove.isCastle[position->king + side]) {               //if this is a castle on the king side
                                                                           //holds where the rook should move bitBoard with origin and dest
        U64 rookFromToBB = sqBB[position->makeMove.move[1] + 1] ^ sqBB[position->makeMove.move[1] - 1];
        position->pieceBB[position->pieces+side] ^= rookFromToBB;           //reinstate this moves pieces BibBoard for the rook move
        position->pieceBB[position->rooks+side] ^= rookFromToBB;            //reinstate this moves rook BitBoard
        position->occupied_squares ^= rookFromToBB;                  //reinstate the occupied BitBoard
                                                                     //reinstate the charBoard corner to rook
        position->charBoard[position->makeMove.move[1] + 1] = position->rooks + side;
        position->charBoard[position->makeMove.move[1] - 1] = EMPTY;        //reinstate the charBoard rook dest back to empty

        position->has_castled[side] = false;
    }

    /*
     CASTLE QUEEN SIDE
     */
    if(position->makeMove.isCastle[position->queens + side]) {             //if this is a castle on the queen side
                                                                           //create a rook origin/dest BB
        U64 rookFromToBB = sqBB[position->makeMove.move[1] - 2] | sqBB[position->makeMove.move[1] + 1];
        position->pieceBB[position->pieces+side] ^= rookFromToBB;           //reinstate pieces BB for the rook
        position->pieceBB[position->rooks+side] ^= rookFromToBB;            //reinstate rook BB
        position->occupied_squares ^= rookFromToBB;                  //reinstate occupied BB
                                                                     //reinstate corner to rook on charBoard
        position->charBoard[position->makeMove.move[1] - 2] = position->rooks + side;
        position->charBoard[position->makeMove.move[1] + 1] = EMPTY;        //reinstate charBoard rook dest to empty

        position->has_castled[side] = false;
    }


    /*
     PAWN PROMOTION
     */                                                       //if white pawn moved to back rank
    if(position->makeMove.promotion) {
        position->pieceBB[position->pawns + side] ^= sqBB[position->makeMove.move[1]];    //remove from pawn bit board
        position->pieceBB[position->makeMove.promotion_piece] ^= sqBB[position->makeMove.move[1]];   //remove the indicated piece from it's bit board
                                                                                                     //position->charBoard[position->makeMove.move[0]] = position->white_pawns;         //change on charBoard
    }
    /*
     if(position->makeMove.piece == position->black_pawns && position->makeMove.move[1] < 8) {
     position->pieceBB[position->black_pawns] ^= sqBB[position->makeMove.move[1]];    //remove from pawn bit board
     position->pieceBB[position->black_queens] ^= sqBB[position->makeMove.move[1]];   //remove queen from her bit board
     position->charBoard[position->makeMove.move[0]] = position->black_pawns;         //change on charBoard
     }
     */
    position->switch_side();          //side switched whenever unmake move is called.

    if(DEBUG_MODE)
        assert(position->integrity());
}

    //////////////////////////////////////////////////////////////////
    //name:make_move
    //returns: boolean indicating success of move
    //parameters: index of move in move_t vector
    //function:updates all board information, changes side to move, returns true if successful. Checks if move leaves king in check or
    //castles illegally through check. If so it undoes the move via unmake move(), and returns false.
    //if successful leaves move_t vector with the nexts sides moves already generated, and the list of the side that just moved still in the vector,
    //if unsuccesful leaves move_t vector with the same list of moves for the current side to reuse
    /////and leaves the board info on the side that should select a different move.
    ///////////////////////////////////////////////////////////////////
bool game::make_move(const int i) {
    if(DEBUG_MODE)
        assert(position->integrity());
    position->makeMove = position->move_t[i];
    bool side = position->makeMove.side;
    bool otherSide = !side;
        //assert(position->integrity());
        //king side castle but rooks not in position.
        //do it here so as not to do all the make/unmake move business.
    if(position->makeMove.isCastle[position->king + side] && (position->charBoard[position->makeMove.move[1] + 1] != (position->rooks + side) ) ) {
        return false; }
        //quuen side castle but rooks not in position.
        //do it here so as not to do all the make/unmake move business.
    if(position->makeMove.isCastle[position->queens + side] && (position->charBoard[position->makeMove.move[1] - 2] != (position->rooks + side) ) ) {
        return false; }

    if(!position->makeMove.capture && (position->makeMove.piece != (position->pawns + side))) {
        position->fifty_move_rule++;
    }

    const int it = position->move_t.size();
    position->switch_side();
        //assert(position->integrity());
    /*
     all moves do the following
     */
    U64 fromToBB = sqBB[position->makeMove.move[0]] ^ sqBB[position->makeMove.move[1]];
    position->pieceBB[position->makeMove.piece] ^= fromToBB;               //update moving piece BB
    position->pieceBB[position->pieces + side] ^= fromToBB;                //update moving side pieces BB
    position->charBoard[position->makeMove.move[0]] = EMPTY;               //update charBoard
    position->charBoard[position->makeMove.move[1]] = position->makeMove.piece;   //update charBoard
                                                                                  //assert(position->integrity());
    /*
     for non captures
     */
    if(!position->makeMove.capture){
        position->occupied_squares ^= fromToBB;                      //update occupied squares
    }
    /*
     for captures
     */
    else {
        U64 fromBB = sqBB[position->makeMove.move[0]];
        U64 toBB = sqBB[position->makeMove.move[1]];
        position->pieceBB[position->makeMove.captured_piece] ^= toBB;       //update piece taken BB BB
        position->pieceBB[position->pieces + (otherSide)] ^= toBB;          //update side attacked pieces BB
        position->occupied_squares ^= fromBB;                        //update occupide squares BB
    }
        //assert(position->integrity());

    /*
     //note: en passant is not considered a capture, so non capture code above is used
     */
    if(position->makeMove.passant_capture) {
        position->occupied_squares ^= sqBB[position->game_t.back().move[1]];               //remove pawn from occupied squares BB
        position->pieceBB[position->pawns + otherSide] ^= sqBB[position->game_t.back().move[1]];  //remove pawn from attacked piece BB
        position->pieceBB[position->pieces + otherSide] ^= sqBB[position->game_t.back().move[1]]; //remove pawn from attacked side pieces BB
        position->charBoard[position->game_t.back().move[1]] = EMPTY;
    }
        //assert(position->integrity());
    /*
     this move removes the following positions castling ability
     */
    if(position->makeMove.canCastle_off[position->king + side])
        position->can_castle[position->king+side] = false;
    if(position->makeMove.canCastle_off[position->queens + side])
        position->can_castle[position->queens + side] = false;
    if(position->makeMove.canCastle_off[position->king + otherSide])
        position->can_castle[position->king+otherSide] = false;
    if(position->makeMove.canCastle_off[position->queens + otherSide])
        position->can_castle[position->queens + otherSide] = false;

        //assert(position->integrity());

    /*
     PAWN PROMOTION
     */
    if(position->makeMove.promotion) {
        position->pieceBB[position->pawns + side] ^= sqBB[position->makeMove.move[1]];               //remove from pawn bit board
        position->pieceBB[position->makeMove.promotion_piece] ^= sqBB[position->makeMove.move[1]];   //add indicated piece to it's pieces bit board
        position->charBoard[position->makeMove.move[1]] = position->makeMove.promotion_piece;                    //change on charBoard
    }

        //push move should be after passant code, because it uses the last move on game_t.
        //but before the king legality code.
    position->push_move(i);

    /*
     this move is a castle on the king side
     */
    if(position->makeMove.isCastle[position->king + side]) {
        U64 rookFromToBB = sqBB[position->makeMove.move[1] + 1] | sqBB[position->makeMove.move[1] - 1];
        position->pieceBB[position->rooks+side] ^= rookFromToBB;                  //update rooks bitboard
        position->occupied_squares ^= rookFromToBB;                        //update occupied sq bb
        position->pieceBB[position->pieces+side] ^= rookFromToBB;                 //missing this line for a while really fucked things hah
        position->charBoard[position->makeMove.move[1] + 1] = EMPTY;              //update charboard rook
        position->charBoard[position->makeMove.move[1] - 1] = position->rooks + side;    //update charboard rook
        position->has_castled[side] = true;
            //assert(position->integrity());
        generate_moves();
        const int sizeNow = position->move_t.size();
            /////////////////////////////
            //this is the check for castling through/out of/into check.
            /////////////////////////////
        for(int j = it; j < sizeNow; j++) {
            if(position->move_t[j].move[1] == position->move_t[i].move[1]) {
                unmake_move();
                position->clear_moves(it, position->move_t.size());
                return false;
            }
            if(position->move_t[j].move[1] == position->move_t[i].move[1]-1) {
                unmake_move();
                position->clear_moves(it, position->move_t.size());
                return false;
            }
            if(position->move_t[j].move[1] == position->move_t[i].move[1]-2) {
                unmake_move();
                position->clear_moves(it, position->move_t.size());
                return false;
            }
        }

        assert(position->integrity());
        return true;
    }

    /*
     this move is a castle on the queen side
     */
    else if(position->makeMove.isCastle[position->queens + side]) {
        U64 rookFromToBB = sqBB[position->makeMove.move[1] - 2] | sqBB[position->makeMove.move[1] + 1];
        position->pieceBB[position->rooks+side] ^= rookFromToBB;
        position->pieceBB[position->pieces+side] ^= rookFromToBB;
        position->occupied_squares ^= rookFromToBB;
        position->charBoard[position->makeMove.move[1] - 2] = EMPTY;
        position->charBoard[position->makeMove.move[1] + 1] = position->rooks + side;
        position->has_castled[side] = true;
            //assert(position->integrity());
        generate_moves();
        int sizeNow = position->move_t.size();

            //this is the check for castling through/out of/into check.
        for(int j = it ; j < sizeNow; j++) {
            if(position->move_t[j].move[1] == position->move_t[i].move[1]) {
                unmake_move();
                position->clear_moves(it, position->move_t.size());
                    //position->clear_move(i);
                return false;
            }
            if(position->move_t[j].move[1] == position->move_t[i].move[1]+1) {
                unmake_move();
                position->clear_moves(it, position->move_t.size());
                    //position->clear_move(i);
                return false;
            }
            if(position->move_t[j].move[1] == position->move_t[i].move[1]+2) {
                unmake_move();
                position->clear_moves(it, position->move_t.size());
                    //position->clear_move(i);
                return false;
            }

        }
        assert(position->integrity());
        return true;
    }
        //assert(position->integrity());
    generate_moves();
    int sizeNow = position->move_t.size();
    for(int j = it ; j < sizeNow; j++) {
        if(position->charBoard[position->move_t[j].move[1]] == position->king + side) {
            unmake_move();
            position->clear_moves(it, position->move_t.size());
            return false;
        }
    }

    assert(position->integrity());
    return true;

}

void game::print_move(const int i) {
    bool side = position->side;
    const char *pieces[] = {"king", "queen", "rook" , "knight" , "bishop" , "pawn"};
    std::cout << "\n\n";
	   std::cout << "i: " << static_cast<int>(i) << std::endl;
    std::cout << static_cast<int>(position->move_t[i].move[0]) << " " << static_cast<int>(position->move_t[i].move[1]) << std::endl;
    position->move_t[i].side ? std::cout << "dark" : std::cout << "light";
    std::cout << std::endl << "piece: " << pieces[(static_cast<int>(position->move_t[i].piece)) >> 1]  << std::endl;
    position->move_t[i].capture? std::cout << "Capture" : std::cout << "Not Capture";
    std::cout << std::endl;
    std::cout << "This move takes away ";
    position->move_t[i].canCastle_off[position->white_king] ? std::cout << "whites king side castling,\n" : std::cout << std::endl;
    position->move_t[i].canCastle_off[position->white_queens] ? std::cout << "and whites queen side castling.\n" : std::cout << std::endl;
    position->move_t[i].canCastle_off[position->black_king] ? std::cout << "blacks king side castling,\n": std::cout << std::endl;
    position->move_t[i].canCastle_off[position->black_queens] ? std::cout << "and blacks queen side castling.\n": std::cout << std::endl;

    std::cout << "Other board information:\n";
   	std::cout << "castling ability: \n";
   	std::cout << "white_kingside: " << position->can_castle[position->white_king];
   	std::cout << " black king side: " << position->can_castle[position->black_king];
   	std::cout << " white queen side: " << position->can_castle[position->white_queens];
   	std::cout << " black queen side: " << position->can_castle[position->black_queens] << std::endl;

   	std::cout << "Bit Boards: \n";
   	std::cout << "occupied squares:\n";
   	position->print_bit_board(position->occupied_squares);

   	bool thisside = position->move_t.back().side;
    std::cout << "kings: \n";
    position->print_bit_board(position->pieceBB[position->king+side]);
    std::cout << "pawns\n";
   	position->print_bit_board(position->pieceBB[position->pawns+side]);
   	std::cout << "knights: \n";
   	position->print_bit_board(position->pieceBB[position->knights+side]);
   	std::cout << "rooks: \n";
   	position->print_bit_board(position->pieceBB[position->rooks + side]);
   	std::cout << "bishops: \n";
   	position->print_bit_board(position->pieceBB[position->bishops + side]);
   	std::cout << "this sides pieces: \n";
   	position->print_bit_board(position->pieceBB[position->pieces + side]);
   	std::cout << "\n";
   	position->print_char_board();
}

void game::print_moves() {
    bool side = position->side;
    const char *pieces[] = {"king", "queen", "rook" , "knight" , "bishop" , "pawn"};
    std::cout << "\n";
    for(int i=0; i<position->move_t.size(); i++) {
        std::cout << "\n\n";
        std::cout << "i: " << static_cast<int>(i) << std::endl;
        std::cout << static_cast<int>(position->move_t[i].move[0]) << " " << static_cast<int>(position->move_t[i].move[1]) << std::endl;
        position->move_t[i].side ? std::cout << "dark" : std::cout << "light";
        std::cout << std::endl << "piece: " << pieces[(static_cast<int>(position->move_t[i].piece)) >> 1]  << std::endl;
        position->move_t[i].capture? std::cout << "Capture" : std::cout << "Not Capture";
        std::cout << std::endl;
        std::cout << "This move takes away ";
        position->move_t[i].canCastle_off[position->white_king] ? std::cout << "whites king side castling,\n" : std::cout << std::endl;
        position->move_t[i].canCastle_off[position->white_queens] ? std::cout << "and whites queen side castling.\n" : std::cout << std::endl;
        position->move_t[i].canCastle_off[position->black_king] ? std::cout << "blacks king side castling,\n": std::cout << std::endl;
        position->move_t[i].canCastle_off[position->black_queens] ? std::cout << "and blacks queen side castling.\n": std::cout << std::endl;

    }

    std::cout << "Other board information:\n";
    std::cout << "castling ability: \n";
    std::cout << "white_kingside: " << position->can_castle[position->white_king];
    std::cout << " black king side: " << position->can_castle[position->black_king];
    std::cout << " white queen side: " << position->can_castle[position->white_queens];
    std::cout << " black queen side: " << position->can_castle[position->black_queens] << std::endl;

    std::cout << "Bit Boards: \n";
    std::cout << "occupied squares:\n";
    position->print_bit_board(position->occupied_squares);

    bool thisside = position->move_t.back().side;
    std::cout << "kings: \n";
    position->print_bit_board(position->pieceBB[position->king+thisside]);
    std::cout << "pawns\n";
    position->print_bit_board(position->pieceBB[position->pawns+thisside]);
    std::cout << "knights: \n";
    position->print_bit_board(position->pieceBB[position->knights+thisside]);
    std::cout << "rooks: \n";
    position->print_bit_board(position->pieceBB[position->rooks + thisside]);
    std::cout << "bishops: \n";
    position->print_bit_board(position->pieceBB[position->bishops + thisside]);
    std::cout << "this sides pieces: \n";
    position->print_bit_board(position->pieceBB[position->pieces + thisside]);
    std::cout <<"number of moves: " << static_cast<int>(position->move_t.size()) << "\n\n";
}

char *game::serialize(U64 x) {
    char i=0;
    while(x) {
        tz[i] = __builtin_ctzll(x);
        x ^= sqBB[tz[i]];
        i++;
    }
    tz[i] = NOT_MOVE;

    return tz;
}

unsigned game::lsb_scan(U64 x){
    return __builtin_ctzll(x);
}

    //need this to return the position as an index to the positional attack array;
unsigned game::msb_scan(U64 b){
    return 63 - __builtin_clzll(b);
}


int game::popcount(U64 b)
{
    b = (b & 0x5555555555555555LU) + (b >> 1 & 0x5555555555555555LU);
    b = (b & 0x3333333333333333LU) + (b >> 2 & 0x3333333333333333LU);
    b = b + (b >> 4) & 0x0F0F0F0F0F0F0F0FLU;
    b = b + (b >> 8);
    b = b + (b >> 16);
    b = b + (b >> 32) & 0x0000007F;

    return static_cast<int>(b);
}
