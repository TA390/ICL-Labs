#include<iostream>

#include"chessPiece.h"
#include"bishop.h"

using namespace std;

/******************************************************************************/

Bishop::Bishop(Colour _player, string _position, Board *_game_board)
  :ChessPiece(Piece::Bishops, _player, _position, _game_board)
{
}

/******************************************************************************/

bool Bishop::move(string destination)
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

bool Bishop::is_move_valid(string destination)
{
  switch(move_type(get_position(), destination))
  {
    case Move::Diagonal:
 
      return true;

    default:
      
      return false;
  }
}

/******************************************************************************/
