#include "Geometry.h"

// ============ Shape class =================

Shape::Shape(int d) {
	if (d < 0) {
		throw std::invalid_argument("d is a negative int.");
	} else {
		d_ = d;
	}
}

bool Shape::setDepth(int d) {
	if (d < 0) {
		return false;
	} else {
		d_ = d;
		return true;
	}
}

int Shape::getDepth() const {
	return d_;
}

int Shape::dim() const {
	return dim_;
}

// =============== Point class ================

Point::Point(float x, float y, int d) : Shape(d) {
	if (d < 0) {
		throw std::invalid_argument("Depth cannot be negative.");
	}
	dim_ = 0;
	x_ = x;
	y_ = y;
}

float Point::getX() const {
	return x_;
}

float Point::getY() const {
	return y_;
}

void Point::translate(float x, float y) {
	x_ += x;
	y_ += y;
}

void Point::rotate() {} // Empty function; has no effect

void Point::scale(float f) { // Has no effect
	if (f <= 0) {
		throw std::invalid_argument("f is 0 or negative.");
	}
}

bool Point::contains(const Point& p) const {
	// Compare both this and the parameter points coordinates
	if ((p.getX() == getX()) && (p.getY() == getY())) return true;
	return false;
}

// =========== LineSegment class ==============

LineSegment::LineSegment(const Point& p, const Point& q) : Shape(p.getDepth()) {
	if (p.getDepth() != q.getDepth()) {
		throw std::invalid_argument("The two endpoints have different depths.");
	} else if ((p.getX() != q.getX()) && (p.getY() != q.getY())) {
		throw std::invalid_argument("The two endpoints are not horizontal or vertical.");
	} else if ((p.getX() == q.getX()) && (p.getY() == q.getY())) {
		throw std::invalid_argument("The two endpoints are the same.");
	}
	dim_ = 1;
	A_ = new Point(p.getX(), p.getY(), p.getDepth());
	B_ = new Point(q.getX(), q.getY(), q.getDepth());
}

LineSegment::~LineSegment() {
	delete A_;
	delete B_;
}

float LineSegment::getXmin() const {
	if (A_->getX() <= B_->getX()) {
		return A_->getX();
	} else {
		return B_->getX();
	}
}

float LineSegment::getXmax() const {
	if (A_->getX() >= B_->getX()) {
		return A_->getX();
	} else {
		return B_->getX();
	}
}

float LineSegment::getYmin() const {
	if (A_->getY() <= B_->getY()) {
		return A_->getY();
	} else {
		return B_->getY();
	}
}

float LineSegment::getYmax() const {
	if (A_->getY() >= B_->getY()) {
		return A_->getY();
	} else {
		return B_->getY();
	}
}

float LineSegment::length() const {
	// Larger coordinate - the smaller coordinate
	if (A_->getY() == B_->getY()) {
		if (A_->getX() > B_->getX()) return (A_->getX() - B_->getX());
		return (B_->getX() - A_->getX());
	} else {
		if (A_->getY() > B_->getY()) return (A_->getY() - B_->getY());
		return (B_->getY() - A_->getY());
	}
}

void LineSegment::translate(float x, float y) {
	// The line constructed will already be vertical or horizontal,
	// So don't need to check the result here
	A_->translate(x, y);
	B_->translate(x, y);
}

void LineSegment::rotate() {
	float halfLength = length()/2; // The distance of each points to the midpoint
	if (A_->getX() == B_->getX()) {
		if (A_->getY() == getYmax()) {
			// Move A right and down
			A_->translate(halfLength, -halfLength);
			// Move B left and up
			B_->translate(-halfLength, halfLength);
		} else {
			// Opposite
			A_->translate(-halfLength, halfLength);
			B_->translate(halfLength, -halfLength);
		}
	} else {
		if (A_->getX() == getXmax()) {
			// Move A down and left
			A_->translate(-halfLength, -halfLength);
			// Move B up and right
			B_->translate(halfLength, halfLength);
		} else {
			// Opposite
			A_->translate(halfLength, halfLength);
			B_->translate(-halfLength, -halfLength);
		}
	}
}

