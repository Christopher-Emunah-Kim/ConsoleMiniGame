#pragma once

class ScreenService;
class DungeonGameState;

class DungeonRenderer
{
	void RenderGame( ScreenService& screen , const DungeonGameState& gameState ) const;
	void RenderUI( ScreenService& screen , const DungeonGameState& gameState ) const;

public:
	void Render( ScreenService& screen , const DungeonGameState& gameState ) const;

};

