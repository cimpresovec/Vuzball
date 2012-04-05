#ifndef LUKA_BOX_H
#define LUKA_BOX_H

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

#include <Box2D.h>

//Windows
#if defined(WIN32) || defined(_MSC_VER)
#pragma comment(lib, "box2d.lib")
#endif
//Add linux

namespace box
{
	//World
	extern b2World world;

	//Step parameters
	extern float32 timeStep;
	extern int32 velocityIterations;
	extern int32 positionIterations;

	//Structures
	struct BoxBody;
	struct BoxFixture;
	struct ContactListener;

	//Enum for Categorys of Fixtures
	enum fixtureCategory
	{
		/*Example
		 FIRST = 0x0001,
		 SECOND = 0x0002,
		 ...
		 LAST = 0xFFFF,
		 */
		BOX = 0x0001,
		FLOOR = 0x0002,
		PLAYER = 0x0004,
	};

	struct BoxBody
	{
		b2Body* body; //Box2D body
		unsigned int type; //Type of the body, used in ignoring collision
		bool b_inContact; //State if in contact with other body
		std::vector<b2Fixture*> inContactWith; //Fixtures from other bodys that are in contact with this body
		std::vector<b2Fixture*> areInContact; //Fixtures in contact
		std::vector<int> timesInContact; //Number of times a fixture is in contact, index is same as in areInContact
		unsigned int i_numberOfFixtures; //Number of fixtures
		std::vector<int> doNotCollideWith; //List of boxbodys not to collide with

		BoxBody();
		~BoxBody();

		//Create body
		void createBody(const b2Vec2& position, const float& angle, const b2BodyType& type = b2_dynamicBody, const bool& fixRotation = false, const bool& bullet = false);

		//Add shape
		void addShape(const b2Vec2* vertices, const int count, const float& density = 1.f, const float& friction = .1f, const float& restitution = 0.f, const b2Vec2& origin = b2Vec2(0.f, 0.f), const float& angle = 0.f);
		void addShapeBox(const float& hx, const float& hy, const float& density = 1.f, const float& friction = .1f, const float& restitution = 0.f, const b2Vec2& origin = b2Vec2(0.f, 0.f), const float& angle = 0.f);
		void addShapeCircle(const b2Vec2& position, const float& radius, const float& density = 1.f, const float& friction = .1f, const float& restitution = 0.f);

		//CollisionFilter
		bool changeFilter(const uint16 categoryBits, const uint16 maskBits, const int fixtureIndex = -1, const int groupIndex = 0);

		//Render
		void renderSolid(const unsigned int& tex = 0, const b2Color& color = b2Color(1.f, 1.f, 1.f));
	};

	//Contact listener
	struct ContactListener : public b2ContactListener
	{
		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	};
}

#endif