void LineSegment::scale(float f) {
	if (f <= 0) {
		throw std::invalid_argument("The factor cannot be 0 or negative.");
	}
	if (f == 1) return;
	float halfLength = length()/2;
	// Scaling down
	if ((f > 0) && (f < 1)) {
		if (A_->getX() == B_->getX()) {
			if (A_->getY() == getYmax()) {
				A_->translate(0, -(halfLength-(halfLength*f)));
				B_->translate(0, (halfLength-(halfLength*f)));
			} else {
				A_->translate(0, (halfLength-(halfLength*f)));
				B_->translate(0, -(halfLength-(halfLength*f)));
			}
		} else {
			if (A_->getX() == getXmax()) {
				A_->translate(-(halfLength-(halfLength*f)), 0);
				B_->translate((halfLength-(halfLength*f)), 0);
			} else {
				A_->translate((halfLength-(halfLength*f)), 0);
				B_->translate(-(halfLength-(halfLength*f)), 0);
			}
		}
	// Scaling up
	} else if (f > 1) {
		if (A_->getX() == B_->getX()) {
			if (A_->getY() == getYmax()) {
				A_->translate(0, ((halfLength*f)-halfLength));
				B_->translate(0, -((halfLength*f)-halfLength));
			} else {
				A_->translate(0, -((halfLength*f)-halfLength));
				B_->translate(0, ((halfLength*f)-halfLength));
			}
		} else {
			if (A_->getX() == getXmax()) {
				A_->translate(((halfLength*f)-halfLength), 0);
				B_->translate(-((halfLength*f)-halfLength), 0);
			} else {
				A_->translate(-((halfLength*f)-halfLength), 0);
				B_->translate(((halfLength*f)-halfLength), 0);
			}
		}
	}
}

bool LineSegment::contains(const Point& p) const {
	if ((A_->getX() == B_->getX()) && (p.getX() == A_->getX()) && (p.getY() >= getYmin()) && (p.getY() <= getYmax())) {
		return true;
	} else if ((A_->getY() == B_->getY()) && (p.getY() == A_->getY()) && (p.getX() >= getXmin()) && (p.getX() <= getXmax())) {
		return true;
	}
	return false;
}

// ============ TwoDShape class ================

TwoDShape::TwoDShape(int d) : Shape(d) {}

// ============== Rectangle class ================

Rectangle::Rectangle(const Point& p, const Point& q) : TwoDShape(p.getDepth()) {
	if (p.getDepth() != q.getDepth()) {
		throw std::invalid_argument("The depths of both points are different.");
	} else if ((p.getX() == q.getX()) || (p.getY() == q.getY())) {
		throw std::invalid_argument("Both points have either the same x-coordinate or y-coordinate. Rectangle therefore cannot be made.");
	}
	dim_ = 2;
	A_ = new Point(p.getX(),p.getY(),p.getDepth());
	B_ = new Point(q.getX(),q.getY(),q.getDepth());
}

Rectangle::~Rectangle() {
	delete A_;
	delete B_;
}

float Rectangle::getXmin() const {
	if (A_->getX() < B_->getX()) {
		return A_->getX();
	}
	return B_->getX();
}

float Rectangle::getYmin() const {
	if (A_->getY() < B_->getY()) {
		return A_->getY();
	}
	return B_->getY();
}

float Rectangle::getXmax() const {
	if (A_->getX() > B_->getX()) {
		return A_->getX();
	}
	return B_->getX();
}

float Rectangle::getYmax() const {
	if (A_->getY() > B_->getY()) {
		return A_->getY();
	}
	return B_->getY();
}

void Rectangle::translate(float x, float y) {
	A_->translate(x, y);
	B_->translate(x, y);
}

void Rectangle::rotate() {
	float hLenX, hLenY; // Half lengths of X and Y sides
	hLenX = (getXmax() - getXmin())/2;
	hLenY = (getYmax() - getYmin())/2;
	if (A_->getX() == getXmax()) {
		if (A_->getY() == getYmax()) {
			A_->translate(-(hLenX - hLenY),-(hLenX + hLenY));
			B_->translate((hLenX - hLenY),(hLenX + hLenY));
		} else {
			A_->translate(-(hLenX + hLenY),-(hLenX - hLenY));
			B_->translate((hLenX + hLenY),(hLenX - hLenY));
		}
	} else {
		if (A_->getY() == getYmax()) {
			A_->translate((hLenX + hLenY),(hLenX - hLenY));
			B_->translate(-(hLenX + hLenY),-(hLenX - hLenY));
		} else {
			A_->translate((hLenX - hLenY),(hLenX + hLenY));
			B_->translate(-(hLenX - hLenY),-(hLenX + hLenY));
		}
	}

}

