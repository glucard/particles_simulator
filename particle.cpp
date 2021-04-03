#include "particle.hpp"
#include <stdexcept>

Position::Position(float px, float py) {
	x = px;
	y = py;
}
float Position::getX() { 
	return x;
}
float Position::getY() {
	return y;
}
void Position::setX(float px) {
	x = px;
}
void Position::setY(float py) {
	y = py;
}
void Position::moveX(float x_) {
	x += x_;
}
void Position::moveY(float y_) {
	y += y_;
}

Velocity::Velocity(float vx, float vy) {
	x = vx;
	y = vy;
}
float Velocity::getX() {
	return x;
}
float Velocity::getY() {
	return y;
}
float Velocity::getVelocityModuleSum() {
	return sqrt(pow(x, 2)) + sqrt(pow(y, 2));
}
void Velocity::setX(float vx) {
	x = vx;
}
void Velocity::setY(float vy) {
	y = vy;
}
void Velocity::accelerateX(float x_) {
	x += x_;
}
void Velocity::accelerateY(float y_) {
	y += y_;
}
void Velocity::invertX() {
	x = -x;
}
void Velocity::invertY() {
	y = -y;
}
void Velocity::limit(float velocity_limit) {
	if (x > velocity_limit) {
		x = velocity_limit;
	}
	else if (x < -velocity_limit) {
		x = -velocity_limit;
	}
	if (y > velocity_limit) {
		y = velocity_limit;
	}
	else if (y < -velocity_limit) {
		y = -velocity_limit;
	}
}

Particle::Particle(float px, float py, float vx, float vy, float _mass, float _radius)
	:
	position(px, py),
	velocity(vx, vy)
{
	if (_radius < 0) {
		throw std::invalid_argument("received negative value");
	}
	mass = _mass;
	radius = _radius;
}
float Particle::getMass() {
	return mass;
}
float Particle::getRadius() {
	return radius;
}
void Particle::limits(float limit_x, float limit_y) {
	float px = position.getX();
	float py = position.getY();
	float diameter = radius * 2;
	if (px < 0) {
		position.setX(0);
		velocity.invertX();
	}
	else if (px + diameter > limit_x) { 
		position.setX(limit_x - diameter);
		velocity.invertX();
	}
	if (py < 0) {
		position.setY(0);
		velocity.invertY();
	}
	else if (py + diameter > limit_y) {
		position.setY(limit_y - diameter);
		velocity.invertY();
	}
}

void Particle::update(float ticket, float limit_x, float limit_y, float velocity_limit) {
	velocity.limit(velocity_limit);
	position.moveX(velocity.getX()/ticket);
	position.moveY(velocity.getY()/ticket);
	limits(limit_x, limit_y);
}
