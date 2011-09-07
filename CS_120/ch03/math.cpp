#include <iostream>

using namespace std;

int main()
{
	int i, j, k, p, m, n;
	double a, b, c, d, e, f, g, h, x, y, z;

	i = 5;
	j = 5;
	k = 11;
	p = 3;
	x = 3.0;
	y = 4.0;


	cout << "----------Section 1 output:"
		"------------" << endl << endl;

	a = x+y;
	b = x - y;
	c = x * y;
	d = x / y;
	e = d + 3.0;
	f = d + 3;
	i = i + 1;
	j = j+ 1;

	cout << "a=" << a << "\t b=" << b << "\t c=" << c << "\t d=" << d << endl;
	cout << "e=" << e << "\t f=" << f << "\t i=" << i << "\t j=" << j << endl;

	cout << "\n\n----------Section 2 output"
		"-------------" << endl << endl;

	m = k % p;
	n = p % k;
	i++;
	++j;
	e--;
	--f;

	cout << "m=" << m << "\t n=" << n << "\t i=" << i << "\t j=" << j << endl;
	cout << "e=" << e << "\t f=" << f << endl << endl;

	x = y + z;

	cout << "With z not initialized, x=y+z = " << x << endl << endl;

	i = 32770;
	cout << "i=" << i << endl;
}