void Rectangle::scale(float f) {
	if (f <= 0) {
		throw std::invalid_argument("The factor cannot be 0 or negative.");
	}
	if (f == 1) return;
	float hLenX, hLenY;
	hLenX = (getXmax() - getXmin())/2;
	hLenY = (getYmax() - getYmin())/2;
	// Scaling down
	if ((f > 0) && (f < 1)) {
		if (A_->getX() == getXmax()) {
			if (A_->getY() == getYmax()) {
				A_->translate(-(hLenX-(hLenX*f)),-(hLenY-(hLenY*f)));
				B_->translate((hLenX-(hLenX*f)),(hLenY-(hLenY*f)));
			} else {
				A_->translate(-(hLenX-(hLenX*f)),(hLenY-(hLenY*f)));
				B_->translate((hLenX-(hLenX*f)),-(hLenY-(hLenY*f)));
			}
		} else {
			if (A_->getY() == getYmax()) {
				A_->translate((hLenX-(hLenX*f)),-(hLenY-(hLenY*f)));
				B_->translate(-(hLenX-(hLenX*f)),(hLenY-(hLenY*f)));
			} else {
				A_->translate(-(hLenX-(hLenX*f)),(hLenY-(hLenY*f)));
				B_->translate((hLenX-(hLenX*f)),-(hLenY-(hLenY*f)));
			}
		}
	// Scaling up
	} else if (f > 1) {
		if (A_->getX() == getXmax()) {
			if (A_->getY() == getYmax()) {
				A_->translate((hLenX*f)-hLenX,(hLenY*f)-hLenY);
				B_->translate(-((hLenX*f)-hLenX),-((hLenY*f)-hLenY));
			} else {
				A_->translate(-((hLenX*f)-hLenX),-((hLenY*f)-hLenY));
				B_->translate((hLenX*f)-hLenX,(hLenY*f)-hLenY);
			}
		} else {
			if (A_->getY() == getYmax()) {
				A_->translate(-((hLenX*f)-hLenX),(hLenY*f)-hLenY);
				B_->translate((hLenX*f)-hLenX,-((hLenY*f)-hLenY));
			} else {
				A_->translate((hLenX*f)-hLenX,-((hLenY*f)-hLenY));
				B_->translate(-((hLenX*f)-hLenX),(hLenY*f)-hLenY);
			}
		}
	}
}

bool Rectangle::contains(const Point& p) const {
	if ((p.getX() <= getXmax()) && (p.getX() >= getXmin()) && (p.getY() <= getYmax()) && (p.getY() >= getYmin())) {
		return true;
	}
	return false;
}

float Rectangle::area() const {
	// Width * height
	return (getXmax()-getXmin())*(getYmax()-getYmin());
}

// ================== Circle class ===================

Circle::Circle(const Point& c, float r) : TwoDShape(c.getDepth()) {
	if (r <= 0){
		throw std::invalid_argument("Radius is 0 or negative.");
	}
	dim_ = 2;
	c_ = new Point(c.getX(), c.getY(), c.getDepth());
	r_ = r;
}

Circle::~Circle() {
	delete c_;
}

float Circle::getX() const {
	return c_->getX();
}

float Circle::getY() const {
	return c_->getY();
}

float Circle::getR() const {
	return r_;
}

void Circle::translate(float x, float y) {
	c_->translate(x, y);
}

void Circle::rotate() { // Empty function; has no effect
}

void Circle::scale(float f) {
	if (f <= 0) {
		throw std::invalid_argument("The factor cannot be 0 or negative.");
	}
	r_ *= f;
}

bool Circle::contains(const Point& p) const {
	float xDist;
	float yDist;
	if (getX() >= p.getX()) {
		xDist = getX() - p.getX();
	} else {
		xDist = p.getX() - getX();
	}
	if (getY() >= p.getY()) {
		yDist = getY() - p.getY();
	} else {
		yDist = p.getY() - getY();
	}
	float dist = sqrt((pow(xDist,2)+pow(yDist,2))); // Distance of Point to parameter Point
	if (dist <= r_) {
		return true;
	}
	return false;
}

float Circle::area() const {
	return PI*r_*r_;
}

// ================= Scene class ===================

Scene::Scene() {
	drawDCall_ = false; // Flag to indicate the draw depth has not been set
}

void Scene::addObject(std::shared_ptr<Shape> ptr) {
	shapeCollection_.push_back(ptr);
}

void Scene::setDrawDepth(int depth) {
	drawD_ = depth;
	drawDCall_ = true; // We can now compare the objects depth to the specified draw depth
}

std::ostream& operator<<(std::ostream& out, const Scene& s) {
	for (int y = s.HEIGHT-1; y >= 0; y--) { // Work backwards, as the first line is top of the scene
		for (int x = 0; x <= s.WIDTH-1; x++) { // Work outwards from start of line
			bool toDraw = false;
			for (std::shared_ptr<Shape> shape : s.shapeCollection_) {
				if (s.drawDCall_) {
					if (((shape->contains(Point(x,y))) && (shape->getDepth() <= s.drawD_))) {
						toDraw = true;
					}
				} else if (shape->contains(Point(x,y))){
					toDraw = true;
				}
			}
			if (toDraw) {
				out << "*"; // Point contains is true, print
			} else {
				out << " "; // Blank space in scene
			}
		}
		out << "\n"; // Next row
	}
	return out;
}
