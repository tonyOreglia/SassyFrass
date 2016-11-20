#ifndef TIMECNTRL_H
#define TIMECNTRL_H

#include <unistd.h>
#include <time.h>
#include <thread>
#include <fstream>
#include <iostream> 

#define CLKS_PER_CENTISEC CLOCKS_PER_SEC*100 

using namespace std;

class sassTime
{
	friend class xBoard;
	public:
		sassTime();
		void start_multithread_timer(); 
		void timer(); 
		void set_engine_time(int x) {engine_time = x;}
		void set_user_time(int x) {user_time = x;}
		void reset_base_time() {t = clock();}
	private:
		clock_t t;  //base time
		short unsigned int moves_per_time_period;
		short unsigned int sec_incremented_each_move;
		int engine_time; //in centiSeconds
		int user_time; //in centiSeconds
		
};
#endif

