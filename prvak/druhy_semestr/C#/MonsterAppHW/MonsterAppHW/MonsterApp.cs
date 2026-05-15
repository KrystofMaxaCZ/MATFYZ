namespace MonsterAppHW;

public class MonsterApp
{
    private Cell[,] _matrix;
    private Monster[] _monsterArr;
    public void RunApp(int repetitionsCount, int monsterCount)
    {
        FieldGenerator generator = new FieldGenerator();
        (Cell[,], Monster[]) data = generator.CreateField(monsterCount);
        _matrix = data.Item1;
        _monsterArr = data.Item2;

        for (int rep = 0; rep < repetitionsCount; rep++) OneStep();
    }

    private void OneStep()
    {
        foreach (Monster monster in _monsterArr) monster.MakeMove(_matrix);
        PrintField();
    }
    
    private void PrintField()
    {
        int rows = _matrix.GetLength(0);
        int cols = _matrix.GetLength(1);
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                Console.Write(_matrix[row, col].Symbol);
            }

            Console.WriteLine();
        }
        Console.WriteLine();
    }
}