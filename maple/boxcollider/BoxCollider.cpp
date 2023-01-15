#include "stdafx.h"
#include "BoxCollider.h"
#include "DeviceManager.h"
BoxCollider::BoxCollider(const _vec3& localPos, const _vec3& localRotation, const _vec3& scale):device_(DeviceManager::getInstance()->getDevice())
{
	transform_.position_ = localPos;
	transform_.rotation_ = localRotation;
	transform_.scale_ = scale;
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::initialize()
{
#ifdef _DEBUG
	debugBuffer_ = dynamic_pointer_cast<Buffer>(DeviceManager::getInstance()->getResources(L"DebugBuffer"));
	debugShader_ = dynamic_pointer_cast<Shader>(DeviceManager::getInstance()->getResources(L"DebugShader"));

	ASSERT_MESSAGE(!debugBuffer_.expired(), "buffer does not exist");
	ASSERT_MESSAGE(!debugShader_.expired(), "shader does not exist");
#endif

	enterCollider_ = [](Entity* entity){};
	stayCollider_ = [](Entity* entity) {};
	exitCollider_ = [](Entity* entity) {};
}

void BoxCollider::update()
{
	collideMode_ = 0;
}

void BoxCollider::lateUpdate()
{
}

void BoxCollider::render()
{
}

void BoxCollider::release()
{
}

COLLIDERRECT BoxCollider::getColliderRect()
{
#ifdef _DEBUG
	ASSERT_MESSAGE(entity->hasComponent<Transform>(), "entity must have transform component");
#endif
	Transform& entityTransform = entity->getComponent<Transform>();

	COLLIDERRECT rect;
	_vec3 worldPos = entityTransform.position_ + transform_.position_; //현재 충돌체 위치 = 부모의 위치 + 로컬 위치
	rect.minPosition = worldPos - transform_.scale_ * 0.5f;
	rect.maxPosition = worldPos + transform_.scale_ * 0.5f;

	return rect;
}

COLLIDERRECT2D BoxCollider::getColliderRect2D()
{
#ifdef _DEBUG
	ASSERT_MESSAGE(entity->hasComponent<Transform>(), "entity must have transform component");
#endif
	Transform& entityTransform = entity->getComponent<Transform>();

	COLLIDERRECT2D rect;
	_vec3 worldPos = entityTransform.position_ + transform_.position_; //현재 충돌체 위치 = 부모의 위치 + 로컬 위치
	_vec2 secondDimensionPos = { worldPos.x,worldPos.y + worldPos.z };
	rect.left = secondDimensionPos.x - transform_.scale_.x * 0.5f;
	rect.right = secondDimensionPos.x + transform_.scale_.x * 0.5f;
	rect.top = secondDimensionPos.y + transform_.scale_.y * 0.5f;
	rect.bottom = secondDimensionPos.y - transform_.scale_.y * 0.5f;


	return rect;
}

void BoxCollider::collide(Entity* colliderEntity)
{
	auto& find_iter = find(stayColliderEntity_.begin(), stayColliderEntity_.end(), colliderEntity);
	if (stayColliderEntity_.end() == find_iter) //처음 충돌
	{
		enterCollider_(colliderEntity);
		stayColliderEntity_.emplace_back(colliderEntity);
		collideMode_ = 1;
	}
	else
	{
		stayCollider_(colliderEntity);
		collideMode_ = 2;
	}


}

void BoxCollider::notCollide(Entity* colliderEntity)
{
	auto& find_iter = find(stayColliderEntity_.begin(), stayColliderEntity_.end(), colliderEntity);
	if (stayColliderEntity_.end() != find_iter)
	{
		stayColliderEntity_.erase(find_iter);
		exitCollider_(colliderEntity);
		collideMode_ = 3;
	}
}

void BoxCollider::bindColliderFunction(function<void(Entity*)> enterCollider, function<void(Entity*)> stayCollider, function<void(Entity*)> exitCollider)
{
	enterCollider_ = enterCollider;
	stayCollider_ = stayCollider;
	exitCollider_ = exitCollider;
}

void BoxCollider::setActive(bool active, Entity* colliderEntity)
{
	
	 if (active == false)
	 {
		 auto& find_iter = find(stayColliderEntity_.begin(), stayColliderEntity_.end(), colliderEntity);
		 if (find_iter == stayColliderEntity_.end()) return;
			 stayColliderEntity_.erase(find_iter);
			 entity->getComponent<BoxCollider>().transform_.position_ = _vec3(-2000.f, -2000.f, -2000.f);
	 }
	 active_ = active;
}

void BoxCollider::setShaderValues()
{
#ifdef _DEBUG
	ASSERT_MESSAGE_NULLRETURN(entity->hasComponent<Transform>(), "entity must have transform component");
	ASSERT_MESSAGE_NULLRETURN(!debugShader_.expired(), "debug shader not set");

	shared_ptr<Shader>& shader = debugShader_.lock();
	Transform& entityTransform = entity->getComponent<Transform>();
	_mat matWorld,matView,matProjection;
	_mat parentWorld;
	_mat matRevolution;
	D3DXMatrixRotationZ(&matRevolution, revolution_);
	D3DXMatrixIdentity(&parentWorld);
	memcpy(parentWorld.m[3], entityTransform.worldMatrix_.m[3], sizeof(_vec3));

	parentWorld.m[3][1] += parentWorld.m[3][2];
	parentWorld.m[3][2] = 0.f;

	matWorld = calculateWorldmatrix(transform_.position_, transform_.rotation_, transform_.scale_) * matRevolution * parentWorld;
	device_->GetTransform(D3DTS_VIEW, &matView);
	device_->GetTransform(D3DTS_PROJECTION, &matProjection);

	shader->setValues(make_pair("worldMatrix", matWorld), make_pair("viewMatrix", matView),
		make_pair("projectionMatrix", matProjection), make_pair("shaderMode", collideMode_));
#endif
}
