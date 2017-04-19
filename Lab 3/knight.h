#ifndef KNIGHT
#define KNIGHT


#include"chessPiece.h"

/******************************************************************************/

class Knight: public ChessPiece{

 public:

  /* Constructor to initialise the base class */
  Knight(Colour _player, std::string _position, Board *_game_board);

  /* Overridden function - Function to test if a move is valid. The 
     parameter 'destination' is the position to move to. The function returns
     true if the piece can make the move and false otherwise */
  bool move(std::string destination);


 private:

  /* Overridden function - Function to test if the type of move is valid. The 
     parameter 'destination' is the position to move to. The function returns
     true if the type of move is one the piece can make and false otherwise */
  bool is_move_valid(std::string destination);

  /* Overridden function - function to return a vector of all possible moves
     available to a knight from its current position */
  std::vector<std::string> move_list();

  /* Overridden function - function to change the values in the array 
     'movements' in order to generate all moves possible. The parameter 'COUNT'
     is the current move being generate in a range of [0:8], with 'MID_POINT'
     the middle of the range. The template is used to deduce the arrays size */
  template<int SIZE>
  void create_next_move(short movements[SIZE],
		        const short COUNT, const short MID_POINT);

};

/******************************************************************************/


#endif
