namespace CestaKralem;

class Program
{
    static void Main(string[] args)
    {
        KingTourApp app = new KingTourApp(8);
        app.FindShortestPath();
    }
}