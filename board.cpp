    //#include <cstdlib>
#include "board.h"
#include "eng_global.h"

board::board()
{
    game_t.reserve(300);
    move_t.reserve(300);
    checkmate = false;
    stalemate = false;
    side = LIGHT;
    pieceBB[white_king] = 1ULL << E1;
    pieceBB[black_king] = 1ULL << E8;
    pieceBB[white_queens] = 1ULL << D1;
    pieceBB[black_queens] = 1ULL << D8;
    pieceBB[white_rooks] = (1ULL << A1) | (1ULL << H1);
    pieceBB[black_rooks] = (1ULL << A8) | (1ULL << H8);
    pieceBB[white_knights] = (1ULL << B1) | (1ULL << G1);
    pieceBB[black_knights] = (1ULL << B8) | (1ULL << G8);
    pieceBB[white_bishops] = (1ULL << C1) | (1ULL << F1);
    pieceBB[black_bishops] = (1ULL << C8) | (1ULL << F8);
    pieceBB[white_pawns] = 0xFF00;
    pieceBB[black_pawns] = 0xFF000000000000;
    occupied_squares = pieceBB[white_king] | pieceBB[black_king] | pieceBB[white_queens]
    | pieceBB[black_queens] | pieceBB[white_rooks] | pieceBB[black_rooks]
    | pieceBB[white_knights] | pieceBB[black_knights] | pieceBB[white_bishops]
    | pieceBB[black_bishops] | pieceBB[white_pawns]  | pieceBB[black_pawns];
    pieceBB[white_pieces] =  pieceBB[white_king] | pieceBB[white_queens] | pieceBB[white_rooks]
    | pieceBB[white_knights]  | pieceBB[white_bishops] | pieceBB[white_pawns];
    pieceBB[black_pieces] = occupied_squares ^ pieceBB[white_pieces];
    
        //move_t.reserve(102400);
    
    for(char i=0;i<64;i++) {
        charBoard[i] = EMPTY;
        if(i==0 || i==7)
            charBoard[i] = white_rooks;
        if(i==1 || i==6)
            charBoard[i] = white_knights;
        if(i==2 || i==5)
            charBoard[i] = white_bishops;
        if(i==3)
            charBoard[i] = white_queens;
        if(i==4)
            charBoard[i] = white_king;
        if(i>7 && i<16)
            charBoard[i] = white_pawns;
        if(i>=48 && i<56)
            charBoard[i] = black_pawns;
        if(i==56 || i==63)
            charBoard[i] = black_rooks;
        if(i==57 || i==62)
            charBoard[i] = black_knights;
        if(i==58 || i== 61)
            charBoard[i] = black_bishops;
        if(i==59)
            charBoard[i] = black_queens;
        if(i==60)
            charBoard[i] = black_king;
    }
    
    can_castle[white_king] = true;
    can_castle[white_queens] = true;
    can_castle[black_king] = true;
    can_castle[black_queens] = true;
    
    has_castled[LIGHT] = false;
    has_castled[DARK] = false;
    
    move1.piece = king;
    move1.move[0] = 7; //row 0 coll 7
    move1.move[1] = 53; //row 6 coll 5
    
    move1.canCastle_off[white_king] = false;
    move1.canCastle_off[white_queens] = false;
    move1.canCastle_off[black_king] = false;
    move1.canCastle_off[black_queens] = false;
    
    move1.isCastle[white_king] = false;
    move1.isCastle[white_queens] = false;
    move1.isCastle[black_king] = false;
    move1.isCastle[black_queens] = false;
    
    move1.enPassant = NOT_MOVE;
    move1.passant_capture = false;
    
    move1.promotion = false;
    move1.promotion_piece = EMPTY;
    
    game_t.push_back(move1);
    
    fifty_move_rule = 0;
        //repetition = 0;
    full_move_count = 0; //updated after blacks moves
    
    /*	char temp[6] = {1, 1, 2, 4, 3, 10}; //Bking = 0, Bqueens, Brooks, Bknights, Bbishops, Bpawns
     for(char k=0; k<6; k++) {
     battle_value[k] = temp[k];
     }
     
     for(char j=0;j<2;j++) {
     for(char k=0; k<64; k++) {
     battle_field[j][k] = 0;
     }
     }
     */
}

