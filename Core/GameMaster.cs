using ConsoleMiniGame.Input;
using ConsoleMiniGame.Levels;
using ConsoleMiniGame.UI;

namespace ConsoleMiniGame.Core;

/// <summary>
/// Central coordinator that manages game state and orchestrates all game components
/// </summary>
public class GameMaster
{
    private readonly InputManager _inputManager;
    private readonly Screen _screen;
    private readonly LevelManager _levelManager;
    private GameInstance? _currentGameInstance;
    private GameState _gameState;

    public GameMaster(InputManager inputManager, Screen screen)
    {
        _inputManager = inputManager;
        _screen = screen;
        _levelManager = new LevelManager();
        _gameState = GameState.MainMenu;
    }

    /// <summary>
    /// Initialize the game master
    /// </summary>
    public void Initialize()
    {
        _levelManager.Initialize();
    }

    /// <summary>
    /// Update the game master and current game state
    /// </summary>
    public void Update()
    {
        // Check for pause input during gameplay
        if (_gameState == GameState.Playing && _inputManager.IsKeyPressed(ConsoleKey.P))
        {
            PauseGame();
            return;
        }

        switch (_gameState)
        {
            case GameState.MainMenu:
                UpdateMainMenu();
                break;
            case GameState.Playing:
                _currentGameInstance?.Update();
                // Check if game is over
                if (_currentGameInstance?.IsGameOver() == true)
                {
                    EndGame();
                }
                break;
            case GameState.Paused:
                UpdatePauseMenu();
                break;
            case GameState.GameOver:
                UpdateGameOver();
                break;
        }
    }

    /// <summary>
    /// Render the current game state
    /// </summary>
    public void Render(Screen screen)
    {
        switch (_gameState)
        {
            case GameState.MainMenu:
                RenderMainMenu(screen);
                break;
            case GameState.Playing:
                _currentGameInstance?.Render(screen);
                break;
            case GameState.Paused:
                _currentGameInstance?.Render(screen);
                RenderPauseMenu(screen);
                break;
            case GameState.GameOver:
                _currentGameInstance?.Render(screen);
                RenderGameOver(screen);
                break;
        }
    }

    /// <summary>
    /// Start a new game instance
    /// </summary>
    public void StartNewGame()
    {
        _currentGameInstance = new GameInstance(_inputManager, _levelManager);
        _currentGameInstance.Initialize();
        _gameState = GameState.Playing;
    }

    /// <summary>
    /// Pause the current game
    /// </summary>
    public void PauseGame()
    {
        if (_gameState == GameState.Playing)
        {
            _gameState = GameState.Paused;
        }
    }

    /// <summary>
    /// Resume the current game
    /// </summary>
    public void ResumeGame()
    {
        if (_gameState == GameState.Paused)
        {
            _gameState = GameState.Playing;
        }
    }

    /// <summary>
    /// End the current game
    /// </summary>
    public void EndGame()
    {
        _currentGameInstance = null;
        _gameState = GameState.GameOver;
    }

    /// <summary>
    /// Return to main menu
    /// </summary>
    public void ReturnToMainMenu()
    {
        _currentGameInstance = null;
        _gameState = GameState.MainMenu;
    }

    /// <summary>
    /// Shutdown the game master
    /// </summary>
    public void Shutdown()
    {
        _currentGameInstance?.Shutdown();
        _levelManager.Shutdown();
    }

    private void UpdateMainMenu()
    {
        if (_inputManager.IsKeyPressed(ConsoleKey.Enter) || _inputManager.IsKeyPressed(ConsoleKey.Spacebar))
        {
            StartNewGame();
        }
    }

    private void UpdatePauseMenu()
    {
        if (_inputManager.IsKeyPressed(ConsoleKey.P) || _inputManager.IsKeyPressed(ConsoleKey.Enter))
        {
            ResumeGame();
        }
        else if (_inputManager.IsKeyPressed(ConsoleKey.Q))
        {
            ReturnToMainMenu();
        }
    }

    private void UpdateGameOver()
    {
        if (_inputManager.IsKeyPressed(ConsoleKey.R))
        {
            StartNewGame();
        }
        else if (_inputManager.IsKeyPressed(ConsoleKey.Q))
        {
            ReturnToMainMenu();
        }
    }

    private void RenderMainMenu(Screen screen)
    {
        screen.DrawText(screen.Width / 2 - 10, screen.Height / 2 - 3, "CONSOLE MINI GAME", ConsoleColor.Yellow);
        screen.DrawText(screen.Width / 2 - 8, screen.Height / 2 - 1, "Press ENTER to Start", ConsoleColor.Green);
        screen.DrawText(screen.Width / 2 - 7, screen.Height / 2 + 1, "Press ESC to Exit", ConsoleColor.Red);
    }

    private void RenderPauseMenu(Screen screen)
    {
        screen.DrawText(screen.Width / 2 - 5, screen.Height / 2 - 2, "GAME PAUSED", ConsoleColor.Yellow);
        screen.DrawText(screen.Width / 2 - 8, screen.Height / 2, "Press P to Resume", ConsoleColor.Green);
        screen.DrawText(screen.Width / 2 - 8, screen.Height / 2 + 1, "Press Q to Quit", ConsoleColor.Red);
    }

    private void RenderGameOver(Screen screen)
    {
        screen.DrawText(screen.Width / 2 - 5, screen.Height / 2 - 2, "GAME OVER", ConsoleColor.Red);
        screen.DrawText(screen.Width / 2 - 8, screen.Height / 2, "Press R to Restart", ConsoleColor.Green);
        screen.DrawText(screen.Width / 2 - 8, screen.Height / 2 + 1, "Press Q to Quit", ConsoleColor.Yellow);
    }
}

/// <summary>
/// Represents the current state of the game
/// </summary>
public enum GameState
{
    MainMenu,
    Playing,
    Paused,
    GameOver
}