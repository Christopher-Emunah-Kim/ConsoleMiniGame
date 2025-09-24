#pragma once
#include "IContent.h"
#include "../Core/Singleton.h"
#include <unordered_map>
#include <memory>


class ContentManager 
{
public:
	void LoadContent();
	void OnUpdate();
	void OnRender();

	void AddContent(int contentId, std::unique_ptr<IContent> content);
	void ChangeContent(int contentId);

	IContent* GetCurrentContent() const { return currentContent; }


private:
	std::unordered_map<int, std::unique_ptr<IContent>> contentsMap;
	IContent* currentContent = nullptr;

};