#ifndef _functions_h_included
#define _functions_h_included

//Stolen from http://www.cplusplus.com/reference/clibrary/ctime/clock.html
void wait ( int seconds )
{
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}

//Function Author: Colby Blair
//I finally got tired of juggling string inputs and int when using the 
//getline, so I made a transformation function, complete with comma filtering.
int strtoi(string s1)
{
    int output = 0;
    int is1 = 0;
    int power = 0;
    for(int i = s1.size() - 1; i >= 0; --i)
    {
     is1 = static_cast<int>(s1[i]);
     //cout << "char val = " << is1 << '\n';
     //This if statement filters out commas in the string number
     if(is1 != 44)
     {
             output += ( (is1 - 48) * pow(10.0, power )); 
             ++power;
     }        
    }
    return(output);
}

/////////////////////////////////////////////////////
//1. Sort by State Names
void InsertStateOrder(Tree &tree1, string state, string capital, int population)
{
     tree1.InsertNodeStateOrder(state, capital, population);    
}

void PrintTreeStateOrder(Tree tree1)
{
     tree1.PrintStateOrder();
}
///////////////////////////////////////////////////////

/////////////////////////////////////////////////////
//2. Sort by Capital Names
void InsertCapitalOrder(Tree &tree2, string state, string capital, int population)
{
     tree2.InsertNodeCapitalOrder(state, capital, population);    
}


void PrintTreeCapitalOrder(Tree tree2)
{
     tree2.PrintCapitalOrder();
}
///////////////////////////////////////////////////////


/////////////////////////////////////////////////////
//3. Sort by Population
void InsertPopulationOrder(Tree &tree3, string state, string capital, int population)
{
     tree3.InsertNodePopulationOrder(state, capital, population);    
}


void PrintTreePopulationOrder(Tree tree3)
{
     tree3.PrintPopulationOrder();
}
///////////////////////////////////////////////////////

//4. Print Certain Populations
void PrintTreeLess5Mil(Tree tree3)
{
      tree3.PrintLess5Mil();              
}

void PrintTreeMore10Mil(Tree tree3)
{
      tree3.PrintMore10Mil();                  
}

#endif
