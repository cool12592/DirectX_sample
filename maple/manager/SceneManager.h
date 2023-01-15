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

	HRESULT insertScene(wstring sceneName, IScene* scene); //�� �̸�, �� ��ü ����
	HRESULT insertSceneTransition(wstring srcSceneName, int transitionCondition, wstring destSceneName);//(���۾� -����-> ������) ���� ����
	HRESULT changeScene(int transitionCondition); //���� ������ ���ǿ� �´� ������ ��ȭ
	HRESULT changeScene(wstring sceneName); //�ش� �̸��� ������ ��ȭ
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

