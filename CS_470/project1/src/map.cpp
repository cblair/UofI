#include "map.h"

void map::usage() {
  cout << "Usage: search <mode>\n";
  cout << "       -modes: breadth, depth\n";
}

bool map::set_dims(string line) {
  int i = 0;
  //set x char
  char *x;
  x = &line[0];
  while(line[i] != ' ' && line[i] != '\n') i++;
  line[i] = '\0'; //terminate x char
  i++;
  while(line[i] == ' ') i++; //skip middle spaces
  //set y char
  char *y;
  y = &line[i];
  while(line[i] != ' ' && line[i] != '\n' && line[i] != '\0')
    i++;
  line[i] = '\0';
  //-1 to offset 0:n-1 map values
  this->xmax = atoi(x);
  this->ymax = atoi(y);
}

bool map::set_start(string line) {
  int i = 0;
  //set x char
  char *x;
  x = &line[0];
  while(line[i] != ' ' && line[i] != '\n') i++;
  line[i] = '\0'; //terminate x char
  i++;
  while(line[i] == ' ') i++; //skip middle spaces
  //set y char
  char *y;
  y = &line[i];
  while(line[i] != ' ' && line[i] != '\n' && line[i] != '\0')
    i++;
  line[i] = '\0';
  //-1 to offset 0:n-1 map values
  this->xstart = atoi(x);
  this->ystart = atoi(y);
}

bool map::set_goal(string line) {
  int i = 0;
  //set x char
  char *x;
  x = &line[0];
  while(line[i] != ' ' && line[i] != '\n') i++;
  line[i] = '\0'; //terminate x char
  i++;
  while(line[i] == ' ') i++; //skip middle spaces
  //set y char
  char *y;
  y = &line[i];
  while(line[i] != ' ' && line[i] != '\n' && line[i] != '\0')
    i++;
  line[i] = '\0';
  this->xgoal = atoi(x);
  this->ygoal = atoi(y);
}

int map::get_cell_cost(char cell) {
  if(cell == 'R') return(1); //road
  if(cell == 'f') return(2); //field
  if(cell == 'F') return(4); //forest
  if(cell == 'h') return(5); //hills
  if(cell == 'r') return(7); //river
  if(cell == 'M') return(10); //mountains
  if(cell == 'W') return(0); //water
}

bool map::set_matrix_row(int rowNum, string line) {
  int i = 0;
  while(line[i] != ' ' && line[i] != '\n' && line[i] != '\0') {
    this->valMatrix[i][rowNum] = new node(line[i],
					  this->get_cell_cost(line[i]),
					  i, rowNum
					  );
    i++;
  }
}

bool map::validCell(int x, int y) {
  if(this->eList->exists(x,y)) return(false);
  if(x < 0 || x >= this->xmax) return(false);
  if(y < 0 || y >= this->ymax) return(false);
  node *nTemp = this->valMatrix[x][y];
  if(nTemp->val < 1) return(false); //cost = 0, invalid
  if(nTemp->x >= this->xmax) return(false);
  if(nTemp->y >= this->ymax) return(false);
  return(true);
}

void map::pushNeighborEnd(node *nTemp, int x, int y) {
  nTemp->addNode(this->valMatrix[x][y]->name,
		       this->valMatrix[x][y]->val,
		       this->valMatrix[x][y]->x,
		       this->valMatrix[x][y]->y
		       );
  //add tracing to matrix
  this->valMatrix[x][y]->parent = this->valMatrix[nTemp->x][nTemp->y];
}

void map::pushNeighborFront(node *nTemp, int adderx, int addery, int x, int y) {
  nTemp->pushNodeFront(this->valMatrix[x][y]->name,
		       this->valMatrix[x][y]->val,
		       this->valMatrix[x][y]->x,
		       this->valMatrix[x][y]->y
		       );
  this->valMatrix[x][y]->parent = this->valMatrix[adderx][addery];
}

void map::pushNeighborsEnd(node *nTemp) {
  if(this->validCell(nTemp->x, nTemp->y - 1)) {
    this->pushNeighborEnd(nTemp, nTemp->x, nTemp->y - 1);
  }
  if(this->validCell(nTemp->x + 1, nTemp->y)) {
    this->pushNeighborEnd(nTemp, nTemp->x + 1, nTemp->y);
  }
  if(this->validCell(nTemp->x, nTemp->y + 1)) {
    this->pushNeighborEnd(nTemp, nTemp->x, nTemp->y + 1);
  }
  if(this->validCell(nTemp->x - 1, nTemp->y)) {
    this->pushNeighborEnd(nTemp, nTemp->x - 1, nTemp->y);
  }
}

