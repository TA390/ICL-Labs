#include<iostream>
#include<cstdlib>
#include<cassert>

#include"ChessBoard.hpp"
/* header file to group the header files for each piece */
#include"pieces.h"
#include"output.h"

using namespace std;

/******************************************************************************/

/* Alias */
typedef map<string, ChessPiece *>::iterator Board_iter;

/******************************************************************************/

/* output object used to output messages */
Output message;

/******************************************************************************/

/* error codes assigned to the ASCII number character values */
enum Error{Input='0', Position, Player, Move, Check, Checkmate, Stalemate, 
           Memory};

/******************************************************************************/
/*                              PUBLIC FUNCTIONS                              */
/******************************************************************************/

ChessBoard::ChessBoard()
{
  resetBoard();
}

/******************************************************************************/

ChessBoard::~ChessBoard()
{
  deallocate_memory();
}

/******************************************************************************/

void ChessBoard::submitMove(const string SOURCE, const string DESTINATION)
{

  try
  {
    /* validate the input and move to be made */
    validate_move(SOURCE, DESTINATION);

    /* perform the move */
    perform_move(SOURCE, DESTINATION);

    /* test for checkmate */
    checkmate();
  }
  catch(runtime_error &error)
  {
    /* use the first character to switch between the error messages */
    char error_char = error.what()[FILE_INDEX];

    switch(error_char)
    {
      case Error::Input:
	message.input_error(SOURCE, DESTINATION);
	return;
      case Error::Position:
	message.piece_error(SOURCE);
	return;
      case Error::Player:
	message.players_turn_error(board[SOURCE]);
	return;
      case Error::Move:	
	message.unsuccessful_move(piece_ptr(SOURCE), DESTINATION);
	return;
      case Error::Check:	
	message.in_check(player_turn);
	return;
      case Error::Checkmate:	
	message.checkmate(player_turn);
	return;
      case Error::Stalemate:	
        message.stalemate(player_turn);
	return;
      case Error::Memory:	
        message.allocation_error();
	exit(1);
      default:	
	cerr << "Unknown error!" << endl;
    }
  }
}

/******************************************************************************/

void ChessBoard::resetBoard()
{
  /* delete all pointers held in the board */
  deallocate_memory();
  
  /* clear all objects contained in the board (map) */
  board.clear();

  /* initialise each side of the board */
  initialise_board(Colour::White);
  initialise_board(Colour::Black);

  /* set king check status to not in check */
  set_king_check(Colour::White, NOT_IN_CHECK);
  set_king_check(Colour::Black, NOT_IN_CHECK);

  /* reset first turn */
  player_turn = Colour::White;

  /* output message */
  message.new_game();
}

/******************************************************************************/
/*                             PRIVATE FUNCTIONS                              */
/******************************************************************************/

void ChessBoard::perform_move(const std::string SOURCE,
			      const std::string DESTINATION)
{
  
  /* test if a successful move is made (castling or generic move) */
  if(castling(SOURCE, DESTINATION) || execute_move(SOURCE, DESTINATION))
  {
    output_message(SOURCE, DESTINATION);
    update_game_information(DESTINATION);

    return;
  }
    
  message.unsuccessful_move(piece_ptr(SOURCE), DESTINATION);
  stalemate();

}

/******************************************************************************/

void ChessBoard::validate_move(const string SOURCE, const string DESTINATION)
{

  /* check that the input corresponds to a location on the board */
  if(parse_input(SOURCE) == false || parse_input(DESTINATION) == false)
  {
    throw runtime_error("0");
  }

  /* check that there is a player at the location 'source' */
  if(piece_at_position(SOURCE) == false)
  {
    throw runtime_error("1");
  }

  /* check that the player whose turn it is, is the one making the move */
  if(player(SOURCE) != player_turn)
  {
    throw runtime_error("2");
  }

  /* test if the piece can make the move */
  if(piece_ptr(SOURCE)->move(DESTINATION) == false)
  {
    throw runtime_error("3");
  }

}

/******************************************************************************/

void ChessBoard::output_message(const string SOURCE, const string DESTINATION)
{
  /* test if a piece was removed from the game & output the relevant message */
  if(buffered_piece.second == nullptr)
  {
    message.successful_move(piece_ptr(DESTINATION), SOURCE, DESTINATION);
  }
  else
  {
    message.capture(piece_ptr(DESTINATION), buffered_piece.second, 
		    SOURCE, DESTINATION);
  }
}

