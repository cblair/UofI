#include <iostream>
#include <windows.h> // for sleep

#define DEBUG 1 // gives some output for nn.h

#include "nn.h"
 
  
using namespace std;

int main() {
    cout << "NN Test\n";
    nnMaster *nTemp = new nnMaster();  
    //Requirements:  The ANN should have 5 inputs: 
    // height, xPosition, Yvelocity, Xvelocity, and fuel. 
    float input[] = {1,2,3,4,5};
    float expected[] = {100,150};
    nTemp->calcNN(input); 
    nTemp->learn(input,expected);
    nTemp->calcNN(input);
    free(nTemp);
    system("pause"); 
}
 
