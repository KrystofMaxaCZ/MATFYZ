namespace MatrixMultiplication;

class Program
{
    static void Main(string[] args)
    {
        MatrixMultiplicator mm = new MatrixMultiplicator(ReadInput("mch.in"));
        mm.MinOperations();
    }
    
    public static List<Matrix> ReadInput(string filePath)
    {
        List<Matrix> matrixArr = new List<Matrix>();

        using var reader = new StreamReader(filePath);
        
        int matrixCount = Convert.ToInt32(reader.ReadLine()) - 1;
        
        int rows = Convert.ToInt32(reader.ReadLine());
        for(int i = 0; i<matrixCount; i++)
        {
            int cols = Convert.ToInt32(reader.ReadLine());

            Matrix A = new Matrix(rows, cols);
            matrixArr.Add(A);
            rows = cols;
        }
        
        // foreach( Matrix matrix in matrixArr)
        // {
        //     Console.WriteLine(matrix);    
        // }

        return matrixArr;
    }
}