//40 min.
//30 min.
//20 min.
//140 min.
//

#include <iostream>
#include <iomanip>

using namespace std;

class C4Board
{
private:
  static const int COL_SIZE = 7;
  static const int ROW_SIZE = 6;
  char board[COL_SIZE][ROW_SIZE];
  char whose_turn;
  char winner;
public:
  C4Board();
  void print(ostream &out = cout);
  char is_win();
  int drop(int col);
};

C4Board::C4Board()
       : whose_turn('X')
{
    for(int x = 0; x < COL_SIZE; ++x)
    {
      for(int y = 0; y < ROW_SIZE; ++y) board[x][y] = '.';
    }
}

inline
ostream& operator<<(ostream &out, C4Board &b)
{
  b.print(out);
  return(out);
}

int main()
{
  C4Board connect4;

  cout << "   ccc            44\n"
       << "  cc             4 4\n"
       << " cc    onnect   4444   Game\n"
       << "  cc              44\n"
       << "   ccc            44\n";
  connect4.print();

  int temp = 0;
  do
    {
      cout << "Please enter the column to drop into: ";
      cin >> temp;
      temp = connect4.drop(temp);
      connect4.print();
      if(temp == 1) cout << "(Drop was ok)\n";
      else if(temp == 2) cout << "Illegal Column Entry!!!\n";
      else if(temp == 3) cout << "The Column is Full!!!"
			      << "Player, please try again!";
      else cerr << "ERROR: drop() did not return an acceptable value\n";
    }
  while(connect4.is_win() == false);
} 

void C4Board::print(ostream &out)
{
  out << '\n';
  for(int i = 1; i <= COL_SIZE; ++i) out << setw(3)
					  << i;
  out << '\n';
  for(int y = 0; y >= 0 && y < ROW_SIZE; ++y)
    {
      for(int x = 0; x >= 0 && x < COL_SIZE; ++x) 
	out << setw(3) << board[x][y];
      out << '\n';
    }

  out << "Current player is ";
  out << whose_turn << '\n';
}

int C4Board::drop(int col)
{
  int DROP_OK = 0, ILLEGAL_COL = 0, COL_FULL = 0;
 
  if(col < 0 || col > COL_SIZE) return ( (ILLEGAL_COL = 2) );
  col -= 1;
  int i = ROW_SIZE - 1;
  while(i >= 0)
   {
     if(board[col][i] == '.') 
       {
	 //TODO: the if statements probably need to be gotten rid of.
	 if(whose_turn == 'X') 
	 {
	   board[col][i] = whose_turn;
	   whose_turn = 'O';
	   return ( (DROP_OK = 1) );
	 }
	 else if(whose_turn == 'O')
	 {
	   board[col][i] = whose_turn;
           whose_turn = 'X';
           return ( (DROP_OK = 1) );
         }
       }
     --i;
   }

 return( (COL_FULL = 3) );
}

char C4Board::is_win()
{
  //check across
  for(int y = ROW_SIZE - 1; y >= 0; --y)
    {
      for(int x = 0; x < COL_SIZE - 4; ++x)
      {
	if(board[x][y] != '.' && board[x][y] == board[x+1][y] 
	   && board[x][y] == board[x+2][y] && board[x][y] == board[x+3][y])
	return( (winner = board[x][y]) );
      }
    }
  //check down
  for(int y = ROW_SIZE - 1; y >= 3; --y)
    {
      for(int x = 0; x < COL_SIZE; ++x)
	{
	  if(board[x][y] != '.' && board[x][y] == board[x][y-1]
	     && board[x][y] == board[x][y-2] && board[x][y] == board[x][y-3])
	    return (true);
	}
    }
  //check the diagonal, negative slop
  for(int y = 0; y <= ROW_SIZE - 4; ++y)
    {
      for(int x = 0; x <= COL_SIZE - 4; ++x)
	{
	  if(board[x][y] != '.' && board[x][y] == board[x+1][y+1]
	     && board[x][y] == board[x+2][y+2] && board[x][y] == board[x+3][y+3])
	    return(true);
	}
    }
  //check the diagonal, positive slope
  for(int y = ROW_SIZE - 1; y >= 3; --y)
    {
      for(int x = 0; x <= COL_SIZE - 4; ++x)
	{
	  if(board[x][y] != '.' && board[x][y] == board[x+1][y-1] && board[x][y] == board[x+2][y-2]
			  && board[x][y] == board[x+3][y-3])
	  return(true);
	}
    }
  return (false);
}
