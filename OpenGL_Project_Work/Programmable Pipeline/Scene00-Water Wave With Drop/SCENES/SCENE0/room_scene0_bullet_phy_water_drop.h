#include <btBulletDynamicsCommon.h>
#include <time.h>

#define PI 3.1415926535

///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

//keep track of the shapes, we release memory at exit.
//make sure to re-use collision shapes among rigid bodies whenever possible!
btAlignedObjectArray<btCollisionShape*> collisionShapes;

btTransform trans;

void btHelloWorldInc()
{
	int i;
	srand(time(0));
	
	///-----initialization_start-----

	dynamicsWorld->setGravity(btVector3(0.0, -0.5, 0.0));

	///-----initialization_end-----

	///create a few basic rigid bodies

	//float val = 0.0f;
	//float R = 1.0f;
	//float r = 0.0f, theta = 0.0f;
	float xCircle = 0.0f;
	float yCircle = 0.0f;
	float zCircle = 0.0f;
	float centerX = 0.0f;
	float centerY = 0.0f;
	float centerZ = 0.0f;
	float d = 0.0f;
	float xSphere = 0.0f;
	float ySphere = 0.0f;
	float zSphere = 0.0f;

	for(int count = 0; count < 1; count++)
	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(0.05));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(0.000034f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3((xSphere), 1.0 + (ySphere), (zSphere)));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		
		body->setRestitution(0.0);

		dynamicsWorld->addRigidBody(body);

		xCircle = 0.0f;
		yCircle = 0.0f;
		zCircle = 0.0f;
		centerX = 0.0f;
		centerY = 0.0f;
		centerZ = 0.0f;
	}


	xSphere = 0.0f;
	ySphere = 0.0f;
	zSphere = 0.0f;
	for(int count = 0; count < NUMBER_OF_PARTICLES; count++)
	{
		//sphere
		xCircle = centerX + (((float) rand() / (RAND_MAX)) * 2.0 - 1.0);
		yCircle = centerY + (((float) rand() / (RAND_MAX)) * 2.0 - 1.0);
		zCircle = centerZ + (((float) rand() / (RAND_MAX)) * 2.0 - 1.0);
		d = xCircle*xCircle + yCircle*yCircle + zCircle*zCircle;
		if (d < 1.0)
		{
			xSphere = xCircle;
			ySphere = yCircle;
			zSphere = zCircle;
		}
		//fprintf(gpLogFile, "xCircle = %f \t yCircle = %f \t zCircle = %f\n",xCircle, yCircle, zCircle);
		//fprintf(gpLogFile, "xSphere = %f \t ySphere = %f \t zSphere = %f\n",xSphere, ySphere, zSphere);


		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(0.01));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(0.000034f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3((xSphere), 1.5 + (ySphere), (zSphere)));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		
		body->setRestitution(0.1);

		dynamicsWorld->addRigidBody(body);

		//val = 0.0f;
		//r = 0.0f;
		//theta = 0.0f;
		xCircle = 0.0f;
		yCircle = 0.0f;
		zCircle = 0.0f;
		centerX = 0.0f;
		centerY = 0.0f;
		centerZ = 0.0f;
	
	}

	// int maxLimitX = 10;
	// int minLimitX = -10;
	// int maxLimitZ = 5;
	// int minLimitZ = -10;

	// float fRandValX[3000];
	// float fRandValZ[3000];

	// for(int count = 0; count < 3000; count++)
	// {
	// 	//create a dynamic rigidbody

	// 	//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
	// 	btCollisionShape* colShape = new btSphereShape(btScalar(0.2));
	// 	collisionShapes.push_back(colShape);

	// 	/// Create Dynamic Objects
	// 	btTransform startTransform;
	// 	startTransform.setIdentity();

	// 	btScalar mass(0.034f);

	// 	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	// 	bool isDynamic = (mass != 0.f);

	// 	btVector3 localInertia(0, 0, 0);
	// 	if (isDynamic)
	// 		colShape->calculateLocalInertia(mass, localInertia);

	// 	fRandValX[count] = (rand() % (maxLimitX - minLimitX + 1)) + minLimitX;
	// 	fRandValZ[count] = (rand() % (maxLimitZ - minLimitZ + 1)) + minLimitZ;

	// 	//fprintf(gpLogFile, "fRandValX:%f\tfRandValZ:%f\n",fRandValX,fRandValZ);
		
	// 	startTransform.setOrigin(btVector3(fRandValX[count], -9.8, fRandValZ[count]));

	// 	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	// 	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	// 	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	// 	btRigidBody* body = new btRigidBody(rbInfo);
		
	// 	body->setRestitution(0.8);

	// 	dynamicsWorld->addRigidBody(body);
	// }

	//bottom
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1.), btScalar(1.), btScalar(1.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -2.0, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		
		body->setRestitution(1.0);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}
	
	//left
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1.), btScalar(1.), btScalar(1.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(-2, 0, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		
		body->setRestitution(1.0);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}
	
	//right
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1.), btScalar(1.), btScalar(1.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(2, 0, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		
		body->setRestitution(1.0);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}
	
	//back
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1.), btScalar(1.), btScalar(1.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, 0, -2));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		
		body->setRestitution(1.0);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

	//front
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1.), btScalar(1.), btScalar(1.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, 0, 2));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		
		body->setRestitution(1.0);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}
}

void btHelloWorldSim()
{

	/// Do some simulation
	int i;
	///-----stepsimulation_start-----
	for (i = 0; i < NUMBER_OF_SIMULATION_STEPS; i++)
	{
		dynamicsWorld->stepSimulation(1.0f / 60.0f, 1);

		//print positions of all objects
		for (int j = dynamicsWorld->getNumCollisionObjects() - 6; j >= 0; j--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}
			fprintf(gpFile,  "world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
			fflush(gpFile);
			transX[j][i] = float(trans.getOrigin().getX());
			transY[j][i] = float(trans.getOrigin().getY());
			transZ[j][i] = float(trans.getOrigin().getZ());
		}
	}

	///-----stepsimulation_end-----
}

void btHelloWorldUnint()
{
	//cleanup in the reverse order of creation/initialization

	///-----cleanup_start-----

	//remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects() - 6; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}
