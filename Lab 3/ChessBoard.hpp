#ifndef CHESSBOARD
#define CHESSBOARD


#include<map>
#include"gameInformation.h"

/******************************************************************************/

/* forward declaration */
class ChessPiece;

/******************************************************************************/

/* Alias for a piece on the board*/
typedef std::pair<std::string, ChessPiece *> Board_piece;

/******************************************************************************/

class ChessBoard : public GameInformation{

 public:

  ChessBoard();
  ~ChessBoard();
  
  /* Function to execute a valid move. The parameter 'source' is the 
     location of the piece to be moved. The parameter 'destination' is 
     the positon to move it to */
  void submitMove(const std::string source, const std::string destination);

  /* Function to reset the board and all pieces to their starting positions */
  void resetBoard();

 private:

  /* ATTRIBUTES */

  /* Container for all the pieces in play. The string argument corresponds to
     the board position. The ChessPiece* argument is a pointer to the piece */
  std::map<std::string, ChessPiece *> board;

  /* Attribute to determine whose go it is */
  Colour player_turn;

  /* Attribute to store the last piece removed from the game */
  Board_piece buffered_piece;


  /* INTERNAL HELPER FUNCTIONS */

  /* SUBMIT_MOVE HELPERS */
  /* Function to carry out the move and output the required message. The
     parameters 'SOURCE' and 'DESTINATION' correspond to the position of the
     piece making the move ('SOURCE') and the position to move to 
     ('DESTINATION') */
  void perform_move(const std::string SOURCE, const std::string DESTINATION);

  /* Function to test that the input provided for a move is valid and that
     the piece can make the move. The parameters 'SOURCE' and 'DESTINATION'
     corresponds to the position of the piece making the move ('SOURCE') and
     the position to move to ('DESTINATION'). The function throws an error
     if it fails to validate a move */
  void validate_move(const std::string SOURCE, const std::string DESTINATION);

  /* Function to output the correct message after completing a successful move.
     The parameters 'SOURCE' and 'DESTINATION' correspond to the original
     position of the piece which made the move ('SOURCE') and the position that
     the piece moved to ('DESTINATION') */
  void output_message(const std::string SOURCE, const std::string DESTINATION);

  /* Function to execute a valid move on the board. The parameters 'SOURCE' and 
     'DESTINATION' correspond to the piece making the move ('SOURCE') and
     the destination square to move to ('DESTINATION'). The parameter 'testing'
     is set to false by default, setting this to true tells the function to 
     simulate the move and then return the piece to its original position. The
     function return true if a move was executed and false if it was not */
  bool execute_move(const std::string SOURCE, const std::string DESTINATION, 
		    bool testing = false);

  /* EXECUTE_MOVE HELPERS */
  /* Function to swap two pieces, then store the piece passed in at the position
     'destination' before deleteing it from the board. The parameter 'source'
     correponds to the piece making the move and 'destination' is the piece
     (if any) to be removed from the game */ 
  void buffer_move(std::string source, std::string destination);

  /* Function to restore the piece placed in the buffer. The parameter
     'position' is the position of the piece which caused the buffered
     piece to be removed from the game. The function returns both pieces
     to their previous positions */
  void reinstate_buffered_move(std::string position);

  /* Function to update the state of the game (players turn, king in check)
     after a successful move. The parameter 'piece' corresponds to the piece
     which made the successful move */
  void update_game_information(std::string piece);


  /* RESET_BOARD HELPER FUNCTIONS */
  /* Function to deallocate all memory acquired before exiting the game */
  void deallocate_memory();

  /* Function to initialise the pieces to their positions on the board. The
     parameter 'player' correponds to the colour to be set up */
  void initialise_board(Colour player);


  /* INITIALISE_BOARD HELPER */
  /* Function to create an individual game piece. The parameter 'player' is
     the colour of the piece and 'piece' is the name of the piece. The parameter
     position is the starting position of the piece. The function returns a
     pointer to the piece created */
  ChessPiece *create_piece(Colour player, Piece piece, std::string position);


  /* STALEMATE */
  /* Function to test whether the game is in a stalemate condition, defined
     as a player not in check who is unable to make a move. The function
     terminates the game if a stalemate situation has occurred */
  void stalemate();

  /* CHECKMATE */
  /* Function to test if a checkmate situation has occurred in the game. The
     function throws an error if a player has been checkmated */
  void checkmate();

  /* STALEMATE / CHECKMATE HELPERS */
  /* Function to test if the piece at the position of the parameter 'PIECE'
     has any moves available. The function returns true if any move is
     available and false otherwise */
  bool available_moves(const std::string PIECE);

  /* Function to test if a king is in check. The parameter 'king' is the colour
     of the king to be tested. The function returns the position of the piece
     which has the king in check or an empty string if not in check */
  std::string king_in_check(Colour king);

 /* CHECKMATE HELPERS */
  /* Function to test if a piece can remove a threat on his king. The
     parameter 'PIECE' is the piece to check. The function returns true if
     the threat can be removed and false if it cannot */
  bool threat_removed(const std::pair<std::string, ChessPiece *> PIECE);


  /* CASTLING */
  /* Function to test if a castling move has been made by a king. The
     parameter 'POSITION' is the current position of the king and
     'DESTINATION' is the position to move to. The function returns true if
     a valid castling move was made and executed or false otherwise */
  bool castling(const std::string POSITION, const std::string DESTINATION);


  /* GENERAL HELPERS */
  /* Function to update the position value of a piece. The parameter
     'NEW_POSITION' is the key value (e.g. "A1") of the piece to be updated */
  void update_piece_position(const std::string NEW_POSITION);

  /* Function to test if a piece is located at the position 'position'. The
     function returns true if a piece is found at the position and false
     otherwise */
  bool piece_at_position(std::string position) const;

  /* Function to return the pointer to the piece found at the position
     'POSITION' on the board. A nullptr is returned if no piece is found */
  ChessPiece *piece_ptr(const std::string POSITION);

  /* Function to return the enum correponding to the name of the piece found 
     at the position 'POSITION' on the board */
  Piece piece_name(const std::string POSITION);

  /* Function to return the enum correponding to the colour of the piece found 
     at the position 'POSITION' on the board */
  Colour player(const std::string POSITION);


};

/******************************************************************************/

/******************************************************************************/
/*                                ENUM HELPERS                                */
/******************************************************************************/

File &operator++(File &file);

/******************************************************************************/

Piece &change_piece(Piece &piece, int &increment);

/******************************************************************************/


#endif
