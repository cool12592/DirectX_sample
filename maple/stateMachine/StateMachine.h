#pragma once
#include "ECS.h"

class IState;

class StateMachine :
	public Component
{
public:
	StateMachine() {}
	//make_pair("상태이름",&전역상태클래스) 형태로 넣기
	template<typename PairNameState,typename... PairNameStates>
	StateMachine(PairNameState pairState,PairNameStates... pairStates):StateMachine(pairStates...) 
		//맴버초기화 리스트에서 또 생성자를 호출함 //첫째인자와 나머지인자를 분리
		//초기화리스트에 나머지인자 넣음
	{
		auto& find_iter = mapStates_.find(pairState.first);
		ASSERT_MESSAGE(mapStates_.end() == find_iter, "state name already exist");
		
		if (mapStates_.empty())
		{
			mapStates_.emplace(pairState.first, pairState.second);
			currentStateName = pairState.first;
		}
		else
			mapStates_.emplace(pairState.first, pairState.second);

	}
	virtual ~StateMachine();

	// Component을(를) 통해 상속됨
	virtual void initialize() override;
	virtual void update() override;
	virtual void lateUpdate() override;
	virtual void render() override;
	virtual void release() override;
public:
	

	HRESULT insertState(wstring stateName, IState* state);

	//(시작상태-조건->도착상태) 정보 넣기
	HRESULT insertTransitionCondition(wstring firstStateName, int transitionCondition, wstring secondStateName);
	HRESULT transitionState(int transitionCondition); //현재 상태와 조건에 맞는 상태가 있으면 변환
	HRESULT changeState(wstring stateName); //해당 이름의 상태로 변환
	wstring getCurrentStateName()const { return currentStateName; }

private:
	wstring currentStateName; //현재 상태 이름
	unordered_map<wstring, IState*> mapStates_; //상태 이름, 상태 포인터
	vector<tuple<wstring, int, wstring>> vecTransitions_;//현재 상태,전이 조건,바뀔 상태
};

