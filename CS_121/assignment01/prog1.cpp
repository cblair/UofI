/* prog1.cpp
 *
 * CS 121.Bolden.........GNU Version 2...........Colby Blair
 * 01/13/08 ...........Dell Inspiron 1521........cblair@vandals.uidaho.edu
 *
 * This program take an input file with a)amount of rows and columns and
 * b) the values for them and stores them into an array. We then output the 
 * values from the array to an output file, in which 1 row gets 1 line.
 * Beginning Implementation Time: 21:50 01/13/08
 * Ending Implementation Time: 22:50 01/13/08
 * Work Time: 1 hour
 *        Notes: 1 hour work time. Everything is working smoothly, except 
 *        cannot input hex numbers; fin hangs on the first 'x'.
 * Beginning Implementation Time: 21:00 01/15/08
 * Ending Implementation Time: 23:59 01/15/08 
 * Work Time: 2 hours 59 minutes
 *        Notes: There was a lot of wasted time with messing around with
 *               bitwise operations, but something I wanted to mess with. There
 *               were also some hangup with formating, and if I had more time, 
 *               I would go back and understand bitwise operations instead of
 *               the current proceedure.
 * Total Work Time: 3 hours 59 minutes
 *---------------------------------------------------------------------
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string hex_to_bin(int hex_value);
bool is_odd(int val);

int main()
{
    ifstream fin;
    ofstream fout;
    
    //Prompt for file names; the input and output names are stored as 
    //Filename String contexts, or FS, so that later we can change them to
    //FCA's, or File Char Arrays for the 'open()' function
    cout << "Enter the name of the input file: ";
    string inputFS;
    cin >> inputFS;
    cout << "Enter the name of the output file: ";
    string outputFS;
    cin >> outputFS;
    
    //open input and output files and check for errors; we hand the Filename 
    //String vals to File Char Arrays for the 'open()' functions used.
    const char *inputFCA;
    const char *outputFCA;
    inputFCA = inputFS.c_str();
    outputFCA = outputFS.c_str();
    fin.open(inputFCA);
    fout.open(outputFCA);
    if(fin.fail())
    {
     cerr << "ERROR: file '" << inputFS << "' not found. Exiting...\n";
     exit(1);              
    }
    if(fout.fail())
    {
     cerr << "ERROR: file '" << outputFS << "' not found. Exiting...\n";
     exit(1);               
    }
    
    //input the total numbers of rows and columns for the input file
    int total_columns;
    int total_rows;
    fin >> total_rows >> total_columns;
    cout << total_rows << " rows, " << total_columns 
         << " columns in the file.\n"
         ;
    
    //input hex information from input file, convert to binary with the 
    //'hex_to_bin()' function, and output to the output file. I was troubled
    //a bit on making the function's return type a 'string', but it can be
    //fout'ed straight out, so this is slightly easing. We translate each hex
    //'cell' to a binary 'cell', and then print a newline for the end of 
    //every row.
    int itemp; //for recieving hex value
    for(int current_row = 0; 
            current_row < total_rows; 
            ++current_row)
    {               
                    for(int current_column = 0; 
                    current_column < total_columns; 
                    ++current_column)
                    {
                     fin >> hex >> itemp;
                     fout << hex_to_bin(itemp);
                    }
                    fout << '\n';
    }
    
    fin.close();
    fout.close();
    cout << "Opereations successful. Exiting...\n";
}

//The fastest, non-bitwise way to do this so far in my development has been
//to return a string (an int array would work too, but would take a bit more
//main() manipulation. We start with a 'byte size' string. The for loop
//starts at the LSB, and we start a dec, in fact, conversion to bin, since 
//main()'s fin >> hex >> <int> has already converted hex to dec for us. We
//see if there is a remainder from a division by 2 with function is_odd(),
//divide hex_val by 2, and repeat until all value to the original hex
//value is gone.
string hex_to_bin(int hex_val)
{
 string bin_string = "0000";
 for(int i = 3; hex_val > 0; --i)
 {
  if(is_odd(hex_val)) bin_string[i] = '1'; 
  hex_val /= 2;
 }

 return(bin_string);
}

bool is_odd(int itemp)
{
 return(itemp %= 2);
}
