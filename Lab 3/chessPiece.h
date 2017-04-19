#ifndef CHESSPIECE
#define CHESSPIECE


#include<map>
#include<vector>
#include"gameInformation.h"

/******************************************************************************/

/* Forward delare 'ChessPiece' to create the typedef 'Board' */
class ChessPiece;

/* Alias for the container used for the game board */
typedef std::map<std::string, ChessPiece *> Board;

/******************************************************************************/

/* ChessPiece is an abstract class containing all common attributes and 
   methods for a chess piece */

class ChessPiece: public GameInformation{

 public:

  /* CONSTRUCTOR / DESTRUCTOR */

  /* The constructor initialises all private and protected attributes. The
     parameter '_name' is an enum value which corresponds to the piece
     (e.g. Piece::Pawns). The parameter '_player' is the enum value which 
     corresponds to the colour of the piece. The parameter '_position' is the
     starting position of a piece and '_game_board' is a pointer to board */
  ChessPiece(Piece _name, Colour _player, 
	     std::string _position, Board *_game_board);

  /* virtual destructor */
  virtual ~ChessPiece();


  /* PURE VIRTUAL FUNCTIONS */

  /* Pure virtual function to test whether a move is possible for a particular
     piece. The parameter 'destination' is the destination square on the board
     to move to. The function returns true if the move is possible and false 
     if not */
  virtual bool move(std::string destination) = 0;


  /* FUNCTIONS */

  /* Function to test if an attack on a player's king can be prevented. The
     function returns true if an attack can be stopped and false otherwise */
  bool capture_or_block();

  /* Function to return a vector containing all the immediate moves in each
     direction that a are valid to a particular piece */
  virtual std::vector<std::string> move_list();


  /* GET AND SET FUNCTION */

  /* Function to return the 'NAME' attribute of a chess piece */
  Piece get_name() const;

  /* Function to return the 'PLAYER' attribute of a chess piece */
  Colour get_player() const;

 /* Function to return the 'first_move' attribute of a chess piece */
  bool get_first_move() const;

  /* Function to set the position of a piece. The parameter 'POSITION' is
     the value to be assigned to the attribute 'position' */
  virtual void set_position(const std::string POSITION);

  /* Function to set the 'first_move' attribute to false, following a pieces
     first successful move */
  void set_first_move();



 protected:

  /* ATTRIBUTES */

  /* Pointer to the game board */
  Board *const GAME_BOARD;


  /* FUNCTIONS */

  /* Function to generate the type of move being executed. The parameters
     'POSITION' and 'DESTINATION' are the start position and final destination
     of the move to be made. The function returns an enum 'Move' which maps to
     one of the possible moves which can be made on the board. The function
     returns Move::Invalid for an invalid move */
  Move move_type(const std::string POSITION, 
		 const std::string DESTINATION) const;

  /* Function to return a vector of all valid moves one step away in any
     direction. The parameter movements is an array of two values. The two 
     values can be manipulated to generate all possible moves (one in each 
     direction) for any piece (e.g. for a knight the values are 1 and 2) */
  std::vector<std::string> move_grid(short movements[]);

  /* Function to return the different between the current file position of a
     piece and the 'destination'. The parameter 'destination' is the
     destination square to move to on the board. The function returns the
     diffference between a pieces current file position and the 'destination'.
     Negative values map to E movements and positive to W */
  short file_move(std::string destination) const;

  /* Function to return the different between the current rank position of a
     piece and the 'destination'. The parameter 'destination' is the
     destination square to move to on the board. The function returns the
     diffference between a pieces current file position and the 'destination'.
     Negative values map to N movements and positive to S */
  short rank_move(std::string destination) const;

  /* Function to test if a game piece is located at the parameter 'position' on
     the board. The function returns true if a piece is found at 'position' and
     false otherwise */
  bool piece_at_position(std::string position) const;

  /* Function to test if another game piece is located between two points. The
     parameter 'source' is the starting square and 'destination' the end square.
     The function returns true is a piece is found between these two points and
     false otherwise. The parameter block is a flag to change the behaviour of
     the function. If set true the function will return true if a piece can
     place itself on the path between 'source' and 'destination' and false
     otherwise */
  bool piece_on_path(std::string source, std::string destination, 
		     bool block = false);

   /* Function to test if the piece at 'position' is of the same colour. The
      parameter 'position' is the location on the board to check. The function
      returns true if they are and false otherwise */
  bool same_player(std::string position) const;


  /* GET FUNCTIONS */

  /* Function to return the 'position' attribute of a chess piece */
  std::string get_position() const;



 private:

  /* ATTRIBUTES */

  /* The enum value correponding to the name of a piece */
  const Piece NAME;

  /* The enum value correponding to the colour of a piece */
  const Colour PLAYER;

  /* Current position of the piece */
  std::string position;

  /* Boolean to indicate if it's the first move (set false after first move) */
  bool first_move;


  /* INTERNAL HELPER FUNCTION */

  /* PURE VIRTUAL FUNCTIONS */
  /* Pure virtual function to test whether the type of move is possible for
     a given piece (e.g. diagonal for Bishop would return true). The parameter
     'destination' is the destination square on the board to move to. The
     function returns true if the type of move is valid and false otherwise */
  virtual bool is_move_valid(std::string destination) = 0;

  /* PIECE_ON_PATH HELPERS() */  
  /* Function to calculate the direction of travel and set the parameters
     'col_step' and 'row_step'. Their possible values are 1 or -1 to reresent
     their direction of travel. 1 maps to all SE directions and -1 to NW. The
     parameters 'POSITION' and 'DIRECTION' are the start and end positions
     on the board */
  void step_values(short &col_step, short &row_step,
		   const std::string POSITION, const std::string DESTINATION);

  /* Function to test if a piece is located at 'position' or a piece can move
     itself to 'position'. The parameter block toggles the behaviour. True will 
     test if a piece can place itself at the 'position' passed as an argument.
     False wil test if a piece is currently at 'position'. The function returns
     true if a piece is found at 'position' or the piece itself can move to
     'position'. The function returns false otherwise */
  bool test_position(std::string position, bool block);

  /* CAPTURE_OR_BLOCK() HELPERS */
    /* Function to test if a piece can remove a threat which has placed its
     king in check. The parameter 'THREAT' is the location of the threatening
     piece on the board. The function returns true if an attack on the king can
     be preveneted and false otherwise */
  virtual bool remove_threat(const std::string THREAT);

  /* MOVE_GRID HELPERS() */  
  /* Function to add a valid move to the parameter 'move_grid' which is
     a vector containing all valid moves. The parameter 'movements' contains
     the values used to generate each move (e.g. knight values are 1 and 2) */
  void add_next_move(short movements[], std::vector<std::string> &move_grid);

  /* Function to change the values contained in the array parameter
     'movements'. The values are changed to generate a new move in each
     direction. The parameter 'COUNT' is the current move being generated in a
     range of [0:8]. The parameter 'MID_POINT' is the middle value of the range */
  virtual void create_next_move(short movements[], 
				const short COUNT, const short MID_POINT);

};

/******************************************************************************/


#endif
