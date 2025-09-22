using ConsoleMiniGame.Input;
using ConsoleMiniGame.Levels;
using ConsoleMiniGame.UI;

namespace ConsoleMiniGame.Core;

/// <summary>
/// Represents a single game session with its own world state and lifecycle
/// </summary>
public class GameInstance
{
    private readonly InputManager _inputManager;
    private readonly LevelManager _levelManager;
    private readonly GameWorld _gameWorld;
    private readonly HUD _hud;
    private bool _isInitialized;

    public GameInstance(InputManager inputManager, LevelManager levelManager)
    {
        _inputManager = inputManager;
        _levelManager = levelManager;
        _gameWorld = new GameWorld(inputManager);
        _hud = new HUD();
    }

    /// <summary>
    /// Initialize the game instance
    /// </summary>
    public void Initialize()
    {
        if (_isInitialized) return;

        _gameWorld.Initialize();
        _hud.Initialize();
        
        // Load the first level
        _levelManager.LoadLevel("Level1");
        
        _isInitialized = true;
    }

    /// <summary>
    /// Update the game instance
    /// </summary>
    public void Update()
    {
        if (!_isInitialized) return;

        // Check for pause input
        if (_inputManager.IsKeyPressed(ConsoleKey.P))
        {
            // Pause will be handled by GameMaster
            return;
        }

        _gameWorld.Update();
        _hud.Update();

        // Update HUD with current game state
        _hud.UpdateScore(_gameWorld.Score);
        _hud.UpdateLives(_gameWorld.Lives);
        _hud.UpdateLevel(_levelManager.GetCurrentLevelName() ?? "Unknown");

        // Handle level progression
        if (_gameWorld.IsLevelComplete())
        {
            _levelManager.NextLevel();
            if (_levelManager.HasNextLevel())
            {
                var currentLevel = _levelManager.GetCurrentLevel();
                if (currentLevel != null)
                {
                    _gameWorld.LoadNewLevel(currentLevel);
                }
            }
            else
            {
                // Game completed!
                _gameWorld.SetGameComplete(true);
            }
        }
    }

    /// <summary>
    /// Render the game instance
    /// </summary>
    public void Render(Screen screen)
    {
        if (!_isInitialized) return;

        _gameWorld.Render(screen);
        _hud.Render(screen);
    }

    /// <summary>
    /// Shutdown the game instance
    /// </summary>
    public void Shutdown()
    {
        if (!_isInitialized) return;

        _gameWorld.Shutdown();
        _hud.Shutdown();
        _isInitialized = false;
    }

    /// <summary>
    /// Get the current game world
    /// </summary>
    public GameWorld GetGameWorld() => _gameWorld;

    /// <summary>
    /// Check if the game is over
    /// </summary>
    public bool IsGameOver() => _gameWorld.IsGameOver() || _gameWorld.IsGameComplete();
}