using ConsoleMiniGame.Core;
using ConsoleMiniGame.UI;

namespace ConsoleMiniGame.Examples;

/// <summary>
/// Example game content that demonstrates how to use the framework
/// This shows how you can build content on top of the base framework
/// </summary>
public class SimpleChaseGame : GameObject
{
    private readonly Random _random;
    private int _moveTimer;
    private const int MOVE_INTERVAL = 30; // Move every half second at 60fps

    public SimpleChaseGame(int x, int y) : base(x, y, '★', ConsoleColor.Yellow)
    {
        _random = new Random();
        _moveTimer = 0;
    }

    public override void Update()
    {
        _moveTimer++;
        
        if (_moveTimer >= MOVE_INTERVAL)
        {
            // Simple AI: move randomly
            var direction = _random.Next(4);
            switch (direction)
            {
                case 0: Y = Math.Max(2, Y - 1); break; // Up
                case 1: Y = Math.Min(18, Y + 1); break; // Down  
                case 2: X = Math.Max(1, X - 1); break; // Left
                case 3: X = Math.Min(78, X + 1); break; // Right
            }
            
            _moveTimer = 0;
        }
    }

    public override void Render(Screen screen)
    {
        if (!IsDestroyed)
        {
            // Add a sparkle effect
            var colors = new[] { ConsoleColor.Yellow, ConsoleColor.White, ConsoleColor.DarkYellow };
            var color = colors[_moveTimer % colors.Length];
            screen.DrawChar(X, Y, Symbol, color);
        }
    }
}

/// <summary>
/// Example power-up collectible
/// </summary>
public class PowerUp : GameObject
{
    private int _pulseTimer;

    public PowerUp(int x, int y) : base(x, y, '◆', ConsoleColor.Magenta)
    {
        _pulseTimer = 0;
    }

    public override void Update()
    {
        _pulseTimer++;
    }

    public override void Render(Screen screen)
    {
        if (!IsDestroyed)
        {
            // Pulsing effect
            var colors = new[] { ConsoleColor.Magenta, ConsoleColor.DarkMagenta };
            var color = colors[(_pulseTimer / 15) % colors.Length];
            screen.DrawChar(X, Y, Symbol, color);
        }
    }
}

/// <summary>
/// Example static obstacle
/// </summary>
public class Wall : GameObject
{
    public Wall(int x, int y) : base(x, y, '█', ConsoleColor.DarkBlue)
    {
    }

    public override void Update()
    {
        // Static objects don't need updates
    }
}