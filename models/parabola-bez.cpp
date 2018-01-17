// Conic Section Parabola Modeled with 2nd order Bezier Control Points
//
// This program models the parabolic intersection of a plane with a cone
// using 3 Bezier control points that roughly corresponding to the normal
// view of the parabola in the plane.¹
//
// ¹Well, the 3D parabola is translated down to the z = y plane and then
//  rotated onto the xy-plane by the generator angle for the cone.
//
// For a 45 degree cone, the parabola modeled is
//
//	p(t) = (1/√2)・(t² - 1)
//
// The control points allow for efficient rendering of the parabola by a
// GPU.  Focus and directrix are also modeled as a point and line
// respectively.
//
// Example output:
// Conic Section Parabola with 2nd order Bezier Control Points
// Cone Angle = 45 degrees.
//
// p(-2.00) = 2.121
// p(-1.50) = 0.884
// p(-1.00) = -0.000
// p(-0.50) = -0.530
// p(0.00) = -0.707
// p(0.50) = -0.530
// p(1.00) = -0.000
// p(1.50) = 0.884
// p(2.00) = 2.121
// vertex(0.000, -0.707)
// focus(0.000, -0.354)
// aCoeff = 0.707
// directrix @ vertex = -1.061
//
// 2nd Order Bezier Control Points
// B0(-2.000, 2.121) B1(0.000, -3.536) B2(2.000, 2.121)
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>

struct Point2D {
	double x;
	double y;
};

struct Line2D {
	Line2D(double slope, double intercept) : m(slope), b(intercept) {}

	double operator()(double t) {
		double y = m * (t) + b;
		return y;
	}

	double m;
	double b;
};

struct ConicParabola {
	ConicParabola(double coneAngle) : n(1.0), phi(coneAngle), m(std::tan(coneAngle)) {
		double m2 = m * m;
		aCoeff = m2 * std::cos(phi) / (2.0 * m) + m2 * std::sin(phi) / 2.0;

		vertex.x = 0.0; // Assumes symmetric about the y-axis.
		vertex.y = (*this)(vertex.x);
		focus.x = vertex.x;
		assert(aCoeff != 0.0);	// TODO: Should be exception.
		focus.y = vertex.y + 1.0/(4.0 * aCoeff);

		double p = 2.0 * abs(focus.y - vertex.y);
		double m = 1.0;

		double b;
		if (vertex.y < focus.y) {
			b = focus.y - p;
		} else {
			b = focus.y + p;
		}
		Line2D line(m, b);
		directrix = line;
	}

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
		double p2 = std::cos(-phi) * term1 - std::sin(-phi) * term2;
		return p2;
	}

	double n;
	double phi;
	double m;
	double aCoeff;
	double p;
	Point2D vertex;
	Point2D focus;
	Line2D directrix{0.0, 0.0};
};

struct BezierControlPoints2D {
	Point2D B0;
	Point2D B1;
	Point2D B2;
};

struct Axis2D {
	Axis2D(Point2D min, Point2D max) : pMin(min), pMax(max) {}
	Point2D pMin;
	Point2D pMax;
};

struct Axes2D {
	Axes2D(Axis2D xAxis, Axis2D yAxis) : i(xAxis), j(yAxis) {}

	Axis2D i;
	Axis2D j;
};

struct BezierParabola2D {
	BezierParabola2D(ConicParabola parabola) : parabola(parabola) {}