void map::pushNeighborsFront(node *nTemp, int x, int y) {
  if(this->validCell(x, y - 1)) {
    this->pushNeighborFront(nTemp, x, y, x, y - 1);
  }
  if(this->validCell(x + 1, y)) {
    this->pushNeighborFront(nTemp, x, y, x + 1, y);
  }
  if(this->validCell(x, y + 1)) {
    this->pushNeighborFront(nTemp, x, y, x, y + 1);
  }
  if(this->validCell(x - 1, y)) {
    this->pushNeighborFront(nTemp, x, y, x - 1, y);
  }
}

void map::pushNeighborsFrontByCost(node *nTemp, int x, int y) {
  node *sorted = new node('0',0,0,0); //temp head
  if(this->validCell(x, y - 1)) {
    sorted->addNode(this->valMatrix[x][y - 1]->name,
		    this->valMatrix[x][y - 1]->val,
		    x, y - 1
		    );
  }
  if(this->validCell(x + 1, y)) {
    sorted->addNode(this->valMatrix[x + 1][y]->name,
		    this->valMatrix[x + 1][y]->val,
		    x + 1, y
		    );
  }
  if(this->validCell(x, y + 1)) {
    sorted->addNode(this->valMatrix[x][y + 1]->name,
		    this->valMatrix[x][y + 1]->val,
		    x, y + 1
		    );
  }
  if(this->validCell(x - 1, y)) {
    sorted->addNode(this->valMatrix[x - 1][y]->name,
		    this->valMatrix[x - 1][y]->val,
		    x - 1, y
		    );
  }
  sorted->popFront(); //pop temp head
  sorted = sorted->sortedValHighLow(); //sort
  //add to fList
  for(node *pTemp = sorted; pTemp != NULL && !pTemp->isEmpty(); pTemp = pTemp->next) {
    this->pushNeighborFront(nTemp, x,y,pTemp->x,pTemp->y);
  }
}

void map::pushNeighborsFrontByCostH(char *h,node *nTemp, int x, int y) {
  node *sorted = new node('0',0,0,0); //temp head
  if(this->validCell(x, y - 1)) {
    sorted->addNode(this->valMatrix[x][y - 1]->name,
		    this->valMatrix[x][y - 1]->val,
		    x, y - 1
		    );
  }
  if(this->validCell(x + 1, y)) {
    sorted->addNode(this->valMatrix[x + 1][y]->name,
		    this->valMatrix[x + 1][y]->val,
		    x + 1, y
		    );
  }
  if(this->validCell(x, y + 1)) {
    sorted->addNode(this->valMatrix[x][y + 1]->name,
		    this->valMatrix[x][y + 1]->val,
		    x, y + 1
		    );
  }
  if(this->validCell(x - 1, y)) {
    sorted->addNode(this->valMatrix[x - 1][y]->name,
		    this->valMatrix[x - 1][y]->val,
		    x - 1, y
		    );
  }
  sorted->popFront(); //pop temp head
  //add hysterious
  for(node *pTemp = sorted; pTemp != NULL && !pTemp->isEmpty(); pTemp = pTemp->next) {
    if(strcmp(h,"ah1") == 0) pTemp->val += this->AH1(pTemp->x,pTemp->y);
    else if(strcmp(h,"ah2") == 0) pTemp->val += this->AH2(pTemp->x,pTemp->y);
  }
  sorted = sorted->sortedValHighLow(); //sort
  //add to fList
  for(node *pTemp = sorted; pTemp != NULL && !pTemp->isEmpty(); pTemp = pTemp->next) {
    this->pushNeighborFront(nTemp, x,y,pTemp->x,pTemp->y);
  }
}

//hysterisous calc for h1
// calcs manhattan distance. pretty simple, underestimates
int map::AH1(int x, int y) {
  int xdiff = x - this->xgoal;
  int ydiff = y - this->ygoal;
  //abs
  if(xdiff < 0) xdiff *= -1;
  if(ydiff < 0) ydiff *= -1;
  return(xdiff + ydiff);
}

