#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>

#include "physics.h"

#ifdef _WIN32
  // mute conversion from 'float' to 'Type', possible loss of data
  #pragma warning(disable : 4244)
#endif

namespace {
    static btDiscreteDynamicsWorld* dynamics_world;
    static btDefaultCollisionConfiguration* config;
    static btCollisionDispatcher* dispatcher;
    static btBroadphaseInterface* broadphase;
    static btSequentialImpulseConstraintSolver* solver;
}

void physics_init(void)
{
    config = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(config);
    broadphase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();

    dynamics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
    dynamics_world->setGravity({ 0, -10, 0 });
}

void physics_shutdown(void)
{
    delete config;
    delete dispatcher;
    delete broadphase;
    delete solver;
    delete dynamics_world;
}

void physics_update(double delta)
{
    dynamics_world->stepSimulation(delta);
}
