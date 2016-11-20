#include "xBoardIO.h"
#include "global.h"

xBoard::xBoard()
{
	engine_time = 0;
	user_time = 0;
	moves_per_time_period = 0;
	sec_incremented_each_move = 0;
	cout.setf(ios::unitbuf);
}

void xBoard::start_multithread_poll()
{
	thread t1(&xBoard::poll, this);
	//t1.join(); //waits for thread to finish if instantiated
	t1.detach();
}
void xBoard::update_timers(sassTime a)
{
	a.moves_per_time_period = moves_per_time_period; 
	a.sec_incremented_each_move = sec_incremented_each_move;
	a.engine_time = engine_time; 
	a.user_time = user_time;
} 

void xBoard::poll()
{	
	ofstream g("out.txt");
	while(true)
	{
		cin >> buff;
		g << buff << endl;
		
		if(!strcmp(buff, "force"))
			;//stop clock, engine will play neither color but will check for legal moves
		else if(!strcmp(buff, "go"))
			;//leave force mode, play/associate engine with the color that is on move. Start clock.
		else if(!strcmp(buff, "xboard"))
			;
		else if(!strcmp(buff, "protover"))
		{
			cin >> buff; 
			cout << "feature myname=\"Tony's Chess Engine: SassyFrass\"";
			cout << " usermove=1";
			cout << " done=1" << endl;
			
			g << buff << endl;
			g << "\tfeature myname=\"Tony's Chess Engine: SassyFrass\"";
			g << " usermove=1 color=0";
			g << " done=1" << endl;
		}
		else if(!strcmp(buff, "new"))
		{
			user_turn = true;//set board to starting position; white to move as user 
		}
		else if(!strcmp(buff, "quit"))
		{
			exit(0);
		}
		else if(!strcmp(buff, "resign"))
		{
			;//user resigned
		}
		else if(!strcmp(buff, "hint"))
		{
			cout << "Hint: FUCK" << endl;
			g << "\tHint: FUCK" << endl;
		}
		else if(!strcmp(buff, "usermove")) 
		{
			cin >> buff; 
			strcpy(user_move, buff);
			user_move_recieved_flag = true;  
			
			g << buff << endl;
		}
		else if(!strcmp(buff, "level"))
		{
			cin >> buff;//number of moves before resetting the given time
			moves_per_time_period = atoi(buff); 
			
			g << buff << endl;
				
			cin >> buff; //time for game
			engine_time = atoi(buff)*60*100; //centiseconds
			user_time = engine_time;
			
			g << buff << endl;
			if(!engine_time)
			{
				strtok(buff, ":");
				engine_time = atoi(strtok(NULL, ":")) * 100; //centiseconds
				user_time = engine_time; 
				g << "\tengine_time/user_time: " << engine_time << endl;
			}
			
			cin >> buff;
			sec_incremented_each_move = atoi(buff); 
			
			g << buff << endl;
			
			set_time_variables_flag = true;
			//atoi(buff) is the time in seconds incremented after each move 
		}
		else if(!strcmp(buff, "time"))
		{
			cin >> buff;//updating engine time left
			g << buff;
			engine_time = atoi(buff); //centiseconds
			update_engine_time_flag = true;
		}
		else if(!strcmp(buff, "otim"))
		{
			cin >> buff;//updating user time left
			g << buff;
			user_time = atoi(buff);  //centiseconds
			update_user_time_flag = true; 
		}
		else if(!strcmp(buff, "draw"))
			;//user is offering a draw.
		else if(!strcmp(buff, "accepted"))
		{
			cin >> buff; //without this the accepted usermove get's sent into the move conditional!
			g << buff << endl;
		}
		else if(!strcmp(buff, "result"))
		{
			cin >> buff; //game over 
			
			g << buff << endl;
		}
		else if(!strcmp(buff, "random"));
		else if(!strcmp(buff, "post"));
		else if(!strcmp(buff, "hard")); //turn on pondering
		else if(!strcmp(buff, "{xboard"));
		else if(!strcmp(buff, "exit}"));
		//else if(!strcmp(buff, "white")); //set white on move, set engine to play black
		//else if(!strcmp(buff, "black")); //set black on move, set engine to play white
		
		else 
		{
			cout << "Unrecognized Command" << endl;
			g << "\tUnrecognized Command" << endl;
		}
	
	}
}
