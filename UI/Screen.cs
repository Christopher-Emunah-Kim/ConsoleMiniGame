namespace ConsoleMiniGame.UI;

/// <summary>
/// Manages console display and rendering operations
/// </summary>
public class Screen
{
    private char[,] _buffer = null!;
    private ConsoleColor[,] _colorBuffer = null!;
    private char[,] _previousBuffer = null!;
    private ConsoleColor[,] _previousColorBuffer = null!;
    
    public int Width { get; private set; }
    public int Height { get; private set; }

    /// <summary>
    /// Initialize the screen with console dimensions
    /// </summary>
    public void Initialize()
    {
        Width = Console.WindowWidth;
        Height = Console.WindowHeight;

        _buffer = new char[Width, Height];
        _colorBuffer = new ConsoleColor[Width, Height];
        _previousBuffer = new char[Width, Height];
        _previousColorBuffer = new ConsoleColor[Width, Height];

        // Initialize buffers with spaces and default color
        Clear();
        
        Console.CursorVisible = false;
        Console.SetCursorPosition(0, 0);
    }

    /// <summary>
    /// Clear the screen buffer
    /// </summary>
    public void Clear()
    {
        for (int x = 0; x < Width; x++)
        {
            for (int y = 0; y < Height; y++)
            {
                _buffer[x, y] = ' ';
                _colorBuffer[x, y] = ConsoleColor.Black;
            }
        }
    }

    /// <summary>
    /// Draw a character at the specified position
    /// </summary>
    public void DrawChar(int x, int y, char character, ConsoleColor color = ConsoleColor.White)
    {
        if (x >= 0 && x < Width && y >= 0 && y < Height)
        {
            _buffer[x, y] = character;
            _colorBuffer[x, y] = color;
        }
    }

    /// <summary>
    /// Draw text at the specified position
    /// </summary>
    public void DrawText(int x, int y, string text, ConsoleColor color = ConsoleColor.White)
    {
        for (int i = 0; i < text.Length && x + i < Width; i++)
        {
            if (y >= 0 && y < Height)
            {
                DrawChar(x + i, y, text[i], color);
            }
        }
    }

    /// <summary>
    /// Draw text centered horizontally at the specified y position
    /// </summary>
    public void DrawTextCentered(int y, string text, ConsoleColor color = ConsoleColor.White)
    {
        int x = (Width - text.Length) / 2;
        DrawText(x, y, text, color);
    }

    /// <summary>
    /// Draw a horizontal line
    /// </summary>
    public void DrawHorizontalLine(int x, int y, int length, char character = '─', ConsoleColor color = ConsoleColor.White)
    {
        for (int i = 0; i < length && x + i < Width; i++)
        {
            DrawChar(x + i, y, character, color);
        }
    }

    /// <summary>
    /// Draw a vertical line
    /// </summary>
    public void DrawVerticalLine(int x, int y, int length, char character = '│', ConsoleColor color = ConsoleColor.White)
    {
        for (int i = 0; i < length && y + i < Height; i++)
        {
            DrawChar(x, y + i, character, color);
        }
    }

    /// <summary>
    /// Draw a rectangle outline
    /// </summary>
    public void DrawRectangle(int x, int y, int width, int height, ConsoleColor color = ConsoleColor.White)
    {
        // Top and bottom lines
        DrawHorizontalLine(x, y, width, '─', color);
        DrawHorizontalLine(x, y + height - 1, width, '─', color);

        // Left and right lines
        DrawVerticalLine(x, y, height, '│', color);
        DrawVerticalLine(x + width - 1, y, height, '│', color);

        // Corners
        DrawChar(x, y, '┌', color);
        DrawChar(x + width - 1, y, '┐', color);
        DrawChar(x, y + height - 1, '└', color);
        DrawChar(x + width - 1, y + height - 1, '┘', color);
    }

    /// <summary>
    /// Fill a rectangular area with a character
    /// </summary>
    public void FillRectangle(int x, int y, int width, int height, char character = ' ', ConsoleColor color = ConsoleColor.Black)
    {
        for (int fx = x; fx < x + width && fx < Width; fx++)
        {
            for (int fy = y; fy < y + height && fy < Height; fy++)
            {
                DrawChar(fx, fy, character, color);
            }
        }
    }

    /// <summary>
    /// Present the buffer to the console (render the frame)
    /// </summary>
    public void Present()
    {
        // Only update changed characters for better performance
        for (int y = 0; y < Height; y++)
        {
            for (int x = 0; x < Width; x++)
            {
                char currentChar = _buffer[x, y];
                ConsoleColor currentColor = _colorBuffer[x, y];

                // Only draw if the character or color has changed
                if (currentChar != _previousBuffer[x, y] || currentColor != _previousColorBuffer[x, y])
                {
                    Console.SetCursorPosition(x, y);
                    Console.ForegroundColor = currentColor;
                    Console.Write(currentChar);

                    _previousBuffer[x, y] = currentChar;
                    _previousColorBuffer[x, y] = currentColor;
                }
            }
        }

        // Reset cursor position
        Console.SetCursorPosition(0, 0);
    }

    /// <summary>
    /// Force a complete redraw of the screen
    /// </summary>
    public void ForceRedraw()
    {
        Console.Clear();
        
        // Reset previous buffer to force complete redraw
        for (int x = 0; x < Width; x++)
        {
            for (int y = 0; y < Height; y++)
            {
                _previousBuffer[x, y] = '\0';
                _previousColorBuffer[x, y] = (ConsoleColor)(-1);
            }
        }
    }

    /// <summary>
    /// Get the character at the specified position
    /// </summary>
    public char GetChar(int x, int y)
    {
        if (x >= 0 && x < Width && y >= 0 && y < Height)
        {
            return _buffer[x, y];
        }
        return ' ';
    }

    /// <summary>
    /// Get the color at the specified position
    /// </summary>
    public ConsoleColor GetColor(int x, int y)
    {
        if (x >= 0 && x < Width && y >= 0 && y < Height)
        {
            return _colorBuffer[x, y];
        }
        return ConsoleColor.Black;
    }

    /// <summary>
    /// Check if a position is within screen bounds
    /// </summary>
    public bool IsInBounds(int x, int y)
    {
        return x >= 0 && x < Width && y >= 0 && y < Height;
    }

    /// <summary>
    /// Shutdown the screen
    /// </summary>
    public void Shutdown()
    {
        Console.ResetColor();
        Console.CursorVisible = true;
        Console.Clear();
    }
}