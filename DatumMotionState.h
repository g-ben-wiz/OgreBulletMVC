#include <btBulletDynamicsCommon.h>
#define PLAYER_CHARACTER 1

class DatumMotionState : public btMotionState {
//http://bulletphysics.org/mediawiki-1.5.8/index.php/MotionStates
public:
    Ogre::SceneNode *sceneNode;
    btTransform transform;

    DatumMotionState(const btTransform &pos, Ogre::SceneNode *node) {
        this->transform = pos;
        this->sceneNode = node;
    }

    virtual ~DatumMotionState() {
    }

    void setNode(Ogre::SceneNode *sn) {
        sceneNode = sn;
    }

    virtual void getWorldTransform(btTransform &transform) const {
        transform = this->transform;
    }

    virtual void setWorldTransform(const btTransform &worldTrans) {
        if(NULL == sceneNode)
            return; 
        
        this->transform = worldTrans; //this line wasn't in wiki

        btQuaternion rot = worldTrans.getRotation();
        sceneNode->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());

        btVector3 pos = worldTrans.getOrigin();
        sceneNode->setPosition(pos.x(), pos.y(), pos.z());

    }
};