/******************************************************************************/

/* execute a move if it is legal, otherwise reverse the move after 
   evaluating the board */

bool ChessBoard::execute_move(const string SOURCE, const string DESTINATION, 
			      bool testing)
{  
  /* buffer the 'DESTINATION' position before removing it from the board */
  buffer_move(SOURCE, DESTINATION);

  /* test if the move places a player's own king in check */
  string king_placed_in_check = king_in_check(player(DESTINATION));

  
  /* reverse any illegal moves or phantom moves for testing */
  if(king_placed_in_check != NOT_IN_CHECK || testing == true)
  {
    reinstate_buffered_move(DESTINATION);
  }
 
  return (king_placed_in_check == NOT_IN_CHECK);
}

/******************************************************************************/

void ChessBoard::buffer_move(string source, string destination)
{

  /* swap the positions on the board */
  swap(board[source], board[destination]);

  /* delete what was last in the buffer before storing the piece previously
     at destination */
  delete buffered_piece.second;
  buffered_piece = {source, board[source]};

  /* update the position of the piece which made the move */
  update_piece_position(destination);

  /* remove the piece previously at the position destination from the board */
  board.erase(source);
}

/******************************************************************************/

void ChessBoard::reinstate_buffered_move(string position)
{
  /* place the buffered piece back on the board */
    board[buffered_piece.first] = buffered_piece.second;

  
  if(board[buffered_piece.first] != nullptr && board[position] != nullptr)
  {
    /* swap the buffered piece and the argument 'position' */
    swap(board[position], board[buffered_piece.first]);
    update_piece_position(buffered_piece.first);
  }
  else
  {
    buffer_move(position, buffered_piece.first);
  }

  /* set the buffered piece pointer to null */
  buffered_piece.second = nullptr;

}

/******************************************************************************/

void ChessBoard::update_game_information(string piece)
{
  /* set first move to false after a successful move */
  piece_ptr(piece)->set_first_move();
  
  /* change the turn of the players */
  player_turn = (player_turn==Colour::White) ? Colour::Black : Colour::White;

  /* check if the last move placed the opponent in check */
  string threat = king_in_check(player_turn);
  /* set the current status as to whether the king is in check */
  set_king_check(player_turn, threat);
}

/******************************************************************************/

void ChessBoard::deallocate_memory()
{

  /* delete the piece held in the buffer */
  delete buffered_piece.second;

  /* delete all memory acquired and held in 'board' */
  Board_iter begin = board.begin();
  Board_iter end = board.end();

  for(; begin != end; begin++)
  {
    delete (begin->second);
  }

}

/******************************************************************************/

void ChessBoard::initialise_board(Colour player)
{
  /* initialise the pieces of the colour passed in as an argument */
  Rank board_rank = (player == Colour::White) ? Rank::FIRST : Rank::LAST;
  const int STEP = (player == Colour::White) ? 1 : -1;

  int increment = 1;

  Piece current_piece = Piece::Rooks;

  /* loop through the back row assigning each piece its position */
  for(File board_file = File::A; board_file <= File::H; ++board_file)
  {
    std::string piece_key = string{board_file, board_rank};
    std::string pawn_key = string{board_file, Rank((board_rank + STEP))};

    board[piece_key] = create_piece(player, current_piece, piece_key);
    board[pawn_key] = create_piece(player, Piece::Pawns, pawn_key);
    
    /* move to the next piece and wrap to the bishop when you pass the queen */
    change_piece(current_piece, increment);
  }
}

/******************************************************************************/

ChessPiece *ChessBoard::create_piece(Colour player, Piece piece, string position)
{
  try
  {
    switch(piece)
    {
      case Piece::Rooks:

	return new Rook{player, position, &board};

      case Piece::Knights:

	return new Knight{player, position, &board};
	
      case Piece::Bishops:

	return new Bishop{player, position, &board};

      case Piece::Kings:

        return new King{player, position, &board};

      case Piece::Queens:
 
	return new Queen{player, position, &board};

      case Piece::Pawns:

	return new Pawn{player, position, &board};
    }
  }
  catch(bad_alloc)
  {
    throw runtime_error("7");
  }
   
  return nullptr;
}


/******************************************************************************/

/* test if the current player can make any legal moves */

