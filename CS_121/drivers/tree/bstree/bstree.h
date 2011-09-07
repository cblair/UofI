#ifndef _tree_h
#define _tree_h

class BSTree
{
 private:
         struct Node
         {
                int m_val;
                Node *left;
                Node *right;
         };
         Node *head, *pI, *pTail, *pNew;           
 public:     
         BSTree()
               : head(NULL), pI(NULL), pTail(NULL), pNew(NULL)
         {}
         
         void InsertNode(int &val)
	   {
	     pNew = new Node;
	     pNew->left = pNew->right = NULL;
	     pNew->m_val = val; 
	     if(head == NULL)
	       {
		 head = pNew;
		 return;
	       }
	     
	     RecursivelyInsertNode(head);
	   }
         
         void RecursivelyInsertNode(Node *&pCurrent)
	   {
	     if(pCurrent == NULL)
	       {
		 pCurrent = pNew;
		 return;            
	       }
	  /*   pNew->M_val <= pCurrent->m_val
	             //         \
	          left = ?      right ?
          
          */
	     if(pNew->m_val <= pCurrent->m_val)
	       {
		 RecursivelyInsertNode(pCurrent->left);
		 return;
	       }
          /*       pCurrent->m_val < val
                       /          \\
                    left = ?      right = ?
          
          */
	     if(pNew->m_val > pCurrent->m_val)
	       {
		 RecursivelyInsertNode(pCurrent->right);
		 return;                  
	       }
	   }
         
         void PrintTree()
	   {
	     RecursivelyPrintTree(head);     
	   }
         
         void RecursivelyPrintTree(Node *pCurrent)
	   {
	     if(pCurrent == NULL) return;
	     RecursivelyPrintTree(pCurrent->left);
	     cout << pCurrent->m_val << '\n';     
	     RecursivelyPrintTree(pCurrent->right);
	   }
};

#endif
