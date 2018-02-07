// Plane from 3 points
//
// This program calculates the coefficients of an equation describing
// a plane that passes through 3 points.  The points should be non-colinear.
//
// Computation fu from:
// https://pihlaja.wordpress.com/2010/10/05/using-glclipplanes-and-plane-equations/
//
// Example output:
//
// Given 3 non-colinear points: (1, 0, 0), (0, 1, 0), (0, 0, 0)
// the corresponding plane is: (0)x + (0)y + (1)z + -0 = 0
//

#include <iostream>
#include <iomanip>
#include <cmath>


struct Point {
	double x, y, z;
};

struct Plane {
	Plane(Point p1, Point p2, Point p3) : p1{p1}, p2{p2}, p3{p3} {
		coeff[0] = (p1.y*(p2.z - p3.z)) + (p2.y*(p3.z - p1.z)) + (p3.y*(p1.z - p2.z));
		coeff[1] = (p1.z*(p2.x - p3.x)) + (p2.z*(p3.x - p1.x)) + (p3.z*(p1.x - p2.x));
		coeff[2] = (p1.x*(p2.y - p3.y)) + (p2.x*(p3.y - p1.y)) + (p3.x*(p1.y - p2.y));
		coeff[3] = -((p1.x*((p2.y*p3.z) - (p3.y*p2.z))) + (p2.x*((p3.y*p1.z) - (p1.y*p3.z))) + (p3.x*((p1.y*p2.z) - (p2.y*p1.z))));
	}

	Point p1, p2, p3;
	double coeff[4];
};

Point p1 = {1.0, 0.0, 0.0};
Point p2 = {0.0, 1.0, 0.0};
Point p3 = {0.0, 0.0, 1.0};

std::ostream& operator<<(std::ostream& os, Plane pl) {
	os << "(" << pl.coeff[0] << ")x + (" << pl.coeff[1] << ")y + (" << pl.coeff[2] << ")z + " << pl.coeff[3] << " = 0";
	return os;
}

std::ostream& operator<<(std::ostream& os, Point p) {
	os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
	return os;
}

int main(int argc, char ** argv) {
	Plane pl = Plane(p1, p2, p3);

	std::cout << "Given 3 non-colinear points: " << p1 << ", " << p2 << ", " << p3 << std::endl;
	std::cout << "the corresponding plane is: " << pl << std::endl;
}