//hysterisous calc for h2
// cost guilt by association
int map::AH2(int x, int y) {
  int sum = 0;
  if(this->validCell(x, y - 1)) {
    sum += this->valMatrix[x][y - 1]->val;
  }
  if(this->validCell(x + 1, y)) {
    sum += this->valMatrix[x + 1][y]->val;
  }
  if(this->validCell(x, y + 1)) {
    sum += this->valMatrix[x][y + 1]->val;
  }
  if(this->validCell(x - 1, y)) {
    sum += this->valMatrix[x - 1][y]->val;
  }
  return(sum);
}

bool map::goalFound() {
  if(this->fList->x == this->xgoal
     && this->fList->y == this->ygoal) {
    return(true);
  }
  return(false);
}

void map::searchBreadth() {
    while(!this->goalFound()) {
    if(!this->eList->exists(this->fList->x, this->fList->y)) {
      this->eList->addNode(this->fList->name, this->fList->val, this->fList->x, this->fList->y);
      this->pushNeighborsEnd(this->fList);
    }
    this->fList->popFront();
  }
  //total just equals current explored list
  this->eListTotal = this->eList->length();
}

void map::searchDepth() {
  int depth = 1;
  int maxdepth = depth;
  while(!this->goalFound()) {
    if(!this->eList->exists(this->fList->x, this->fList->y)) {
      this->eList->addNode(this->fList->name, this->fList->val, this->fList->x, this->fList->y);
      int x = this->fList->x; int y = this->fList->y;
      this->fList->popFront();
      this->pushNeighborsFront(this->fList,x,y);
      depth++;
    } else {this->fList->popFront();} 
  }
  //total just equals current explored list
  this->eListTotal = this->eList->length();
}

void map::searchIDepth() {
  int depth = 1;
  int maxdepth = 2;
  while(!this->goalFound()) {
    if(!this->eList->exists(this->fList->x, this->fList->y) 
       && depth <= maxdepth) {
	//add to eList
	this->eList->addNode(this->fList->name, this->fList->val, this->fList->x, this->fList->y);
	//pop front
	int x = this->fList->x; int y = this->fList->y;
	this->fList->popFront();
	//push neighbors to front
	this->pushNeighborsFront(this->fList,x,y);
	this->eListTotal++;
	depth++;
    } else {this->fList->popFront();} 

    //all explored within max depth and no goal reached
    if(this->fList->isEmpty() && depth > maxdepth) {
      //reinit fList
      delete this->fList;
      this->fList = new node(this->valMatrix[this->xstart][this->ystart]->name,
			     this->valMatrix[this->xstart][this->ystart]->val,
			     this->xstart, this->ystart
			     );
      if(!this->goalFound()) {
	this->eList = new node(this->valMatrix[this->xstart][this->ystart]->name,
			       this->valMatrix[this->xstart][this->ystart]->val,
			       this->xstart, this->ystart
			       );
	this->pushNeighborsEnd(this->fList); //always end, so we get rid of start
	this->fList->popFront();
      }
      //end reinit
      maxdepth++;
      depth = 1;
    }
  }
}

void map::searchByCost() {
  while(!this->goalFound()) {
    if(!this->eList->exists(this->fList->x, this->fList->y)) {
      this->eList->addNode(this->fList->name, this->fList->val, this->fList->x, this->fList->y);
      int x = this->fList->x; int y = this->fList->y;
      this->fList->popFront();
      this->pushNeighborsFrontByCost(this->fList,x,y);
    } else {this->fList->popFront();} 
  }
  //total just equals current explored list
  this->eListTotal = this->eList->length();
}

void map::searchIDepthByCost() {
  int maxdepth = 2;
  while(!this->goalFound()) {
    int cost = this->valMatrix[this->fList->x][this->fList->y]->sumAncestorsVal();
    if(!this->eList->exists(this->fList->x, this->fList->y) && cost <= maxdepth) {
      this->eList->addNode(this->fList->name, this->fList->val, this->fList->x, this->fList->y);
      int x = this->fList->x; int y = this->fList->y;
      this->eListTotal++;
      this->fList->popFront();
      this->pushNeighborsFrontByCost(this->fList,x,y);
    } else {this->fList->popFront();} 

    //all explored within max depth and no goal reached
    if(this->fList->isEmpty() 
       || (this->fList->isEmpty() && cost > maxdepth)) { 
      //reinit fList
      delete this->fList;
      this->fList = new node(this->valMatrix[this->xstart][this->ystart]->name,
			     this->valMatrix[this->xstart][this->ystart]->val,
			     this->xstart, this->ystart
			     );
      this->eList = new node(this->valMatrix[this->xstart][this->ystart]->name,
			     this->valMatrix[this->xstart][this->ystart]->val,
			     this->xstart, this->ystart
			     );
      this->pushNeighborsEnd(this->fList); //always end, so we get rid of start
      this->fList->popFront();
      //end reinit
      maxdepth++;
    }
  }
}

