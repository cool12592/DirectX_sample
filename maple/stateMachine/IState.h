#pragma once
#include "InputManager.h"
#include "TimeManager.h"

using namespace std;

enum Transition { IDLE=0, WALK, DASH, JUMP, ATTACK,SKILL, MOVE, Hitted , HITTEDTOMOVE, NENGUARD,SKILL4,GROGGY, NENGROGGY ,STAND, FLASH,ROPE, KNEELDOWN};

class Entity;
class IState
{
public:
	IState(){}
	virtual ~IState(){}

public:
	virtual void enter(Entity* entity) = 0;
	virtual void update(Entity* entity) = 0;
	virtual void exit(Entity* entity) = 0;
};