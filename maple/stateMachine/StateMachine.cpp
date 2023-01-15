#include "stdafx.h"
#include "StateMachine.h"
#include "IState.h"

StateMachine::~StateMachine()
{
	release();
}

void StateMachine::initialize()
{
}

void StateMachine::update()
{
#ifdef _DEBUG
	auto& find_iter = mapStates_.find(currentStateName);
	ASSERT_MESSAGE(mapStates_.end() != find_iter, "current state does not set");
#endif
	mapStates_[currentStateName]->update(entity);
}

void StateMachine::lateUpdate()
{
}

void StateMachine::render()
{
}

void StateMachine::release()
{
}

HRESULT StateMachine::insertState(wstring stateName, IState* state)
{
#ifdef _DEBUG
	if (!mapStates_.empty())
	{
		auto& find_iter = mapStates_.find(stateName);
		ASSERT_MESSAGE_RETURN(mapStates_.end() == find_iter, "state name already exist", E_FAIL);
	}
#endif
	if (mapStates_.empty())
	{
		mapStates_.emplace(stateName, state);
		currentStateName = stateName;
	}
	else
		mapStates_.emplace(stateName, state);

	return S_OK;
}

HRESULT StateMachine::insertTransitionCondition(wstring firstStateName, int transitionCondition, wstring secondStateName)
{
#ifdef _DEBUG
	auto& find_iter = find_if(vecTransitions_.begin(), vecTransitions_.end(), [&](const tuple<wstring, int, wstring>& transition) {
		if (get<0>(transition) == firstStateName&&get<1>(transition) == transitionCondition)
			return true;
		return false;
	});
	ASSERT_MESSAGE_RETURN(vecTransitions_.end() == find_iter, "transition data is overlapped", E_FAIL);
#endif
	vecTransitions_.emplace_back(make_tuple(firstStateName, transitionCondition, secondStateName));

	return S_OK;
}

HRESULT StateMachine::transitionState(int transitionCondition)
{
	auto& find_iter = find_if(vecTransitions_.begin(), vecTransitions_.end(), [&](const tuple<wstring, int, wstring>& transition) {
		if (get<0>(transition) == currentStateName&&get<1>(transition) == transitionCondition)
			return true;
		return false;
	});

	if (vecTransitions_.end() == find_iter)
		return E_FAIL;
	if (FAILED(changeState(get<2>(*find_iter))))
		return E_FAIL;
	return S_OK;
}

HRESULT StateMachine::changeState(wstring stateName)
{
#ifdef _DEBUG
	auto& find_iter = mapStates_.find(stateName);
	ASSERT_MESSAGE_RETURN(mapStates_.end() != find_iter, "state does not exist", E_FAIL);
#endif
	mapStates_[currentStateName]->exit(entity);
	currentStateName = stateName;
	mapStates_[currentStateName]->enter(entity);
	return S_OK;
}
