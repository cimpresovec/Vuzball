#include "Luka.h"
#include "LukaBox.h"

namespace box
{
	b2World world(b2Vec2(.0f, -10.f));
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	//Body class
	BoxBody::BoxBody()
	{
		body = NULL;
		type = 0;
		b_inContact = false;
		i_numberOfFixtures = 0;
	}

	BoxBody::~BoxBody()
	{
		inContactWith.clear();
		areInContact.clear();
		timesInContact.clear();
	}

	void BoxBody::createBody(const b2Vec2& position, const float& angle, const b2BodyType& type /* = b2_dynamicBody */, const bool& fixRotation /* = false */, const bool& bullet /* = false */)
	{
		//Body definition
		b2BodyDef def;
		def.position.Set(position.x, position.y);
		def.angle = angle * DEGTORAD;
		def.type = type;
		def.fixedRotation = fixRotation;
		def.bullet = bullet;

		body = world.CreateBody(&def);
		
		//Add user data to body for this BoxBody
		body->SetUserData(this);
	}

	//Add shape
	void BoxBody::addShape(const b2Vec2* vertices, const int count, const float& density /*= 1.f*/, const float& friction /*= .1f*/, const float& restitution, const b2Vec2& origin /* = */, const float& angle)
	{
		//Create shape
		b2PolygonShape shape;
		shape.Set(vertices, count);

		b2Transform xf;
		xf.p = origin;
		xf.q.Set(angle);

		// Transform vertices and normals.
		for (int32 i = 0; i < count; ++i)
		{
			shape.m_vertices[i] = b2Mul(xf, shape.m_vertices[i]);
			shape.m_normals[i] = b2Mul(xf.q, shape.m_normals[i]);
		}

		//Fixture
		b2FixtureDef fixture;
		fixture.shape = &shape;
		fixture.density = density;
		fixture.friction = friction;
		fixture.restitution = restitution;

		body->CreateFixture(&fixture);
		i_numberOfFixtures++;
	}

	//Box shape
	void BoxBody::addShapeBox(const float& hx, const float& hy, const float& density /* = 1.f */, const float& friction /* = .1f */, const float& restitution /* = 0.f */, const b2Vec2& origin /* = b2Vec2 */, const float& angle)
	{
		//Create shape
		b2PolygonShape shape;
		shape.SetAsBox(hx, hy, origin, angle);

		//Fixture
		b2FixtureDef fixture;
		fixture.shape = &shape;
		fixture.density = density;
		fixture.friction = friction;
		fixture.restitution = restitution;
		body->CreateFixture(&fixture);
		i_numberOfFixtures++;
	}

	//Circle shape
	void BoxBody::addShapeCircle(const b2Vec2& position, const float& radius, const float& density /* = 1.f */, const float& friction /* = .1f */, const float& restitution /* = 0.f */)
	{
		//Create shape
		b2CircleShape shape;
		shape.m_p.Set(position.x, position.y);
		shape.m_radius = radius;

		//Fixture
		b2FixtureDef fixture;
		fixture.shape = &shape;
		fixture.density = density;
		fixture.friction = friction;
		fixture.restitution = restitution;

		body->CreateFixture(&fixture);
		i_numberOfFixtures++;
	}

	//Change collision filter
	bool BoxBody::changeFilter(const uint16 categoryBits, const uint16 maskBits, const int fixtureIndex /* = -1 */, const int groupIndex /* = 0 */)
	{
		//If fixtureIndex is specified
		if (fixtureIndex >= 0)
		{
			//Check if bigger than number of fixtures
			if (fixtureIndex >= (signed)i_numberOfFixtures)
			{
				return false;
			}
		}

		//Get correct fixture
		b2Fixture* fix = body->GetFixtureList();
		
		//Get last one if not specified
		if (fixtureIndex == -1)
		{
			for (int n = 0; n < (signed)(i_numberOfFixtures-1); n++) fix->GetNext();
		}

		//Get the specified
		else
		{
			for (int n = 0; n < fixtureIndex; n++) fix->GetNext();
		}

		//Change the filter data
		b2Filter fil = fix->GetFilterData();
		fil.categoryBits = categoryBits;
		fil.maskBits = maskBits;
		fil.groupIndex = groupIndex;
		fix->SetFilterData(fil);

		return true;
	}

