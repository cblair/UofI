#include <iostream>

using namespace std;

#include <fstream>
#include <string>
#include <cmath>
#include "hash.h"
#include "functions.h"

int main()
{
  string state = "Texas";
  string capital = "Austin";
  int population = 1000000000;
  cout << "TS: 1\n";
  insert(state, capital, population);
  state = "Idaho";
  capital = "Boise";
  population = 112233;
  insert(state, capital, population);
  state = "Washington";
  capital = "Olympia";
  population = 445566;
  insert(state, capital, population);

  print_hash_table();
}
