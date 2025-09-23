#include "ContentManager.h"
#include "../Core/Common.h"

void ContentManager::LoadContent()
{
	//AddContent(static_cast<int>(EContent::INTRO), new IntroContent());

	ChangeContent(static_cast<int>(EContent::INTRO));
}

void ContentManager::OnUpdate()
{
	if (currentContent)
	{
		currentContent->OnUpdate();
	}
}

void ContentManager::OnRender()
{
	if(currentContent)
	{
		currentContent->OnRender();
	}
}

void ContentManager::AddContent(int contentId, IContent* content)
{
	if (contentsMap.find(contentId) == contentsMap.end())
	{
		contentsMap.insert(std::make_pair(contentId, content));
	}
}

void ContentManager::ChangeContent(int contentId)
{
	if (currentContent)
	{
		currentContent->OnRelease();
	}

	if(contentsMap.find(contentId) != contentsMap.end())
	{
		currentContent = contentsMap[contentId];
		currentContent->OnInit();
	}
}
