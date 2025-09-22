namespace ConsoleMiniGame.UI;

/// <summary>
/// Heads-Up Display for showing game information and UI elements
/// </summary>
public class HUD
{
    private readonly Dictionary<string, HudElement> _elements;
    private bool _isVisible;

    public HUD()
    {
        _elements = new Dictionary<string, HudElement>();
        _isVisible = true;
    }

    /// <summary>
    /// Initialize the HUD
    /// </summary>
    public void Initialize()
    {
        CreateDefaultElements();
    }

    /// <summary>
    /// Update the HUD
    /// </summary>
    public void Update()
    {
        foreach (var element in _elements.Values)
        {
            element.Update();
        }
    }

    /// <summary>
    /// Render the HUD
    /// </summary>
    public void Render(Screen screen)
    {
        if (!_isVisible) return;

        foreach (var element in _elements.Values)
        {
            element.Render(screen);
        }
    }

    /// <summary>
    /// Add a HUD element
    /// </summary>
    public void AddElement(string name, HudElement element)
    {
        _elements[name] = element;
    }

    /// <summary>
    /// Remove a HUD element
    /// </summary>
    public bool RemoveElement(string name)
    {
        return _elements.Remove(name);
    }

    /// <summary>
    /// Get a HUD element by name
    /// </summary>
    public HudElement? GetElement(string name)
    {
        return _elements.TryGetValue(name, out var element) ? element : null;
    }

    /// <summary>
    /// Update the score display
    /// </summary>
    public void UpdateScore(int score)
    {
        if (GetElement("Score") is TextElement scoreElement)
        {
            scoreElement.Text = $"Score: {score:D6}";
        }
    }

    /// <summary>
    /// Update the lives display
    /// </summary>
    public void UpdateLives(int lives)
    {
        if (GetElement("Lives") is TextElement livesElement)
        {
            livesElement.Text = $"Lives: {lives}";
        }
    }

    /// <summary>
    /// Update the level display
    /// </summary>
    public void UpdateLevel(string levelName)
    {
        if (GetElement("Level") is TextElement levelElement)
        {
            levelElement.Text = $"Level: {levelName}";
        }
    }

    /// <summary>
    /// Show or hide the HUD
    /// </summary>
    public void SetVisible(bool visible)
    {
        _isVisible = visible;
    }

    /// <summary>
    /// Show a message temporarily
    /// </summary>
    public void ShowMessage(string message, int duration = 180) // 3 seconds at 60fps
    {
        var messageElement = new MessageElement(40, 10, message, ConsoleColor.Yellow, duration);
        AddElement("Message", messageElement);
    }

    /// <summary>
    /// Shutdown the HUD
    /// </summary>
    public void Shutdown()
    {
        _elements.Clear();
    }

    private void CreateDefaultElements()
    {
        // Score display
        AddElement("Score", new TextElement(2, 0, "Score: 000000", ConsoleColor.White));

        // Lives display
        AddElement("Lives", new TextElement(20, 0, "Lives: 3", ConsoleColor.Red));

        // Controls help
        AddElement("Controls", new TextElement(2, Console.WindowHeight - 2, 
            "WASD/Arrows: Move | P: Pause | ESC: Exit", ConsoleColor.DarkGray));
    }
}

/// <summary>
/// Base class for HUD elements
/// </summary>
public abstract class HudElement
{
    public int X { get; set; }
    public int Y { get; set; }
    public bool IsVisible { get; set; } = true;
    public ConsoleColor Color { get; set; }

    protected HudElement(int x, int y, ConsoleColor color = ConsoleColor.White)
    {
        X = x;
        Y = y;
        Color = color;
    }

    public abstract void Update();
    public abstract void Render(Screen screen);
}

/// <summary>
/// Text element for displaying text on the HUD
/// </summary>
public class TextElement : HudElement
{
    public string Text { get; set; }

