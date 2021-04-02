#ifndef PARTICLE_HPP
#define PARTICLE_HPP

class Position {
	float x;
	float y;
public:
	Position(float px, float py);
	float getX();
	float getY();
	void setX(float px);
	void setY(float py);
	void moveX(float x_);
	void moveY(float y_);
};

class Velocity {
	float x;
	float y;
public:
	Velocity(float vx, float vy);
	float getX();
	float getY();
	float getVelocityModuleSum();
	void setX(float vx);
	void setY(float vy);
	void accelerateX(float x_);
	void accelerateY(float y_);
	void invertX();
	void invertY();
	void limit(float velocity_limit);
};

class Particle {
	float mass;
	float radius;
public:
	Position position;
	Velocity velocity;
	Particle(float px, float py, float vx, float vy, float _mass, float _radius);
	void update(float ticket, float limit_x, float limit_y, float velocity_limit);
	float getMass();
	float getRadius();
	void limits(float limit_x, float limit_y);
};



#endif