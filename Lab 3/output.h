#ifndef OUTPUT
#define OUTPUT


/******************************************************************************/

class ChessPiece;

/******************************************************************************/

/* Output is a class dedicated to displaying all messages during the game */

class Output{

 public:

  /* output new game message */
  void new_game();

  /* display message when there is no piece found at 'position' */
  void piece_error(std::string position);

  /* display message when it is not the turn of the 'piece' making a move */
  void players_turn_error(ChessPiece *piece);

  /* display message when a successful move is made by 'piece' from 
     'source' to 'destination' */
  void successful_move(ChessPiece *piece, 
		       std::string source, std::string destination);

  /* display message when an usuccessful move is made by 'piece' from 
     'source' to 'destination' */
  void unsuccessful_move(ChessPiece *piece, std::string destination);

  /* display message when a king is in check. The parameter 'king' is the
     colour of the king in check */
  void in_check(Colour king);

  /* display message when a successful move is made by 'piece_1' to capture
     'piece_2'. The parameters 'source' and 'destination' correpspond to the
     move*/
  void capture(ChessPiece *piece_1, ChessPiece *piece_2, 
	       std::string source, std::string destination);

  /* display message when a king is in checkmate. The parameter player is the
     colour of the player in checkmate */
  void checkmate(Colour player);

  /* display message when a game is in stalemate. The parameter player is the
     colour of the player which cannot make a move */
  void stalemate(Colour player);

  /* display an error message when failing to allocate memory for a piece */
  void allocation_error();

  /* diplay message when invalid input is entered */
  void input_error(std::string source, std::string destination);


 private:

  /* HELPER FUNCTIONS */
  /* Function to retrieve the colour of a piece and output it as a string */
  std::string player_colour(ChessPiece *piece);

  /* Function to retrieve the name of a piece and output it as a string */
  std::string player_piece(ChessPiece *piece);

};

/******************************************************************************/

/* HELPER FUNCTION */
/* Operator overload - output colour as a string */

std::ostream &operator<<(std::ostream &out_stream, Colour player);

/******************************************************************************/


#endif
