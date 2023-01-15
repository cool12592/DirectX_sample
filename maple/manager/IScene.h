#pragma once
#include "EntityManager.h"
#include "ObjectPool.h"
class IScene
{
public:
	virtual void initialize() = 0;
	virtual void update() = 0;
	virtual void lateUpdate() = 0;
	virtual void render() = 0;
	virtual void release() = 0;
	virtual void refresh() = 0;

	ObjectPool<Entity>* getObjectPool() { return &sceneObjectPool_; }
protected:
	ObjectPool<Entity> sceneObjectPool_;
	EntityManager* entityManager_ = EntityManager::getInstance();
	
};