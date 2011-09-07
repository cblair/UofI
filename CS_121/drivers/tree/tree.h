#ifndef _tree_h
#define _tree_h

class Tree
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
         Tree()
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
         
         void RecursivelyInsertNode(Node *pCurrent)
	   {
	  /*            pCurrent
	               /         \
	             NULL        NULL
           Left leaning tree if we reach this condition!
	  */
          if(pCurrent->left == NULL && pCurrent->right == NULL)
          {
	    pCurrent->left = pNew;  
	    return;               
          }

          /*               pCurrent
                         /         \
                      NULL         m_right
          */
          if(pCurrent->left == NULL && pCurrent->right != NULL)
          {
           pCurrent->left = pNew;
           return;                  
          }
          
          /*               pCurrent
                         /         \
                      m_left       NULL
          */
          if(pCurrent->left != NULL && pCurrent->right == NULL)
          {
           pCurrent->right = pNew;
           return;
          }
          /*               pCurrent
                         /         \
                      m_left       m_right
          Left leaning tree if we reach this condition!
          */
          RecursivelyInsertNode(pCurrent->left);
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
