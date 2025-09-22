# ConsoleMiniGame

A basic game loop pattern-based console game framework for C#. This framework provides a solid foundation for building console-based games with minimal setup, allowing you to focus on creating content rather than managing the underlying game architecture.

## Framework Components

### Core Components

- **GameApplication**: Main entry point that manages the overall application lifecycle
- **GameMaster**: Central coordinator that manages game state and orchestrates all components  
- **GameInstance**: Represents a single game session with its own world state
- **GameWorld**: Holds game world state, entities, and game logic

### Supporting Systems

- **InputManager**: Handles keyboard input with key press, hold, and release detection
- **LevelManager**: Manages game levels and progression
- **Screen**: Manages console display and rendering operations
- **HUD**: Heads-up display for showing game information and UI elements

## Getting Started

### Basic Usage

The framework is designed to be used by creating your game content on top of the existing architecture. The main game loop and core systems are already implemented.

```csharp
// The framework automatically handles:
// - Game loop (60 FPS)
// - Input management (WASD/Arrow keys + other keys)
// - Screen rendering with buffering
// - Game state management (Menu, Playing, Paused, Game Over)
// - Level progression
// - HUD display
```

### Running the Game

```bash
dotnet run
```

### Controls

- **WASD** or **Arrow Keys**: Move player
- **P**: Pause/Resume game
- **ESC**: Exit game
- **Enter/Space**: Start game from main menu
- **R**: Restart game (from game over)
- **Q**: Quit to main menu (from pause/game over)

## Framework Architecture

### Game Loop Pattern

The framework implements a standard game loop pattern:

1. **Initialize**: Set up all components
2. **Update**: Process input and update game state
3. **Render**: Draw everything to screen
4. **Present**: Display the frame
5. **Repeat**: Continue until shutdown

### Component Relationships

```
GameApplication
├── GameMaster
│   ├── GameInstance
│   │   ├── GameWorld
│   │   │   └── GameObject[] (Player, Enemies, etc.)
│   │   └── HUD
│   │       └── HudElement[] (Score, Lives, etc.)
│   └── LevelManager
│       └── Level[]
├── InputManager
└── Screen
```

## Creating Custom Content

### Custom Game Objects

Extend the `GameObject` base class to create your own entities:

```csharp
public class MyCustomEnemy : GameObject
{
    public MyCustomEnemy(int x, int y) : base(x, y, 'M', ConsoleColor.Red)
    {
    }

    public override void Update()
    {
        // Custom enemy logic here
    }

    public override void Render(Screen screen)
    {
        // Custom rendering if needed
        base.Render(screen); // Or implement your own
    }
}
```

### Custom HUD Elements

Create custom UI elements by extending `HudElement`:

```csharp
public class HealthBar : HudElement
{
    private int _health;
    private int _maxHealth;

    public HealthBar(int x, int y, int maxHealth) : base(x, y, ConsoleColor.Green)
    {
        _maxHealth = maxHealth;
        _health = maxHealth;
    }

    public override void Update()
    {
        // Update health logic
    }

    public override void Render(Screen screen)
    {
        // Draw health bar
        for (int i = 0; i < _maxHealth; i++)
        {
            char symbol = i < _health ? '♥' : '♡';
            screen.DrawChar(X + i, Y, symbol, Color);
        }
    }
}
```

### Custom Levels

Extend the `Level` class or modify the `LevelManager` to create custom level layouts:

```csharp
var customLevel = new Level("Boss Level", "Face the final boss");
customLevel.AddLevelObject(new Wall(10, 10));
customLevel.AddLevelObject(new PowerUp(20, 15));
levelManager.AddLevel("BossLevel", customLevel);
```

## Framework Features

### Input System
- Key press detection (single frame)
- Key hold detection (continuous)
- Key release detection
- Input buffering
- Support for all console keys

### Rendering System
- Double-buffered rendering for smooth display
- Character and color support
- Efficient screen updates (only changed characters)
- Drawing primitives (text, lines, rectangles)
- Screen boundary checking

### Game State Management
- Main menu
- Gameplay
- Pause system
- Game over handling
- Automatic state transitions

### Level System
- Level progression
- Dynamic level loading
- Level-specific objects
- Automatic boundaries

## Example Game Objects

The framework includes example game objects in the `Examples` folder:

- **SimpleChaseGame**: Collectible star that moves randomly
- **PowerUp**: Pulsing collectible power-up
- **Wall**: Static obstacle

## Extending the Framework

The framework is designed to be extended. You can:

1. **Add new GameObject types** for different entities
2. **Create custom HUD elements** for specialized UI
3. **Implement new Level types** for different gameplay modes
4. **Extend the InputManager** for additional input devices
5. **Add sound effects** through the existing update/render cycle
6. **Implement new GameState types** for different game modes

## Technical Details

- **Target Framework**: .NET 8.0
- **Performance**: ~60 FPS with efficient rendering
- **Memory**: Minimal allocations during gameplay
- **Platform**: Cross-platform console support

## License

This framework is provided as-is for educational and development purposes.
