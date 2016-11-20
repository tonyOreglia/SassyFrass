Board holds all relevant data about the position of the pieces, castl data, en passante, and repetition.
Game class must have immediate access to all of this information. Game class will evaluate positions, check move validity, and simulate moves.


To Do:
   -move ordering                (2)
   -research hashing
   -opening book
   -quessient search             (4)
   -handle mate                  (x) needs testing
   -refine evaluation()          (2)
   -FEN game input               (x)
   -save games in pgn
   -find way to play online
   -fifty move rule              (x)
   -repetition rule              (1)
   -insufficient material        (1)
   -increase readability
   -No repeated code!!           (1)
   -make_move, unmake: just do charBoard, then call fxn to update all BB's based on charBoard.
   -iterative deepening          (3)
   -bug free!!                   (1)
      +perft divide              (1)
   -push_promotion doesn't need side parameter     (x)
   -i don't like the mix of piece+side and true value pieces
   -i don't like the duality of side information in moves and as a board state
   -handle checkmates
   -debug mode
   -no old code