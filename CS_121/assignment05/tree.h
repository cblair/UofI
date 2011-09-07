#ifndef _tree_h_included
#define _tree_h_included

#include <iomanip>

//In Order Tree Class
//Customer Data types for the Node for CS121 Assignment 05
class Tree
{
 private:
         static const int MAX_SIZE = 40;
         struct Node
         {
          string m_state;
          string m_capital;
          int m_population; 
          Node *m_left;
          Node *m_right;      
         };
         Node *head, *pI, *pTail;
 public:
         Tree()
               :head(NULL), pI(NULL), pTail(NULL)
         {}
         
	 bool IsLeaf(Node *pCurrent)
	   {
	     return(pCurrent->m_left == NULL && pCurrent->m_right == NULL);     
	   }
         bool empty()
	   {
	     return(head == NULL);     
	   }

	 /*

	 //should be passed 'head' by calling function to start from
	 // the head of the tree
	 Tree Seek(Tree pSeek, string state)
	   {
	     if(pSeek == NULL) return(NULL);
	     
	     Seek(pSeek->left, state);
	     if(pSeek->m_state == state) return(pSeek);
	     Seek(pSeek->right, state);
	     
	     return
	   }

	 */

	 void DeleteNode(string state)
	   {
	     //traverse until it is found
	     //delete the found node
	     //
	   }


	 /////////////////////////////////////////////////////
         //1. Sort by State Names
         void InsertNodeStateOrder(string state, string capital, int population)
         {
          //This Add function was originally designed by: Prof. Bruce Bolden,
          //CS Dept. U of I. I just couldn't get my own add function to work,
          //I will have to resolve troublshooting on this later.
          Node *newPtr = new Node;
        
          newPtr->m_state = state;
          newPtr->m_capital = capital;
          newPtr->m_population = population;
          newPtr->m_left  = NULL;
          newPtr->m_right = NULL;
          
          if( head == NULL )
          {
             head = newPtr;
          }
          else   
          {
             Node   *treePtr = head;
             Node   *targetNodePtr;
             
             while( treePtr != NULL )
             {
               targetNodePtr = treePtr;
               if(newPtr->m_state < treePtr->m_state) 
                  treePtr = treePtr->m_left; 
               else   
                  treePtr = treePtr->m_right; 
             }
             if(newPtr->m_state < targetNodePtr->m_state)
                targetNodePtr->m_left = newPtr;
             else 
                targetNodePtr->m_right = newPtr;
           }
         }
         
         void PrintStateOrder()
         {
          if(empty()) 
          {
           cout << "(empty)\n";
           return;
          }
           PrintStateOrderRecursively(head);
         }
         
         void PrintStateOrderRecursively(Node *pCurrent)
         {
          if(pCurrent == NULL) return;
          if(IsLeaf(pCurrent)) 
          {
           cout << setw(26) << pCurrent->m_state << "    "
                << setw(26) << pCurrent->m_capital << "    "
                << setw(26) << pCurrent->m_population << '\n'
                ;
           return;
           }
          PrintStateOrderRecursively(pCurrent->m_left);
          cout << setw(26) << pCurrent->m_state << "    "
	       << setw(26) << pCurrent->m_capital << "    "
	       << setw(26) << pCurrent->m_population << '\n'
	    ;
          PrintStateOrderRecursively(pCurrent->m_right);  
         }
         /////////////////////////////////////////////////////
         
         /////////////////////////////////////////////////////
         //2. Sort by Capital Names
         void InsertNodeCapitalOrder(string state, string capital, int population)
         {
          //This Add function was originally designed by: Prof. Bruce Bolden,
          //CS Dept. U of I. I just couldn't get my own add finction to work,
          //I will have to resolve troublshooting on this later.
          Node *newPtr = new Node;
        
          newPtr->m_state = state;
          newPtr->m_capital = capital;
          newPtr->m_population = population;
          newPtr->m_left  = NULL;
          newPtr->m_right = NULL;
          
          if( head == NULL )
          {
             head = newPtr;
          }
          else   
          {
             Node   *treePtr = head;
             Node   *targetNodePtr;
             
             while( treePtr != NULL )
             {
               targetNodePtr = treePtr;
               if(newPtr->m_capital < treePtr->m_capital) 
                  treePtr = treePtr->m_left; 
               else   
                  treePtr = treePtr->m_right; 
             }
             if(newPtr->m_capital < targetNodePtr->m_capital)
                targetNodePtr->m_left = newPtr;
             else 
                targetNodePtr->m_right = newPtr;
           }
         }
         
         void PrintCapitalOrder()
         {
          if(empty()) 
          {
           cout << "(empty)\n";
           return;
          }
          PrintCapitalOrderRecursively(head);
         }
         
