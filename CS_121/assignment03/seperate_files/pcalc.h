#ifndef CLASSP_H
#define CLASSP_H

class P_Calc
{
private:
  static const int MAX_SIZE = 30;
  struct Node
  {
    double m_dval;
    Node *m_next;
  };
  Node *head;
  Node *pI;
  Node *pTail;
  bool prog_quit;
  string calc_op;
  int input_cnt;
public:
  P_Calc()
    : head(NULL), pI(NULL), pTail(NULL), prog_quit(false), 
      calc_op(""), input_cnt(0)
  {}

  void input()
  {
    string stemp = "";
    cin >> stemp;

    //check for quit; if true, then set the prog_quit var to true
    if(stemp == "quit") 
      {
	prog_quit = true;
	return;
      }
    //check for and note operator here
    if(is_op(stemp)) 
      {
	calc_op = stemp;
	return;
      } 
    //If the input is not a number, by this point, we return here.
    if( !is_num(stemp) )
      return;
    else  push(s_to_d(stemp));
    ++input_cnt;
  }

  void push(double dval)
  {
    //new node
    pI = new Node;
    pI->m_next = head;
    pI->m_dval = dval;
    head = pI;
  }

  double peek()
  {
    return(head->m_dval);
  }

  void pop()
  {
    if(is_empty()) return;
    
    pTail = head->m_next;
    head->m_next = NULL;
    delete head;
    head = pTail;
  }

  bool is_empty()
  {
    return(head == NULL);
  }

  bool print_stack()
  {
    if(is_empty() ) return(false);

    if(!is_empty())
      {
	for(pTail = head; pTail != NULL; pTail = pTail->m_next)
	  {
	    cout << pTail->m_dval;
	    cout << ' ';
	  }
      }    
    return( is_empty() );
  }

  bool is_num(string stemp)
  {
    int i = 0; 
    while(stemp[i] != '\0') 
      {
	if(!isdigit(stemp[i]) && stemp[i] != '.'  ) return(false);
	++i;
      }
    return(true);
  }

  double s_to_d(string stemp)
  {
    istringstream ssin(stemp);
    double dtemp;
    ssin >> dtemp;
    return(dtemp);
  }

  bool is_op(string stemp)
  {
    //takes the one and only char from the string
    //char op = stemp[0];

    return( stemp == "*" ||
	    stemp == "/" ||
	    stemp == "+" ||
	    stemp == "-" ||
	    stemp == "sq" ||	    
	    stemp == "sqrt" ||
	    stemp == "dup" ||
	    stemp == "swap" ||
	    stemp == "ps" 
	    );
     }

  bool quit()
  {
    return(prog_quit);
  }

  bool perform_op()
  {
    if(quit()) return(true);

    if(calc_op != "")
      {
	double dtemp;

	    ///////////Regular Arithmatic; MDAS
	    switch (static_cast<int>(calc_op[0]) )
	      {
		// '*' case
	      case 42:
		for(dtemp = 1.0; input_cnt > 0; --input_cnt)
		  {
		    dtemp *= head->m_dval;
		    pop();
		  }
		push(dtemp);
		break;
		
		// '/' case
	      case 47:
		dtemp = head->m_dval;
		pop();
		--input_cnt;
		while(input_cnt > 0)
		  {
		    dtemp = head->m_dval / dtemp;
		    pop();
		    --input_cnt;
		  }
		push(dtemp);
		break;
		
		// '+'
	      case 43:
		for(dtemp = 0; input_cnt > 0; --input_cnt)
		  {
		    dtemp += head->m_dval;
		    pop();
		  }
		push(dtemp);
		break;

		// '-'
	      case 45:
		dtemp = head->m_dval;
		pop();
		--input_cnt;
		while(input_cnt > 0)
		  {
		    dtemp = head->m_dval - dtemp;
		    pop();
		    --input_cnt;
		  }
		push(dtemp);
		break;
	      }
	    
	    /////////////End MDAS

	    //Specialized Functions
	    
	    if(calc_op == "sq")
	      head->m_dval = pow (head->m_dval, 2);
	    else if(calc_op == "sqrt")
	      head->m_dval = sqrt(head->m_dval);
	    else if(calc_op == "dup")
	      push(head->m_dval);
	    else if(calc_op == "swap")
	      {
		if(head != NULL && head->m_next != NULL)
		  {
		    pTail = head;
		    pI = head->m_next;
		    pTail->m_next = pI->m_next;
		    pI->m_next = pTail;
		    head = pI;
		  }
	      }
	    else if(calc_op == "ps")
	      {
		cout << "Stack contents: ";
		print_stack();
		cout << '\n';
	      }
	  
	    //Finalize operations
	    calc_op = "";
	    return(true);
      }
    return(false);
  }
};
#endif
