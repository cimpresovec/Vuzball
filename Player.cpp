#include "Classes.h"

Player::Player(int player)
{
	b_left = false;
	b_right = false;
	b_up = false;
	b_action = false;

	b_inAir = false;
	b_jump = false;
	b_jumping = false;

	if (player == 1)
	{
		body.createBody(b2Vec2(-5.f, 0.f), 0.f, b2_dynamicBody, true);
		body.addShapeCircle(b2Vec2(.0f, .5f), .5f);
		body.type = PLAYER;

		//Foot
		foot.createBody(b2Vec2(0.f,-.2f), 0.f);
		foot.addShapeBox(.05f,.4f, 0.0f, .0f, 0.f, b2Vec2(0.f, .5f));
		//Tip
		foot.addShapeBox(.3f, .2f, .1f, .0f, 1.f, b2Vec2(0.f, 0.f));
		foot.type = FOOT;
		foot.doNotCollideWith.push_back(PLAYER);
		foot.doNotCollideWith.push_back(FLOOR);

		//Keys
		left = SDLK_LEFT;
		right = SDLK_RIGHT;
		up = SDLK_UP;
		action = SDLK_p;

		//Kick stuff
		b_direction = true;
		f_lowLimit = -.3f;
		f_highLimit = 1.3f;
		f_motorSpeed = 10.f;
		f_walkSpeed = 10.f;
	}

	else if (player == 2)
	{
		body.createBody(b2Vec2(5.f, 0.f), 0.f, b2_dynamicBody, true);
		body.addShapeCircle(b2Vec2(.0f, .5f), .5f);
		body.type = PLAYER;

		//Foot
		foot.createBody(b2Vec2(0.f,-.2f), 0.f);
		foot.addShapeBox(.05f,.4f, 0.0f, .0f, 0.f, b2Vec2(0.f, .5f));
		//Tip
		foot.addShapeBox(.3f, .2f, .1f, .0f, 1.f);
		foot.type = FOOT;
		foot.doNotCollideWith.push_back(PLAYER);
		foot.doNotCollideWith.push_back(FLOOR);

		//Keys
		left = SDLK_a;
		right = SDLK_d;
		up = SDLK_w;
		action = SDLK_g;

		//Kick stuff
		b_direction = false;
		f_lowLimit = -1.3f;
		f_highLimit = .3f;
		f_motorSpeed = -10.f;
		f_walkSpeed = 10.f;
	}

	//Joint with player
	b2RevoluteJointDef joint;
	joint.bodyA = body.body;
	joint.bodyB = foot.body;
	joint.localAnchorA = body.body->GetLocalCenter();
	joint.localAnchorB = foot.body->GetLocalPoint(b2Vec2(0.f, .4f));
	footJoint = (b2RevoluteJoint*)box::world.CreateJoint(&joint);


	footJoint->SetMaxMotorTorque(100.f);
	footJoint->EnableLimit(true);

	if (!b_direction)
	{
	
		footJoint->SetLimits(f_highLimit, f_highLimit);
	}
	else
	{
		footJoint->SetLimits(f_lowLimit, f_lowLimit);
	}
}

void Player::handleInput()
{
	if (frm::event.type == SDL_KEYDOWN)
	{
		if (frm::event.key.keysym.sym == left) b_left = true;
		else if (frm::event.key.keysym.sym == right) b_right = true;
		else if (frm::event.key.keysym.sym == up) b_up = true;
		else if (frm::event.key.keysym.sym == action) b_action = true;
	}

	else if (frm::event.type == SDL_KEYUP)
	{
		if (frm::event.key.keysym.sym == left) b_left = false;
		else if (frm::event.key.keysym.sym == right) b_right = false;
		else if (frm::event.key.keysym.sym == up) b_up = false;
		else if (frm::event.key.keysym.sym == action) b_action = false;
	}
}

void Player::logic()
{
	//Keys
	if (b_left)
	{
		if (body.body->GetLinearVelocity().x > -f_walkSpeed)
		{
			body.body->ApplyForceToCenter(b2Vec2(-20.f, -.0f));
		}
	}
	if (b_right)
	{
		if (body.body->GetLinearVelocity().x < f_walkSpeed)
		{
			body.body->ApplyForceToCenter(b2Vec2(20.f, -.0f));
		}
	}
	if (!b_left && !b_right)
	{
		body.body->ApplyForceToCenter(b2Vec2(body.body->GetLinearVelocity().x * -2.f, 0.f));
	}
	if (b_up)
	{
		b_jump = true;
	}
	else
	{
		b_jump = false;
	}

	//Delej
	if (b_jump && !b_jumping)
	{
		b_jumping = true;

		if (body.body->GetLinearVelocity().y <= 0.5f)
		{
			body.body->ApplyForceToCenter(b2Vec2(0.f, 300.f));
		}
	}

	//Check floor
	if (body.b_inContact)
	{
		for (int n = 0; n < (signed)body.inContactWith.size(); n++)
		{
			UINT t = ((box::BoxBody*)body.inContactWith.at(n)->GetBody()->GetUserData())->type;
			if (t == FLOOR)
			{
				b_jumping = false;
			}
		}
	}
	//Kick
	if (b_action)
	{	
		if (footJoint->GetMotorSpeed() != f_motorSpeed)
		{
			footJoint->EnableMotor(true);
			footJoint->SetLimits(f_lowLimit, f_highLimit);
			footJoint->SetMotorSpeed(f_motorSpeed);
		}

		if (b_direction)
		{

			if (footJoint->GetJointAngle() >= f_highLimit)
			{
				footJoint->EnableMotor(false);
				footJoint->SetLimits(f_highLimit, f_highLimit);
			}
		}
		else
		{
			if (footJoint->GetJointAngle() <= f_lowLimit)
			{
				footJoint->EnableMotor(false);
				footJoint->SetLimits(f_lowLimit, f_lowLimit);
			}
		}
	}
	else
	{
		if (footJoint->GetMotorSpeed() != -f_motorSpeed)
		{
			footJoint->EnableMotor(true);
			footJoint->SetLimits(f_lowLimit, f_highLimit);
			footJoint->SetMotorSpeed(-f_motorSpeed);
		}

		if (b_direction)
		{
		
		if (footJoint->GetJointAngle() <= f_lowLimit)
		{
			footJoint->EnableMotor(false);
			footJoint->SetLimits(f_lowLimit, f_lowLimit);
		}
		}
		else
		{
			if (footJoint->GetJointAngle() >= f_highLimit)
			{
				footJoint->EnableMotor(false);
				footJoint->SetLimits(f_highLimit, f_highLimit);
			}
		}
	}
}

void Player::render()		
{
	if (b_direction)
	{
	
	foot.renderSolid(3);



	glPushMatrix();
	glTranslatef(body.body->GetWorldCenter().x, body.body->GetWorldCenter().y, 0.f);
	glScalef(1.1f, 1.1f, 0.f);
	glRotatef(180.f, 1.f, 0.f, 0.f);
	glTranslatef(-body.body->GetWorldCenter().x, -body.body->GetWorldCenter().y, 0.f);
	body.renderSolid(4);

	glPopMatrix();
	}
	else
	{
		foot.renderSolid(2);

		glPushMatrix();
		glTranslatef(body.body->GetWorldCenter().x, body.body->GetWorldCenter().y, 0.f);
		glScalef(1.1f, 1.2f, 0.f);
		glRotatef(180.f, 1.f, 0.f, 0.f);
		glTranslatef(-body.body->GetWorldCenter().x, -body.body->GetWorldCenter().y, 0.f);
		body.renderSolid(5);

		glPopMatrix();
	}
	
}