void board::print_char_board() {
    const char *pieces[] = {"K", "k", "Q", "q", "R", "r", "N", "n", "B", "b", "P", "p"};
    char i, j;
    std::cout << "\t\t8: ";
    for(i=0;i<8;i++) { //indicates the row
        for(j=0;j<8;j++) { //goes through each square
            if(charBoard[63 - ((7-j)+8*i)] != EMPTY)
                std::cout << " " << pieces[charBoard[63 - ((7-j)+8*i)]] << " ";
            else if(((j+i)%2) == 0)
                std::cout << " - ";
            else
                std::cout << " . ";
            if((j==7) && (i!= 7))
                std::cout << "\n\n\t\t" << 7-i << ": ";
        }
    }
    std::cout << "\n\t\t   ________________________";
    std::cout << "\n\t\t    a  b  c  d  e  f  g  h\n\n";
}

bool board::import_FEN(char *fen, char *side_to_move, char *castling_rights, char *en_passant, char *fifty_move, char *full_moves) {
    char letter, sq;
    int aRank, aFile;
    char *str_token;
    
    str_token = fen;
        //assert(str_token);
    
    for(char k=0; k<64; k++) charBoard[k] = EMPTY;
    
    char j = 63;
    for(char i=0; i<strlen(str_token); i++) {\
        char index = j + 7 - (2 * (j % 8));
        letter = str_token[i];
        
        switch (letter)
        {
            case 'p' : charBoard[index] = black_pawns; break;
            case 'r' : charBoard[index] = black_rooks; break;
            case 'n' : charBoard[index] = black_knights; break;
            case 'b' : charBoard[index] = black_bishops; break;
            case 'q' : charBoard[index] = black_queens; break;
            case 'k' : charBoard[index] = black_king; break;
            case 'P' : charBoard[index] = white_pawns; break;
            case 'R' : charBoard[index] = white_rooks; break;
            case 'N' : charBoard[index] = white_knights; break;
            case 'B' : charBoard[index] = white_bishops; break;
            case 'Q' : charBoard[index] = white_queens; break;
            case 'K' : charBoard[index] = white_king; break;
            case '/' : j++; break;
            case '1' : break;
            case '2' : j--; break;
            case '3' : j-=2; break;
            case '4' : j-=3; break;
            case '5' : j-=4; break;
            case '6' : j-=5; break;
            case '7' : j-=6; break;
            case '8' : j-=7; break;
            default: return false;
        }
        j--;
    }
    print_char_board();
    
        //declare whose turn it is
    str_token = side_to_move;
    assert(str_token);
    if(str_token[0] == 'w') side = LIGHT;
    else if(str_token[0] == 'b') side = DARK;
    else return false;
    
    game_t.back().side = !side;
    
    std::cout << "\n\n\tside: " << side << std::endl;
    
    str_token = castling_rights;
    assert(str_token);
    can_castle[0] = false;
    can_castle[1] = false;
    can_castle[2] = false;
    can_castle[3] = false;
    for(char i=0; i<strlen(str_token); i++) {
        letter = str_token[i];
        switch(letter) {
            case 'K' : can_castle[white_king] = true; break;
            case 'Q' : can_castle[white_queens] = true; break;
            case 'k' : can_castle[black_king] = true; break;
            case 'q' : can_castle[black_queens] = true; break;
            case '-' : break;
            default : return false;
        }
    }
    
    std::cout << "\n\t\tK: " << can_castle[white_king];
    std::cout << "\n\t\tQ: " << can_castle[white_queens];
    std::cout << "\n\t\tk: " << can_castle[black_king];
    std::cout << "\n\t\tq: " << can_castle[black_queens];
    
    
        //en passant square
    str_token = en_passant;
    assert(str_token);
    
    std::cout << "\n\tenPassant: " << str_token[0] << str_token[1];
    
    if(str_token[0] != '-')  {
        sq = str_token[0] - 97;       // 'a' = 0
        sq = sq + (str_token[1] - 49) * 8;
        game_t.back().enPassant = sq;
        game_t.back().side = !side;
        if(side == DARK) {game_t.back().move[1] = sq + 8;}
        else {game_t.back().move[1] = sq - 8;}
    }
    
    
    std::cout << "\n\tenPassant: " << static_cast<int>(game_t.back().enPassant);
    
        //full and half move counts;
    str_token = fifty_move;
    assert(str_token);
    fifty_move_rule = atoi(str_token);
    
    std::cout << "\n\thalf move non pawn non capture count: " << fifty_move_rule << std::endl;
    
    str_token = full_moves;
    assert(str_token);
    full_move_count = atoi(str_token);
    
    std::cout << "\n\tfull move count: " << full_move_count << std::endl;
    
    pieceBB[white_king] = 0ULL;
    pieceBB[black_king] = 0ULL;
    pieceBB[white_queens] = 0ULL;
    pieceBB[black_queens] = 0ULL;
    pieceBB[white_rooks] = 0ULL;
    pieceBB[black_rooks] = 0ULL;
    pieceBB[white_knights] = 0ULL;
    pieceBB[black_knights] = 0ULL;
    pieceBB[white_bishops] = 0ULL;
    pieceBB[black_bishops] = 0ULL;
    pieceBB[white_pawns] = 0ULL;
    pieceBB[black_pawns] = 0ULL;
    
    for(char k=0; k<64; k++) {
        if(charBoard[k] != EMPTY) {
            pieceBB[charBoard[k]] |= (1ULL << k);
        }
    }
    
    occupied_squares = pieceBB[white_king] | pieceBB[black_king] | pieceBB[white_queens]
    | pieceBB[black_queens] | pieceBB[white_rooks] | pieceBB[black_rooks]
    | pieceBB[white_knights] | pieceBB[black_knights] | pieceBB[white_bishops]
    | pieceBB[black_bishops] | pieceBB[white_pawns]  | pieceBB[black_pawns];
    pieceBB[white_pieces] =  pieceBB[white_king] | pieceBB[white_queens] | pieceBB[white_rooks]
    | pieceBB[white_knights]  | pieceBB[white_bishops] | pieceBB[white_pawns];
    pieceBB[black_pieces] = pieceBB[black_king] | pieceBB[black_queens] | pieceBB[black_rooks]
    | pieceBB[black_knights]  | pieceBB[black_bishops] | pieceBB[black_pawns];
    
        //assert(integrity());
    return true;
}

