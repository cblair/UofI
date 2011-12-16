#ifndef _GA_I_H
#define _GA_I_H

//individuals' attribute vector size
#define GA_MAX_ATTR_SIZE 50

//genetic algorithm individual
struct ga_i
{
	double attr[GA_MAX_ATTR_SIZE];
	int nattr;
	double (*random)(double); //how to create random values for attrs
};


struct ga_i *ga_i_init(double *, int);
void ga_i_copy(struct ga_i**, struct ga_i**);
int ga_i_del(struct ga_i**);

void ga_i_2p_crossover(struct ga_i **, struct ga_i **);
void ga_i_uniform_mutation(struct ga_i**);

void ga_i_print_attr(struct ga_i*);
#endif
