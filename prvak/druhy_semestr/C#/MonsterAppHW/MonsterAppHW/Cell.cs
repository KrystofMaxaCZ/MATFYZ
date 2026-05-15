namespace MonsterAppHW;

public class Cell
{
    public (int, int) Coordinates { get; private set; } // coordinates are set only once on the start
    public bool Availability { get; set; }
    public char Symbol { get;  set; }

    public Cell(int x, int y, char symbol , bool value)
    {
        Coordinates = (x, y);
        Availability = value;
        Symbol = symbol;
    }
}