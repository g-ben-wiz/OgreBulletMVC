#include "Datum.h"

Datum::Datum() {
}

Datum::Datum(btScalar mass, Ogre::SceneNode* sceneNode, Ogre::Entity* ogreEntity, btCollisionShape* collisionShape, btVector3 location) {
    id = 0;
    touched = false;
    canJump = false;
    touchingGround = false;

    animationState = NULL;
    walkingForward = false;
    walkingBack    = false;
    walkingLeft    = false;
    walkingRight   = false;
    turningLeft    = false;
    turningRight   = false;

    this->mass = mass;
    this->sceneNode = sceneNode;
    this->ogreEntity = ogreEntity;
    this->collisionShape = collisionShape;

    btTransform pos(btQuaternion(0, 0, 0, 1), location);

    this->motionState = new DatumMotionState(pos, this->sceneNode);

    btRigidBody::btRigidBodyConstructionInfo ci (mass, this->motionState, this->collisionShape, location);
    this->physicsBody = new btRigidBody(ci);

    physicsBody->setUserPointer( (Datum*)this);

}

Datum::~Datum() {
    sceneNode->getCreator()->destroyEntity(ogreEntity); 
    sceneNode->getParent()->removeChild(sceneNode);
    sceneNode->getCreator()->destroySceneNode(sceneNode);

    delete motionState;
    delete physicsBody;
    delete collisionShape;
}

void Datum::swat(Datum* target) {
    if (!(this->ogreEntity && target->ogreEntity))
        return;

    btScalar distance;

    distance = physicsBody->getCenterOfMassPosition().distance(target->physicsBody->getCenterOfMassPosition());

    animationState = ogreEntity->getAnimationState("Attack1");

    if (!animationState)
        return;

    animationState->setLoop(false);
    animationState->setEnabled(true);

    if (distance > 10.0) 
        return;

    btVector3 av;
    btScalar force = -20;

    btTransform transform;
    physicsBody->getMotionState()->getWorldTransform(transform);
    av = transform.getBasis().getColumn(2); 

    av.normalize();
    av *= force;

    av += target->physicsBody->getLinearVelocity();

    target->physicsBody->setLinearVelocity(av);
}

void Datum::jump() {
    if (canJump && touchingGround) {
        btVector3 v = physicsBody->getLinearVelocity();
        v += btVector3(0, 10, 0);
        physicsBody->setLinearVelocity(v);
        touchingGround = false;
    }
}

void Datum::turn() {
    btScalar turningSpeed = 2.0;
    physicsBody->setAngularVelocity(btVector3(0, 0, 0)); //this line may interfere with other things 
                                                        // like things that fling the player around

    if (turningLeft)
        physicsBody->setAngularVelocity(btVector3(0, turningSpeed, 0));

    if (turningRight)
        physicsBody->setAngularVelocity(btVector3(0, -turningSpeed, 0));
}

btVector3 Datum::getLocalZ() {
    btTransform transform;
    physicsBody->getMotionState()->getWorldTransform(transform);

    btVector3 localZ = transform.getBasis().getColumn(2); //column 2 is z, 1 is y, 0 is x
    localZ.normalize();

    return localZ;
}

btVector3 Datum::getLocalX() {
    btTransform transform;
    physicsBody->getMotionState()->getWorldTransform(transform);

    btVector3 localX = transform.getBasis().getColumn(0); //column 2 is z, 1 is y, 0 is x
    localX.normalize();

    return localX;
}

void Datum::walk() {
    btScalar speed = 9.0;

    btVector3 v(0, physicsBody->getLinearVelocity().y(), 0);

    btVector3 localZ = this->getLocalZ(); 
    btVector3 localX = this->getLocalX();

    if (walkingForward) {
        v -= localZ * speed;
    }

    if (walkingBack) {
        v += localZ * speed;
    }

    if (walkingLeft) {
        v -= localX * speed;
    }

    if (walkingRight) {
        v += localX * speed;
    }

    physicsBody->setLinearVelocity(v);
}
