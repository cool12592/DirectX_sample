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
	BoxCollider(const _vec3& localPos, const _vec3& localRotation, const _vec3& scale);//�浹ü ���� ��ġ,ȸ��, ũ�� ����
	virtual ~BoxCollider();
	// Component��(��) ���� ��ӵ�
	virtual void initialize() override;
	virtual void update() override;
	virtual void lateUpdate() override;
	virtual void render() override;
	virtual void release() override;

	COLLIDERRECT getColliderRect();//�浹ü �簢�� ��ȯ
	COLLIDERRECT2D		getColliderRect2D(); //2���� ����� �浹ü ��ȯ
	void collide(Entity* colliderEntity);
	void notCollide(Entity* colliderEntity);
	
	//�浹��,�浹��,�浹Ż�� ���� ������ ����.bind(&�ش�Ŭ������ �Լ��̸�,this,placeholders::_1)�� ���·� �ѱ��
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
	int collideMode_ = 0;//��Ȳ�� ���� ���� ���� ����(0: ����, 1: enter, 2: stay, 3: exit)
#ifdef _DEBUG
	weak_ptr<Buffer> debugBuffer_; //�浹ü ��¿� ����
	weak_ptr<Shader> debugShader_; //�浹ü ��¿� ���̴�
#endif
};

