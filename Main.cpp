#include "Classes.h" 

int main(int argc, char* args[])
{
	//Object
	box::ContactListener listener;
	box::world.SetContactListener(&listener);

	frm::initialize("Vuzball!", 1200, 600, false);

	Player* o_player1 = new Player(1);
	Player* o_player2 = new Player(2);

	//Font
	FTGLPixmapFont* font = new FTGLPixmapFont("FreeMono.ttf");

	//Score
	int score1 = 0, score2 = 0;

	//FPS
	frm::setFPS();
	bool play = true;

	//Static floor
	box::BoxBody* o_floor = new box::BoxBody();
	o_floor->createBody(b2Vec2(0.f, -5.f), 0.f, b2_staticBody);
	o_floor->addShapeBox(10.f, .3f);
	o_floor->type = FLOOR;

	//Walls
	box::BoxBody* wall = new box::BoxBody();
	wall->createBody(b2Vec2(10.f, 0.f), 0.f, b2_staticBody);
	wall->addShapeBox(.3f, 5.f);
	wall->type = WALL;

	box::BoxBody* wall2 = new box::BoxBody();
	wall2->createBody(b2Vec2(-10.f, 0.f), 0.f, b2_staticBody);
	wall2->addShapeBox(.3f, 5.f);
	wall2->type = WALL;

	box::BoxBody* wall3 = new box::BoxBody();
	wall3->createBody(b2Vec2(0.f, 5.f), 0.f, b2_staticBody);
	wall3->addShapeBox(10.f, .3f);
	wall3->type = WALL;

	//Ball
	box::BoxBody* o_ball = new box::BoxBody();
	o_ball->createBody(b2Vec2(0.f, 2.f), 0.f, b2_dynamicBody, false, true);
	o_ball->addShapeCircle(b2Vec2(0.f, 0.f), .3f, 0.01f, .1f, .7f);
	o_ball->type = BALL;

	//Goal 1
	box::BoxBody* goal1 = new box::BoxBody();
	goal1->createBody(b2Vec2(-9.f, -1.2f), 0.f, b2_staticBody);
	goal1->addShapeBox(1.f, 0.05f);
	goal1->type = GOAL;

	//Goal 2
	box::BoxBody* goal2 = new box::BoxBody();
	goal2->createBody(b2Vec2(9.f, -1.2f), 0.f, b2_staticBody);
	goal2->addShapeBox(1.f, 0.05f);
	goal2->type = GOAL;

	//Goalsensors
	box::BoxBody* goal1Sensor = new box::BoxBody();
	goal1Sensor->createBody(b2Vec2(-9.f, -3.f), 0.f, b2_staticBody);
	goal1Sensor->addShapeBox(0.8f, 1.7f);
	goal1Sensor->doNotCollideWith.push_back(GOAL);
	goal1Sensor->doNotCollideWith.push_back(WALL);
	goal1Sensor->doNotCollideWith.push_back(BALL);
	goal1Sensor->doNotCollideWith.push_back(PLAYER);
	goal1Sensor->doNotCollideWith.push_back(FLOOR);
	goal1Sensor->doNotCollideWith.push_back(FOOT);
	goal1Sensor->type = GOALSENSOR;

	box::BoxBody* goal2Sensor = new box::BoxBody();
	goal2Sensor->createBody(b2Vec2(9.f, -3.f), 0.f, b2_staticBody);
	goal2Sensor->addShapeBox(0.8f, 1.7f);
	goal2Sensor->doNotCollideWith.push_back(GOAL);
	goal2Sensor->doNotCollideWith.push_back(WALL);
	goal2Sensor->doNotCollideWith.push_back(BALL);
	goal2Sensor->doNotCollideWith.push_back(PLAYER);
	goal2Sensor->doNotCollideWith.push_back(FLOOR);
	goal2Sensor->doNotCollideWith.push_back(FOOT);
	goal2Sensor->type = GOALSENSOR;

	//Images
	frm::Image ball("ball.png");
	frm::Image shoe("shoe.png");
	frm::Image shoe2("shoe2.png");
	frm::Image gusta("gusta.png");
	frm::Image troll("troll.png");

	while (play)
	{
		//EVENTS
		while (SDL_PollEvent(&frm::event))
		{
			o_player1->handleInput();
			o_player2->handleInput();

			if (frm::event.type == SDL_KEYDOWN && frm::event.key.keysym.sym == SDLK_ESCAPE)
			{
				play = false;
			}
		}

		//LOGIC
		o_player1->logic();
		o_player2->logic();

		box::world.Step(box::timeStep, box::velocityIterations, box::positionIterations);

		//Check ball in goal
		//GOal1
		if (goal1Sensor->b_inContact)
		{
			for (int n = 0; n < (signed)goal1Sensor->inContactWith.size(); n++)
			{
				if (((box::BoxBody*)(goal1Sensor->inContactWith.at(n)->GetBody()->GetUserData()))->type == BALL)
				{
					//Mova BALL
					o_ball->body->SetTransform(b2Vec2(0.f, 2.f), 0.f);
					o_ball->body->SetLinearVelocity(b2Vec2(0.f, 0.f));
					o_ball->body->SetAngularVelocity(0.f);

					//Player1
					o_player1->body.body->SetTransform(b2Vec2(-5.f, 0.f), 0.f);
					o_player1->body.body->SetLinearVelocity(b2Vec2(0.f, 0.f));

					//Player2
					o_player2->body.body->SetTransform(b2Vec2(5.f, 0.f), 0.f);
					o_player2->body.body->SetLinearVelocity(b2Vec2(0.f, 0.f));

					score2++;

					break;
				}
			}
		}

		//GOal1
		if (goal2Sensor->b_inContact)
		{
			for (int n = 0; n < (signed)goal2Sensor->inContactWith.size(); n++)
			{
				if (((box::BoxBody*)(goal2Sensor->inContactWith.at(n)->GetBody()->GetUserData()))->type == BALL)
				{
					//Mova BALL
					o_ball->body->SetTransform(b2Vec2(0.f, 2.f), 0.f);
					o_ball->body->SetLinearVelocity(b2Vec2(0.f, 0.f));
					o_ball->body->SetAngularVelocity(0.f);

					//Player1
					o_player1->body.body->SetTransform(b2Vec2(-5.f, 0.f), 0.f);
					o_player1->body.body->SetLinearVelocity(b2Vec2(0.f, 0.f));

					//Player2
					o_player2->body.body->SetTransform(b2Vec2(5.f, 0.f), 0.f);
					o_player2->body.body->SetLinearVelocity(b2Vec2(0.f, 0.f));

					score1++;
					break;
				}
			}
		}

		frm::setFPS();

		//RENDER
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glScalef(0.1f, 0.2f, 1.f);

		o_floor->renderSolid();
		o_ball->renderSolid(1);
		o_player1->render();
		o_player2->render();
		wall->renderSolid();
		wall2->renderSolid();
		wall3->renderSolid();
		goal1->renderSolid();
		goal2->renderSolid();
		//goal1Sensor->renderSolid(0, b2Color(1.f,0.f, 0.f));
		//goal2Sensor->renderSolid(0, b2Color(1.f,0.f, 0.f));

		std::stringstream score;
		score << score1 << " : " << score2;
		frm::renderText(score.str(), font, 100.f, -2.5f, 3.f, frm::Color(255,0,0));

		SDL_GL_SwapBuffers();

		frm::delayFPS();
	}

	return 0;
}