#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "node.h"

using namespace std;

#ifndef MAPH
#define MAPH

#define MAX_BUF 256

class map {
 private:
  int xmax;
  int ymax;
  int xstart;
  int ystart;
  int xgoal;
  int ygoal;

  node *valMatrix[MAX_BUF][MAX_BUF];
  node *fList; //frontier list
  node *eList; //explored list. Cleared every iter of iterative depth search
  int eListTotal; //overall explored list count, used in iterative depth search

 public:
 
	map(char *fname) {
	  this->xmax = this->ymax = this->xstart = this->ystart = this->xgoal \
	    = this->ygoal = 0;
	  
	  cout << "Initializing from map '" << fname << "'\n";
	  ifstream file(fname);
	  string line;
	  int lineNum = 1;
	  getline(file, line);
	  while ( file.good()){
	    //cout << setw(2) << lineNum << ": " << line << endl;
	    //read dimensions
	    if(lineNum == 1) {
	      this->set_dims(line);
	    } else if(lineNum == 2) { //read start position
	      this->set_start(line);
	    } else if(lineNum == 3) { //read goal position
	      this->set_goal(line);
	    } else {
	      //else insert line into value matrix
	      this->set_matrix_row(lineNum - 4,line);
	    }
	    ++lineNum;
	    getline(file, line);
	  }

	}
	
	~map() {
	}

	void usage();
	bool set_dims(string);
	bool set_start(string);
	bool set_goal(string);
	int get_cell_cost(char);
	bool set_matrix_row(int,string);
	bool validCell(int,int);
	void pushNeighborEnd(node*,int,int);
	void pushNeighborFront(node*,int,int,int,int);
	void pushNeighborsEnd(node*);
	void pushNeighborsFront(node*,int,int);
	void pushNeighborsFrontByCost(node*,int,int);
	void pushNeighborsFrontByCostH(char*,node*,int,int);
	int AH1(int,int);
	int AH2(int,int);
	bool goalFound();

	void searchBreadth();
	void searchDepth();
	void searchIDepth();
	void searchIDepthByCost();
	void searchByCost();
	void searchAH(char*);
	void search(char*);

	int printStats();

	//test funcs
	void print_params();
	void print_valMatrix();
	void print_valMatrixExplored();
};

#endif
