#include "GameWorld.h"

GameWorld::~GameWorld()
{
	Release();
}

void GameWorld::Initialize()
{
}

void GameWorld::Init()
{
	//m_levels[L"TITLE"] = make_unique<TownLevel>(L"TITLE");
	m_currentLevel = m_levels[L"TITLE"].get();
	Initialize();

	//m_currentLevel->Init();
}

void GameWorld::Update()
{
	if (IsNextLevelExist())
	{
		ChangeLevel();
	}
	//m_currentLevel->Update();
}

void GameWorld::Render()
{
	//m_currentLevel->Render();
}

void GameWorld::Release()
{
	if (m_currentLevel)
	{
		//m_currentLevel->Release();
		//m_currentLevel = nullptr;
	}

	m_nextLevel = nullptr;

	for(unordered_map<wstring, unique_ptr<BaseLevel>>::iterator iter = m_levels.begin(); iter != m_levels.end(); ++iter)
	{
		if(iter->second)
		{
			//iter->second->Release();
			iter->second.reset();
		}
	}

	m_levels.clear();
}

bool GameWorld::IsNextLevelExist() const
{
	return m_nextLevel != nullptr;
}

void GameWorld::SetNextLevel(EAreaName name)
{
	if (m_nextLevel != nullptr)
	{
		return;
	}

	unordered_map<wstring, unique_ptr<BaseLevel>>::iterator iter;

	if (iter != m_levels.end())
	{
		m_nextLevel = iter->second.get();
	}
}

void GameWorld::ChangeLevel()
{
	if (m_nextLevel)
	{
		//이후 정리
	}
}

