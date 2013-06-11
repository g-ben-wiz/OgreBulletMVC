#include <OgreCamera.h>
#include <OgreConfigFile.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreViewport.h>
#include <OgreWindowEventUtilities.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>

#include "DatumMotionState.h"

class Datum {
public:
    Datum();
    Datum(btScalar mass, Ogre::SceneNode* sceneNode, Ogre::Entity* ogreEntity, btCollisionShape* collisionShape, btVector3 location);
    ~Datum();

    int id;
    bool touched;
    bool canJump;
    bool touchingGround;
    bool turningLeft;
    bool turningRight;
    Ogre::AnimationState* animationState;

    void swat(Datum* target);
    void jump();
    void turn();
    void walk();

    bool walkingForward;
    bool walkingBack;
    bool walkingLeft;
    bool walkingRight;

    btVector3 getLocalZ();
    btVector3 getLocalX();

    Ogre::SceneNode* sceneNode;
    Ogre::Entity* ogreEntity;
    btRigidBody* physicsBody;

    btCollisionShape* collisionShape;
    DatumMotionState* motionState;

    btScalar mass;
};