void board::print_bit_board(U64 x)
{
    std::bitset<64> bb (x);
    for(int i=0;i<8;i++)
        std::cout << bb[i+56];
    std::cout << std::endl;
    for(int i=0;i<8;i++)
        std::cout << bb[(i)+48];
    std::cout << std::endl;
    for(int i=0;i<8;i++)
        std::cout << bb[i+40];
    std::cout << std::endl;
    for(int i=0;i<8;i++)
        std::cout << bb[i+32];
    std::cout << std::endl;
    for(int i=0;i<8;i++)
        std::cout << bb[i+24];
    std::cout << std::endl;
    for(int i=0;i<8;i++)
        std::cout << bb[i+16];
    std::cout << std::endl;
    for(int i=0;i<8;i++)
        std::cout << bb[i+8];
    std::cout << std::endl;
    for(int i=0;i<8;i++)
        std::cout << bb[i];
    std::cout << std::endl;
    std::cout << std::endl;
    
}

bool board::integrity() {
    bool ret = true;
        //charBoard matches bitboards
    for(char i=0; i < 64; i++) {
        if(charBoard[i] != EMPTY) {
            if(! (pieceBB[charBoard[i]] & (1ULL << i))) {
                std::cout << "charBoard doesn't match piece BBs\n";
                ret =  false;
            }
            if(! (occupied_squares & (1ULL << i))) {
                std::cout << "charBoard doesn't match occupied BB\n";
                ret =  false;
            }
        }
        else {
            if(!((1ULL << i) & (~occupied_squares))) {
                std::cout << "empty charBoard not empty on occupied_squares BB\n";
                ret =  false;
            }
        }
    }
        //piece BBs match occupied BB
    if((pieceBB[white_pieces] | pieceBB[black_pieces]) != occupied_squares) {
        std::cout << "Piece BBs don't match occupied BB\n";
        ret =  false;
    }
    
        //castling rights make sense
    if(can_castle[white_king]) {
        if(charBoard[7] != white_rooks) {
            if(charBoard[4] != white_king) {
                std::cout << "castling rights don't make sense: white Kingside\n";
                ret =  false;
            }
        }
    }
    if(can_castle[black_king]) {
        if(charBoard[63] != black_rooks) {
            if(charBoard[60] != black_king) {
                std::cout << "castling rights don't make sense: black kingside.\n";
                ret =  false;
            }
        }
    }
    if(can_castle[white_queens]) {
        if(charBoard[0] != white_rooks) {
            if(charBoard[4] != white_king) {
                std::cout << "castling rights don't make sense: white queenside.\n";
                ret =  false;
            }
        }
    }
    if(can_castle[black_queens]) {
        if(charBoard[56] != black_rooks) {
            if(charBoard[60] != black_king) {
                std::cout << "castling rights don't make sense: black queenside.\n";
                ret =  false;
            }
        }
    }
    
        //white piece BB match piece BB
    U64 temp = pieceBB[white_king] | pieceBB[white_queens] | pieceBB[white_rooks] | pieceBB[white_knights]  | pieceBB[white_bishops] | pieceBB[white_pawns];
    if(pieceBB[white_pieces] != temp) {
        std::cout << "white pieces BB doesn't match the sum of each piece BB\n";
        ret =  false;
    }
    U64 temp2 = pieceBB[black_king] | pieceBB[black_queens] | pieceBB[black_rooks] | pieceBB[black_knights]  | pieceBB[black_bishops] | pieceBB[black_pawns];
    if(pieceBB[black_pieces] != temp2) {
        std::cout << "black pieces BB doesn't match the sum of each piece BB\n";
        ret =  false;
    }
    if((temp | temp2) != occupied_squares) {
        std::cout << "sum of each piece BB doesn't match occupied BB\n";
        ret = false;
    }
    if(!ret) {
        std::cout << "Game Moves:\n";
        print_game_moves();
        std::cout << "\nIntegrity Failed.\n\n";
        
        std::cout << "Bit Boards: \n";
        std::cout << "occupied squares:\n";
        print_bit_board(occupied_squares);
        
        
        std::cout << "kings: \n";
        print_bit_board(pieceBB[king+side]);
        std::cout << "pawns\n";
        print_bit_board(pieceBB[pawns+side]);
        std::cout << "knights: \n";
        print_bit_board(pieceBB[knights+side]);
        std::cout << "rooks: \n";
        print_bit_board(pieceBB[rooks + side]);
        std::cout << "bishops: \n";
        print_bit_board(pieceBB[bishops + side]);
        std::cout << "this sides pieces: \n";
        print_bit_board(pieceBB[pieces + side]);
        std::cout << "\n";
        
        bool otherSide = !side;
        std::cout << "other side kings: \n";
        print_bit_board(pieceBB[king+otherSide]);
        std::cout << "others pawns\n";
        print_bit_board(pieceBB[pawns+otherSide]);
        std::cout << "others knights: \n";
        print_bit_board(pieceBB[knights+otherSide]);
        std::cout << "others rooks: \n";
        print_bit_board(pieceBB[rooks + otherSide]);
        std::cout << "others bishops: \n";
        print_bit_board(pieceBB[bishops + otherSide]);
        std::cout << "other sides pieces: \n";
        print_bit_board(pieceBB[pieces + otherSide]);
        std::cout << "\n";
        print_char_board();
        
        int input = 1;
        while(input >= -1) {
            std::cout << "Enter move index to print: ";
            std::cin >> input;
            if(input != -1)
                print_specified_move(input);
            if(input == -1) {
                std::cout << "\tmove_t size: " << move_t.size() << std::endl;
            }
        }
        
    }
    
    return ret;
}