    public TextElement(int x, int y, string text, ConsoleColor color = ConsoleColor.White) 
        : base(x, y, color)
    {
        Text = text;
    }

    public override void Update()
    {
        // Text elements typically don't need per-frame updates
    }

    public override void Render(Screen screen)
    {
        if (IsVisible)
        {
            screen.DrawText(X, Y, Text, Color);
        }
    }
}

/// <summary>
/// Temporary message element that disappears after a set duration
/// </summary>
public class MessageElement : TextElement
{
    private int _remainingDuration;

    public MessageElement(int x, int y, string text, ConsoleColor color, int duration) 
        : base(x, y, text, color)
    {
        _remainingDuration = duration;
    }

    public override void Update()
    {
        _remainingDuration--;
        if (_remainingDuration <= 0)
        {
            IsVisible = false;
        }
    }
}

/// <summary>
/// Progress bar element for showing health, progress, etc.
/// </summary>
public class ProgressBarElement : HudElement
{
    private int _maxValue;
    private int _currentValue;
    private int _width;
    private char _fillChar;
    private char _emptyChar;

    public int MaxValue 
    { 
        get => _maxValue; 
        set => _maxValue = Math.Max(1, value); 
    }

    public int CurrentValue 
    { 
        get => _currentValue; 
        set => _currentValue = Math.Clamp(value, 0, _maxValue); 
    }

    public ProgressBarElement(int x, int y, int width, int maxValue, ConsoleColor color = ConsoleColor.Green) 
        : base(x, y, color)
    {
        _width = width;
        _maxValue = maxValue;
        _currentValue = maxValue;
        _fillChar = '█';
        _emptyChar = '░';
    }

    public override void Update()
    {
        // Progress bars typically update their values externally
    }

    public override void Render(Screen screen)
    {
        if (!IsVisible) return;

        int filledWidth = (int)((float)_currentValue / _maxValue * _width);

        // Draw filled portion
        for (int i = 0; i < filledWidth; i++)
        {
            screen.DrawChar(X + i, Y, _fillChar, Color);
        }

        // Draw empty portion
        for (int i = filledWidth; i < _width; i++)
        {
            screen.DrawChar(X + i, Y, _emptyChar, ConsoleColor.DarkGray);
        }
    }
}

/// <summary>
/// Menu element for displaying selectable options
/// </summary>
public class MenuElement : HudElement
{
    private readonly List<string> _options;
    private int _selectedIndex;

    public int SelectedIndex 
    { 
        get => _selectedIndex; 
        set => _selectedIndex = Math.Clamp(value, 0, _options.Count - 1); 
    }

    public string SelectedOption => _options.Count > 0 ? _options[_selectedIndex] : "";

    public MenuElement(int x, int y, List<string> options, ConsoleColor color = ConsoleColor.White) 
        : base(x, y, color)
    {
        _options = new List<string>(options);
        _selectedIndex = 0;
    }

    public void AddOption(string option)
    {
        _options.Add(option);
    }

    public void RemoveOption(string option)
    {
        _options.Remove(option);
        if (_selectedIndex >= _options.Count)
        {
            _selectedIndex = Math.Max(0, _options.Count - 1);
        }
    }

    public void MoveUp()
    {
        _selectedIndex = (_selectedIndex - 1 + _options.Count) % _options.Count;
    }

    public void MoveDown()
    {
        _selectedIndex = (_selectedIndex + 1) % _options.Count;
    }

    public override void Update()
    {
        // Menu navigation is typically handled externally
    }

    public override void Render(Screen screen)
    {
        if (!IsVisible) return;

        for (int i = 0; i < _options.Count; i++)
        {
            var optionColor = i == _selectedIndex ? ConsoleColor.Yellow : Color;
            var prefix = i == _selectedIndex ? "> " : "  ";
            screen.DrawText(X, Y + i, prefix + _options[i], optionColor);
        }
    }
}