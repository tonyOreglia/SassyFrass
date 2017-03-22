#ifndef GLOBAL_H
#define GLOBAL_H
typedef uint64_t U64;
extern bool DEBUG_MODE;
extern char DEPTH;
extern U64 kingAtt[64];
extern U64 knightAtt[64];
extern U64 north[64];
extern U64 northEast[64];
extern U64 east[64];
extern U64 southEast[64];
extern U64 south[64];
extern U64 southWest[64];
extern U64 west[64];
extern U64 northWest[64];
extern U64 sqBB[65];
extern short int pawn_bonus_b[64];
extern short int pawn_bonus_w[64];
extern short int knight_bonus[64];
extern short int bishop_bonus_b[64];
extern short int bishop_bonus_w[64];
extern short int king_bonus_b[64];
extern short int king_bonus_w[64];
extern short int king_endgame_bonus_b[64];
extern short int king_endgame_bonus_w[64];
#define ENGINE 0
#define USER 1
#define Hfile 0x8080808080808080ULL
#define Afile 0x101010101010101ULL
#define notHfile 0x7F7F7F7F7F7F7F7FULL
#define notAfile 0xFEFEFEFEFEFEFEFEULL
#define row4 0xFF000000ULL
#define row5 0xFF00000000ULL
#define LIGHT 0
#define DARK 1
#define EMPTY 16
#define NOT_MOVE 64
#define PAWN1_MASK   0b1000000000000000
#define PAWN2_MASK   0b0100000000000000
#define KNIGHT1_MASK 0b0010000000000000
#define KNIGHT2_MASK 0b0001000000000000
#define BISHOP1_MASK 0b0000100000000000
#define BISHOP2_MASK 0b0000010000000000
#define ROOK1_MASK   0b0000001000000000
#define ROOK2_MASK   0b0000000100000000
#define QUEEN1_MASK  0b0000000010000000
#define QUEEN2_MASK  0b0000000001000000
#define KING_MASK    0b0000000000100000
#define EXTRA1_MASK  0b0000000000010000
#define EXTRA2_MASK  0b0000000000001000
#endif
