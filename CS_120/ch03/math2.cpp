#include <iostream>

using namespace std;

int main ()
{
	int	i = 1, j = 1,
		k1 = 10, k2 = 20, k3 = 30, k4 = 40, k5 = 50,
		k, h, m, n;

	double a = 7, b = 6, c = 5, d = 4,
		e, p, q, x, y, z;

	cout << "************ Section 1 ***************" << endl;
	cout << "Before increment, i=" << i << ", j=" << j << endl;

	k = i++;
	h = ++j;

	cout << "After increment, i=" << i << ", j=" << j <<
				", k=" << k << ", h=" << h << endl << endl;

	cout << "************ Section 2 ****************" << endl;


	m = 6 / 4;
	p = 6 / 4;
	n = 6 / 4.0;
	q = 6 / 4.0;

	cout << "m=" << m << ", p=" << p << ", n=" << n <<", q=" << q << endl << endl;


	cout << "************ Section 3 ************" << endl;
	cout << "Original k1=" << k1 << ", k2=" << k2 << ", k3=" << k3 <<
		", k4=" << k4 << ", k5=" << k5 << endl;

	k1 += 2;
	k2 -= i;
	k3 *= (8/4);
	k4 /= 2.0;
	k5 %= 2;

	cout << "New 	k1=" << k1 << ", k2=" << k2 << ", k3=" << k3 << ", k4=" << k4 <<
			", k5=" << k5 << endl << endl;


	cout << "************ Section 4 ************" << endl;

	e = 3;
	x = a + b - c / d * e;
	y = a + (b-c) / d * e;
	z = ((a + b) - c / d) * e;

	cout << "a=" << a << ", b=" << b <<", c=" << c << ", d=" << d << ", e=" << e << endl << endl;

	cout 	<< "x = a + b - c / d * e = " << x << endl
		<< "y = a + (b - c) / d * e = " << y << endl
		<< "z = ((a + b) - c / d) * e = " << z << endl;
}

