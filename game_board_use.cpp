#include <iostream>
#include <string>
#include <climits>

#include "macros.txt"
#include "eng_global.h"

#include "game.h"
#include "board.h"

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

using namespace std;

bool convert_algebraic(string x, string y, int &xx, int &yy);

int main(int argc, char *argv[]) {
    string   cmnd;
	string   from;
	string   dest;
	string   input;
	string   quit = "quit";
	string   undo = "undo";
	string   move = "move";
	string   help = "help";
	string   observe = "observe";
	string   debug = "debug";
	string   set_depth = "set_depth";
	string   light = "light";
	string   dark = "dark";
	int      x, y;
	int      move_index;
	unsigned char move_count;
	bool     valid;
	bool     eng_v_eng = false;
	board    info;
	game     play(&info);
	bool     user_color, eng_color;
	assert(info.integrity());
	
	DEPTH = 5;
	DEBUG_MODE = false;
	
	for(int i=0; i < argc; i++) {
	   if(strcmp(argv[i], "-fen") == 0) {
         assert(info.import_FEN(argv[i+1], argv[i+2], argv[i+3], argv[i+4], argv[i+5], argv[i+6]));
	      assert(info.integrity());
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
	
	std::cout << "depth: " << static_cast<int>(DEPTH) << ", debug mode: " << DEBUG_MODE << std::endl;
	
	/*if(argc > 1) {
	   assert(info.import_FEN(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]));
	   assert(info.integrity());
	*///}
	
   std::cout << "\n\n\t\tWelcome to Tony's Chess Engine 3.0: SassyFrass\n\n\n";
	info.print_char_board();
	play.generate_moves();
	//play.print_moves();

   
   while(true) {
      std::cout << "\n\t\tWould you like to play as light or dark? ";
	   std::cin >> input;
   	if(input.compare(light) == 0) {
   	   user_color = LIGHT;
   	   break;
   	}
   	else if(input.compare(dark) == 0) {
   	   user_color = DARK;
   	   break;
   	}
   	else
   	   std::cout << "\n\t\tInvalid Entry.";
   }
   eng_color = !user_color;
	
	
	while(true) {
	   
	   if((info.get_side() == user_color) && (!eng_v_eng)) {
	      
	      if(info.get_move_count()) {
	         
	         while(true) {
	            std::cout << "\n\t\tEnter a command: ";
	            std::cin >> cmnd;
	            //std::cout << cmnd.length();
   	         if(cmnd.compare(move) == 0) {
         		   std::cout << "\n\t\tEnter origin and destination by algabriac notation: ";
         		   std::cin >> from;
         		   std::cin >> dest;
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
   	         else if(cmnd.length() == 2) {
         		   from = cmnd;
         		   std::cin >> dest;
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
         		else if(cmnd.compare(undo) == 0) {
         		   if(info.get_game_mv_cnt() > 2) {
            		   play.unmake_move();
            		   play.unmake_move();
            		   info.clear_all_moves();
            		   play.generate_moves();
            		   info.print_char_board();
         		   }
         		   else {
         		      play.unmake_move();
         		      info.clear_all_moves();
         		      play.generate_moves();
         		      info.print_char_board();
         		      break;
         		   }
         		}
         		else if(cmnd.compare(help) == 0) {
         		   std::cout << "\n\t\t:::::HELP MENU:::::\n\tType any of the following commands to learn more.";
         		   std::cout << "\n\t'move'\n\t'undo'\n\t'set_depth'\n\t'debug'\n\t'observe'\n\t'quit'";
         		}
         		else if(cmnd.compare(observe) == 0) {
         		   eng_v_eng = true;
         		   break;
         		}
         		else if(cmnd.compare(set_depth) == 0) {
         		   int depth_temp;
         		   std:cout << "\n\t\tSet ply: ";
         		   std::cin >> depth_temp;
         		   DEPTH = depth_temp;
         		   std::cout << "\n\t\tDepth set.";
         		}
         		else if(cmnd.compare(debug) == 0) {
         		   if(DEBUG_MODE) {
         		      DEBUG_MODE = false;
         		      std::cout << "\n\t\tDebug mode is now off.";
         		   }
         		   else {
            		   DEBUG_MODE = true;
            		   std::cout << "\n\t\tDebug mode is now on.";
         		   }
         		}
         		else if(cmnd.compare(quit) == 0) {
         		   info.print_game_moves();
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
	   if((info.get_side() != user_color) || eng_v_eng) {
	      if(info.get_side() == LIGHT) {
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
	      move_index = play.get_engine_move();
	      if(DEBUG_MODE)
	         std::cout << "\n\tperft_total:\t" << play.get_perft_total() << std::endl;
	      std::cout << "engine move: " << static_cast<int>(play.get_engine_move()) << std::endl;
	   }
     
      move_count = info.get_move_count();
	   if(move_index != NOT_MOVE) {                 //is move in the generated move list
	      if(play.make_move(move_index)) {          //make move fxn checks for king safety, and legal castling (not checked for in move generator)
   	      info.clear_moves(0, move_count);             //clear move list, don't clear the generated dark moves since those are
   	         if(info.fifty_move_check())
   	            std::cout << "\n\t\tFifty Move Resignation option available\n\n";
      	      //info.clear_moves(move_count, info.get_move_count());                                              //the proper move list for next move.
      	      //play.print_moves();                          //print new valid moves list for dark
      	      info.print_char_board();                     //print the new valid position
      	      std::cout << "\tLast Move:";
      	      info.print_algebraic(info.get_game_mv_cnt()-1);
      	      cout << "\n\tscore: " << play.evaluate() << "\n\n";
      	      
      	      //int count = 0;
      	      /*for(int i=move_count; i < info.get_move_count(); i++) {
      	         if(play.make_move(i)) {
      	            play.unmake_move();
      	            count = 1;
      	            break;
      	         }
      	      }
      	      if(!count) {
      	         std::cout << "\n\t\tCheckmate!";
      	      }*/
   	      
	      }
	      else {
	         std::cout << "\n\n\t\tInvalid Move.\n\t\tLeave's King in Check or castles illegally.\n\n";
	      }
	   }
      else { //the indicated move is not on the generated move list
         std::cout << "\n\n\t\tInvalid Move.\n\n";
      }
	}
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
