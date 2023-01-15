#pragma once
#include "ECS.h"
#include "Buffer.h"
#include "Shader.h"
#include "Transform.h"
#include <functional>
class BoxCollider :
	public Component
{
public:
	BoxCollider(const _vec3& localPos, const _vec3& localRotation, const _vec3& scale);//충돌체 로컬 위치,회전, 크기 세팅
	virtual ~BoxCollider();
	// Component을(를) 통해 상속됨
	virtual void initialize() override;
	virtual void update() override;
	virtual void lateUpdate() override;
	virtual void render() override;
	virtual void release() override;

	COLLIDERRECT getColliderRect();//충돌체 사각형 반환
	COLLIDERRECT2D		getColliderRect2D(); //2차원 모양의 충돌체 반환
	void collide(Entity* colliderEntity);
	void notCollide(Entity* colliderEntity);
	
	//충돌시,충돌중,충돌탈출 때의 로직을 연결.bind(&해당클래스의 함수이름,this,placeholders::_1)의 형태로 넘긴다
	void bindColliderFunction(function<void(Entity*)> enterCollider, function<void(Entity*)> stayCollider, function<void(Entity*)> exitCollider);
	void setActive(bool active) {active_ = active;}
	void setBoxActive(bool active) { active_ = active; }
	void setActive(bool active, Entity* colliderEntity);

	void setRevolution(float angle) { revolution_ = D3DXToRadian(angle); }
private:
	void setShaderValues();
public:
	Transform transform_;
	bool active_ = true;
	//bool colliderOnGround_ = false;
	float lastColliderPos;

private:
	LPDIRECT3DDEVICE9 device_;
	float revolution_ = 0.f;
	function<void(Entity*)> enterCollider_;
	function<void(Entity*)> stayCollider_;
	function<void(Entity*)> exitCollider_;
	vector<Entity*> stayColliderEntity_;
	int collideMode_ = 0;//상황에 따른 버퍼 색깔 결정(0: 보통, 1: enter, 2: stay, 3: exit)
#ifdef _DEBUG
	weak_ptr<Buffer> debugBuffer_; //충돌체 출력용 버퍼
	weak_ptr<Shader> debugShader_; //충돌체 출력용 셰이더
#endif
};

