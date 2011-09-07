////////////////////////////////////////
//    Class/Section:  CS-120/#3
//    Date:           09/17/07
//    Author:         Colby Blair
//    Lab:            5-1
////////////////////////////////////////

//This program has a known bug in which the cos original polynomial
//and decompoesed polynomial functions are equal.  This is traced 
// all the way to the return vals from the function.  There is 
// clearly a minor logic issue here, and I would be most 
//interested in any findings.  email @ cblair@uidaho.edu

#include <iostream>
#include <cmath>

using namespace std;

const double PI = 4.0*atan(1.0);	//declares a good PI const
						//globally, why not

double original_sin( double );	//assoc. w/ main section 2.
double original_cos( double );	//assoc. w/ main section 2.
double decomposed_sin( double );	//assoc. w/ main section 3.
double decomposed_cos( double );	//assoc. w/ main section 3.

int main( )
{
	cout << "Enter a value for x between 0 and 360: ";
	double x;
	cin >> x;

	//1. Display in radians; after this statement,
	//x will now be in radians
	cout << "x = " << x << " degrees = ";
	x = x * ((2 * PI) / 360);
	cout << x << " radians\n";


	//2. Original polynomial values
	double oSinX = original_sin(x);	//vars for the 2 functions to
	double oCosX = original_cos(x);	//handle the return vals
	
	cout << "Original polynomial values: sin(x) = " << oSinX
		<< ", cos(x) = " << oCosX << '\n';


	//3. Decomposed polynomial values
	double dSinX = decomposed_sin( x );		//same format as 2.
	double dCosX = decomposed_cos( x );

	cout << "Decomposed polynomial values: sin(x) = " << dSinX
		<< ", cos(x) = " << dCosX << '\n';


	//4. Math Library function values (sure glad these are here for 	//the future!)
	cout << "Math library function values: sin(x) = " << sin(x)
		<< ", cos(x) = " << cos(x) << '\n';


	//5. Delta of original to decomposed
	double sinDeltaX = oSinX - dSinX;
	double cosDeltaX = oCosX - dCosX;

	cout << "Delta original, decomposed: for sin(): " << sinDeltaX
 		<< ", for cos(): " << cosDeltaX << '\n';
	

	//6. Delta of decomposed and cmath
	sinDeltaX = sin(x) - dSinX;
	cosDeltaX = cos(x) - dCosX;
	
	cout << "Delta decomposed, computer: for sin(): " << sinDeltaX 		<< ", for cos(): " << cosDeltaX << '\n';

}

/////////////////////////
//Name: original_sin
//Desc: Finds the original polynomial of sine
//Args: Takes 1 var, x, from main
//Rets: Returns the double value from the Desc.
//To:		main, section 2.
/////////////////////////
double original_sin( double x )
{
	double funcVal_oSinX = -0.005643 * pow(x, 5) + 0.08863 * pow(x,
 	4) - 0.4016 * pow(x, 3) + 0.2862 * pow(x, 2) + 0.8612 * x 
	  + 0.01593;
	
	return ( funcVal_oSinX );
}

/////////////////////////
//Name: original_cos
//Desc: Finds the original polynomial of cosine
//Args: Takes 1 var, x, from main
//Rets: Returns the double value from the Desc.
//To:		main, section 2.
/////////////////////////
double original_cos( double x )
{
  double funcVal_oCosX = (0.0009544 * pow(x, 6)) +
    (-0.01799 * pow(x, 5)) + (0.1023 * pow(x, 4)) +
    (-0.10154 * pow(x, 3)) + (-0.421745 * pow(x, 2))
    + (-0.0203 * x) +1.0001;

	return ( funcVal_oCosX );
}

/////////////////////////
//Name: decomposed_sin
//Desc: Finds the decomposed polynomial of sine
//Args: Takes 1 var, x, from main
//Rets: Returns the double value from the Desc.
//To:		main, section 3.
/////////////////////////
double decomposed_sin( double x )
{
	double funcVal_dSinX = ((((-0.005643 * x + 0.08863) * x -
	0.4016) * x + 0.2862) * x + 0.8612) * x + 0.01593;

	return ( funcVal_dSinX );
}

/////////////////////////
//Name: decomposed_cos
//Desc: Finds the decomposed polynomial of cosine
//Args: Takes 1 var, x, from main
//Rets: Returns the double value from the Desc.
//To:		main, section 3.
/////////////////////////
double decomposed_cos( double x )
{
	double funcVal_dCosX = (((((0.0009544 * x - 0.01799) *
 	x + 0.1023) * x - 0.10154) * x - 0.421745) * x - 0.0203) * x + 	1.0001;

	return ( funcVal_dCosX );
}
