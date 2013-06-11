#include "View.h"
#include "CollisionCallbacks.h"

class Controller: public Ogre::WindowEventListener, public OIS::KeyListener {
public:
    Controller();
    ~Controller();

    void loadResources();
    void createData();
    void destroyData();

    bool interpretInput();
    void runMainLoop();

    void setupInput();
    void deleteInput();
    bool keyPressed(const OIS::KeyEvent&);
    bool keyReleased(const OIS::KeyEvent&);

    void setupPhysics();
    void deletePhysics();
    void checkTouched();

    bool running;

    Ogre::Root* ogreRoot;
    Ogre::SceneManager* sceneManager;

    OIS::InputManager* inputManager;
    OIS::Keyboard* keyboard;

    btDiscreteDynamicsWorld*             physicsWorld;
    btBroadphaseInterface*               broadphase;
    btDefaultCollisionConfiguration*     collisionConf;
    btCollisionDispatcher*               dispatcher;
    btSequentialImpulseConstraintSolver* solver;

    btClock* clock;

    Datum* playerCharacter;
    Datum* pickup;
    Datum* ground;
    Datum* statue;

    View* view;
};
