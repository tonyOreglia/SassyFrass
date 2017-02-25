OBJS = sass.o board.o game.o eng_global.o
CC = g++
DEBUG = -g
CFLAGS = -c $(DEBUG)
LFLAGS = $(DEBUG)
ISO = -std=c++0x

sass : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) $(ISO) -o sass

game_board_use.o : sass.cpp board.h game.h eng_global.h
	$(CC) $(CFLAGS) $(ISO) sass.cpp

board.o : board.h board.cpp eng_global.h
	$(CC) $(CFLAGS) $(ISO) board.cpp

game.o : game.h game.cpp board.h eng_global.h
	$(CC) $(CFLAGS) $(ISO) game.cpp

eng_global.o : eng_global.h eng_global.cpp
	$(CC) $(CFLAGS) $(ISO) eng_global.cpp

clean:
	\rm *.o *~ sass

tar:
	tar cfv sass.tar sass.cpp board.h board.cpp game.h game.cpp eng_global.h eng_global.cpp macros.txt
