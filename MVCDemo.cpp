#include "Controller.h"

int main(int argc, char** argv) {
    Controller* controller;

    controller = new Controller();

    controller->running = true;
    controller->runMainLoop();

    delete(controller);

    return 0;
}
