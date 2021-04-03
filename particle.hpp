#ifndef PARTICLE_HPP
#define PARTICLE_HPP

class Position {
	float x; // x value.
	float y; // y_value.
public:
	Position(float px, float py);

	/* Return x value. */
	float getX();

	/* Return  value. */
	float getY();

	/* Set a new x value. */
	void setX(float px);

	/* Set a new y value. */
	void setY(float py);

	/* Sum x_ to x. */
	void moveX(float x_);

	/* Sum y_ to y. */
	void moveY(float y_);
};

class Velocity {
	float x; // x value.
	float y; // y value.
public:
	Velocity(float vx, float vy);
	/* Return x value. */
	float getX();

	/* Return y value. */
	float getY();

	float getVelocityModuleSum();

	/* Set a new x value. */
	void setX(float vx);

	/* Set a new y value. */
	void setY(float vy);

	/* Sum x_ to x. */
	void accelerateX(float x_);

	/* Sum y_ to y. */
	void accelerateY(float y_);

	/* Invert x signal. */
	void invertX();

	/* Invert x signal. */
	void invertY();

	/* Limit the x and y values by velocity_limit. */
	void limit(float velocity_limit);
};

class Particle {
	float mass; // Mass value.
	float radius; // Radiuus value.
public:
	Position position;
	Velocity velocity;
	Particle(float px, float py, float vx, float vy, float _mass, float _radius);
	
	/* Update particle values. */
	void update(float ticket, float limit_x, float limit_y, float velocity_limit);
	
	/* Return mass value. */
	float getMass();

	/* Return radius value. */
	float getRadius();

	/* Limit the edges of a particles. */
	void limits(float limit_x, float limit_y);
};

#endif