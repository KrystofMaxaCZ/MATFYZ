namespace MatrixMultiplication;

public class Matrix
{
    public int RowsCount { get; private set; }
    public int ColsCount { get; private set; }

    public Matrix(int rows, int cols)
    {
        RowsCount = rows;
        ColsCount = cols;
    }

    public override string ToString()
    {
        return "RowsCount: " + RowsCount + "; ColsCount: " + ColsCount +  ";";
    }
}