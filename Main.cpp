#include "Classes.h" 

int main(int argc, char* args[])
{
	//Object
	box::ContactListener listener;
	box::world.SetContactListener(&listener);

	frm::initialize("Vuzball!", 1200, 600, false);

	Player* o_player1 = new Player(1);
	Player* o_player2 = new Player(2);

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
	o_ball->addShapeCircle(b2Vec2(0.f, 0.f), .3f, 0.01f, .1, .7f);
	o_ball->type = BALL;

	//Goal 1
	box::BoxBody* goal1 = new box::BoxBody();
	goal1->createBody(b2Vec2(-9.f, -1.2f), 0.f, b2_staticBody);
	goal1->addShapeBox(1.f, 0.05f);

	//Goal 2
	box::BoxBody* goal2 = new box::BoxBody();
	goal2->createBody(b2Vec2(9.f, -1.2f), 0.f, b2_staticBody);
	goal2->addShapeBox(1.f, 0.05f);

	//Goalsensors


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

		SDL_GL_SwapBuffers();

		frm::delayFPS();
	}

	return 0;
}