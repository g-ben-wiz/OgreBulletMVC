struct PickupCallback: public btCollisionWorld::ContactResultCallback {
    PickupCallback() : btCollisionWorld::ContactResultCallback() { 
        pickup = NULL;
    }

    btRigidBody *pickup;

    bool needsCollision(btBroadphaseProxy* proxy) const {
        if(!btCollisionWorld::ContactResultCallback::needsCollision(proxy))
            return false;

        return pickup->checkCollideWithOverride(static_cast<btCollisionObject*>(proxy->m_clientObject));
    }

    btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colWObj0, int partId0, int index0,
        const btCollisionObjectWrapper* colWObj1, int partId1, int index1) {

        const btCollisionObject* colObj0 = colWObj0->getCollisionObject();
        const btCollisionObject* colObj1 = colWObj1->getCollisionObject();

        Datum* up0 = (Datum*)(colObj0->getUserPointer());
        Datum* up1 = (Datum*)(colObj1->getUserPointer());
        Datum* pickupDatum = (Datum*)pickup->getUserPointer();

        if (!(up0 && up1 && pickupDatum))
            return 0;

        if (up0->id == PLAYER_CHARACTER || up1->id == PLAYER_CHARACTER) {
            pickupDatum->touched = true;
        }

        return 0; 
    }
};


struct GroundCallback: public btCollisionWorld::ContactResultCallback {
    GroundCallback() : btCollisionWorld::ContactResultCallback() { 
        ground = NULL;
    }

    btRigidBody *ground;

    bool needsCollision(btBroadphaseProxy* proxy) const {
        if(!btCollisionWorld::ContactResultCallback::needsCollision(proxy))
            return false;

        return ground->checkCollideWithOverride(static_cast<btCollisionObject*>(proxy->m_clientObject));
    }

    btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colWObj0,int partId0,int index0,
        const btCollisionObjectWrapper* colWObj1,int partId1,int index1) {

        const btCollisionObject* colObj0 = colWObj0->getCollisionObject();
        const btCollisionObject* colObj1 = colWObj1->getCollisionObject();

        Datum* up0 = (Datum*)(colObj0->getUserPointer());
        Datum* up1 = (Datum*)(colObj1->getUserPointer());
        Datum* groundDatum = (Datum*)ground->getUserPointer();

        if (!(up0 && up1 && groundDatum))
            return 0;

        if (up0->id == PLAYER_CHARACTER) {
            up0->touchingGround = true;
        }
        if (up1->id == PLAYER_CHARACTER) {
            up1->touchingGround = true;
        }

        return 0; 
    }
};
