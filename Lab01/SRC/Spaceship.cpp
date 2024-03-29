#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "Spaceship.h"

using namespace std;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/**  Default constructor. */
Spaceship::Spaceship()
	: GameObject("Spaceship"), mThrust(0)
{
	//mBoundingShape = make_shared<BoundingSphere>(shared_from_this(), 4);
}

/** Construct a spaceship with given position, velocity, acceleration, angle, and rotation. */
Spaceship::Spaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: GameObject("Spaceship", p, v, a, h, r), mThrust(0)
{
	//mBoundingShape = make_shared<BoundingSphere>(shared_from_this(), 4);
}

/** Copy constructor. */
Spaceship::Spaceship(const Spaceship& s)
	: GameObject(s), mThrust(0)
{
	//mBoundingShape = make_shared<BoundingSphere>(shared_from_this(), 4);
}

/** Destructor. */
Spaceship::~Spaceship(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this spaceship. */
void Spaceship::Update(int t)
{
	// Call parent update function
	GameObject::Update(t);
}

/** Render this spaceship. */
void Spaceship::Render(void)
{
	// Disable lighting to get solid lines
	glDisable(GL_LIGHTING);
	// Set pen colour to very light grey
	glColor3f(0.8, 0.8, 0.8);
	// Start drawing closed shape
	glBegin(GL_LINE_LOOP);
		// Add vertices of ship's body
		glVertex3f(-3,-2, 0);
		glVertex3f( 4, 0, 0);
		glVertex3f(-3, 2, 0);
	// Finish drawing closed shape
	glEnd();

	if(mThrust > 0)
	{
		glBegin(GL_LINE_STRIP);
				// Set pen colour to dark orange
				glColor3f(0.8, 0.4, 0.1);
				// Add vertices of ship's flame
				glVertex3f(-2,-1, 0);
				glVertex3f(-4, 0, 0);
				glVertex3f(-2, 1, 0);
			// Finish drawing open line
			glEnd();
	}

	// Enable lighting
	glEnable(GL_LIGHTING);
	// Call base class to render debug graphics if required
	GameObject::Render();
}

// PUBLIC INSTANCE METHODS IMPLEMENTING IWeapon ///////////////////////////////

/** Fire the rockets. */
void Spaceship::Thrust(float t)
{
	mThrust = t;

	mAcceleration.x = mThrust * cos(DEG2RAD*mAngle);
	mAcceleration.y = mThrust * sin(DEG2RAD*mAngle);
}

/** Set the rotation. */
void Spaceship::Rotate(float r)
{
	mRotation = r;
}

/** Shoot a bullet. */
void Spaceship::Shoot(void)
{
	// Check the world exists
	if (!mWorld) return;
	// Construct a unit length vector in the direction the spaceship is headed
	GLVector3f spaceship_heading(cos(DEG2RAD*mAngle), sin(DEG2RAD*mAngle), 0);
	spaceship_heading.normalize();
	// Calculate the point at the node of the spaceship from position and heading
	GLVector3f bullet_position = mPosition + (spaceship_heading * 4);
	// Calculate how fast the bullet should travel
	float bullet_speed = 30;
	// Construct a vector for the bullet's velocity
	GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;
	// Construct a new bullet
	Bullet bullet = Bullet(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000);
	shared_ptr<GameObject> b = make_shared<Bullet>(bullet);
	// Add the new bullet to the game world
	mWorld->AddObject(b);
}

bool Spaceship::CollisionTest(shared_ptr<GameObject> o)
{
	return false;
}

void Spaceship::OnCollision(const GameObjectList &objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}