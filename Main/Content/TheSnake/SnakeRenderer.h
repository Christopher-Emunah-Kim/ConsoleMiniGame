#pragma once
#include "SnakeType.h"

class ScreenService;
class SnakeGameState;

class SnakeRenderer final
{
	void RenderGame( ScreenService& screen , const SnakeGameState& gameState ) const;
	void RenderUI( ScreenService& screen , const SnakeGameState& gameState ) const;

public:
	void Render( ScreenService& screen , const SnakeGameState& gameState ) const;
};

