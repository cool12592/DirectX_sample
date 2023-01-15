#pragma once
#include "ECS.h"
#include "BoxCollider.h"
class CollisionDetection
{
public:

	//aabb �浹���

	static void BoxColliderDetection(Entity* firstEntity, Entity* secondEntity)
	{
		if (!firstEntity->hasComponent<BoxCollider>() || !secondEntity->hasComponent<BoxCollider>() 
			|| !firstEntity->getComponent<BoxCollider>().active_ || !secondEntity->getComponent<BoxCollider>().active_)
			return;

		BoxCollider& firstCollider = firstEntity->getComponent<BoxCollider>();
		BoxCollider& secondCollider = secondEntity->getComponent<BoxCollider>();

		//�浹 üũ �� ó��
		COLLIDERRECT2D firstColRect, secondColRect;
		firstColRect = firstCollider.getColliderRect2D();
		secondColRect = secondCollider.getColliderRect2D();



		if (firstColRect.right<secondColRect.left || firstColRect.left>secondColRect.right||
			firstColRect.top<secondColRect.bottom || firstColRect.bottom>secondColRect.top )	
		{
			firstCollider.notCollide(secondEntity);
			secondCollider.notCollide(firstEntity);
		}
		else
		{
			firstCollider.collide(secondEntity);
			secondCollider.collide(firstEntity);
		}

	}

	//obb �浹 ���

	D3DXVECTOR2 len2B;

	static bool OBBColliderDetection(Entity* firstEntity, Entity* secondEntity)
	{
		if (!firstEntity->hasComponent<BoxCollider>() || !secondEntity->hasComponent<BoxCollider>()
			|| !firstEntity->getComponent<BoxCollider>().active_ || !secondEntity->getComponent<BoxCollider>().active_)
			return false;

		BoxCollider& firstCollider = firstEntity->getComponent<BoxCollider>();
		BoxCollider& secondCollider = secondEntity->getComponent<BoxCollider>();

		//�浹 üũ �� ó��
		COLLIDERRECT2D firstColRect, secondColRect;
		firstColRect = firstCollider.getColliderRect2D();
		secondColRect = secondCollider.getColliderRect2D();

		//����
		float radianA = (firstCollider.transform_.rotation_.z);
		float radianB = (secondCollider.transform_.rotation_.z);
		//�簢���鰣 �Ÿ�
		D3DXVECTOR2 distance = D3DXVECTOR2((firstEntity->getComponent<Transform>().position_.x - secondEntity->getComponent<Transform>().position_.x), (firstEntity->getComponent<Transform>().position_.y - secondEntity->getComponent<Transform>().position_.y));

		//���⺤��
		D3DXVECTOR2 dir1A = D3DXVECTOR2(cosf(radianA), sinf(radianA)); //A��������ƮX��
		D3DXVECTOR2 dir2A = D3DXVECTOR2(cosf(radianA - D3DXToRadian(180.f)*0.5f), sinf(radianA - D3DXToRadian(180.f)*0.5f));//x�࿡�� 90����Ƽ� y�ุ��(����� ����180������)

		D3DXVECTOR2 dir1B = D3DXVECTOR2(cosf(radianB), sinf(radianB)); //B ��������ƮX��
		D3DXVECTOR2 dir2B = D3DXVECTOR2(cosf(radianB - D3DXToRadian(180.f)*0.5f), sinf(radianB - D3DXToRadian(180.f)*0.5f));//x�࿡�� 90����Ƽ� y�ุ��(����� ����180������)

		//����� ���̸� ������
		
		D3DXVECTOR2 len1A = dir1A * (firstColRect.right - firstColRect.left)*0.5f;
		D3DXVECTOR2 len2A = dir2A * (firstColRect.top - firstColRect.bottom)*0.5F;
		
		D3DXVECTOR2 len1B = dir1B * (secondColRect.right - secondColRect.left)*0.5f;
		D3DXVECTOR2 len2B = dir2B * (secondColRect.top - secondColRect.bottom)*0.5F;

		float lenthA;
		float lenthB;
		float lenth;
		//�������� �˻����
		//dir1A��˻�
		//���� dir1A�� �翵�� (��������������ٰ�����ϳ�)
		//�׸�����̺�
		lenthA = D3DXVec2Length(&len1A); //dir1A�� �����ϱ� ������ ���̸� �˸��
		lenthB = fabsf(D3DXVec2Dot(&dir1A, &len1B)) + fabs(D3DXVec2Dot(&dir1A, &len2B)); // B��������Ʈ�� dir1A �࿡ �翵�Ͽ� ���̸�����(����)
		lenth = fabsf(D3DXVec2Dot(&dir1A, &distance)); //�翵�� ���̸� ���� ��ġ�� lenth���� �۰�, ��ġ�������� length���� ��������� �Ǵ�
		if (lenth > lenthA + lenthB)
		{

			return false;
		}
		
		//dir2A��˻�
		lenthA = D3DXVec2Length(&len2A); 
		lenthB = fabsf(D3DXVec2Dot(&dir2A, &len1B)) + fabs(D3DXVec2Dot(&dir2A, &len2B)); 
		lenth = fabsf(D3DXVec2Dot(&dir2A, &distance)); 
		if (lenth > lenthA + lenthB)
		{
	
			return false;
		}

		//dir1B��˻�
		lenthA = D3DXVec2Length(&len1B);
		lenthB = fabsf(D3DXVec2Dot(&dir1B, &len1A)) + fabs(D3DXVec2Dot(&dir1B, &len2A));
		lenth = fabsf(D3DXVec2Dot(&dir1B, &distance));
		if (lenth > lenthA + lenthB)
		{
	
			return false;
		}

		//dir2B��˻�
		lenthA = D3DXVec2Length(&len2B);
		lenthB = fabsf(D3DXVec2Dot(&dir2B, &len1A)) + fabs(D3DXVec2Dot(&dir2B, &len2A));
		lenth = fabsf(D3DXVec2Dot(&dir2B, &distance));
		if (lenth > lenthA + lenthB)
		{
		
			
			return false;
		}

	
		return true;

	}
};