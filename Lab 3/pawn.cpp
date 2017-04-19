#include<iostream>

#include"chessPiece.h"
#include"pawn.h"

using namespace std;

/******************************************************************************/

/* Initialise base class and private attributes */

Pawn::Pawn(Colour _player, string _position, Board *_game_board)
  :ChessPiece(Piece::Pawns, _player, _position, _game_board)
{
  direction = (_player == Colour::White) ? -1 : 1;

  min_move = 1 * direction;
  max_move = 2 * direction;

  start_position = _position;
}

/******************************************************************************/

bool Pawn::move(string destination)
{
  if(is_move_valid(destination) == false)
  {
    return false;
  }

  if(piece_on_path(get_position(), destination) == true)
  {  
    return false;
  }

  return true;
}

/******************************************************************************/

/* test that the type of move is valid for a pawn */

bool Pawn::is_move_valid(string destination)
{

  const short MOVE = rank_move(destination);
  bool capture = piece_at_position(destination);

  switch(move_type(get_position(), destination))
  {
    case Move::Vertical:
 
      return vertical_move(MOVE, capture);
      
    case Move::Diagonal:
      
      return diagonal_move(MOVE, capture);

    default:
      
      return false;
  }
}

/******************************************************************************/

/* test vertical moves */

bool Pawn::vertical_move(const short MOVE, bool capture) const
{
  if(capture == false)
  {
    return (MOVE == min_move || MOVE == max_move);
  }
  else
  {
    return false;
  }
}

/******************************************************************************/

/* test diagonal moves */

bool Pawn::diagonal_move(const short MOVE, bool capture) const
{
  if(capture == true)
  {
    return (MOVE == min_move);
  }  
  else
  {
    return false;
  }
}

/******************************************************************************/
void Pawn::set_position(string new_position)
{
  ChessPiece::set_position(new_position);

  /* if 'new_positon' is the pawn's original position, set max_move back to 2
     as a test move may have been executed previously */
  max_move = (equal_start_position(new_position)) ? 2 : 1;

}

/******************************************************************************/

bool Pawn::equal_start_position(string position)
{
  return (position == start_position);
}

/******************************************************************************/