void map::searchAH(char* h) {
  while(!this->goalFound()) {
    if(!this->eList->exists(this->fList->x, this->fList->y)) {
      this->eList->addNode(this->fList->name, this->fList->val, this->fList->x, this->fList->y);
      int x = this->fList->x; int y = this->fList->y;
      this->fList->popFront();
      this->pushNeighborsFrontByCostH(h,this->fList,x,y);
    } else {this->fList->popFront();} 
  }
  //total just equals current explored list
  this->eListTotal = this->eList->length();
}

void map::search(char *mode) {
  this->fList = new node(this->valMatrix[this->xstart][this->ystart]->name,
			 this->valMatrix[this->xstart][this->ystart]->val,
			 this->xstart, this->ystart
			 );
  if(!this->goalFound()) {
    this->eList = new node(this->valMatrix[this->xstart][this->ystart]->name,
			   this->valMatrix[this->xstart][this->ystart]->val,
			   this->xstart, this->ystart
			   );
    this->pushNeighborsEnd(this->fList); //always end, so we get rid of start
    this->fList->popFront();
    //end init
  } else {return;}
  
  if(strcmp(mode, "breadth") == 0) {
    this->searchBreadth();
  } else if(strcmp(mode, "depth") == 0) { 
    this->searchDepth();
  } else if (strcmp(mode,"idepth") == 0) {
    this->searchIDepth();
  } else if (strcmp(mode,"idepthbycost") == 0) {
    this->searchIDepthByCost();
  } else if (strcmp(mode,"bycost") == 0) {
    this->searchByCost();
  } else if (strcmp(mode,"ah1") == 0
	     || strcmp(mode,"ah2") == 0
	     ) {
    this->searchAH(mode);
  } else {
    this->usage();
    exit(1);
  }
  
  
  //cout << "TS163\n";
  //this->fList->printAncestors(this->valMatrix[this->xgoal][this->ygoal]);
}

int map::printStats() {
  cout << "Total Cost: " << this->valMatrix[this->xgoal][this->ygoal]->sumAncestorsVal() << endl;
  cout << "Explored list contained " << this->eList->length() << " elements\n";
  cout << "Path length: " << this->valMatrix[this->xgoal][this->ygoal]->nAncestors() << endl;
  cout << "Frontier List contained " << this->fList->length() << " elements\n";
  cout << "Total of all Explored Lists contained " << this->eListTotal << " elements" << endl;
  cout << "  (different in iterative depth searches only)\n";
  cout << " Space Complexity (E List + F Queue): " << this->eList->length() + this->fList->length() << endl;
  cout << " Time Complexity (E List Total, = E List except on Iter Depth): ";
  cout << this->eListTotal << endl;

}

//test funcs
void map::print_params() {
  cout << "Map Parameters:\n";
  cout << "Size: (" << this->xmax << ", " << this->ymax << ")\n";
  cout << "Start: (" << this->xstart << ", " << this->ystart << ")\n";
  cout << "Goal: (" << this->xgoal << ", " << this->ygoal << ")\n";
}

void map::print_valMatrix() {
  int rowNum = 0;
  while(rowNum < this->ymax) {
    int i = 0;
    while(i < this->xmax) {
      if(this->fList->areRelatives(this->valMatrix[this->xgoal][this->ygoal], this->valMatrix[i][rowNum])) {
	printf("  %3c_",this->valMatrix[i][rowNum]->name);
      } else {
	printf(" %4c ",this->valMatrix[i][rowNum]->name);
      }
      i++;
    }
    cout << endl;
    rowNum++;
  }
}

void map::print_valMatrixExplored() {
  int rowNum = 0;
  while(rowNum < this->ymax) {
    int i = 0;
    while(i < this->xmax) {
      if(this->eList->exists(i,rowNum)) {
	printf("  %3c_",this->valMatrix[i][rowNum]->name);
      } else {
	printf(" %4c ",this->valMatrix[i][rowNum]->name);
      }
      i++;
    }
    cout << endl;
    rowNum++;
  }
}
