#pragma once
#include "ECS.h"
#include "Define.h"

#include "ComponentsHeader.h"
#include "MonoBehaviourHeader.h"
#include "ObjectPool.h"
class EntityManager
{
	DECLARE_SINGLETON(EntityManager)
public:
	explicit EntityManager();
	~EntityManager();
public:
	void update();
	void lateUpdate();
	void render();
	void releaseAll(ObjectPool<Entity>* entityPool = nullptr); //entities_ ���� ���. entityPool�� ��ȯ
	void releaseEntity(Entity::ENTITYTYPE entityType, ObjectPool<Entity>* entityPool = nullptr);
	void refresh(ObjectPool<Entity>* entityPool = nullptr); //��Ȱ�� entity�鸸 ���. entityPool�� ��ȯ

	HRESULT insertEntity(shared_ptr<Entity>& insertEntity); //��ƼƼ ����
	Entity* findEntity(int userIDN, int entityNumber, Entity::ENTITYTYPE entityType);
	shared_ptr<Entity> findEntitySharedPtr(int userIDN, int entityNumber, Entity::ENTITYTYPE entityType);
	shared_ptr<Entity> findEntityWithPointer(Entity* findEntity);
	void eraseEntity(int userIDN, int entityNumber, Entity::ENTITYTYPE entityType);

	unordered_map<Entity::ENTITYTYPE, vector<shared_ptr<Entity>>>* getEntitiesPtr() { return &entities_; }
	void getColliderEntities(vector<shared_ptr<Entity>>& entities);
	vector<shared_ptr<Entity>>* getEntityList(Entity::ENTITYTYPE entityType) { return &entities_[entityType]; }
private:
	unordered_map<Entity::ENTITYTYPE,vector<shared_ptr<Entity>>> entities_; //key : �±�, value : ��ƼƼ ����Ʈ
	vector<shared_ptr<Entity>> entitiesForRenderSorting_;
	vector<shared_ptr<Entity>> entitiesForCollision_; //�浹 ������Ʈ ���� ��ƼƼ�� ����
	vector<pair<Entity::ENTITYTYPE,shared_ptr<Entity>>> entitiesInserted_;

	mutex entityLock_;
	mutex releaseLock_;
};

