using ConsoleMiniGame.UI;

namespace ConsoleMiniGame.Levels;

/// <summary>
/// Manages game levels and level progression
/// </summary>
public class LevelManager
{
    private readonly Dictionary<string, Level> _levels;
    private readonly List<string> _levelOrder;
    private int _currentLevelIndex;
    private Level? _currentLevel;

    public LevelManager()
    {
        _levels = new Dictionary<string, Level>();
        _levelOrder = new List<string>();
        _currentLevelIndex = 0;
    }

    /// <summary>
    /// Initialize the level manager with default levels
    /// </summary>
    public void Initialize()
    {
        // Create default levels
        CreateDefaultLevels();
    }

    /// <summary>
    /// Load a specific level by name
    /// </summary>
    public bool LoadLevel(string levelName)
    {
        if (_levels.TryGetValue(levelName, out var level))
        {
            _currentLevel = level;
            _currentLevelIndex = _levelOrder.IndexOf(levelName);
            return true;
        }
        return false;
    }

    /// <summary>
    /// Move to the next level in sequence
    /// </summary>
    public bool NextLevel()
    {
        if (_currentLevelIndex < _levelOrder.Count - 1)
        {
            _currentLevelIndex++;
            var nextLevelName = _levelOrder[_currentLevelIndex];
            return LoadLevel(nextLevelName);
        }
        return false; // No more levels
    }

    /// <summary>
    /// Move to the previous level in sequence
    /// </summary>
    public bool PreviousLevel()
    {
        if (_currentLevelIndex > 0)
        {
            _currentLevelIndex--;
            var previousLevelName = _levelOrder[_currentLevelIndex];
            return LoadLevel(previousLevelName);
        }
        return false; // No previous level
    }

    /// <summary>
    /// Check if there is a next level available
    /// </summary>
    public bool HasNextLevel()
    {
        return _currentLevelIndex < _levelOrder.Count - 1;
    }

    /// <summary>
    /// Check if there is a previous level available
    /// </summary>
    public bool HasPreviousLevel()
    {
        return _currentLevelIndex > 0;
    }

    /// <summary>
    /// Get the current level
    /// </summary>
    public Level? GetCurrentLevel() => _currentLevel;

    /// <summary>
    /// Get the current level name
    /// </summary>
    public string? GetCurrentLevelName()
    {
        return _currentLevelIndex >= 0 && _currentLevelIndex < _levelOrder.Count 
            ? _levelOrder[_currentLevelIndex] 
            : null;
    }

    /// <summary>
    /// Add a level to the manager
    /// </summary>
    public void AddLevel(string name, Level level)
    {
        _levels[name] = level;
        if (!_levelOrder.Contains(name))
        {
            _levelOrder.Add(name);
        }
    }

    /// <summary>
    /// Remove a level from the manager
    /// </summary>
    public bool RemoveLevel(string name)
    {
        if (_levels.Remove(name))
        {
            _levelOrder.Remove(name);
            return true;
        }
        return false;
    }

    /// <summary>
    /// Get total number of levels
    /// </summary>
    public int TotalLevels => _levelOrder.Count;

    /// <summary>
    /// Get current level number (1-based)
    /// </summary>
    public int CurrentLevelNumber => _currentLevelIndex + 1;

    /// <summary>
    /// Restart the current level
    /// </summary>
    public void RestartCurrentLevel()
    {
        _currentLevel?.Reset();
    }

    /// <summary>
    /// Shutdown the level manager
    /// </summary>
    public void Shutdown()
    {
        _levels.Clear();
        _levelOrder.Clear();
        _currentLevel = null;
        _currentLevelIndex = 0;
    }

    private void CreateDefaultLevels()
    {
        // Level 1: Tutorial
        var level1 = new Level("Tutorial Level", "Learn the basic controls");
        AddLevel("Level1", level1);

        // Level 2: Easy
        var level2 = new Level("Easy Challenge", "Face a few enemies");
        AddLevel("Level2", level2);

        // Level 3: Medium
        var level3 = new Level("Medium Challenge", "More enemies appear");
        AddLevel("Level3", level3);

        // Level 4: Hard
        var level4 = new Level("Hard Challenge", "Survive the onslaught");
        AddLevel("Level4", level4);
    }
}