         void PrintCapitalOrderRecursively(Node *pCurrent)
         {
          if(pCurrent == NULL) return;
          if(IsLeaf(pCurrent)) 
          {
           cout << setw(26) << pCurrent->m_state << "    "
                << setw(26) << pCurrent->m_capital << "    "
                << setw(26) << pCurrent->m_population << '\n'
                ; 
	   return;
           }
          PrintCapitalOrderRecursively(pCurrent->m_left);
	  cout << setw(26) << pCurrent->m_state << "    "
	       << setw(26) << pCurrent->m_capital << "    "
	       << setw(26) << pCurrent->m_population << '\n'
	    ; 
	  PrintCapitalOrderRecursively(pCurrent->m_right);  
         }
         
         /////////////////////////////////////////////////////
         
         
         /////////////////////////////////////////////////////
         //3. Sort by Population Names
         void InsertNodePopulationOrder(string state, string capital, int population)
         {
          //This Add function was originally designed by: Prof. Bruce Bolden,
          //CS Dept. U of I. I just couldn't get my own add finction to work,
          //I will have to resolve troublshooting on this later.
          Node *newPtr = new Node;
        
          newPtr->m_state = state;
          newPtr->m_capital = capital;
          newPtr->m_population = population;
          newPtr->m_left  = NULL;
          newPtr->m_right = NULL;
          
          if( head == NULL )
          {
             head = newPtr;
          }
          else   
          {
             Node   *treePtr = head;
             Node   *targetNodePtr;
             
             while( treePtr != NULL )
             {
               targetNodePtr = treePtr;
               if(newPtr->m_population < treePtr->m_population) 
                  treePtr = treePtr->m_left; 
               else   
                  treePtr = treePtr->m_right; 
             }
             if(newPtr->m_population < targetNodePtr->m_population)
                targetNodePtr->m_left = newPtr;
             else 
                targetNodePtr->m_right = newPtr;
           }
         }
         
         void PrintPopulationOrder()
         {
          if(empty()) 
          {
           cout << "(empty)\n";
           return;
          }
          PrintPopulationOrderRecursively(head);
         }
         
         void PrintPopulationOrderRecursively(Node *pCurrent)
         {
          if(pCurrent == NULL) return;
          if(IsLeaf(pCurrent)) 
          {
	    cout << setw(26) << pCurrent->m_state << "    "
	       << setw(26) << pCurrent->m_capital << "    "
	       << setw(26) << pCurrent->m_population << '\n'
	    ; 
	    return;
           }
          PrintPopulationOrderRecursively(pCurrent->m_left);
	  cout << setw(26) << pCurrent->m_state << "    "
	       << setw(26) << pCurrent->m_capital << "    "
	       << setw(26) << pCurrent->m_population << '\n'
	    ;
          PrintPopulationOrderRecursively(pCurrent->m_right);  
         }
         
         /////////////////////////////////////////////////////
         
         /////////////////////////////////////////////////////
         //4.
         void PrintLess5Mil()
         {
          if(empty()) 
          {
           cout << "(empty)\n";
           return;
          }
          PrintLess5MilRecursively(head);
         }
         
         void PrintLess5MilRecursively(Node *pCurrent)
         {
          if(pCurrent == NULL) return;
          if(pCurrent->m_population > 5000000) return;
          if(IsLeaf(pCurrent)) 
	    {
	      cout << setw(26) << pCurrent->m_state << "    "
		   << setw(26) << pCurrent->m_capital << "    "
		   << setw(26) << pCurrent->m_population << '\n'
		;
	      return;
	    }
          PrintLess5MilRecursively(pCurrent->m_left);
	  cout << setw(26) << pCurrent->m_state << "    "
	       << setw(26) << pCurrent->m_capital << "    "
	       << setw(26) << pCurrent->m_population << '\n'
	    ;
          PrintLess5MilRecursively(pCurrent->m_right);  
         }              
         void PrintMore10Mil()
	   {
	     if(empty()) 
	       {
		 cout << "(empty)\n";
		 return;
	       }
	     PrintMore10MilRecursively(head);
	   }
         
         void PrintMore10MilRecursively(Node *pCurrent)
	   {
	     if(pCurrent == NULL) return;
	     if(pCurrent->m_population < 1000000) return;
	     if(IsLeaf(pCurrent)) 
	       {
		 cout << setw(26) << pCurrent->m_state << "    "
		      << setw(26) << pCurrent->m_capital << "    "
		      << setw(26) << pCurrent->m_population << '\n'
		   ;
		 return;
	       }
	     PrintMore10MilRecursively(pCurrent->m_left);
	     
		 cout << setw(26) << pCurrent->m_state << "    "
		      << setw(26) << pCurrent->m_capital << "    "
		      << setw(26) << pCurrent->m_population << '\n'
		   ;
		 PrintMore10MilRecursively(pCurrent->m_right);  
	   }
         
      
};

#endif
