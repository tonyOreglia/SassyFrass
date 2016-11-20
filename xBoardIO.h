#ifndef XBOARDIO_H
#define XBOARDIO_H

#include <cstring>
#include <iostream>
#include <thread>
#include <fstream>

#include "timeControl.h"
#include "global.h"

using namespace std;

class xBoard
{
	public:
		xBoard();
		void start_multithread_poll();
		void poll(); 

		int get_engine_time() {return engine_time;}
		int get_user_time() {return user_time;}
		void update_timers(sassTime a);
	private:
		char buff[200];
		char engine_move[8];
		char user_move[8];
		
		short unsigned int moves_per_time_period;
		short unsigned int sec_incremented_each_move;
		int engine_time; //in centi-seconds
		int user_time; //in centi-seconds
};
#endif