void ChessBoard::stalemate()
{
  if(king_check_status(player_turn) == true)
  {
    return;
  }

  /* auto binds to the pair - std::pair<std::string, ChessPiece*> */
  for(const auto &PIECE : board)
  {
    /* check all pieces of the player whose turn it is */
    if(player_turn == player(PIECE.first))
    {
      /* if any legal move is found return */
      if(available_moves(PIECE.first) == true)
      {
	return;
      }
    } 
  }

  /* end the game if no move was found */
  throw runtime_error("6");

}

/******************************************************************************/

void ChessBoard::checkmate()
{
  if(king_check_status(player_turn) == false)
  {
    return;
  }

  /* test if the king can move out of threat */
  if(available_moves(king_position(player_turn)) == true)
  {
    throw runtime_error("4");
  }

  for(const auto PIECE : board)
  {
    /* test if a piece can block or capture the threatening piece */    
    if(threat_removed(PIECE) == true)
    {
      throw runtime_error("4");
    }
  }

  /* throw an error if the king has been checkmated */
  throw runtime_error("5");
}


/******************************************************************************/

bool ChessBoard::available_moves(const string CURRENT_POSITION)
{

  /* cycle through a vector containing a given players immediate moves */
  for(const string MOVE : piece_ptr(CURRENT_POSITION)->move_list())
  { 
    /* return true if a move is found that does not place its king in check */
    if(execute_move(CURRENT_POSITION, MOVE, true) == true)
    {
      return true;
    }
  }

  return false;
}


/******************************************************************************/

/* test if an opponents piece can hit the 'king' of the given colour */

string ChessBoard::king_in_check(Colour king)
{

  /* auto binds to the pair - std::pair<std::string, ChessPiece*> */
  for(const auto &PIECE : board)
  {
    /* if an opposing piece can hit the king, return its position */
    if(piece_ptr(PIECE.first)->move(king_position(king)))
    {
      return PIECE.first;
    }
  }

  return NOT_IN_CHECK;
}

/******************************************************************************/

bool ChessBoard::threat_removed(const pair<string, ChessPiece *> PIECE)
{
  string position = PIECE.first;

  /* test if a piece can make a block or capture the threat */
  if(player_turn == player(position) && (PIECE.second)->capture_or_block())
  {
    /* if the king can capture the threat himself, test the move to check it
       is valid */
    if(piece_name(position) == Piece::Kings)
    {
      return execute_move(position, threatening_piece(player_turn), true);
    }

    return true;
  }

  return false;
}  

/******************************************************************************/

bool ChessBoard::castling(const string POSITION, const string DESTINATION)
{
  string rook_to_castle = get_king_castling();

  if(rook_to_castle == NO_CASTLING)
  {
    return false;
  }
  
  short direction = get_castling_direction();
  string move = POSITION;

  move_piece(direction, 0, move);
  string rook_destination = move;

  /* test that each position in which the king passes and ends at does not put
     him in check */
  for(; move <= DESTINATION; move_piece(direction, 0, move))
  {
    if(execute_move(POSITION, move, true) == false)
    {
      return false;
    }
  }

  /* update the positions of the rook and king */
  buffer_move(rook_to_castle, rook_destination);
  buffer_move(POSITION, DESTINATION);
  GameInformation::set_king_castling(NO_CASTLING);

  return true;
}

/******************************************************************************/

void ChessBoard::update_piece_position(const string NEW_POSITION)
{
  piece_ptr(NEW_POSITION)->set_position(NEW_POSITION);
}

/******************************************************************************/

bool ChessBoard::piece_at_position(string position) const
{
  return board.count(position);
}

/******************************************************************************/

/* used to assert the pointer before being used */

ChessPiece *ChessBoard::piece_ptr(const string POSITION)
{
  assert(board[POSITION]);

  return board[POSITION];
}

/******************************************************************************/

Piece ChessBoard::piece_name(const string POSITION)
{
  return piece_ptr(POSITION)->get_name();
}

/******************************************************************************/

Colour ChessBoard::player(const string POSITION)
{
  return piece_ptr(POSITION)->get_player();
}

/******************************************************************************/
/*                           ENUM HELPER DEFINITIONS                          */
/******************************************************************************/

File &operator++(File &file)
{
  file = File(file + 1);
  return file;
}

/******************************************************************************/

Piece &change_piece(Piece &piece, int &increment)
{
  piece = Piece(piece + increment);

  if(piece == Piece::Pawns)
  {
    increment = -increment;
    piece = Piece::Bishops;
  }

  return piece;

}

/******************************************************************************/