void board::print_game_moves() {
    const char *pieces[] = {"king", "queen", "rook" , "knight" , "bishop" , "pawn"};
    std::cout << "\n";
    std::cout <<"number of moves: " << static_cast<int>(game_t.size()) << "\n\n";
    if(game_t.size() <= 1) return;
    for(char i=1; i<game_t.size(); i++) {
        std::cout << "\n\n";
        std::cout << "i: " << static_cast<int>(i) << std::endl;
        std::cout << static_cast<int>(game_t[i].move[0]) << " " << static_cast<int>(game_t[i].move[1]) << std::endl;
        game_t[i].side ? std::cout << "dark" : std::cout << "light";
        std::cout << std::endl << "piece: " << pieces[(static_cast<int>(game_t[i].piece)) >> 1]  << std::endl;
        game_t[i].capture? std::cout << "Capture" : std::cout << "Not Capture";
        std::cout << std::endl;
        std::cout << "This move takes away ";
        game_t[i].canCastle_off[white_king] ? std::cout << "whites king side castling,\n" : std::cout << std::endl;
        game_t[i].canCastle_off[white_queens] ? std::cout << "and whites queen side castling.\n" : std::cout << std::endl;
        game_t[i].canCastle_off[black_king] ? std::cout << "blacks king side castling,\n": std::cout << std::endl;
        game_t[i].canCastle_off[black_queens] ? std::cout << "and blacks queen side castling.\n": std::cout << std::endl;
    }
}

