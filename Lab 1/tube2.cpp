#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <limits>

using namespace std;

#include "tube.h"

/******************************/
/********** CONSTANTS *********/
/******************************/

/* Array files listed in order for indexing specific files.
   The final variable will be used to denote the arrays size */
enum {STATIONS_FILE, LINES_FILE, NUMBER_OF_FILES};
/* Array containing all files needed to search */
const char* DATA_FILES[NUMBER_OF_FILES] = {"stations.txt", "lines.txt"};

/* MAX line length when searching a file line by line */
const int MAX_LINE_LENGTH = 128;
/* Character symbol for stations not found */
const char NO_STATION = ' ';

/******************************/
/****** END OF CONSTANTS ******/
/******************************/


/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }
  
  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}


/******************************/
/********* QUESTION 1 *********/
/******************************/

/* function definition for get_symbol_position */
/* Locate 'target' in 'map'. If found update 'row' and 'col' to the index position of target within 
   the 2D array map. If not found set 'row' and 'col' to -1 */
bool get_symbol_position(char **map, const int height, const int width, const char target, int& row, int& col)
{
  for(row=0; row<height; row++)
    for(col=0; col<width; col++)
      if(map[row][col] == target)
	return true;

  row = col = -1; 
  return false;
}

/******************************/
/***** END OF QUESTION 1 ******/
/******************************/



/******************************/
/********* QUESTION 2 *********/
/******************************/

/* HELPER FUNCTION */
/* function definition for open_file */
/* Attempt to open the file 'file_name' using the ifstream 'in_stream'
   The function terminates the program and displays an error the opration failed */
void open_file(ifstream& in_stream, const char* file_name)
{
  in_stream.open(file_name);
  if(!in_stream)
  {
    cerr << "*The file " << file_name << " could not be opened* - the program has been terminated\n"; 
    exit(1);
  }
}

/* HELPER FUNCTION */
/* function definition for search_file_for_station */
/* Search a file for 'name' in the DATA_FILES array at index position 'array_index' */
char search_file_for_name(const char* name, int array_index)
{
  char line[MAX_LINE_LENGTH];
  ifstream in_stream;
  
  open_file(in_stream, DATA_FILES[array_index]);
  for(char symbol=0; in_stream >> symbol; ) // read a character and check stream state
  {    
    in_stream >> ws; // remove whitespace
    in_stream.getline(line, MAX_LINE_LENGTH);
    
    if(!strcmp(name, line))
      return symbol;
  }
  in_stream.close();

  return false;
}

/* function definition for get_symbol_for_station_or_line */
/* Take a const char* as argument and return its character symbol found in files "station.txt"
   or "lines.txt" (all files listed in the array of files). 
   Return the character constant NO_STATION if 'name' was not found */
char get_symbol_for_station_or_line(const char* name)
{ 
  for(int i=0; i<NUMBER_OF_FILES; i++) // loop all files in the array
    if(char symbol = search_file_for_name(name, i)) //search the ith file in the array
      return symbol;
  
  return NO_STATION;
}

/******************************/
/***** END OF QUESTION 2 ******/
/******************************/



/******************************/
/********* QUESTION 3 *********/
/******************************/

/* HELPER FUNCTION */
/* COMMENT */
int is_route_out_of_bounds(const int row_height, const int column_height, const int current_row, const int current_col)
{
  if(current_row < 0 || current_row >= row_height || current_col < 0 || current_col >= column_height)
    return ERROR_OUT_OF_BOUNDS;

  return false;
}


/* HELPER FUNCTION */
/* COMMENT - UPDATED */
int is_position_off_track(const char position_on_map)
{
  if(isspace(position_on_map))
    return ERROR_OFF_TRACK;

  return false;
}

/* HELPER FUNCTION */
/* COMMENT */
int is_line_change_valid(const char previous_position, const char current_position, char& train_line, int& line_changes)
{  
  if(!isalnum(current_position))
  {
     if(!isalnum(previous_position)  && (previous_position != current_position))
       return ERROR_LINE_HOPPING_BETWEEN_STATIONS;

     if(train_line != 0 && train_line != current_position)
       line_changes++;

     train_line = current_position;
  }
  return false;
}

