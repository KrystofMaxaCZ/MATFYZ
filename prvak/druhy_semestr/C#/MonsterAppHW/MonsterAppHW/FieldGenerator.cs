namespace MonsterAppHW;

public class FieldGenerator
{
    // will create and return a 2D field 
    public (Cell[,], Monster[]) CreateField(int monstersCount)
    {
        int width = Int32.Parse(Console.ReadLine());
        int height = Int32.Parse(Console.ReadLine());

        Cell[,] matrix = new Cell[ height, width]; // matrix of cells from input
        Monster[] monstersArr = new Monster[monstersCount]; // arr of monsters for abstraction
        int monsters = 0; // actual number of monsters 
        

        for (int row = 0; row < height; row++)
        {
            string line = Console.ReadLine();
            for (int col = 0; col < line.Length; col++)
            {
                char symbol = line[col];
                Cell cell;
                if (symbol == 'X')
                {
                    cell = new Cell(row, col, symbol, false);
                }
                else if (symbol == '.')
                {
                    cell = new Cell(row, col, symbol, true);
                }
                else
                {
                    cell = new Cell(row, col, symbol, true);
                    
                    Monster monster = new MonsterRightHand(row, col, symbol);
                    monstersArr[monsters] = monster;
                    monsters++;
                }

                matrix[row, col] = cell;
            }
        }

        return (matrix, monstersArr);
    }
}