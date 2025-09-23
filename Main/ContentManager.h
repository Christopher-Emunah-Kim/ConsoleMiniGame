#pragma once
#include "IContent.h"
#include "../Core/Singleton.h"
#include <map>

class ContentManager : public Singleton<ContentManager>
{
public:
	void LoadContent();
	void OnUpdate();
	void OnRender();

	void AddContent(int contentId, IContent* content);
	void ChangeContent(int contentId);

	IContent* GetCurrentContent() const { return currentContent; }

private:
	std::map<int, IContent*> contentsMap;
	IContent* currentContent = nullptr;
};

