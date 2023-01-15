#include "stdafx.h"
#include "SceneManager.h"
#include "ExcelLoader.h"
#include "IScene.h"

IMPLEMENT_SINGLETON(SceneManager)
SceneManager::SceneManager():currentSceneName_(L"")
{
}


SceneManager::~SceneManager()
{
}

HRESULT SceneManager::insertScene(wstring sceneName, IScene * scene)
{
	auto& find_iter = mapScenes_.find(sceneName);
	ASSERT_MESSAGE_RETURN(mapScenes_.end() == find_iter, "Scene already exist", E_FAIL);
	
	mapScenes_.emplace(sceneName, scene);

	auto& find_Number_iter = find_if(vecSceneNumber_.begin(), vecSceneNumber_.end(), 
		[&](auto& pair) {
			if (wcscmp(sceneName.c_str(), pair.first.c_str()) == 0)
				return true;
			else
				return false;
		});
	ASSERT_MESSAGE_RETURN(vecSceneNumber_.end() == find_Number_iter, "SceneNumber already exist", E_FAIL);

	int sceneNumber = ExcelLoader::getSceneNumberByName(L"../ExcelFiles/SceneData.xlsx", sceneName);
	if (-1 == sceneNumber)
		return S_OK;

	vecSceneNumber_.emplace_back(make_pair(sceneName, sceneNumber));
	
	return S_OK;
}

HRESULT SceneManager::insertSceneTransition(wstring srcSceneName, int transitionCondition, wstring destSceneName)
{
	auto& find_iter = find_if(vecSceneTransition_.begin(), vecSceneTransition_.end(), [&](const tuple<wstring, int, wstring>& transition) {
		if (get<0>(transition) == srcSceneName&&get<1>(transition) == transitionCondition)
			return true;
		return false;
	});
	ASSERT_MESSAGE_RETURN(vecSceneTransition_.end() == find_iter, "transition data is overlapped", E_FAIL);

	vecSceneTransition_.emplace_back(make_tuple(srcSceneName, transitionCondition, destSceneName));

	return S_OK;
}

HRESULT SceneManager::changeScene(int transitionCondition)
{
	ASSERT_MESSAGE_RETURN(currentSceneName_ != L"", "please set initial scene", E_FAIL);
	auto& find_iter = find_if(vecSceneTransition_.begin(), vecSceneTransition_.end(), [&](const tuple<wstring, int, wstring>& transition) {
		if (get<0>(transition) == currentSceneName_&&get<1>(transition) == transitionCondition)
			return true;
		return false;
	});
	if (vecSceneTransition_.end() == find_iter)
		return E_FAIL;
	if (FAILED(changeScene(get<2>(*find_iter))))
		return E_FAIL;
	
	return S_OK;
}

HRESULT SceneManager::changeScene(wstring sceneName)
{
#ifdef _DEBUG
	auto& find_iter = mapScenes_.find(sceneName);
	ASSERT_MESSAGE_RETURN(mapScenes_.end() != find_iter, "scene does not exist", E_FAIL);
#endif
	if (!currentScene_.expired())
	{
		pastSceneName_ = currentSceneName_;
		currentScene_.lock()->release();
		currentScene_.reset();
		mapScenes_.erase(currentSceneName_);
		for (int i = 0; i < vecSceneNumber_.size(); i++)
		{
			if (vecSceneNumber_[i].first == currentSceneName_)
			{
				vecSceneNumber_.erase(vecSceneNumber_.begin() + i);
				break;
			}
		}
	}

	currentScene_ = mapScenes_[sceneName];

	if(!currentScene_.expired())
		currentScene_.lock()->initialize();

	currentSceneName_ = sceneName;
	return S_OK;
}

wstring SceneManager::getSceneNameByNumber(int sceneNumber)
{
	auto& find_iter = find_if(vecSceneNumber_.begin(), vecSceneNumber_.end(),
		[&](auto& pair) {
			if (pair.second==sceneNumber)
				return true;
			else
				return false;
		});
	if (vecSceneNumber_.end() == find_iter)
		return L"";
	return (*find_iter).first;
}

int SceneManager::getSceneNumberByName(wstring sceneName)
{
	auto& find_iter = find_if(vecSceneNumber_.begin(), vecSceneNumber_.end(),
		[&](auto& pair) {
			if (wcscmp(sceneName.c_str(), pair.first.c_str()) == 0)
				return true;
			else
				return false;
		});
	if (vecSceneNumber_.end() == find_iter)
		return -1;

	return (*find_iter).second;
}

void SceneManager::update()
{
	if (!currentScene_.expired())
		currentScene_.lock()->update();
}

void SceneManager::lateUpdate()
{
	if (!currentScene_.expired())
		currentScene_.lock()->lateUpdate();
}

void SceneManager::render()
{
	if (!currentScene_.expired())
		currentScene_.lock()->render();
}

void SceneManager::refresh()
{
	if (!currentScene_.expired())
		currentScene_.lock()->refresh();
}