void board::print_generated_moves() {
    const char *pieces[] = {"king", "queen", "rook" , "knight" , "bishop" , "pawn"};
    std::cout << "\n";
    for(char i=0; i<move_t.size(); i++) {
        std::cout << "\n\n";
        std::cout << "i: " << static_cast<int>(i) << std::endl;
        std::cout << static_cast<int>(move_t[i].move[0]) << " " << static_cast<int>(move_t[i].move[1]) << std::endl;
        move_t[i].side ? std::cout << "dark" : std::cout << "light";
        std::cout << std::endl << "piece: " << pieces[(static_cast<int>(move_t[i].piece)) >> 1]  << std::endl;
        move_t[i].capture? std::cout << "Capture" : std::cout << "Not Capture";
        std::cout << std::endl;
        std::cout << "This move takes away ";
        move_t[i].canCastle_off[white_king] ? std::cout << "whites king side castling,\n" : std::cout << std::endl;
        move_t[i].canCastle_off[white_queens] ? std::cout << "and whites queen side castling.\n" : std::cout << std::endl;
        move_t[i].canCastle_off[black_king] ? std::cout << "blacks king side castling,\n": std::cout << std::endl;
        move_t[i].canCastle_off[black_queens] ? std::cout << "and blacks queen side castling.\n": std::cout << std::endl;
        
    }
}

void board::print_specified_move(int i) {
    const char *pieces[] = {"king", "queen", "rook" , "knight" , "bishop" , "pawn"};
    std::cout << "\n\n";
    std::cout << "i: " << i << std::endl;
    std::cout << static_cast<int>(move_t[i].move[0]) << " " << static_cast<int>(move_t[i].move[1]) << std::endl;
    move_t[i].side ? std::cout << "dark" : std::cout << "light";
    std::cout << std::endl << "piece: " << pieces[(static_cast<int>(move_t[i].piece)) >> 1]  << std::endl;
    move_t[i].capture? std::cout << "Capture" : std::cout << "Not Capture";
    std::cout << std::endl;
    std::cout << "This move takes away ";
    move_t[i].canCastle_off[white_king] ? std::cout << "whites king side castling,\n" : std::cout << std::endl;
    move_t[i].canCastle_off[white_queens] ? std::cout << "and whites queen side castling.\n" : std::cout << std::endl;
    move_t[i].canCastle_off[black_king] ? std::cout << "blacks king side castling,\n": std::cout << std::endl;
    move_t[i].canCastle_off[black_queens] ? std::cout << "and blacks queen side castling.\n": std::cout << std::endl;
    
}

void board::print_algebraic(int i) {
    char st = game_t[i].move[0];
    char dest = game_t[i].move[1];
    
    int row, collumn;
    
    char coll_lett[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    collumn = st % 8;
    row = (st / 8) + 1;
    
    std::cout << "\t" << coll_lett[collumn] << row;
    
    collumn = dest % 8;
    row = (dest / 8) + 1;
    
    std::cout << " " << coll_lett[collumn] << row;
}
