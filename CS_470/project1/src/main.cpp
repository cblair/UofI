#include <iostream>
#include "map.h"

using namespace std;

int main(int argc, char *argv[]) {
  cout << "Map Search!\n";
  map *m1 = new map("data/map01.txt");
  if(argc < 2) {
    m1->usage();
    exit(1);
  }

  m1->print_params();
  cout << "Map before search:\n";
  m1->print_valMatrix();
  cout << "Searching in '" << argv[1] << "' mode...\n";
  m1->search(argv[1]);
  cout << "Map after search:\n";
  cout << " Explored Map:\n";
  m1->print_valMatrixExplored();
  cout << " Path in Map:\n";
  m1->print_valMatrix();
  m1->printStats();
}
