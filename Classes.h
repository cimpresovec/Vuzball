#ifndef CLASSES_H
#define CLASSES_H

#include "Luka.h"
#include "LukaBox.h"

enum type
{
	PLAYER, FOOT, FLOOR, BALL, WALL, GOAL, GOALSENSOR
};

class Player
{
private:
	
	b2RevoluteJoint* footJoint;
	bool b_left, b_right, b_up, b_action;
	bool b_jump, b_jumping, b_inAir, b_direction;
	float f_lowLimit, f_highLimit, f_motorSpeed, f_walkSpeed;
	int i_player;
	SDLKey left, right, up, action;

public:
	box::BoxBody body;
	box::BoxBody foot;
	Player(int player);

	void handleInput();
	void logic();
	void render();
};

#endif