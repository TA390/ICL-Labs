#include<iostream>

#include"chessPiece.h"
#include"rook.h"

using namespace std;

/******************************************************************************/

Rook::Rook(Colour _player, string _position, Board *_game_board)
  :ChessPiece(Piece::Rooks, _player, _position, _game_board)
{
}

/******************************************************************************/

bool Rook::move(string destination)
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

bool Rook::is_move_valid(string destination)
{
  switch(move_type(get_position(), destination))
  {
    case Move::Vertical: 
    case Move::Horizontal:

      return true;

    default:
      
      return false;
  }
}

/******************************************************************************/