/* HELPER FUNCTION */
/* COMMENT */
int is_move_backtracking(const char* previous_direction, const char* current_direction)
{
  if(*previous_direction)
  {
    const int number_of_directions = 8;
    /* using a clockwise ordering of directions to find the opposite direction */
    const int opposite_direction = number_of_directions / 2;
    Direction prev_direction = string_to_direction(previous_direction);
    Direction curr_direction = string_to_direction(current_direction);

    /* using modulo to wrap around the last direction and set backtrack_direction */
    int backtrack_direction = (curr_direction+opposite_direction) % number_of_directions;

    if(prev_direction == backtrack_direction)
      return ERROR_BACKTRACKING_BETWEEN_STATIONS;
  }
  return false;
}


/* HELPER FUNCTION */
/* COMMENT */
int travel_on_map(int& row_position, int& col_position, const char* direction)
{ 
    Direction move_on_map = string_to_direction(direction);

    switch(move_on_map)
    {
    case N:
      row_position--;
      break;
    case NE:
      row_position--;
      col_position++; 
      break;
    case E:
      col_position++; 
      break;
    case SE:
      row_position++;
      col_position++; 
      break;
    case S:
      row_position++;
      break;
    case SW:
      row_position++;
      col_position--;
      break;
    case W:
      col_position--;
      break;
    case NW:
      row_position--;
      col_position--;
      break;
    default:
      return ERROR_INVALID_DIRECTION;
    }

    return false;
}

/* HELPER FUNCTION */
/* function definition for validate_start_station */
int validate_start_station(char **map, const int height, const int width, const char* start_station, int& row, int& col)
{
  char station_symbol = search_file_for_name(start_station, STATIONS_FILE);
  if(station_symbol == NO_STATION)
    return ERROR_START_STATION_INVALID;
  
  //set row and col values
  get_symbol_position(map, height, width, station_symbol, row, col);  

  return false;
}


/* HELPER FUNCTION */
/* OVERLOADED */
char search_file_for_name(char* destination, const char search_symbol, int array_index)
{
  ifstream in_stream;
  
  open_file(in_stream, DATA_FILES[array_index]);
  for(char station_symbol=0; in_stream >> station_symbol; ) // read a character and check stream state
  {    
    if(station_symbol == search_symbol)
    {
      in_stream >> ws; // remove whitespace
      in_stream.getline(destination, MAX_LINE_LENGTH);
      
      return false;
    }
    else
      in_stream.ignore(numeric_limits<streamsize>::max(), '\n');
  }
  in_stream.close();

  return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
}


/* COMMENT */
int validate_route(char **map, const int height, const int width, const char* start_station, const char* route, char* destination)
{

  /* track current position on the map */
  int row_position = 0;
  int col_position = 0;
 
  string current_direction;
  const char str_delimiter = ',';
  /* istringstream to read route from delimited by 'str_delimiter' */
  istringstream in_str_stream{route}; 

  int line_changes = 0; 
  char train_line = 0;
 
  int error_code;
 
  /* validate start station and set row_position and col_position */
  if((error_code=validate_start_station(map, height, width, start_station, row_position, col_position)))
    return error_code;
  
  for(string previous_direction; getline(in_str_stream, current_direction, str_delimiter);
      previous_direction = current_direction)
  {
    char previous_position_on_track = map[row_position][col_position];
    if((error_code=travel_on_map(row_position, col_position, current_direction.c_str()));
    else if((error_code=is_route_out_of_bounds(height, width, row_position, col_position)));
    char current_position_on_track = map[row_position][col_position];

    if(error_code)
      return error_code;
        
    if((error_code=is_position_off_track(current_position_on_track)));
    else if((error_code=is_line_change_valid(previous_position_on_track, current_position_on_track, train_line, line_changes)));
    else if((error_code=is_move_backtracking(previous_direction.c_str(), current_direction.c_str())));
  
    if(error_code)
      return error_code;
  }

    if((error_code=search_file_for_name(destination, map[row_position][col_position], STATIONS_FILE)))
      return error_code;

  return line_changes;
}

/******************************/
/***** END OF QUESTION 3 ******/
/******************************/
