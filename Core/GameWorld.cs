using ConsoleMiniGame.Input;
using ConsoleMiniGame.Levels;
using ConsoleMiniGame.UI;

namespace ConsoleMiniGame.Core;

/// <summary>
/// Represents the game world state, entities, and game logic
/// </summary>
public class GameWorld
{
    private Level? _currentLevel;
    private readonly List<GameObject> _gameObjects;
    private readonly Random _random;
    private readonly InputManager? _inputManager;
    private bool _isLevelComplete;
    private bool _isGameOver;
    private bool _isGameComplete;
    private int _score;
    private int _lives;

    public GameWorld(InputManager inputManager)
    {
        _gameObjects = new List<GameObject>();
        _random = new Random();
        _lives = 3;
        _score = 0;
        _inputManager = inputManager;
    }

    /// <summary>
    /// Initialize the game world
    /// </summary>
    public void Initialize()
    {
        _gameObjects.Clear();
        _isLevelComplete = false;
        _isGameOver = false;
        _isGameComplete = false;
        
        // Add a simple player object as example
        AddGameObject(new Player(40, 12, _inputManager));
        AddGameObject(new Enemy(10, 5));
        AddGameObject(new Enemy(70, 8));
    }

    /// <summary>
    /// Update the game world
    /// </summary>
    public void Update()
    {
        // Update all game objects
        foreach (var gameObject in _gameObjects)
        {
            gameObject.Update();
        }

        // Handle collisions and game logic
        HandleCollisions();
        CheckLevelCompletion();
        
        // Remove destroyed objects
        _gameObjects.RemoveAll(obj => obj.IsDestroyed);
    }

    /// <summary>
    /// Render the game world
    /// </summary>
    public void Render(Screen screen)
    {
        // Render all game objects
        foreach (var gameObject in _gameObjects)
        {
            gameObject.Render(screen);
        }

        // Render level-specific elements
        _currentLevel?.Render(screen);
    }

    /// <summary>
    /// Load a new level into the game world
    /// </summary>
    public void LoadNewLevel(Level level)
    {
        _currentLevel = level;
        _isLevelComplete = false;
        
        // Clear existing dynamic objects but keep player
        var player = _gameObjects.FirstOrDefault(obj => obj is Player);
        _gameObjects.Clear();
        
        if (player != null)
        {
            _gameObjects.Add(player);
        }

        // Add new level objects
        SpawnLevelObjects();
    }

    /// <summary>
    /// Add a game object to the world
    /// </summary>
    public void AddGameObject(GameObject gameObject)
    {
        _gameObjects.Add(gameObject);
    }

    /// <summary>
    /// Get all game objects of a specific type
    /// </summary>
    public IEnumerable<T> GetGameObjects<T>() where T : GameObject
    {
        return _gameObjects.OfType<T>();
    }

    /// <summary>
    /// Check if the current level is complete
    /// </summary>
    public bool IsLevelComplete() => _isLevelComplete;

    /// <summary>
    /// Check if the game is over
    /// </summary>
    public bool IsGameOver() => _isGameOver;

    /// <summary>
    /// Check if the game is complete
    /// </summary>
    public bool IsGameComplete() => _isGameComplete;

    /// <summary>
    /// Set game complete status
    /// </summary>
    public void SetGameComplete(bool complete) => _isGameComplete = complete;

    /// <summary>
    /// Get current score
    /// </summary>
    public int Score => _score;

    /// <summary>
    /// Get remaining lives
    /// </summary>
    public int Lives => _lives;

    /// <summary>
    /// Add score points
    /// </summary>
    public void AddScore(int points) => _score += points;

    /// <summary>
    /// Lose a life
    /// </summary>
    public void LoseLife()
    {
        _lives--;
        if (_lives <= 0)
        {
            _isGameOver = true;
        }
    }

    /// <summary>
    /// Shutdown the game world
    /// </summary>
    public void Shutdown()
    {
        _gameObjects.Clear();
        _currentLevel = null;
    }

