#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

int main(int argc, char *argv[])
{
  int samples = 0;
  double cycles = 1;
  double amplitude = 1;
  double phase = 10;
  string ofname = "";

  //A funky way of getting the samples #
  samples = static_cast<int>(*argv[2]);
  samples -= 48;
  cout << "samples after manip = " << samples << '\n';

  if(argc < 2 || samples <= 0)
    {
      cerr << "usage: lab14-1 -s <#> -c <#> -a <#> -p <#> <outfile.txt>\n"
	   << "-s <# of samples>\n"
	   << "-c <# of cycles>\n"
	   << "-a <# of the amplitude>\n"
	   << "-p <# value of phase shift\n"
	   << "<outfile.txt> (this is the output file to write to\n"
	   << "(-s and <outfile.txt> is the minimum amount of arguments\n"
	   << "allowed to run the program)\n"
	;
      exit(1);
    }
  
  cout << "samples = " << samples << '\n'
       << "cycles = " << cycles << '\n'
       << "amplitude = " << amplitude << '\n'
       << "phase = " << phase << '\n'
       << "output file = " << ofname << '\n'
    ;

  //TS code to see the main args
  cout << "The command line worked!\n"
       << "argc = " << argc << ",...\n";
  for(int i = 0; i <= argc; ++i)
    {
      cout << "\nargv[" << i << "] = " << argv[i] << '\n';
    }
  //

  //creating and printing the list
  double list[samples];
  double w = (2 * 3.14 * cycles) / (samples - 1);
  cout << "w = " << w << '\n';
  for(int i = 0; i < samples; ++i)
    {
      list[i] = amplitude * (sin(w * (i - 1) + phase));
      cout << list[i] << '\n';
    }
}
