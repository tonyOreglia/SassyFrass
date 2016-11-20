#include "timeControl.h"
#include "global.h"

sassTime::sassTime()
{
	engine_time=0;
	user_time = 0;
	moves_per_time_period = 0;
	sec_incremented_each_move = 0;
}

void sassTime::start_multithread_timer()
{
	std::thread t2(&sassTime::timer, this);
	t2.detach();
}

void sassTime::timer()
{
	ofstream g("time_out.txt"); 
	g << "engine: " << engine_time << " user: " << user_time << endl << moves_per_time_period << endl << sec_incremented_each_move << endl;
	float t_transpired;
	while(true)
	{
		while(start_timer)
		{
			t = clock();
			while(engine_turn)
			{
				t_transpired = (float)(clock() - t) / CLKS_PER_CENTISEC; 
				if(t_transpired > engine_time)
					cout << "resign" << endl;
			}
			t = clock();
			engine_time += sec_incremented_each_move*100;
			while(user_turn)
			{
				usleep(1000000);
				t_transpired = (float)(clock() - t) / CLKS_PER_CENTISEC; 
				g << t_transpired << endl;
				if(t_transpired > user_time)
					;//print result
			}
			user_time += sec_incremented_each_move*100;
			g << endl;
		}
	}
}
