#pragma once
#include "ECS.h"
#include "BoxCollider.h"
class CollisionDetection
{
public:

	//aabb 충돌방식

	static void BoxColliderDetection(Entity* firstEntity, Entity* secondEntity)
	{
		if (!firstEntity->hasComponent<BoxCollider>() || !secondEntity->hasComponent<BoxCollider>() 
			|| !firstEntity->getComponent<BoxCollider>().active_ || !secondEntity->getComponent<BoxCollider>().active_)
			return;

		BoxCollider& firstCollider = firstEntity->getComponent<BoxCollider>();
		BoxCollider& secondCollider = secondEntity->getComponent<BoxCollider>();

		//충돌 체크 및 처리
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

	//obb 충돌 방식

	D3DXVECTOR2 len2B;

	static bool OBBColliderDetection(Entity* firstEntity, Entity* secondEntity)
	{
		if (!firstEntity->hasComponent<BoxCollider>() || !secondEntity->hasComponent<BoxCollider>()
			|| !firstEntity->getComponent<BoxCollider>().active_ || !secondEntity->getComponent<BoxCollider>().active_)
			return false;

		BoxCollider& firstCollider = firstEntity->getComponent<BoxCollider>();
		BoxCollider& secondCollider = secondEntity->getComponent<BoxCollider>();

		//충돌 체크 및 처리
		COLLIDERRECT2D firstColRect, secondColRect;
		firstColRect = firstCollider.getColliderRect2D();
		secondColRect = secondCollider.getColliderRect2D();

		//각도
		float radianA = (firstCollider.transform_.rotation_.z);
		float radianB = (secondCollider.transform_.rotation_.z);
		//사각형들간 거리
		D3DXVECTOR2 distance = D3DXVECTOR2((firstEntity->getComponent<Transform>().position_.x - secondEntity->getComponent<Transform>().position_.x), (firstEntity->getComponent<Transform>().position_.y - secondEntity->getComponent<Transform>().position_.y));

		//방향벡터
		D3DXVECTOR2 dir1A = D3DXVECTOR2(cosf(radianA), sinf(radianA)); //A스프라이트X축
		D3DXVECTOR2 dir2A = D3DXVECTOR2(cosf(radianA - D3DXToRadian(180.f)*0.5f), sinf(radianA - D3DXToRadian(180.f)*0.5f));//x축에서 90도깍아서 y축만듬(참고로 라디안180은파이)

		D3DXVECTOR2 dir1B = D3DXVECTOR2(cosf(radianB), sinf(radianB)); //B 스프라이트X축
		D3DXVECTOR2 dir2B = D3DXVECTOR2(cosf(radianB - D3DXToRadian(180.f)*0.5f), sinf(radianB - D3DXToRadian(180.f)*0.5f));//x축에서 90도깍아서 y축만듬(참고로 라디안180은파이)

		//방향과 길이를 곱해줌
		
		D3DXVECTOR2 len1A = dir1A * (firstColRect.right - firstColRect.left)*0.5f;
		D3DXVECTOR2 len2A = dir2A * (firstColRect.top - firstColRect.bottom)*0.5F;
		
		D3DXVECTOR2 len1B = dir1B * (secondColRect.right - secondColRect.left)*0.5f;
		D3DXVECTOR2 len2B = dir2B * (secondColRect.top - secondColRect.bottom)*0.5F;

		float lenthA;
		float lenthB;
		float lenth;
		//이제부터 검사시작
		//dir1A축검사
		//전부 dir1A에 사영함 (같은각도를만든다고봐야하나)
		//그리고길이비교
		lenthA = D3DXVec2Length(&len1A); //dir1A와 평행하기 때문에 길이만 알면됨
		lenthB = fabsf(D3DXVec2Dot(&dir1A, &len1B)) + fabs(D3DXVec2Dot(&dir1A, &len2B)); // B스프라이트를 dir1A 축에 사영하여 길이를구함(내적)
		lenth = fabsf(D3DXVec2Dot(&dir1A, &distance)); //사영한 길이를 비교해 겹치면 lenth보다 작고, 겹치지않으면 length보다 길어짐으로 판단
		if (lenth > lenthA + lenthB)
		{

			return false;
		}
		
		//dir2A축검사
		lenthA = D3DXVec2Length(&len2A); 
		lenthB = fabsf(D3DXVec2Dot(&dir2A, &len1B)) + fabs(D3DXVec2Dot(&dir2A, &len2B)); 
		lenth = fabsf(D3DXVec2Dot(&dir2A, &distance)); 
		if (lenth > lenthA + lenthB)
		{
	
			return false;
		}

		//dir1B축검사
		lenthA = D3DXVec2Length(&len1B);
		lenthB = fabsf(D3DXVec2Dot(&dir1B, &len1A)) + fabs(D3DXVec2Dot(&dir1B, &len2A));
		lenth = fabsf(D3DXVec2Dot(&dir1B, &distance));
		if (lenth > lenthA + lenthB)
		{
	
			return false;
		}

		//dir2B축검사
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