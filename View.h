#include "Datum.h"

class View {
public:
    View();
    ~View();

    void adjustCamera(Datum* focus);
    void setupVideo(Ogre::Root* root, Ogre::SceneManager* sm);

    Ogre::RenderWindow* window;
    Ogre::Camera* cam;
    Ogre::SceneNode *camNode;
};
