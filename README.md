# SassyFrass
Tony's Chess Engine

2nd Chess engine. Major changes is in board representation. This chess engine employs bitboards to represent a given chess position. Bitboards use 64 bit integers to represent a specific aspect of the position, like location of White Kings.

By combining bitboards you can efficient store position. More importantly with bitwise operations you can generate valid moves very efficiently.
