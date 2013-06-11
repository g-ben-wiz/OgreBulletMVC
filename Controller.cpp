#include "Controller.h"

Controller::Controller() {
    this->running = 0;

    Ogre::String pluginsFilename("plugins.cfg");    
    Ogre::String configFilename("ogre.cfg");

    pickup = NULL;
    playerCharacter = NULL;
    ground = NULL;
    statue = NULL;

    this->ogreRoot = new Ogre::Root(pluginsFilename, configFilename);

    sceneManager = ogreRoot->createSceneManager(Ogre::ST_GENERIC, "sceneManager");

    this->view = new View();
    view->setupVideo(ogreRoot, sceneManager);
    setupInput();
    loadResources();

    Ogre::WindowEventUtilities::addWindowEventListener(view->window, this);

    setupPhysics();

    createData();
}

Controller::~Controller() {
    destroyData();
    Ogre::WindowEventUtilities::removeWindowEventListener(view->window, this);

    deleteInput();
    deletePhysics();
    
    delete(this->ogreRoot);
}

void Controller::loadResources() {
    int i;
    Ogre::String resourcesFilename = "resources.cfg";

    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load(resourcesFilename);
 
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void Controller::createData() {
    btVector3 location;
    Ogre::SceneNode* sn;
    Ogre::Entity*    en;

    location = btVector3(0, 0, 0);
    sn = sceneManager->getRootSceneNode()->createChildSceneNode();

    Ogre::Real width = 1500;
    Ogre::Real height = 1500;
    int xSegments = 20;
    int ySegments = 20;
    bool perpendicularNormals = true;
    unsigned short coordSets = 1;
    Ogre::Real uTiles = 5.0;
    Ogre::Real vTiles = 5.0;

    //plane's "up" vector (its v axis) has to be perpendicular to Ogre::Plane instance's normal vector
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,  Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), width, height, xSegments, ySegments, perpendicularNormals, coordSets, uTiles, vTiles, Ogre::Vector3::UNIT_Z);

    en = sceneManager->createEntity("groundInstance", "ground");
    en->setMaterialName("Examples/Rocky");
    en->setCastShadows(false);
    sn->attachObject(en);

    ground = new Datum( 0.0, sn, en, new btStaticPlaneShape(btVector3(0, 1, 0), 0), location );

    //ninja
    location = btVector3(0, 5, 0);
    sn = sceneManager->getRootSceneNode()->createChildSceneNode();
    en = sceneManager->createEntity("ninja", "ninja.mesh");
    sn->attachObject(en);
    sn->setScale(0.04, 0.04, 0.04);

    playerCharacter = new Datum( 1.0, sn, en, new btCapsuleShape(1.0, 1.8), location);
    playerCharacter->ogreEntity->setCastShadows(true);
    playerCharacter->physicsBody->setSleepingThresholds(0.0, 0.0);
    playerCharacter->physicsBody->setAngularFactor(0.0);

    playerCharacter->id = PLAYER_CHARACTER;

    //statue
    location = btVector3(10, 15, 0);
    sn = sceneManager->getRootSceneNode()->createChildSceneNode();
    en = sceneManager->createEntity("statue", "athene.mesh");
    sn->attachObject(en);
    sn->setScale(0.065, 0.065, 0.065);

    statue= new Datum( 1.0, sn, en, new btCylinderShape(btVector3(1, 5, 1)), location);
    statue->ogreEntity->setMaterialName("Examples/Chrome");

    //pickup
    location = btVector3(-10, 15, 0);
    sn = sceneManager->getRootSceneNode()->createChildSceneNode();
    en = sceneManager->createEntity("pickup", "sphere.mesh");
    sn->attachObject(en);
    sn->setScale(0.04, 0.04, 0.04);

    pickup = new Datum( 1.0, sn, en, new btSphereShape(3), location);

    physicsWorld->addRigidBody(ground->physicsBody);
    physicsWorld->addRigidBody(playerCharacter->physicsBody);
    physicsWorld->addRigidBody(statue->physicsBody);
    physicsWorld->addRigidBody(pickup->physicsBody);

    //not doing lights & fog
}

void Controller::destroyData() {
    int i;
    for (i = physicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        physicsWorld->removeCollisionObject( physicsWorld->getCollisionObjectArray()[i] );
    }   

    if (pickup) {
        delete(pickup);
    }

    delete(statue);
    delete(playerCharacter);
    delete(ground);
}

void Controller::setupInput() {
    unsigned long hWnd;
    view->window->getCustomAttribute("WINDOW", &hWnd);

    OIS::ParamList pl;
    pl.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));
 
    inputManager = OIS::InputManager::createInputSystem( pl );
    keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, true));

    keyboard->setEventCallback(this);
}

void Controller::deleteInput() {
    this->inputManager->destroyInputObject(this->keyboard);
    OIS::InputManager::destroyInputSystem(inputManager);
    inputManager = 0;
}

