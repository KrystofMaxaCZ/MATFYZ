namespace CestaKralem;

public class KingTourApp
{
    private Cell[,] _board;
    private Cell _startCell;
    private Cell _endCell;
    
    private PriorityQueue<Cell, int> _queue = new PriorityQueue<Cell, int>();
    public KingTourApp(int size)
    {
        _board = new Cell[size, size];
        // here, we are preparing cells on the game board
        PrepareBoard();
    }

    
    // prints the shortest path from start to end or -1 if doesn't exist
    // using A* alg with priority queue
    public void FindShortestPath()
    {
        ASTAR();
    }
    
    private void ProcessNeighbor(int row, int col, Cell currentCell){
        Cell neighborCell = _board[row,col];

        if (neighborCell.Visited == false && 
            (neighborCell.Type == Cell.CellType.Free || neighborCell.Type == Cell.CellType.End)){
            neighborCell.Distance = currentCell.Distance + 1;
            neighborCell.Predecessor = currentCell;
            neighborCell.Visited = true;
            _queue.Enqueue(neighborCell, neighborCell.CellScore(_endCell));
        }
    }

    private void WayBack(Cell cell)
    {
        if (cell.Type == Cell.CellType.Start)
        {
            Console.WriteLine((cell.Row + 1) + " " + (cell.Col + 1));
            return;
        }
        WayBack(cell.Predecessor);
        Console.WriteLine((cell.Row + 1) + " " + (cell.Col + 1));
    }

    private void ASTAR()
    {
        _queue.Enqueue(_startCell, 0);
        _startCell.Visited = true;

        while(_queue.Count > 0){
            Cell currentCell = _queue.Dequeue();

            if (currentCell == _endCell)
            {
                WayBack(currentCell);
                return;
            }

            int row = currentCell.Row;
            int col = currentCell.Col;
            int boardSize = _board.GetLength(0);

            
            // checks if the kings doesn't want to go outside the board  
            if ((row - 1) >= 0)
                ProcessNeighbor(row - 1, col, currentCell);

            if ((row + 1) < boardSize)
                ProcessNeighbor(row + 1, col, currentCell);

            if ((col - 1) >= 0)
                ProcessNeighbor(row, col - 1, currentCell);
            
            if ((col + 1) < boardSize)
                ProcessNeighbor(row, col + 1, currentCell);
            
            if ((col - 1) >= 0 && (row - 1) >= 0)
                ProcessNeighbor(row - 1, col - 1, currentCell);

            if ((col + 1) < boardSize && (row + 1) < boardSize)
                ProcessNeighbor(row + 1, col + 1, currentCell);
                
            if ((col - 1) >= 0 && (row + 1) < boardSize)
                ProcessNeighbor(row + 1, col - 1, currentCell);

            if ((col + 1) < boardSize && (row - 1) >= 0)
                ProcessNeighbor(row - 1, col + 1, currentCell);
        }

        // If path does not exist, print -1
        Console.WriteLine(-1);
    }


    /*
     * Input Processing and Game Board Preparation
     */
    private void PrepareBoard()
    {
        CreateBoard();
        ReadObstacles();
        ReadStart();
        ReadEnd();
    }
    
    private void CreateBoard()
    {
        int rows = _board.GetLength(0);
        int cols = rows; // just for easier reading in forloop

        for (int row = 0; row < rows; row++)
            for (int col = 0; col < cols; col++) 
                _board[row, col] = new Cell(row, col, Cell.CellType.Free);
    }

    private void ReadObstacles()
    {
        int obstacleCount = Convert.ToInt32(Console.ReadLine());

        for (int i = 0; i < obstacleCount; i++)
        {
            (int, int) coords = ReadCoordinates();
            _board[coords.Item1 - 1, coords.Item2 - 1].Type = Cell.CellType.Obstacle;
        }
    }

    private void ReadStart()
    {
        (int, int) coords = ReadCoordinates();
        _board[coords.Item1 - 1, coords.Item2 - 1].Type = Cell.CellType.Start;
        _startCell = _board[coords.Item1 - 1, coords.Item2 - 1];
    }

    private void ReadEnd()
    {
        (int, int) coords = ReadCoordinates();
        _board[coords.Item1 - 1, coords.Item2 - 1].Type = Cell.CellType.End; 
        _endCell = _board[coords.Item1 - 1, coords.Item2 - 1];
    }

    private (int, int) ReadCoordinates()
    {
        string? line = Console.ReadLine();
        string[]? lineArr = line?.Split();
        int x = Convert.ToInt32(lineArr?[0]);
        int y = Convert.ToInt32(lineArr?[1]);

        return (x, y);
    }
}