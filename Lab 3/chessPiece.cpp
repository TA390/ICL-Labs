#include<iostream>
#include<cstdlib>
#include<cassert>


#include"chessPiece.h"

using namespace std;


/******************************************************************************/
/*                              PUBLIC  FUNCTIONS                             */
/******************************************************************************/

/* initialise all private and protected attributes */

ChessPiece::ChessPiece(Piece _name, Colour _player, string _position, 
		       Board *_game_board)
  : GAME_BOARD{_game_board}, NAME{_name}, PLAYER{_player}, first_move{true}
{
  position = _position;
}

/******************************************************************************/

ChessPiece::~ChessPiece()
{
}

/******************************************************************************/

bool ChessPiece::capture_or_block()
{
  /* retrieve the position of a player's king and the threatening piece */
  const string KING = king_position(get_player());
  const string THREAT = threatening_piece(get_player());

  /* determine the move of the threatening piece to know whether a block is
     feasible (i.e. if not a knight block or capture, otherwise capture only */

  switch(move_type(THREAT, KING))
  {
    case Move::Horizontal:
    case Move::Vertical:
    case Move::Diagonal:
    
      /* attempt to block or capture the threatening piece */
      return remove_threat(THREAT);
      
    case Move::Horse:
    
      /* attempt to capture the knight */
      return move(THREAT);

    default:
    
      return false;
  }
}

/******************************************************************************/

vector<string> ChessPiece::move_list()
{
  /* using a combination of zero's and one's generate all moves one step away
   for any piece (this function is overridden by the knight class) */

  short movements[] = {-1,0};
  return move_grid(movements);
}

/******************************************************************************/

Piece ChessPiece::get_name() const
{
  return NAME;
}

/******************************************************************************/

Colour ChessPiece::get_player() const
{
  return PLAYER;
}

/******************************************************************************/

bool ChessPiece::get_first_move() const
{
  return first_move;
}

/******************************************************************************/

void ChessPiece::set_position(const string new_position)
{
  position = new_position;
}

/******************************************************************************/

void ChessPiece::set_first_move()
{
  first_move = false;
}

/******************************************************************************/
/*                             PROTECTED FUNCTIONS                            */
/******************************************************************************/

/* determine the type of move being executed */

Move ChessPiece::move_type(const string POSITION, const string DESTINATION) const
{  

  /* straight line moves */
  const short LINEAR_MOVE = 0;

  /* move by a knight */
  const short KNIGHT_MOVE = 2;

  /* calculate the difference between the file and rank for a move */
  const int FILE_MOVE = abs(POSITION[FILE_INDEX] - DESTINATION[FILE_INDEX]);
  const int RANK_MOVE = abs(POSITION[RANK_INDEX] - DESTINATION[RANK_INDEX]);

  /* multiply the absolute values to determine the type of move being made */
  switch(FILE_MOVE * RANK_MOVE)
  {
    case LINEAR_MOVE:

      if(FILE_MOVE == 0)
      {
	return Move::Vertical;
      }
      else
      {
	return Move::Horizontal;
      }

    case KNIGHT_MOVE:
      
      return Move::Horse;

    default:

      if(FILE_MOVE == RANK_MOVE) 
      {	
	return Move::Diagonal;
      }
      else
      {
	return Move::Invalid;
      }
  }
}

/******************************************************************************/

/* evaluate each move in every direction and create a vector of all valid moves
   for a piece */ 

vector<string> ChessPiece::move_grid(short movements[])
{
  /* max of 8 moves, one in every direction */
  const short TOTAL_MOVES = 8;
  const short MID_POINT = TOTAL_MOVES / 2;

  vector<string> move_grid;

  for(short count = 0; count < TOTAL_MOVES; count++)
  {
    add_next_move(movements, move_grid);

    create_next_move(movements, count, MID_POINT);
  }

  /* use move semantics (std::move()) to return the vector created */
  return std::move(move_grid);
}

/******************************************************************************/

short ChessPiece::file_move(string destination) const
{
  return position[FILE_INDEX] - destination[FILE_INDEX];
}

/******************************************************************************/

short ChessPiece::rank_move(string destination) const
{
  return position[RANK_INDEX] - destination[RANK_INDEX];
}

/******************************************************************************/

/* return true if a piece is found at 'position' */

bool ChessPiece::piece_at_position(string position) const
{
  assert(GAME_BOARD);
  return GAME_BOARD->count(position);
}

/******************************************************************************/

/* test if there is a piece on the path of a given move */

bool ChessPiece::piece_on_path(string source, string destination, bool block)
{
  short col_step = 0;
  short row_step = 0;

  /* give 'col_step' and 'row_step' values based on the direction of travel */
  step_values(col_step, row_step, source, destination);

  /* move source to the first square */
  move_piece(col_step, row_step, source);

  for(; source != destination; move_piece(col_step, row_step, source))
  {
    if(test_position(source, block) == true)
    {
      return true;
    }
  }

  return (block == true) ? false : same_player(destination);

}

/******************************************************************************/

/* test if the piece at position is of the same colour */

bool ChessPiece::same_player(string position) const
{
  if(piece_at_position(position) == true)
  {
    return (get_player() == (*GAME_BOARD)[position]->get_player());
  }

  return false;
}

/******************************************************************************/

string ChessPiece::get_position() const
{
  return position;
}

/******************************************************************************/
/*                              PRIVATE FUNCTIONS                             */
/******************************************************************************/

/* determine the direction of travel and set col_step and row_step */

void ChessPiece::step_values(short &col_step, short &row_step,
			     const string POSITION, const string DESTINATION)
{

  col_step = POSITION[FILE_INDEX] - DESTINATION[FILE_INDEX];
  row_step = POSITION[RANK_INDEX] - DESTINATION[RANK_INDEX];

  if(col_step != 0)
  {
    col_step = (col_step < 0) ? 1 : -1;
  }

  if(row_step != 0)
  {
    row_step = (row_step < 0) ? 1 : -1;
  }

}

/******************************************************************************/

/* toggle block to either test if a piece if blocking the path ('false') or
   a move can be made to remove the piece ('true') */

bool ChessPiece::test_position(string position, bool block)
{
  switch(block)
  {
    case false:
      /* test if there is a piece at 'position' */
      return piece_at_position(position);
    
    case true:
      
      /* test if a piece can move to 'position' to block the path */
      return move(position);
  }
}

/******************************************************************************/
bool ChessPiece::remove_threat(const string THREAT)
{
  const string KING = king_position(get_player());

  /* test if a block or move on the threatening piece is possible */
  return (piece_on_path(KING, THREAT, true) || move(THREAT));
}


/******************************************************************************/

void ChessPiece::add_next_move(short movements[], vector<string> &move_grid)
{
    /* create the next move */
    string next_move{
      char(position[FILE_INDEX] + movements[FILE_INDEX]),
      char(position[RANK_INDEX] + movements[RANK_INDEX])
    };

    /* push_back the move if it is within the board boundaries and legal */
    if(parse_input(next_move) == true && move(next_move) == true)
    {
      move_grid.push_back(next_move);
    }
}

/******************************************************************************/

void ChessPiece::create_next_move(short movements[], 
				  const short COUNT, const short MID_POINT)
{
  if((COUNT+1) == MID_POINT)
  {
    movements[RANK_INDEX] = 1;
  }

  /* flip the values from positive to negative to keep changing the moves */
  const short negate = -1;
  movements[FILE_INDEX] *= negate;

  /* swap the values in the array movements to create a new move  */
  swap(movements[FILE_INDEX], movements[RANK_INDEX]);

}
/******************************************************************************/