	BezierControlPoints2D operator()(double minX, double maxX) {
		// test minX < maxX

		// left control point is just parabola endpoint on left
		bcp.B0.x = minX;
		bcp.B0.y = parabola(minX);

		// center point midway between endpoints B0 and B2
		double cx = bcp.B0.x + std::abs(maxX - minX)/2.0;
		double cy = bcp.B0.y;

		// midpoint between B0.x and cx
		double mx = bcp.B0.x + abs(bcp.B0.x - cx)/2.0;

		// parabola vertex
		double vx = cx;
		double vy = parabola(vx);

		// middle control point
		bcp.B1.x = vx;

		// std::cout << "abs(vy - cy) = " << abs(vy - cy) << std::endl;
		// std::cout << "abs(bcp.B0.x - cx) = " << abs(bcp.B0.x - cx) << std::endl;

		double deltaY = abs(vy - cy) * abs(bcp.B0.x - cx) / abs(mx - cx);

		// std::cout << "cy = "     << cy     << std::endl;
		// std::cout << "deltaY = " << deltaY << std::endl;
		// std::cout << "vx = " << vx << std::endl;
		// std::cout << "vy = " << vy << std::endl;
		// std::cout << "cy = " << cy << std::endl;

		bcp.B1.y = vy < cy ? cy - deltaY : cy + deltaY;

		// right control point is just parabola endpoint on right
		bcp.B2.x = maxX;
		bcp.B2.y = parabola(bcp.B2.x);

		return bcp;
	}

	ConicParabola parabola;
	BezierControlPoints2D bcp;
};

struct GpuParabola2D {
	GpuParabola2D(BezierParabola2D bezParabola, Axes2D axes) : bp(bezParabola), axes(axes) {};

	BezierParabola2D bp;
	Axes2D axes;
};

std::ostream& operator<<(std::ostream& os, const BezierControlPoints2D& bcp) {
	os << "B0(" << bcp.B0.x << ", " << bcp.B0.y << ") ";
	os << "B1(" << bcp.B1.x << ", " << bcp.B1.y << ") ";
	os << "B2(" << bcp.B2.x << ", " << bcp.B2.y << ")";
	return os;
}

const double ConeAngle45 = M_PI / 4.0;
const double ConeAngle60 = M_PI / 3.0;

int main() {
	double coneAngle = ConeAngle45;
	ConicParabola parabola(coneAngle);
	BezierParabola2D bezierParabola(parabola);

	std::string banner("Conic Section Parabola with 2nd order Bezier Control Points");
	std::cout << banner << std::endl;

	std::cout << "Cone Angle = " << coneAngle * 180 / M_PI << " degrees." ;
	std::cout << std::endl << std::endl;
	
	// Evalulate the parabola over a range of x-values.

	double minX = -2.0;
	double maxX =  2.0;

	double step = 0.5;
	for (double x = minX; x <= maxX; x += step) {
		std::cout << "p(" << std::fixed << std::setprecision(2) 
		          << x << ") = " 
		          << std::fixed << std::setprecision(3) 
		          << parabola(x) 
	                  << std::endl;
	}
	std::cout << "vertex(" << parabola.vertex.x << ", " << parabola.vertex.y << ")" << std::endl;
	std::cout << "focus(" << parabola.focus.x << ", " << parabola.focus.y << ")" << std::endl;
	std::cout << "aCoeff = " << parabola.aCoeff << std::endl;
	std::cout << "directrix @ vertex = " << parabola.directrix(parabola.vertex.x) << std::endl;
	std::cout << std::endl;

	BezierControlPoints2D parabolaCP = bezierParabola(minX, maxX);
	std::cout << "2nd Order Bezier Control Points" << std::endl;
	std::cout << parabolaCP << std::endl << std::endl;

	Point2D pMinX;
	pMinX.x = minX;
	pMinX.y = 0.0;
	Point2D pMaxX;
	pMaxX.x = maxX;
	pMaxX.y = 0.0;
	Axis2D xAxis(pMinX, pMaxX);

	Point2D pMinY;
	pMinY.x = 0.0;
	pMinY.y = -parabola(minX);
	Point2D pMaxY;
	pMaxY.x = 0.0;
	pMaxY.y = parabola(minX);
	Axis2D yAxis(pMinX, pMaxX);

	Axes2D axes(xAxis, yAxis);
	GpuParabola2D gpuParabola(bezierParabola, axes);
}
