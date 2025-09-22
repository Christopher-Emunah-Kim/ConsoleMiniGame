namespace ConsoleMiniGame.Input;

/// <summary>
/// Manages keyboard input in the console application
/// </summary>
public class InputManager
{
    private readonly HashSet<ConsoleKey> _currentKeys;
    private readonly HashSet<ConsoleKey> _previousKeys;
    private readonly Queue<ConsoleKeyInfo> _keyBuffer;
    private readonly object _lockObject = new();
    private bool _isInitialized;

    public InputManager()
    {
        _currentKeys = new HashSet<ConsoleKey>();
        _previousKeys = new HashSet<ConsoleKey>();
        _keyBuffer = new Queue<ConsoleKeyInfo>();
    }

    /// <summary>
    /// Initialize the input manager
    /// </summary>
    public void Initialize()
    {
        if (_isInitialized) return;
        
        _isInitialized = true;
    }

    /// <summary>
    /// Update input state - call this every frame
    /// </summary>
    public void Update()
    {
        if (!_isInitialized) return;

        lock (_lockObject)
        {
            // Copy current keys to previous keys
            _previousKeys.Clear();
            foreach (var key in _currentKeys)
            {
                _previousKeys.Add(key);
            }

            // Read available keys
            _currentKeys.Clear();
            while (Console.KeyAvailable)
            {
                var keyInfo = Console.ReadKey(true);
                _keyBuffer.Enqueue(keyInfo);
                _currentKeys.Add(keyInfo.Key);
            }
        }
    }

    /// <summary>
    /// Check if a key is currently being held down
    /// </summary>
    public bool IsKeyDown(ConsoleKey key)
    {
        lock (_lockObject)
        {
            return _currentKeys.Contains(key);
        }
    }

    /// <summary>
    /// Check if a key was just pressed this frame (not held from previous frame)
    /// </summary>
    public bool IsKeyPressed(ConsoleKey key)
    {
        lock (_lockObject)
        {
            return _currentKeys.Contains(key) && !_previousKeys.Contains(key);
        }
    }

    /// <summary>
    /// Check if a key was just released this frame
    /// </summary>
    public bool IsKeyReleased(ConsoleKey key)
    {
        lock (_lockObject)
        {
            return !_currentKeys.Contains(key) && _previousKeys.Contains(key);
        }
    }

    /// <summary>
    /// Get the next key from the input buffer
    /// </summary>
    public ConsoleKeyInfo? GetNextKey()
    {
        lock (_lockObject)
        {
            return _keyBuffer.Count > 0 ? _keyBuffer.Dequeue() : null;
        }
    }

    /// <summary>
    /// Clear the input buffer
    /// </summary>
    public void ClearBuffer()
    {
        lock (_lockObject)
        {
            _keyBuffer.Clear();
        }
    }

    /// <summary>
    /// Get all currently pressed keys
    /// </summary>
    public IEnumerable<ConsoleKey> GetPressedKeys()
    {
        lock (_lockObject)
        {
            return _currentKeys.ToList();
        }
    }

    /// <summary>
    /// Shutdown the input manager
    /// </summary>
    public void Shutdown()
    {
        lock (_lockObject)
        {
            _currentKeys.Clear();
            _previousKeys.Clear();
            _keyBuffer.Clear();
        }
        _isInitialized = false;
    }
}