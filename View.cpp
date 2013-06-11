#include "View.h"

View::View() {
}

View::~View() {
    camNode->detachObject(cam);
    delete(camNode); 
}

void View::adjustCamera(Datum* focus) {
    btVector3 fp = focus->physicsBody->getCenterOfMassPosition();
    cam->lookAt(Ogre::Vector3(fp.x(), fp.y() + 2.0, fp.z()));
}

void View::setupVideo(Ogre::Root* root, Ogre::SceneManager* sm) {
    if (root->showConfigDialog())
        this->window = root->initialise(true, Ogre::String("Hello World"));

    sm->setAmbientLight(Ogre::ColourValue(0.66f, 0.66f, 0.66f));
    sm->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);

    cam = sm->createCamera("cam");

    camNode = sm->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(Ogre::Vector3(0, 8.0, -30));
    camNode->attachObject(cam);

    cam->lookAt(Ogre::Vector3(0, 0, 0));
    cam->setNearClipDistance(1);

    Ogre::Viewport* viewport = window->addViewport(cam);
    viewport->setBackgroundColour(Ogre::ColourValue(0.8f, 0.8f, 0.8f));
    Ogre::Real ratio = Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight());
    cam->setAspectRatio(ratio);
}
