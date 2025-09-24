//#pragma once
//#include "Common.h"
//
//
////TODO : Level쓸건지? GameWorld랑 ContentManager랑 역할 구분이 잘 안됨.
//class BaseLevel;
//
//class GameWorld final
//{
//public:
//	GameWorld() = default;
//	virtual ~GameWorld();
//
//private:
//	void Initialize();
//
//public:
//	void Init();
//	void Update();
//	void Render();
//	void Release();
//
//	bool IsNextLevelExist() const;
//	void SetNextLevel(EAreaName name);
//	void ChangeLevel();
//
//	inline BaseLevel* GetCurrentLevel() const { return m_currentLevel; }
//	inline BaseLevel* GetNextLevel() const { return m_nextLevel; }
//
//private:
//	BaseLevel* m_currentLevel{ nullptr };
//	BaseLevel* m_nextLevel{ nullptr };
//	unordered_map<wstring, unique_ptr<BaseLevel>> m_levels;
//};
//