    private void HandleCollisions()
    {
        // Simple collision detection example
        var player = _gameObjects.FirstOrDefault(obj => obj is Player) as Player;
        if (player == null) return;

        var enemies = _gameObjects.OfType<Enemy>().ToList();
        foreach (var enemy in enemies)
        {
            if (Math.Abs(player.X - enemy.X) <= 1 && Math.Abs(player.Y - enemy.Y) <= 1)
            {
                // Collision detected
                enemy.IsDestroyed = true;
                LoseLife();
                AddScore(100);
            }
        }
    }

    private void CheckLevelCompletion()
    {
        // Example: level complete when all enemies are destroyed
        if (_gameObjects.OfType<Enemy>().Count() == 0)
        {
            _isLevelComplete = true;
        }
    }

    private void SpawnLevelObjects()
    {
        // Spawn some enemies for the new level
        int enemyCount = _random.Next(3, 7);
        for (int i = 0; i < enemyCount; i++)
        {
            int x = _random.Next(5, 75);
            int y = _random.Next(3, 15);
            AddGameObject(new Enemy(x, y));
        }
    }
}

/// <summary>
/// Base class for all game objects
/// </summary>
public abstract class GameObject
{
    public int X { get; set; }
    public int Y { get; set; }
    public bool IsDestroyed { get; set; }
    public char Symbol { get; protected set; }
    public ConsoleColor Color { get; protected set; }

    protected GameObject(int x, int y, char symbol, ConsoleColor color)
    {
        X = x;
        Y = y;
        Symbol = symbol;
        Color = color;
    }

    public abstract void Update();

    public virtual void Render(Screen screen)
    {
        if (!IsDestroyed)
        {
            screen.DrawChar(X, Y, Symbol, Color);
        }
    }
}

/// <summary>
/// Player game object
/// </summary>
public class Player : GameObject
{
    private readonly InputManager? _inputManager;

    public Player(int x, int y, InputManager? inputManager = null) : base(x, y, '@', ConsoleColor.Green)
    {
        _inputManager = inputManager;
    }

    public override void Update()
    {
        if (_inputManager == null) return;

        // Handle player movement with both WASD and arrow keys
        int newX = X;
        int newY = Y;

        if (_inputManager.IsKeyPressed(ConsoleKey.W) || _inputManager.IsKeyPressed(ConsoleKey.UpArrow))
        {
            newY = Math.Max(2, Y - 1); // Don't go above level border
        }
        else if (_inputManager.IsKeyPressed(ConsoleKey.S) || _inputManager.IsKeyPressed(ConsoleKey.DownArrow))
        {
            newY = Math.Min(18, Y + 1); // Don't go below level border
        }
        
        if (_inputManager.IsKeyPressed(ConsoleKey.A) || _inputManager.IsKeyPressed(ConsoleKey.LeftArrow))
        {
            newX = Math.Max(1, X - 1); // Don't go left of level border
        }
        else if (_inputManager.IsKeyPressed(ConsoleKey.D) || _inputManager.IsKeyPressed(ConsoleKey.RightArrow))
        {
            newX = Math.Min(78, X + 1); // Don't go right of level border
        }

        X = newX;
        Y = newY;
    }
}

/// <summary>
/// Enemy game object
/// </summary>
public class Enemy : GameObject
{
    private readonly Random _random = new();
    private int _moveCounter;

    public Enemy(int x, int y) : base(x, y, 'E', ConsoleColor.Red)
    {
    }

    public override void Update()
    {
        // Simple AI: move randomly occasionally
        _moveCounter++;
        if (_moveCounter > 60) // Move every ~1 second
        {
            int direction = _random.Next(4);
            switch (direction)
            {
                case 0: Y = Math.Max(1, Y - 1); break; // Up
                case 1: Y = Math.Min(18, Y + 1); break; // Down
                case 2: X = Math.Max(1, X - 1); break; // Left
                case 3: X = Math.Min(78, X + 1); break; // Right
            }
            _moveCounter = 0;
        }
    }
}