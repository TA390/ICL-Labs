#include<iostream>

#include"chessPiece.h"
#include"output.h"


using namespace std;

/******************************************************************************/

void Output::new_game()
{ 
  cout << "A new chess game is started!" << endl; 
}

/******************************************************************************/

void Output::piece_error(string position)
{ 
  cerr << "There is no piece at position " << position << '!' << endl;
}

/******************************************************************************/

void Output::players_turn_error(ChessPiece *piece)
{  
  cerr << "It is not " << player_colour(piece) << "'s turn to move!" 
       << endl;
}

/******************************************************************************/

void Output::successful_move(ChessPiece *piece, string source, string destination)
{ 
  cout << player_colour(piece) <<"'s " << player_piece(piece) <<" moves from "
       << source << " to " << destination << endl;
}

/******************************************************************************/

void Output::unsuccessful_move(ChessPiece *piece, string destination)
{ 
  cerr << player_colour(piece) <<"'s " << player_piece(piece) <<" cannot move"
       << " to " << destination << '!' << endl;
}

/******************************************************************************/

void Output::in_check(Colour king)
{ 
  cout << king <<" is in check" << endl;
} 

/******************************************************************************/

void Output::capture(ChessPiece *piece_1, ChessPiece *piece_2, 
		     std::string source, std::string destination)
{ 
  cout << player_colour(piece_1) <<"'s " << player_piece(piece_1)
       <<" moves from " << source << " to " << destination << " taking " 
       << player_colour(piece_2) <<"'s " << player_piece(piece_2)
       << endl;
}

/******************************************************************************/

void Output::checkmate(Colour player)
{ 
  cout << player << " is in checkmate" << endl;
}

/******************************************************************************/
void Output::allocation_error()
{
  cerr << "ERROR: failed to allocate memory for every game piece, " 
       << "the programme has been terminated" << endl;
}

/******************************************************************************/

void Output::input_error(string source, string destination)
{
  cerr << "ERROR: " << source << " and " << destination << " are invalid inputs" 
       << endl;
}

/******************************************************************************/

void Output::stalemate(Colour player)
{
  cout << "Stalemate: " << player << " is unable to make a move. The game has"
       << " ended in a draw" << endl;
}

/******************************************************************************/
/*                               HELPER FUNCTION                              */
/******************************************************************************/

string Output::player_colour(ChessPiece *piece)
{
  switch(piece->get_player())
  {
    case Colour::White:
      return "White";

    case Colour::Black:
      return "Black";
  }

  return "Unknown";
}

/******************************************************************************/

string Output::player_piece(ChessPiece *piece)
{
  switch(piece->get_name())
  {
    case Piece::Rooks:
      return "Rook";

    case Piece::Knights:
      return "Knight";

    case Piece::Bishops:
      return "Bishop";

    case Piece::Kings:
      return "King";

    case Piece::Queens:
      return "Queen";

    case Piece::Pawns:
      return "Pawn";
  }

  return "Unknown";
}

/******************************************************************************/

ostream &operator<<(ostream &out_stream, Colour player)
{
  out_stream << ((player == Colour::White) ? "White" : "Black");
 
  return out_stream;
}

/******************************************************************************/
