namespace MonsterAppHW;

public class MonsterRightHand : Monster
{
    private bool _lastTurnWasRight = false;

    public MonsterRightHand(int x, int y, char symbol) : base(x, y, symbol){ }

    public override void MakeMove(Cell[,] matrix)
    {
        int row = Position.Item1;
        int col = Position.Item2;
        
        switch (Direction)
        {
            case DirectionType.UP:
                MakeMoveBasedOnDirection(matrix, row - 1, col, matrix[row, col + 1]);
                break;
            case DirectionType.DOWN:
                MakeMoveBasedOnDirection(matrix, row + 1, col, matrix[row, col - 1]);
                break;
            case DirectionType.LEFT:
                MakeMoveBasedOnDirection(matrix, row, col - 1, matrix[row - 1, col]);
                break;
            case DirectionType.RIGHT:
                MakeMoveBasedOnDirection(matrix, row, col + 1, matrix[row + 1, col]);
                break;
        }
    }

    
    private void MakeMoveBasedOnDirection(Cell[,] matrix, int row, int col, Cell cellOnTheRightSide)
    {
        bool isCellAheadAvailible = matrix[row, col].Symbol == '.';
        bool isCellRightAvailible = cellOnTheRightSide.Symbol == '.';
        
        if (_lastTurnWasRight && isCellAheadAvailible)
        {
            MoveForward(matrix, row, col);
            _lastTurnWasRight = false;
            // Console.WriteLine("Moved to " + row + " " + col + " bc. lastTurnWasRight");
        }
        else if (isCellRightAvailible)
        {
            TurnRight(matrix);
            _lastTurnWasRight = true;
            // Console.WriteLine("Turn right");
        }
        else if (isCellAheadAvailible)
        {
            MoveForward(matrix, row, col);
            _lastTurnWasRight = false;
            // Console.WriteLine("Moved to " + row + " " + col + " bc. wall");
        }
        else
        {
            TurnLeft(matrix);
            _lastTurnWasRight = false;
            // Console.WriteLine("Turn left");
        }
    }

}