	//Render solid
	void BoxBody::renderSolid(const unsigned int& tex, const b2Color& color)
	{
		glPushMatrix();
		
		//Translation
		glTranslatef(body->GetPosition().x, body->GetPosition().y, 0);
		//Rotation
		glRotatef(body->GetAngle() * RADTODEG, 0.f, 0.f, 1.f);

		glColor4f(color.r, color.g, color.b, 1.f);

		int childNumber = 0;
		//For every fixture
		for (b2Fixture* fixs = body->GetFixtureList(); fixs; fixs = fixs->GetNext() )
		{
			b2PolygonShape* shape = (b2PolygonShape*)fixs->GetShape();
			childNumber++;

			switch (fixs->GetType())
			{
			//Circle
			case 0:
				{
					b2CircleShape* shape = (b2CircleShape*)fixs->GetShape();
					/*
					const float32 k_segments = 16.0f;
					const float32 k_increment = 2.0f * b2_pi / k_segments;
					float32 theta = 0.0f;

					glBegin(GL_TRIANGLE_FAN);
					for (int32 i = 0; i < k_segments; ++i)
					{
						b2Vec2 v = shape->m_p + shape->m_radius * b2Vec2(cosf(theta), sinf(theta));
						glVertex2f(v.x, v.y);
						theta += k_increment;
					}
					glEnd();
					*/
					if (tex)
					{
						//Quad texture
						b2AABB neke;
						b2Transform tr;
						tr.Set(b2Vec2(0.f, 0.f), 0);
						shape->ComputeAABB(&neke, tr, 0);

						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, tex);
						glPointSize(5);
						glBegin(GL_QUADS);
						glTexCoord2f(1,1); glVertex2f(neke.upperBound.x,neke.upperBound.y);
						glTexCoord2f(0,1); glVertex2f(neke.lowerBound.x,neke.upperBound.y);
						glTexCoord2f(0,0); glVertex2f(neke.lowerBound.x,neke.lowerBound.y);
						glTexCoord2f(1,0); glVertex2f(neke.upperBound.x,neke.lowerBound.y);
						glEnd();

						glDisable(GL_TEXTURE_2D);
					}

					break;
				}
			//Polygon
			case 2:
				{
					b2PolygonShape* shape = (b2PolygonShape*)fixs->GetShape();
					
					if (!tex)
					{

					
					glBegin(GL_TRIANGLE_FAN);
					for (int32 i = 0; i < shape->GetVertexCount() ; ++i)
					{
						glVertex2f(shape->GetVertex(i).x, shape->GetVertex(i).y);
					}
					glEnd();
					}

					if (tex)
					{
						//Quad texture
						b2AABB neke;
						b2Transform tr;
						tr.Set(b2Vec2(0.f, 0.f), 0);
						shape->ComputeAABB(&neke, tr, 0);

						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, tex);
						glPointSize(5);
						glBegin(GL_QUADS);
						glTexCoord2f(1,1); glVertex2f(neke.upperBound.x,neke.upperBound.y);
						glTexCoord2f(0,1); glVertex2f(neke.lowerBound.x,neke.upperBound.y);
						glTexCoord2f(0,0); glVertex2f(neke.lowerBound.x,neke.lowerBound.y);
						glTexCoord2f(1,0); glVertex2f(neke.upperBound.x,neke.lowerBound.y);
						glEnd();

						glDisable(GL_TEXTURE_2D);
					}
					break;
				}	
			}
			
		}
		glPopMatrix();
	}

	//Begin contact
	void ContactListener::BeginContact(b2Contact* contact)
	{
		//get the BoxBodys
		BoxBody* bodyA = (BoxBody*)contact->GetFixtureA()->GetBody()->GetUserData();
		BoxBody* bodyB = (BoxBody*)contact->GetFixtureB()->GetBody()->GetUserData();

		//Set their is touching
		bodyA->b_inContact = true;
		bodyB->b_inContact = true;

		//Set in contact with
		//Find is contact in list
		bool found = false;
		for (int n = 0; n < (signed)bodyA->inContactWith.size(); n++)
		{
			if (contact->GetFixtureB() == bodyA->inContactWith.at(n))
			{
				found = true;
				break;
			}
		}
		//Add contact if needed
		if (!found)
		{
			bodyA->inContactWith.push_back(contact->GetFixtureB());
		}

		//Same for body b
		found = false;
		for (int n = 0; n < (signed)bodyB->inContactWith.size(); n++)
		{
			if (contact->GetFixtureA() == bodyB->inContactWith.at(n))
			{
				found = true;
				break;
			}
		}
		//Add contact if needed
		if (!found)
		{
			bodyB->inContactWith.push_back(contact->GetFixtureA());
		}
		
		/////////////////////////////////////////////////
		//Searching and adding fixtures to the list of areInContact
		found = false;
		for (int n = 0; n < (signed)bodyA->areInContact.size(); n++)
		{
			if (contact->GetFixtureA() == bodyA->areInContact.at(n))
			{
				found = true;
				bodyA->timesInContact.at(n)++; //Increase the amout of times in contact for that body
				break;
			}
		}
		//Add contact if needed
		if (!found)
		{
			bodyA->areInContact.push_back(contact->GetFixtureA());
			bodyA->timesInContact.push_back(int(1));
		}

		/////////Body b
		found = false;
		for (int n = 0; n < (signed)bodyB->areInContact.size(); n++)
		{
			if (contact->GetFixtureB() == bodyB->areInContact.at(n))
			{
				found = true;
				bodyB->timesInContact.at(n)++; //Increase the amouut of times in contact for that body
				break;
			}
		}
		//Add contact if needed
		if (!found)
		{
			bodyB->areInContact.push_back(contact->GetFixtureB());
			bodyB->timesInContact.push_back(int(1));
		}
	}

	void ContactListener::EndContact(b2Contact* contact)
	{
		//get the BoxBodys
		BoxBody* bodyA = (BoxBody*)contact->GetFixtureA()->GetBody()->GetUserData();
		BoxBody* bodyB = (BoxBody*)contact->GetFixtureB()->GetBody()->GetUserData();

		//Find contact in list and remove it
		for (int n = 0; n < (signed)bodyA->inContactWith.size(); n++)
		{
			if (bodyA->inContactWith.at(n) == contact->GetFixtureB())
			{
				bodyA->inContactWith.erase(bodyA->inContactWith.begin()+n);
				break;
			}
		}
		//Body B
		for (int n = 0; n < (signed)bodyB->inContactWith.size(); n++)
		{
			if (bodyB->inContactWith.at(n) == contact->GetFixtureA())
			{
				bodyB->inContactWith.erase(bodyB->inContactWith.begin()+n);
				break;
			}
		}

		//Check if zero size to set inContact to false
		if (bodyA->inContactWith.size() == 0)
		{
			bodyA->b_inContact = false;
		}

		if (bodyB->inContactWith.size() == 0)
		{
			bodyB->b_inContact = false;
		}

		//Find areInContact fixtures and delete them if no more collisions
		for (int n = 0; n < (signed)bodyA->areInContact.size(); n++)
		{
			if (bodyA->areInContact.at(n) == contact->GetFixtureA())
			{
				if (--bodyA->timesInContact.at(n) <= 0)
				{
					bodyA->areInContact.erase(bodyA->areInContact.begin()+n);
					bodyA->timesInContact.erase(bodyA->timesInContact.begin()+n);
				}
				break;
			}
		}

		//Body B
		for (int n = 0; n < (signed)bodyB->areInContact.size(); n++)
		{
			if (bodyB->areInContact.at(n) == contact->GetFixtureB())
			{
				if (--bodyB->timesInContact.at(n) <= 0)
				{
					bodyB->areInContact.erase(bodyB->areInContact.begin()+n);
					bodyB->timesInContact.erase(bodyB->timesInContact.begin()+n);
				}
				break;
			}
		}
	}

	//Disable collision between two objects if one doesn't want to collide
	void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		//Get boxBodys
		BoxBody* bodyA = (BoxBody*)contact->GetFixtureA()->GetBody()->GetUserData();
		BoxBody* bodyB = (BoxBody*)contact->GetFixtureB()->GetBody()->GetUserData();

		//Check if bodyA doesn't want to collide with bodyB
		for (int n = 0; n < (signed)bodyA->doNotCollideWith.size(); n++)
		{
			if (bodyB->type == bodyA->doNotCollideWith.at(n))
			{
				//Disable contact and end function for performance
				contact->SetEnabled(false);
				return;
			}
		}

		//Same procedure for bodyB
		for (int n = 0; n < (signed)bodyB->doNotCollideWith.size(); n++)
		{
			if (bodyA->type == bodyB->doNotCollideWith.at(n))
			{
				//Disable contact and end function for performance
				contact->SetEnabled(false);
				return;
			}
		}
	}
}
