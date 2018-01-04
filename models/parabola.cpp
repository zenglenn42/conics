// Conic Section Parabola
//
// This program describes a parabola as the intersection of a plane with a
// 3D cone.  A functor is used to model the 3D parabola as a projection¹
// onto a 2D normal plane.
//
// ¹Well, the 3D parabola is translated down to the z = y plane and then
//  rotated onto the xy-plane by the generator angle for the cone.
//
// For a 45 degree cone, the parabola evaluated is
//
//	p(t) = (1/√2)・(t² - 1)
//
// Example output:
// Conic Section Parabola
// Cone Angle = 45 degrees.
//
// p(-2) = 2.12
// p(-1.5) = 0.884
// p(-1) = -1.57e-16
// p(-0.5) = -0.53
// p(0) = -0.707
// p(0.5) = -0.53
// p(1) = -1.57e-16
// p(1.5) = 0.884
// p(2) = 2.12

#include <iostream>
#include <iomanip>
#include <cmath>

struct functor {
	functor(double coneAngle) : n(1.0), phi(coneAngle), m(std::tan(coneAngle)) {}

	// On z² = m²(x² + y²), a cone with
	//	z = |r|, 
	//	m = tan(ɸ), where ɸ = cone angle
	//
	// the z = 1 + m∙y plane cuts out the parabola
	//
	// p₀(t) = <t, (m²t² - 1)/2m, (m²t² + 1)/2> parabola
	// p₁(t) = <t, (m²t² - 1)/2m, (m²t² - 1)/2> translate down 1 unit to
	//                                          z = y plane
	//
	// then rotated -ɸ ("clockwise") onto the xy-plane:
	//
	// p₂(t) = <t, cos(-ɸ)・(m²t² - 1)/2m + sin(-ɸ)・(m²t² - 1)/2, 0>

	double operator()(double t) {
		double m2 = m * m;
		double t2 = t * t;
		double term2 = (m2 * t2 - 1) / 2.0;
		double term1 = term2 / m;
		return std::cos(-phi) * term1 - std::sin(-phi) * term2;
	}

	double n;
	double phi;
	double m;
};

const double ConeAngle45 = M_PI / 4.0;
const double ConeAngle60 = M_PI / 3.0;

int main() {
	double coneAngle = ConeAngle45;
	functor parabola(coneAngle);

	std::string banner("Conic Section Parabola");
	std::cout << banner << std::endl;
	std::cout << "Cone Angle = " << coneAngle * 180 / M_PI << " degrees." ;
	std::cout << std::endl << std::endl;
	
	// Evalulate the parabola over a range of x-values.

	double minX = -2.0;
	double maxX = 2.0;
	double step = 0.5;

	for (double x = minX; x <= maxX; x += step) {
		std::cout << "p(" << std::setprecision(4) 
		          << x << ") = " 
		          << std::setprecision(3) 
		          << parabola(x) 
	                  << std::endl;
	}
}
