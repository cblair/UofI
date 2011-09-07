#include <stdio.h>

int rand_seed = 0;
int fibfn2 = 3;
int fibfn1 = 5;

int fib()
{
  int ret_val = fibfn1 + fibfn2;
  fibfn2 = fibfn1;
  fibfn1 = ret_val;
  return(ret_val);
}

int rand_num()
{
  return(fib());
}

void print_board(int x, int y, int z)
{
  int row;
  int column;
  for(row = 9; row > 0; row--)
    {
      if(x < row) printf("\t | ");
      else printf("\t(|)");
      
      if(y < row) printf("\t | ");
      else printf("\t(|)");

      if(z < row) printf("\t | \n");
      else printf("\t(|)\n");
    }
 
  for(column = 0; column <= 36; column++)
    printf("=");
  printf("\n");
  printf("\t X \t Y \t Z\n");
}

void print_your_turn()
{

}

char computer_select_column(int x, int y, int z)
{
  int rand_column_num = rand_num();
  //if only one column remains, take it
  if(x <= 0 && y <= 0 && z > 0)
      return('z');
  if(x <= 0 && y > 0 && z <= 0)
      return('y');
  if(x > 0 && y <= 0 && z <= 0)
    return('x'); 

  //three columns left
  if(x > 0 && y > 0 && z > 0)
  {
    switch( (rand_column_num % 3)  )
      {
        case 0:
          return('x');
	case 1:
            return('y');
	case 2:
          return('z');
      }
  }

//two columns left 
  if(x > 0 && y > 0)
  {
    switch( (rand_column_num % 2) )
      {
        case 0:
          return('x');
	case 1:
            return('y');
      }
  }
  if(y > 0 && z > 0)
  {
    switch( (rand_column_num % 2) )
      {
        case 0:
          return('y');
	case 1:
            return('z');
      }
  }
  if(x > 0 && z > 0)
  {
    switch( (rand_column_num % 2) )
      {
        case 0:
          return('x');
	case 1:
            return('z');
      } 
  }

  //10 for error if it gets this far, but there is no logical
  //way to get this far. But the compiler liker a return here.
  return(10);
}

int computer_num_of_discs(int x, int y, int z, char column)
{
  //if only one column is left, take it... and win!
  if(x <= 0 && y <= 0 && z > 0)
    return(9);
  if(x <= 0 && y > 0 && z <= 0)
    return(9);
  if(x > 0 && y <= 0 && z <= 0)
  { return(9); }

  else return(fib() % 10);

}

int check_for_winner(x, y, z)
{
  if(x <= 0 && y <= 0 && z <= 0)
    return(1);
  else return(0);
}

int is_empty(int x, int y, int z, char column)
{
  switch(column)
    {
    case 'x':
      if(x <= 0) return(1);
      else return(0);
      break;
    case 'y':
      if(y <= 0) return(1);
      else return(0);
      break;
    case 'z':
      if(z <= 0) return(1);
      else return(0);
      break;
    }
}

int main()
{
  int x = rand_num() % 10;
  int y = rand_num() % 10;
  int z = rand_num() % 10;
  char column;
  int num_of_discs = 0;
  char winner = 'n';
  char newline = '\n';
  char play_again = 'y';

  while(play_again == 'y')
    {
      x = rand_num() % 10;
      y = rand_num() % 10;
      z = rand_num() % 10;
      num_of_discs = 0;
      winner = 'n';

      print_board(x, y, z);
      
      while(winner == 'n')
	{
	  //Player 1's turn
	  do 
	    {
	      //get input
	      printf("Your Turn. Enter column, number: ");
	      scanf("%c, %d%c", &column, &num_of_discs, &newline);
	      printf("\n");
	      //filter input
	      if(column != 'x' && column != 'y' && column != 'z') 
		printf("Not a valid column! Please try again.\n");
	      if(num_of_discs < 1)
		printf("Not enough discs! Please try again.\n");
	      if(num_of_discs > 9)
		printf("Too many discs! Please try again.\n");
	      if( is_empty(x, y, z, column) )
		printf("Column %c is empty. Please try again.\n", column);
	    } 
	  while( (column != 'x' && column != 'y' && column != 'z' ) 
		 || num_of_discs < 1 || num_of_discs > 9 
		 || is_empty(x, y, z, column) );
	  
	  //take discs
	  switch(column)
	    {
	    case 'x':
	      x = x - num_of_discs;
	      break;
	    case 'y':
	      y = y - num_of_discs;
	      break;
	    case 'z':
	      z = z - num_of_discs;
	      break;
	    }
	  
	  if( check_for_winner(x, y, z) )
	    {
	      winner = '1';
	      printf("Player 1 is the winner!\n");
	      break;
	    }
	  
	  
	  print_board(x, y, z);
	  
	  
	  //Computer's turn
	  do
	    {
	      column = computer_select_column(x, y, z);
	    }
	  while(is_empty(x, y, z, column));
	  num_of_discs = computer_num_of_discs(x, y, z, column);
	  //take discs
	  switch(column)
	    {
	    case 'x':
	      x = x - num_of_discs;
	      break;
	    case 'y':
	      y = y - num_of_discs;
	      break;
	    case 'z':
	      z = z - num_of_discs;
	      break;
	    } 
	  printf("Computer's takes %d from %c\n", num_of_discs, column);
	  
	  if( check_for_winner(x, y, z) )
	    {
	      winner = 'c';
	      printf("Computer is the winner!\n");
	      break;
	    }

	  print_board(x, y, z);
	}
      
      print_board(x, y, z);

      printf("Would you like to play again?(y or n): ");
      scanf("%c", &play_again);
    }

  return(0);
}
