#include<iostream>
#include <cstdlib>

#include"chessPiece.h"
#include"king.h"

using namespace std;

/******************************************************************************/

/* Initialise base class */

King::King(Colour _player, string _position, Board *_game_board)
  :ChessPiece(Piece::Kings, _player, _position, _game_board), MAX_MOVE{1}
{
  /* GameInformation is a class inherited by ChessPiece */
  GameInformation::set_king_position(_player, _position);
}

/******************************************************************************/

bool King::move(string destination)
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

bool King::is_move_valid(string destination)
{

  const short FILE_MOVEMENT = file_move(destination);
  const short RANK_MOVEMENT = rank_move(destination);

  switch(move_type(get_position(), destination))
  {
    case Move::Vertical:          
    case Move::Diagonal:

      return is_move_available(FILE_MOVEMENT, RANK_MOVEMENT);

    case Move::Horizontal:
      
      return is_castling_available(FILE_MOVEMENT, RANK_MOVEMENT);

    default:
      
      return false;
  }
}

/******************************************************************************/

bool King::is_move_available(short file_move, short rank_move) const
{
  return (abs(file_move) == MAX_MOVE || abs(rank_move) == MAX_MOVE);
}

/******************************************************************************/

bool King::remove_threat(const string THREAT)
{
  return move(THREAT);
}

/******************************************************************************/

void King::set_position(string new_position)
{
  /* set the static variable in game information when a king changes position */
  GameInformation::set_king_position(get_player(), new_position);

  /* set a king's own position variable */
  ChessPiece::set_position(new_position);
}

/******************************************************************************/
/*                                  CASTLING                                  */
/******************************************************************************/

/* filter any horizontal moves to test if an attempt at castling is made */

bool King::is_castling_available(const short FILE_MOVE, const short RANK_MOVE)
{
  if(abs(FILE_MOVE) == (MAX_MOVE +1) && RANK_MOVE == 0)
  {
    return castle_move(FILE_MOVE);
  }

  return is_move_available(FILE_MOVE, FILE_MOVE);
}

/******************************************************************************/

bool King::castle_move(const short FILE_MOVE)
{
  if(get_first_move() == false)
  {
    return false;
  }

  string move = get_position();
  /* 1 is left, -1 is right */
  short direction = (FILE_MOVE < 0) ? 1 : -1;
  /* the number of steps between the king and the rook */
  short steps = (direction == 1) ? 2 : 3;

  move_piece(direction, 0, move);

  /* test that there are no pieces between the king and the rook */
  for(short iter = 0; iter < steps; move_piece(direction, 0, move), iter++)
  {
    if(piece_at_position(move) == true)
    {
      return false;
    }
  }


  return castling_available(move, direction);
}

/******************************************************************************/

bool King::castling_available(string end_file, short direction)
{
  if(piece_at_position(end_file) == false)
  {
    return false;
  }

  /* test if the piece at the end of the file is a rook that has not moved */
  if((*GAME_BOARD)[end_file]->get_name() == Piece::Rooks &&
     (*GAME_BOARD)[end_file]->get_first_move() == true)
  {
    GameInformation::set_king_castling(end_file);
    GameInformation::set_castling_direction(direction);
    return true;
  }

  return false;
}

/******************************************************************************/
