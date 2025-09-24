#include "ContentManager.h"
#include "../Core/Common.h"
#include "../Core/GameMaster.h"
#include "Content/TitleContent.h"

void ContentManager::LoadContent()
{
	contentsMap.clear();
	currentContent = nullptr;

	AddContent( static_cast<int>( EContent::TITLE ) , make_unique<TitleContent>() );

	ChangeContent( static_cast<int>( EContent::TITLE ) );
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

void ContentManager::AddContent( int contentId , unique_ptr<IContent> content )
{
	if ( !content )
	{
		return;
	}

	if (contentsMap.find(contentId) == contentsMap.end())
	{
		contentsMap.insert( std::make_pair( contentId , std::move( content ) ) );
	}
}

void ContentManager::ChangeContent(int contentId)
{
	if (currentContent)
	{
		currentContent->OnRelease();
		currentContent = nullptr;
	}

	std::unordered_map<int , unique_ptr<IContent>>::iterator iter = contentsMap.find( contentId );
	if( iter != contentsMap.end() )
	{
		currentContent = iter->second.get();

		GAME_MASTER->GetInputService().Clear();
		GAME_MASTER->GetHUD().ClearText();


		currentContent->OnInit();
	}
}
