#ifndef QUEEN
#define QUEEN


#include"chessPiece.h"

/******************************************************************************/

class Queen: public ChessPiece{

 public:

  /* Constructor to initialise the base class */
  Queen(Colour _player, std::string _position, Board *_game_board);

  /* Overridden function - Function to test if a move is valid. The 
     parameter 'destination' is the position to move to. The function returns
     true if the piece can make the move and false otherwise */
  bool move(std::string destination);


 private:

  /* Overridden function - Function to test if the type of move is valid. The 
     parameter 'destination' is the position to move to. The function returns
     true if the type of move is one the piece can make and false otherwise */
  bool is_move_valid(std::string destination);

};

/******************************************************************************/


#endif
