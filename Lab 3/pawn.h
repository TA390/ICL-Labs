#ifndef PAWN
#define PAWN


#include"chessPiece.h"

/******************************************************************************/

class Pawn: public ChessPiece{

 public:

  /* Constructor to initialise the base class and private attributes */
  Pawn(Colour _player, std::string _position, Board *_game_board);

  /* Overridden function - Function to test if a move is valid. The 
     parameter 'destination' is the position to move to. The function returns
     true if the piece can make the move and false otherwise */
  bool move(std::string destination);


 private:

  /* Attribute to store the starting position */
  std::string start_position;

  /* Attribute to store the min move possible */
  short min_move;

  /* Attribute to store the max move possible */
  short max_move;

  /* Attribute to store the direction of travel on the board */
  short direction;


  /* Overridden function - Function to test if the type of move is valid. The 
     parameter 'destination' is the position to move to. The function returns
     true if the type of move is one the piece can make and false otherwise */
  bool is_move_valid(std::string destination);
  
  /* Function to perform a valid vertical move on the board. The parameter
     'MOVE' is the number of the steps to be taken in the pawn's direction of
     travel. The parameter capture signals whether there is a piece at the
     destination square */
  bool vertical_move(const short MOVE, bool capture) const;

    /* Function to perform a valid diagonal move on the board. The parameter
       'MOVE' is the number of the steps to be taken in the pawn's direction of
       travel. The parameter capture signals whether there is a piece at the
       destination square */
  bool diagonal_move(const short MOVE, bool capture) const;

  /* Overridden function - Function to set attribute 'position' in the base
     class. The parameter 'new_position' is the value to assign */
  void set_position(std::string new_position);

  /* Function to test if the 'new_position' parameter is equal to a pawn's
     starting position. The function returns true if they are equal and
     false otherwise */
  bool equal_start_position(std::string new_position);
  
};

/******************************************************************************/


#endif
