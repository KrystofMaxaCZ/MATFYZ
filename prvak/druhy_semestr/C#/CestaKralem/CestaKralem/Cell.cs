namespace CestaKralem;

public class Cell
{
    public int Row { get; set; }
    public int Col { get; set; }
    public int Distance { get; set; }
    public bool Visited { get; set; }
    public Cell Predecessor { get; set; }
    public CellType Type { get; set; }

    public enum CellType { Free, Obstacle, Start, End }

    public Cell(int x, int y, CellType value)
    {
        Row = x;
        Col = y;
        Type = value;
        Visited = false;
        Distance = 0;
    }
    
    public int CellScore(Cell endCell){
        /*
            Slouzi k heruistice pro ASTAR alg.
            Score = vzdalenost od startovniho policka + relativni vzdalenost od ciloveho policka,
            Tzn. policko ktere je dal od cile je pro nas horsi zpracovat jako prvni, tedy
            jako prvni prvky z fronty chceme brat ty s nizsim scorem, tedy ktere jsou blize k cilovemu policku
        */
        int score = Math.Max(Math.Abs(endCell.Row - Row), Math.Abs(endCell.Col - Col));
        score = score + Distance;
        return score;
    }
}