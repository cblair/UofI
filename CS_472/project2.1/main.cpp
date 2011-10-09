#include <iostream>
#include "tree.h"

#ifdef DEBUG
#include "test.h"
#endif

using namespace std;

int main()
{
	#ifdef DEBUG
	test_nodes();
	test_darray();
	test_trees();
	#endif

	return(0);
}
