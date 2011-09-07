#define MAX_BUF 256
#define NINPUTS 5
#define NDEPTH 2 //depth of neural network
#define NWIDTH 7
#define LC 0.5   //learning constant

#include <iostream>
using namespace std; //for test couts
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

class nn { //neural node
private:
        nn *input[NINPUTS];
        //nn *output[NINPUTS];
        
public:
       float weight[NINPUTS];
       int inputMax;
       float outputVal; //output value
       int active;

       nn(int n) {
            this->inputMax = n;
            //default vals
            this->active = 1; //1 or 0
            this->outputVal = 0;
       }     
       
       bool setWeight(int, int);
       bool initRandomWeights();
       bool addInput(int, nn *);
       bool calcOutputVal();
       bool adjust(float);
};       

bool nn::setWeight(int i, int val) {
     if(i >= this->inputMax) return(false);
     this->weight[i] = val;
     return(true);     
}

bool nn::initRandomWeights() {
     for(int i = 0; i < this->inputMax; i++) {
             this->weight[i] = (float)((rand() % 100) + 1) / 100;
             #ifdef DEBUG
             cout << " Set weight " << i << " to random val " << this->weight[i] << endl;
             #endif
     }
}

bool nn::addInput(int i, nn *input) {
     if(i >= this->inputMax) return(false);
     this->input[i] = input;
     return(true);
}

//g() for nodes ouput
bool nn::calcOutputVal() {
     float sum = 0;
     for(int y = 0; y < this->inputMax; y++) { 
             sum += (this->input[y]->outputVal * this->weight[y]);
             //cout << "  TS53: " << this->input[y]->outputVal << endl;        
     }
     this->outputVal = sum;

     return(true);  
}



bool nn::adjust(float expected) {
      for(int y = 0; y < this->inputMax; y++) {
             //w[i,j] (sum) = w[i,j] + (learning constant * active * error * g())
             float error = (this->outputVal - expected) / (expected + this->outputVal);
             #ifdef DEBUG
             cout << "DEBUG: Error : " << error << ", weight : " << this->weight[y] << endl;
             #endif
             //w = (LC * this->active * this->input[y]->outputVal * this->weight[y] * error * this->g(input derivative));
             //this->weight[y] = this->weight[y] + (-1 * LC * this->active * error * nn::gP(this->input[y])); //need g() 
             this->weight[y] = this->weight[y] + (-1 * LC * this->active * error); //need g()       
     }
     return(true);    
}


class nnMaster {
      
private:
        //neural list input, the front of the nn
        nn *nList[NDEPTH][NWIDTH];
        //collectors, the end of the nn that collects inputs from the network
        nn *collector1;
        nn *collector2;
        
public:
       nnMaster() {
                  cout << "Initializing neural network...\n";
                  srand(time(0));  // Initialize random number generator.

                  //init input nodes
                  for(int y = 0; y < NINPUTS; y++) {
                                   #ifdef DEBUG
                                   cout << "DEBUG: Init'ing node 0," << y << endl;
                                   #endif
                                   this->nList[0][y] = new nn(NINPUTS);
                                   this->nList[0][y]->initRandomWeights();
                  }
                  //init the rest of the nodes
                  for(int x = 1; x < NDEPTH; x++) {
                          for(int y = 0; y < NWIDTH; y++) {
                                  system("pause");
                           #ifdef DEBUG
                           cout << "DEBUG: Init'ing node " << x << "," << y << endl;
                           #endif
                           int ninput_nodes;
                           if(x == 1) ninput_nodes = NINPUTS;
                           else       ninput_nodes = NWIDTH;

                           this->nList[x][y] = new nn(ninput_nodes);
                           this->nList[x][y]->initRandomWeights();
                           //add all previous nodes as inputs
                           for(int y2 = 0; y2 < ninput_nodes; y2++) {
                            this->nList[x][y]->addInput(
                             y2,this->nList[x - 1][y2]
                            );
                            #ifdef DEBUG
                            cout << " DEBUG: Adding node " << x - 1 << "," << y2 << " as input\n";
                            #endif
                           }  
                          }
                  }

                  //init the collectors
                  int ninput_nodes;
                  if(NDEPTH > 1) ninput_nodes = NWIDTH;
                  else           ninput_nodes = NINPUTS;
                  this->collector1 = new nn(ninput_nodes);
system("pause"); 
                  this->collector2 = new nn(ninput_nodes);
                  for(int y2 = 0; y2 < ninput_nodes; y2++) {
                            this->collector1->addInput(
                             y2,this->nList[NDEPTH - 1][y2]
                            );        
                            this->collector2->addInput(
                             y2,this->nList[NDEPTH - 1][y2]
                            );
                  }
system("pause"); 
                  this->collector1->initRandomWeights();
                  this->collector2->initRandomWeights();
       }
       
       ~nnMaster() {
                  //destroy nodes
                  for(int x = 0; x < NDEPTH; x++) {
                                    for(int y = 0; y < NINPUTS; y++) { //needs NWIDTH for y > 0
                                            free(this->nList[x][y]);
                                    }
                  }
                  free(collector1);           
                  free(collector2);           
       }
       
       float calcNN(float []);
       bool learn(float [], float []);
};

float nnMaster::calcNN(float input[NINPUTS]) {
      //calc all vals except the collector's
      for(int x = 0; x < NDEPTH; x++) {
              for(int y = 0; y < NINPUTS; y++) {
                      if(x == 0) {
                           this->nList[x][y]->outputVal = input[y];
                           //cout << "Input " << x << ": " << this->nList[x][y]->outputVal << endl;
                      } else {
                           this->nList[x][y]->calcOutputVal();
                      }
                      //cout << " TS122: " << x << "," << y << " o-val = " << this->nList[x][y]->outputVal << endl; 
              }
     }
     //calulate the collector's
     this->collector1->calcOutputVal();
     this->collector2->calcOutputVal();
     #ifdef DEBUG
     cout << "#############\n";
     cout << " DEBUG: collector1 o-val: " << this->collector1->outputVal << endl;
     cout << " DEBUG: collector2 o-val: " << this->collector2->outputVal << endl;
     cout << "\n";
     #endif
     return(true);    
}


bool nnMaster::learn(float input[], float expected[]) {     
     for(int i = 0; i < 200; i++) { // should be some sort of time messurment
             this->collector1->adjust(expected[0]);        
             this->collector2->adjust(expected[1]);
             this->calcNN(input);
     }
     return(true); //hmm, did we learn something?    
}
