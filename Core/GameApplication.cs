using ConsoleMiniGame.Input;
using ConsoleMiniGame.Levels;
using ConsoleMiniGame.UI;

namespace ConsoleMiniGame.Core;

/// <summary>
/// Main application class that manages the overall game lifecycle
/// </summary>
public class GameApplication
{
    private bool _isRunning;
    private GameMaster? _gameMaster;
    private readonly InputManager _inputManager;
    private readonly Screen _screen;
    
    public GameApplication()
    {
        _inputManager = new InputManager();
        _screen = new Screen();
    }

    /// <summary>
    /// Initialize the application
    /// </summary>
    public void Initialize()
    {
        Console.CursorVisible = false;
        Console.Clear();
        
        _screen.Initialize();
        _inputManager.Initialize();
        
        _gameMaster = new GameMaster(_inputManager, _screen);
        _gameMaster.Initialize();
    }

    /// <summary>
    /// Main application run loop
    /// </summary>
    public void Run()
    {
        _isRunning = true;
        
        while (_isRunning)
        {
            Update();
            Render();
            
            // Control frame rate
            Thread.Sleep(16); // ~60 FPS
        }
    }

    /// <summary>
    /// Update application state
    /// </summary>
    private void Update()
    {
        _inputManager.Update();
        _gameMaster?.Update();
        
        // Check for exit conditions
        if (_inputManager.IsKeyPressed(ConsoleKey.Escape))
        {
            Shutdown();
        }
    }

    /// <summary>
    /// Render the application
    /// </summary>
    private void Render()
    {
        _screen.Clear();
        _gameMaster?.Render(_screen);
        _screen.Present();
    }

    /// <summary>
    /// Shutdown the application gracefully
    /// </summary>
    public void Shutdown()
    {
        _isRunning = false;
        _gameMaster?.Shutdown();
        _screen.Shutdown();
        _inputManager.Shutdown();
        
        Console.CursorVisible = true;
        Console.Clear();
        Console.WriteLine("Game application shutdown complete.");
    }
}