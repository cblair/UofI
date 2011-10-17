#ifndef _DARRAY_H
#define _DARRAY_H


#define MAX_BUF 200
class darray
{
private:
	int size;

public:
	double a[MAX_BUF];

	darray(int, bool);
	bool copy(darray**);

	//getters
	double get_val(int);
	int get_size();	

	//debug
	bool print_vals();
};

#endif