/// <summary>
/// Represents a single level with its properties and behavior
/// </summary>
public class Level
{
    private readonly List<LevelObject> _levelObjects;
    
    public string Name { get; }
    public string Description { get; }
    public int Width { get; set; } = 80;
    public int Height { get; set; } = 20;

    public Level(string name, string description)
    {
        Name = name;
        Description = description;
        _levelObjects = new List<LevelObject>();
        InitializeLevelObjects();
    }

    /// <summary>
    /// Update the level
    /// </summary>
    public void Update()
    {
        foreach (var obj in _levelObjects)
        {
            obj.Update();
        }
    }

    /// <summary>
    /// Render the level
    /// </summary>
    public void Render(Screen screen)
    {
        // Draw level boundaries
        DrawBorder(screen);
        
        // Draw level objects
        foreach (var obj in _levelObjects)
        {
            obj.Render(screen);
        }

        // Draw level info
        screen.DrawText(2, 0, $"Level: {Name}", ConsoleColor.Cyan);
    }

    /// <summary>
    /// Reset the level to its initial state
    /// </summary>
    public void Reset()
    {
        _levelObjects.Clear();
        InitializeLevelObjects();
    }

    /// <summary>
    /// Add a level object
    /// </summary>
    public void AddLevelObject(LevelObject obj)
    {
        _levelObjects.Add(obj);
    }

    /// <summary>
    /// Remove a level object
    /// </summary>
    public void RemoveLevelObject(LevelObject obj)
    {
        _levelObjects.Remove(obj);
    }

    /// <summary>
    /// Get all level objects
    /// </summary>
    public IReadOnlyList<LevelObject> GetLevelObjects() => _levelObjects.AsReadOnly();

    private void InitializeLevelObjects()
    {
        // Add some static level objects as examples
        // These could be walls, power-ups, obstacles, etc.
        
        // Add corner markers
        _levelObjects.Add(new LevelObject(1, 1, '+', ConsoleColor.Yellow));
        _levelObjects.Add(new LevelObject(Width - 2, 1, '+', ConsoleColor.Yellow));
        _levelObjects.Add(new LevelObject(1, Height - 2, '+', ConsoleColor.Yellow));
        _levelObjects.Add(new LevelObject(Width - 2, Height - 2, '+', ConsoleColor.Yellow));
    }

    private void DrawBorder(Screen screen)
    {
        // Draw top and bottom borders
        for (int x = 0; x < Width; x++)
        {
            screen.DrawChar(x, 1, '═', ConsoleColor.White);
            screen.DrawChar(x, Height - 1, '═', ConsoleColor.White);
        }

        // Draw left and right borders
        for (int y = 1; y < Height; y++)
        {
            screen.DrawChar(0, y, '║', ConsoleColor.White);
            screen.DrawChar(Width - 1, y, '║', ConsoleColor.White);
        }

        // Draw corners
        screen.DrawChar(0, 1, '╔', ConsoleColor.White);
        screen.DrawChar(Width - 1, 1, '╗', ConsoleColor.White);
        screen.DrawChar(0, Height - 1, '╚', ConsoleColor.White);
        screen.DrawChar(Width - 1, Height - 1, '╝', ConsoleColor.White);
    }
}

/// <summary>
/// Represents a static object within a level
/// </summary>
public class LevelObject
{
    public int X { get; set; }
    public int Y { get; set; }
    public char Symbol { get; set; }
    public ConsoleColor Color { get; set; }
    public bool IsActive { get; set; }

    public LevelObject(int x, int y, char symbol, ConsoleColor color)
    {
        X = x;
        Y = y;
        Symbol = symbol;
        Color = color;
        IsActive = true;
    }

    public virtual void Update()
    {
        // Override in derived classes for dynamic behavior
    }

    public virtual void Render(Screen screen)
    {
        if (IsActive)
        {
            screen.DrawChar(X, Y, Symbol, Color);
        }
    }
}