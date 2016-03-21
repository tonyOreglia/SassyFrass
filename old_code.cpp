///was used in game.cpp (sassyfrass folder) to check if a move is legal
///given x and y 0..63 origin, destination coordinates.
///replaced with a generate list of valid moves
bool game::generate_moves(board a, unsigned short int x, unsigned short int y){
	bool valid=false;
	U64 valid_attack_set=0;
	U64 temp;
	char sigBit;

	if(a.pieceBB[a.white_bishops] & (1ULL << x)){
		valid_attack_set |= northEastAttack(x, a);
		valid_attack_set |= northWestAttack(x, a);
		valid_attack_set |= southEastAttack(x, a);
		valid_attack_set |= southWestAttack(x, a);
		
		valid_attack_set &= (~a.pieceBB[a.white_pieces]);
		
		a.print_bit_board(valid_attack_set);
		return valid;
	}
	else if(a.pieceBB[a.black_bishops] & (1ULL << x)){
		valid_attack_set |= northEastAttack(x, a);
		valid_attack_set |= northWestAttack(x, a);
		valid_attack_set |= southEastAttack(x, a);
		valid_attack_set |= southWestAttack(x, a);
		
		valid_attack_set &= (~a.pieceBB[a.black_pieces]);
		
		a.print_bit_board(valid_attack_set);
		return (valid_attack_set & (1ULL << y));
	}
	else if(a.pieceBB[a.white_rooks] & (1ULL << x)) {
		valid_attack_set |= northAttack(x, a);
		valid_attack_set |= eastAttack(x, a);
		valid_attack_set |= southAttack(x, a);
		valid_attack_set |= westAttack(x, a);
		
		valid_attack_set &= (~a.pieceBB[a.white_pieces]);
		
		a.print_bit_board(valid_attack_set);
		return (valid_attack_set & (1ULL << y));
	}
	else if(a.pieceBB[a.black_rooks] & (1ULL << x)) {
		valid_attack_set |= northAttack(x, a);
		valid_attack_set |= eastAttack(x, a);
		valid_attack_set |= southAttack(x, a);
		valid_attack_set |= westAttack(x, a);
		
		valid_attack_set &= (~a.pieceBB[a.black_pieces]);
		
		a.print_bit_board(valid_attack_set);
		return (valid_attack_set & (1ULL << y));
	}
	else if(a.pieceBB[a.white_queens] & (1ULL << x)) {
		valid_attack_set |= northAttack(x, a);
		valid_attack_set |= eastAttack(x, a);
		valid_attack_set |= southAttack(x, a);
		valid_attack_set |= westAttack(x, a);
		valid_attack_set |= northEastAttack(x, a);
		valid_attack_set |= northWestAttack(x, a);
		valid_attack_set |= southEastAttack(x, a);
		valid_attack_set |= southWestAttack(x, a);
		
		valid_attack_set &= (~a.pieceBB[a.white_pieces]);
			
		a.print_bit_board(valid_attack_set);
		return (valid_attack_set & (1ULL << y));
	}
	else if(a.pieceBB[a.black_queens] & (1ULL << x)) {
		valid_attack_set |= northAttack(x, a);
		valid_attack_set |= eastAttack(x, a);
		valid_attack_set |= southAttack(x, a);
		valid_attack_set |= westAttack(x, a);
		valid_attack_set |= northEastAttack(x, a);
		valid_attack_set |= northWestAttack(x, a);
		valid_attack_set |= southEastAttack(x, a);
		valid_attack_set |= southWestAttack(x, a);
		
		valid_attack_set &= (~a.pieceBB[a.black_pieces]);
		
		a.print_bit_board(valid_attack_set);
		return (valid_attack_set & (1ULL << y));
	}
	
	else if(a.pieceBB[a.white_knights] & (1ULL << x))
	{
		valid_attack_set = (knightAtt[x] & (~a.pieceBB[a.white_pieces]));
		a.print_bit_board(valid_attack_set);
		return (valid_attack_set & (1ULL << y));
	}
	else if(a.pieceBB[a.white_king] & (1ULL << x))
	{
		valid_attack_set = (kingAtt[x] & (~a.pieceBB[a.white_pieces]));
		
		a.print_bit_board(valid_attack_set);
		return (valid_attack_set & (1ULL << y));
	}
	else if(a.pieceBB[a.black_knights] & (1ULL << x))
	{
		valid_attack_set = (knightAtt[x] & (~a.pieceBB[a.black_pieces]));
		a.print_bit_board(valid_attack_set);
		return (valid_attack_set & (1ULL << y));
	}
	else if(a.pieceBB[a.black_king] & (1ULL << x))
	{
		valid_attack_set = (kingAtt[x] & (~a.pieceBB[a.black_pieces]));
		a.print_bit_board(valid_attack_set);
		return (valid_attack_set & (1ULL << y));
	}
	
	else if(a.pieceBB[a.white_pawns] & (1ULL << x)) {
		
		valid_attack_set = wpush1[x] & (~a.occupied_squares);
		valid_attack_set |= wpush2[x] & (~a.occupied_squares);
		valid_attack_set |= wAtt[x] & a.pieceBB[a.black_pieces];
		a.print_bit_board(valid_attack_set);
		return (valid_attack_set & (1ULL << y));
	}
	else if(a.pieceBB[a.black_pawns] & (1ULL << x)) {
		valid_attack_set = bpush1[x] & (~a.occupied_squares);
		valid_attack_set |= bpush2[x] & (~a.occupied_squares);
		valid_attack_set |= bAtt[x] & a.pieceBB[a.white_pieces];
		a.print_bit_board(valid_attack_set);
		return (valid_attack_set & (1ULL << y));
	}
	else
		return valid;
		}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

 std::cout << "white kings: \n";
      info.print_bit_board(info.get_whiteKingBB());
      std::cout << "white pawns\n";
		info.print_bit_board(info.get_whitePawnsBB());
		std::cout << "occupied: \n";
		info.print_bit_board(info.get_occupiedBB());
		std::cout << "white knights: \n";
		info.print_bit_board(info.get_whiteKnights());
		
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
int game::search_max(char depth, int start) {
   if( depth == 0) return evaluate();
   int max = -30000;
   bool root = depth == DEPTH;
   int total = a->move_t.size();
   if(root) {
      move_count = a->move_t.size();
   }
   for(int j = start; j < total; j++) {
      if(make_move(j)) {
         score = search_min(depth - 1, total);
         unmake_move(j);
         a->clear_moves(total, a->move_t.size());
         if(score > max) {
            max = score;
            if(root) engine_move = j;
         }
      }
      //else {
       //  j--;
      //   total--;
      //}
   }
   return max;
}

   int game::search_min(char depth, int start) {
   if( depth == 0) return evaluate();
   int min = 30000;
   bool root = depth == DEPTH;
   int total = a->move_t.size();
   if(root) {
      move_count = a->move_t.size();
   }
   for(int j=start; j < total; j++) {
      if(make_move(j)) {
         score = search_max(depth - 1, total);
         unmake_move(j);
         a->clear_moves(total, a->move_t.size());
         if(score < min) {
            min = score;
            if(root) engine_move = j;
         }
      }
      //else {
       //  j--;
         //total--;
      //}
   }
   return min;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

   const char *algebraic[] = {"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
			"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
			"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
			"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
			"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
			"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
			"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
			"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"};
			
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
int game::evaluate_new(){
   int pos = 0;
   char light_bishop_count = 0;
   char dark_bishop_count = 0;
   
   
   
   
   //int piece_values[] = {2000, 900, 500, 320, 325, 100};
   //pawn_value=100, knight_value = 320, bishop_value = 325, queen_value=900, rook_value=500;
	for(char i=0; i<64; i++) {
	   switch(a->charBoard[i]) {
	      case a->white_king:
	         pos += 2000;
	         pos += king_bonus_w[i];
	         //pos += a->battle_field[LIGHT][i];
	         //pos -= a->battle_field[DARK][i];
	         //if(a->battle_field[LIGHT][i] < a->battle_field[DARK][i])
	           // pos -= ((a->battle_field[DARK][i] - a->battle_field[LIGHT][i]) * 10);
	         break;
	      case a->white_queens:
	         pos += 900;
	         //pos += a->battle_field[LIGHT][i];
	         //pos -= a->battle_field[DARK][i];
	         //if(a->battle_field[LIGHT][i] < a->battle_field[DARK][i])
	           // pos -= ((a->battle_field[DARK][i] - a->battle_field[LIGHT][i]) * 10);
	         break;
	      case a->white_rooks:
	         pos += 500;
	        // pos += a->battle_field[LIGHT][i];
	        // pos -= a->battle_field[DARK][i];
	        //if(a->battle_field[LIGHT][i] < a->battle_field[DARK][i])
	        //    pos -= ((a->battle_field[DARK][i] - a->battle_field[LIGHT][i]) * 10);
	         break;
	      case a->white_bishops:
	         pos += 325;
	         pos += bishop_bonus_w[i];
	         light_bishop_count++;
	  //       pos += a->battle_field[LIGHT][i];
	    //     pos -= a->battle_field[DARK][i];
	      //   if(a->battle_field[LIGHT][i] < a->battle_field[DARK][i])
	        //    pos -= ((a->battle_field[DARK][i] - a->battle_field[LIGHT][i]) * 10);
	         break;
	      case a->white_knights:
	         pos += 320;
	         pos += knight_bonus[i];
//	         pos += a->battle_field[LIGHT][i];
//	         pos -= a->battle_field[DARK][i];
//	         if(a->battle_field[LIGHT][i] < a->battle_field[DARK][i])
//	            pos -= ((a->battle_field[DARK][i] - a->battle_field[LIGHT][i]) * 10);
	         break;
	      case a->white_pawns:
	         pos += 100;
	         pos += pawn_bonus_w[i];
//	         pos += a->battle_field[LIGHT][i];
//	         pos -= a->battle_field[DARK][i];
//	         if(a->battle_field[LIGHT][i] < a->battle_field[DARK][i])
//	            pos -= ((a->battle_field[DARK][i] - a->battle_field[LIGHT][i]) * 10);
	         break;
	      case a->black_king:
	         pos -= 2000;
	         pos -= king_bonus_b[i];
//	         pos -= a->battle_field[DARK][i];
//	         pos += a->battle_field[LIGHT][i];
//	         if(a->battle_field[DARK][i] < a->battle_field[LIGHT][i])
//	            pos += ((a->battle_field[LIGHT][i] - a->battle_field[DARK][i]) * 10);
	         break;
	      case a->black_queens:
	         pos -= 900;
//	         pos -= a->battle_field[DARK][i];
//	         pos += a->battle_field[LIGHT][i];
//	         if(a->battle_field[DARK][i] < a->battle_field[LIGHT][i])
//	            pos += ((a->battle_field[LIGHT][i] - a->battle_field[DARK][i]) * 10);
	         break;
	      case a->black_rooks:
	         pos -= 500;
//	         pos -= a->battle_field[DARK][i];
//	         pos += a->battle_field[LIGHT][i];
//	         if(a->battle_field[DARK][i] < a->battle_field[LIGHT][i])
//	            pos += ((a->battle_field[LIGHT][i] - a->battle_field[DARK][i]) * 10);
	         break;
	      case a->black_bishops:
	         pos -= 325;
	         pos -= bishop_bonus_b[i];
	         dark_bishop_count++;
//	         pos -= a->battle_field[DARK][i];
//	         pos += a->battle_field[LIGHT][i];
//	         if(a->battle_field[DARK][i] < a->battle_field[LIGHT][i])
//	            pos += ((a->battle_field[LIGHT][i] - a->battle_field[DARK][i]) * 10);
	         break;
	      case a->black_knights:
	         pos -= 320;
	         pos -= knight_bonus[i];
//	         pos -= a->battle_field[DARK][i];
//	         pos += a->battle_field[LIGHT][i];
//	         if(a->battle_field[DARK][i] < a->battle_field[LIGHT][i])
//	            pos += ((a->battle_field[LIGHT][i] - a->battle_field[DARK][i]) * 10);
	         break;
	      case a->black_pawns:
	         pos -= 100;
	         pos -= pawn_bonus_b[i];
//	         pos -= a->battle_field[DARK][i];
//	         pos += a->battle_field[LIGHT][i];
//	         if(a->battle_field[DARK][i] < a->battle_field[LIGHT][i])
//	            pos += ((a->battle_field[LIGHT][i] - a->battle_field[DARK][i]) * 10);
	         break;
	   }
	}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
bool game::generate_attack_BBs() {


   U64 valid_attack_set, temp;
	char sigBit, sq;
	
      
      //BISHOP MOVES
      
		temp = a->pieceBB[a->bishops+a->side];
		while(temp) {
			sq = lsb_scan(temp);
			temp ^= sqBB[sq];

			valid_attack_set = northEastAttack(sq);
			valid_attack_set |= northWestAttack(sq);
			valid_attack_set |= southEastAttack(sq);
			valid_attack_set |= southWestAttack(sq);
			//valid_attack_set &= (~a->pieceBB[a->pieces+a->side]);

			if(valid_attack_set) {
				dest_sqs = serialize(valid_attack_set);
				for(char i=0; dest_sqs[i]!=NOT_MOVE; i++) {
				   a->battle_field[a->side][dest_sqs[i]] += a->battle_value[a->Bbishops];
				}
			}
		}
		
		//////////////////////////////////////
		//ROOK MOVES
		/////////////////////////////////////
		char j=0;
		temp = a->pieceBB[a->rooks+a->side];
		while(temp) {
			sq = lsb_scan(temp);
			temp ^= sqBB[sq];

			valid_attack_set = northAttack(sq);
			valid_attack_set |= southAttack(sq);
			valid_attack_set |= eastAttack(sq);
			valid_attack_set |= westAttack(sq);
			//valid_attack_set &= (~a->pieceBB[a->pieces+a->side]);
			
			///need to turn off castling for rook moves! ?????/////
			if(valid_attack_set) {
			   dest_sqs = serialize(valid_attack_set);
				for(char i=0; dest_sqs[i]!=NOT_MOVE; i++) {
				   a->battle_field[a->side][dest_sqs[i]] += a->battle_value[a->Brooks];
				}
			}
		}
      
      
		temp = a->pieceBB[a->queens+a->side];
		
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
			
			//valid_attack_set &= (~a->pieceBB[a->pieces+a->side]);
			
			if(valid_attack_set) {
			   dest_sqs = serialize(valid_attack_set);
			   for(char i=0; dest_sqs[i]!=NOT_MOVE; i++) {
				   a->battle_field[a->side][dest_sqs[i]] += a->battle_value[a->Bqueens];
				}
			}
			
		}
		
		////////////////////////////////
		//KNIGHT MOVES
		////////////////////////////////
		temp = a->pieceBB[a->knights+a->side];
		while(temp) {
			sq = lsb_scan(temp);
			temp ^= sqBB[sq];
			
			valid_attack_set = knightAtt[sq];
			
			dest_sqs = serialize(valid_attack_set);
			for(char i=0; dest_sqs[i]!=NOT_MOVE; i++) {
				   a->battle_field[a->side][dest_sqs[i]] += a->battle_value[a->Bknights];
				}
			
		}
		/////////////////////////////////////////////////////
		//KING MOVES//
		////////////////////////////////////////////////////
		
		char king = a->king+a->side;
		temp = a->pieceBB[king];
		
		sq = lsb_scan(temp);
		valid_attack_set = kingAtt[sq];
		
		if(valid_attack_set) {
		   dest_sqs = serialize(valid_attack_set);
		   for(char i=0; dest_sqs[i]!=NOT_MOVE; i++) {
				   a->battle_field[a->side][dest_sqs[i]] += a->battle_value[a->Bking];
				}
		}
			
		
		//////////////////////////////////////////
		//PAWN MOVES///
		//////////////////////////////////////////
		if(a->side==LIGHT) {
			
			valid_attack_set = a->pieceBB[a->white_pawns]; //attack right
			valid_attack_set = valid_attack_set << 9;
			valid_attack_set &= notAfile;
			//valid_attack_set &= (a->pieceBB[a->black_pieces] | sqBB[a->game_t.back().enPassant]);   //a->enPassant[0]);
			
			if(valid_attack_set) {
			   dest_sqs = serialize(valid_attack_set);
		      for(char i=0; dest_sqs[i]!=NOT_MOVE; i++) {
				   a->battle_field[a->side][dest_sqs[i]] += a->battle_value[a->Bpawns];
				}
			}
			
			valid_attack_set = a->pieceBB[a->white_pawns]; //attack left
			valid_attack_set = valid_attack_set << 7;
			valid_attack_set &= notHfile;
			//valid_attack_set &= (a->pieceBB[a->black_pieces] | sqBB[a->game_t.back().enPassant]);
			
			if(valid_attack_set) {
			   dest_sqs = serialize(valid_attack_set);
		      for(char i=0; dest_sqs[i]!=NOT_MOVE; i++) {
				   a->battle_field[a->side][dest_sqs[i]] += a->battle_value[a->Bpawns];
				}
			}
		}
		//DARK//
		else {
			valid_attack_set = a->pieceBB[a->black_pawns]; //attack down and left
			valid_attack_set = valid_attack_set >> 9;
			valid_attack_set &= notHfile;
			//valid_attack_set &= (a->pieceBB[a->white_pieces] | sqBB[a->game_t.back().enPassant]);
			
			if(valid_attack_set) {
				dest_sqs = serialize(valid_attack_set);
				 for(char i=0; dest_sqs[i]!=NOT_MOVE; i++) {
				   a->battle_field[a->side][dest_sqs[i]] += a->battle_value[a->Bpawns];
				}
			}
			
			
			valid_attack_set = a->pieceBB[a->black_pawns]; //attack down and right
			valid_attack_set = valid_attack_set >> 7;
			valid_attack_set &= notAfile;
			//valid_attack_set &= (a->pieceBB[a->white_pieces] | sqBB[a->game_t.back().enPassant]);
			
			if(valid_attack_set) {
				dest_sqs = serialize(valid_attack_set);
				 for(char i=0; dest_sqs[i]!=NOT_MOVE; i++) {
				   a->battle_field[a->side][dest_sqs[i]] += a->battle_value[a->Bpawns];
				}
			}
		}
   
   return true;
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////