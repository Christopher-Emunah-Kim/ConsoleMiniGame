using ConsoleMiniGame.Core;

namespace ConsoleMiniGame;

/// <summary>
/// Main entry point for the Console Mini Game application
/// </summary>
public class Program
{
    public static void Main(string[] args)
    {
        var gameApplication = new GameApplication();
        
        try
        {
            gameApplication.Initialize();
            gameApplication.Run();
        }
        catch (Exception ex)
        {
            Console.Clear();
            Console.WriteLine($"An error occurred: {ex.Message}");
            Console.WriteLine("Press any key to exit...");
            Console.ReadKey();
        }
        finally
        {
            gameApplication.Shutdown();
        }
    }
}
