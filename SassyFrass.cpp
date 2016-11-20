//Header File includes
#include <signal.h> //signal() fxn to ignore SIGTERM and SIGINT signals
#include <unistd.h>

//Class includes
#include "xBoardIO.h"
#include "timeControl.h"
#include "board.h"
#include "game.h"

//Global Flags
#include "global.h"

//Function Declarations
void change_turn();

using namespace std;

int main()
{
	signal(SIGTERM, SIG_IGN); //ignore interrupt signal SIGTERM sent from xBoard
	signal(SIGINT, SIG_IGN);  //ignort interrupt signal SIGINT sent from xBoard
	
	cout.setf(ios::unitbuf); // Make sure that the outputs are sent straight away to the GUI
	
	//declare classes:
	xBoard io;
	sassTime Timer; 
	board sassyBoard; 
	game SassyFrass; 
	 
	//declare variables:
	char move[8];   
	short unsigned int x=0;
	
	//initiate global flags to false;
	user_move_recieved_flag = false;
	set_time_variables_flag = false;
	update_engine_time_flag = false;
	update_user_time_flag = false;
	engine_turn = false;
	user_turn = false;
	start_timer = false;
	number_of_moves = 0;
	
	//initiate threads:
	io.start_multithread_poll(); //polls for xBoard input, and sends appropriate responses
	Timer.start_multithread_timer(); //deals with the timing of the game.
	
	/*sassyBoard.print_bit_board(sassyBoard.get_whiteKingBB());
	sassyBoard.print_bit_board(sassyBoard.get_blackKingBB());
	sassyBoard.print_bit_board(sassyBoard.get_occupiedBB());
	sassyBoard.print_bit_board(sassyBoard.get_whitePawnsBB());
	sassyBoard.print_bit_board(sassyBoard.get_blackPawnsBB());*/
	
	while(true)
		{
			if(set_time_variables_flag)//level command recieved
			{
				io.update_timers(Timer);
				set_time_variables_flag = false;
			}
			if(update_engine_time_flag) //time command sent
			{
				Timer.reset_base_time();
				Timer.set_engine_time(io.get_engine_time());
				update_engine_time_flag = false;
			}
			if(update_user_time_flag) //otim command sent
			{
				Timer.reset_base_time();
				Timer.set_user_time(io.get_user_time());
				update_user_time_flag = false;
			}
	
			if(user_move_recieved_flag)
			{
				//engine simulation will go in here; 
				//no pondering 
				change_turn();
				if(!x)
				{
					cout << "move e7e5" << endl;
					x=1;
					start_timer = true;
				}
				else
				{
					cout << "move d7d6" << endl;
				}
				user_move_recieved_flag = false;
				change_turn();
				number_of_moves++;
			}
		}
	
	return 0;
}

void change_turn()
{
	if(engine_turn)
	{
		engine_turn = false;
		user_turn = true;
	}
	else
	{
		engine_turn = true;
		user_turn = false;
	}
	
}
