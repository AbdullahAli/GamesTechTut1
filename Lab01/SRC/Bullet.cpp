#include "GameWorld.h"
#include "Bullet.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Bullets live for 2s by default. */
Bullet::Bullet()
	: GameObject("Bullet"), mTimeToLive(2000)
{
}

/** Construct a new bullet with given position, velocity, acceleration, angle, rotation and lifespan. */
Bullet::Bullet(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r, int ttl)
	: GameObject("Bullet", p, v, a, h, r), mTimeToLive(ttl)
{
}

/** Copy constructor. */
Bullet::Bullet(const Bullet& b)
	: GameObject(b),
	  mTimeToLive(b.mTimeToLive)
{
}

/** Destructor. */
Bullet::~Bullet(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update bullet, removing it from game world if necessary. */
void Bullet::Update(int t)
{
	// Update position/velocity
	GameObject::Update(t);
	mTimeToLive -=t;

	if(mTimeToLive == 0)
	{
		if(mWorld)
		{
			mWorld->FlagForRemoval(GetThisPtr());
		}
	}
}

/** Render this bullet. */
void Bullet::Render(void)
{
	// Disable lighting for solid colour lines
	glDisable(GL_LIGHTING);
	// Start drawing lines
	glBegin(GL_LINES);
		// Set colour to greenish hue
		glColor3f(0.2, 1.0, 0.6);
		// Add vertices to draw a short line
		glVertex3f(2.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
	// Finish drawing lines
	glEnd();
	// Enable lighting
	glEnable(GL_LIGHTING);
	// Call base class to render debug graphics if required
	GameObject::Render();
}

bool Bullet::CollisionTest(shared_ptr<GameObject> o)
{
	return false;
}

void Bullet::OnCollision(const GameObjectList &objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}