void Controller::setupPhysics() {
    this->clock = new btClock();

    this->broadphase    = new btDbvtBroadphase();
    this->collisionConf = new btDefaultCollisionConfiguration();
    this->dispatcher    = new btCollisionDispatcher(collisionConf);
    this->solver        = new btSequentialImpulseConstraintSolver();

    this->physicsWorld = new btDiscreteDynamicsWorld(
        dispatcher,
        broadphase,
        solver,
        collisionConf
     );

    physicsWorld->setGravity(btVector3(0, -9.8, 0));
}

void Controller::deletePhysics() {
    delete(physicsWorld);
    delete(solver);
    delete(dispatcher);
    delete(collisionConf);
    delete(broadphase);
    delete(clock);
}


bool Controller::keyPressed(const OIS::KeyEvent& e) {
/*
 left/right arrow keys edit heading
 wsad keys walk, backpedal, and strafe
 space jump
 enter swat
 esc exits
*/
    switch(e.key) {
        case OIS::KC_ESCAPE:
            this->running = false;
        break;

        case OIS::KC_W:
            playerCharacter->walkingForward = true;
        break;

        case OIS::KC_S:
            playerCharacter->walkingBack = true;
        break;

        case OIS::KC_A:
            playerCharacter->walkingLeft = true;
        break;

        case OIS::KC_D:
            playerCharacter->walkingRight = true;
        break;

        case OIS::KC_LEFT:
            playerCharacter->turningLeft = true;
        break;

        case OIS::KC_RIGHT:
            playerCharacter->turningRight = true;
        break;

        case OIS::KC_RETURN:
            playerCharacter->swat(statue);
        break;

        case OIS::KC_SPACE:
            playerCharacter->jump();
        break;

        default:
        break;
    }

    return true;
}

bool Controller::keyReleased(const OIS::KeyEvent& e) {
    switch (e.key) {
        case OIS::KC_W:
            playerCharacter->walkingForward = false;
        break;

        case OIS::KC_S:
            playerCharacter->walkingBack = false;
        break;

        case OIS::KC_A:
            playerCharacter->walkingLeft = false;
        break;

        case OIS::KC_D:
            playerCharacter->walkingRight = false;
        break;

        case OIS::KC_LEFT:
            playerCharacter->turningLeft = false;
        break;

        case OIS::KC_RIGHT:
            playerCharacter->turningRight = false;
        break;

        case OIS::KC_RETURN:
        break;

        case OIS::KC_SPACE:
        break;

        default:
        break;
    }


    return true;
}

void Controller::runMainLoop() {
    PickupCallback pickupCB;
    if (pickup) {
        pickupCB.pickup = pickup->physicsBody;
    }
    else {
        pickupCB.pickup = NULL;
    }

    GroundCallback groundCB; 
    if (ground) {
        groundCB.ground = ground->physicsBody;
    }
    else {
        groundCB.ground = NULL;
    }

    long long t = 0;
    long long dt = 16;

    long long accumulator = 0;

    long long stateTime = 0;

    double blendingFactor = 0.0;

    long long previousTime = clock->getTimeMilliseconds();
    long long currentTime;
    long long frameTime;

    while ( this->running )
    {
        running = !(view->window->isClosed()); 
        this->keyboard->capture();

        if (!running) {
            return;
        }

        currentTime = clock->getTimeMilliseconds();

        frameTime   = currentTime - previousTime;
        previousTime = clock->getTimeMilliseconds();

        if (frameTime > 32) {
            frameTime = 32;
        }

        accumulator += frameTime;

        playerCharacter->turn();
        playerCharacter->walk();
        playerCharacter->touchingGround = false;

        if (pickup) {
            physicsWorld->contactTest(pickup->physicsBody, pickupCB);
            if (pickup->touched) {
                playerCharacter->canJump = true;

                physicsWorld->removeCollisionObject(pickup->physicsBody);
                delete(pickup);
                pickup = NULL;
            }
        }
        
        if (ground) {
            physicsWorld->contactTest(ground->physicsBody, groundCB);
        }

        while ( accumulator >= dt )
        {
            this->physicsWorld->stepSimulation((float)(accumulator / 1000.0), 16);

            currentTime += dt;
            accumulator -= dt;
        }

        blendingFactor = accumulator / dt;

        //stateTime = currentTime * blendingFactor + previousTime * ( 1.0 - blendingFactor );
        if (playerCharacter->animationState) {
            playerCharacter->animationState->addTime( dt / 1000.0 ); 

            if(playerCharacter->animationState->getLength() == playerCharacter->animationState->getTimePosition()) 
            { 
                playerCharacter->animationState->setTimePosition(0); 
                playerCharacter->animationState->setEnabled(false); 
            }
        }

        if (view->cam && playerCharacter) {
            //view->adjustCamera(playerCharacter);
        }

        view->window->setActive(true);
        ogreRoot->renderOneFrame();
        Ogre::WindowEventUtilities::messagePump();

    }

}

void Controller::checkTouched() {
}
