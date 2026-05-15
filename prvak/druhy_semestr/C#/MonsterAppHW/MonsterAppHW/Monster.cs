namespace MonsterAppHW;

public abstract class Monster
{
    public enum DirectionType { UP, DOWN, LEFT, RIGHT, NONE }

    public (int x, int y) Position { get; set; }
    public DirectionType Direction { get; set; }
    public char Symbol { get; protected set; }

    public Monster(int x, int y, char symbol)
    {
        Position = (x, y);
        Direction = FormatDirection(symbol);
        Symbol = symbol;
    }

    public abstract void MakeMove(Cell[,] matrix);
    
    // moves monster forward on the given row and col, and sets new positions
    protected void MoveForward(Cell[,] matrix, int row, int col)
    {
        // store last position to rewrite availability 
        int lastPosX = Position.Item1;
        int lastPosY = Position.Item2;
        
        Position = (row, col);
        matrix[row, col].Availability = false;
        matrix[row, col].Symbol = Symbol;
        
        matrix[lastPosX, lastPosY].Availability = true;
        matrix[lastPosX, lastPosY].Symbol = '.';

    }
    
    protected void TurnLeft(Cell[,] matrix)
    {
        switch (Direction)
        {
            case DirectionType.UP:
                Direction = DirectionType.LEFT;
                break;
            case DirectionType.DOWN:
                Direction = DirectionType.RIGHT;
                break;
            case DirectionType.LEFT:
                Direction = DirectionType.DOWN;
                break;
            case DirectionType.RIGHT:
                Direction = DirectionType.UP;
                break;
        }

        FormatSymbol();
        matrix[Position.Item1, Position.Item2].Symbol = Symbol;
    }

    protected void TurnRight(Cell[,] matrix)
    {
        switch (Direction)
        {
            case DirectionType.UP:
                Direction = DirectionType.RIGHT;
                break;
            case DirectionType.DOWN:
                Direction = DirectionType.LEFT;
                break;
            case DirectionType.LEFT:
                Direction = DirectionType.UP;
                break;
            case DirectionType.RIGHT:
                Direction = DirectionType.DOWN;
                break;
        }

        FormatSymbol();
        matrix[Position.Item1, Position.Item2].Symbol = Symbol;
    }
    
    /*
     I think that it is better to work with the enum DirectionType, rather than with chars as '^' ...
     and also if we would want to change Symbols that represent directions in input/output,
     we dont have to change it in the whole code, bc. the logic works with one enum
     
     So symbols for direction are defined here
     */
    private void FormatSymbol()
    {
        switch (Direction)
        {
            case DirectionType.UP:
                Symbol = '^';
                break;
            case DirectionType.DOWN:
                Symbol = 'v';
                break;
            case DirectionType.LEFT:
                Symbol = '<';
                break;
            case DirectionType.RIGHT:
                Symbol = '>';
                break;
        }
    }

    private DirectionType FormatDirection(char symbol)
    {
        switch (symbol)
        {
            case '^':
                return DirectionType.UP;
            case 'v':
                return DirectionType.DOWN;
            case '<':
                return DirectionType.LEFT;
            case '>':
                return DirectionType.RIGHT;
            default:
                return DirectionType.NONE;
        }
    }
}