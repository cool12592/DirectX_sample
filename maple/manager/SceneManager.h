#pragma once

class IScene;

class SceneManager
{
	DECLARE_SINGLETON(SceneManager)
public:
	enum SCENE_TRANSITION{TEST,STAGE1,STAGE2};
public:
	explicit SceneManager();
	~SceneManager();

	HRESULT insertScene(wstring sceneName, IScene* scene); //¾À ÀÌ¸§, ¾À °´Ã¼ »ðÀÔ
	HRESULT insertSceneTransition(wstring srcSceneName, int transitionCondition, wstring destSceneName);//(½ÃÀÛ¾À -Á¶°Ç-> µµÂø¾À) Á¤º¸ »ðÀÔ
	HRESULT changeScene(int transitionCondition); //ÇöÀç ¾À¿¡¼­ Á¶°Ç¿¡ ¸Â´Â ¾ÀÀ¸·Î º¯È­
	HRESULT changeScene(wstring sceneName); //ÇØ´ç ÀÌ¸§ÀÇ ¾ÀÀ¸·Î º¯È­
	IScene* getCurrentScene()
	{
		if (currentScene_.expired())
			return nullptr;
		return &*currentScene_.lock();
	}
	wstring GetCurrentSceneName() { return currentSceneName_; }
	void SetCurrentSceneName(wstring sceneName) { currentSceneName_ = sceneName; }
	wstring getSceneNameByNumber(int sceneNumber);
	int getSceneNumberByName(wstring sceneName);
	_vec2 getCurrentSceneSize_() { return CurrentSceneSize_; }
	wstring getPastSceneName() { return pastSceneName_; }

	void update();
	void lateUpdate();
	void render();
	void refresh();
	
private:

	weak_ptr<IScene> currentScene_;
	wstring currentSceneName_;
	map<wstring, shared_ptr<IScene>> mapScenes_;
	vector<pair<wstring, int>> vecSceneNumber_;
	vector<tuple<wstring, int, wstring>> vecSceneTransition_;
	_vec2 CurrentSceneSize_;
	wstring pastSceneName_;

};

