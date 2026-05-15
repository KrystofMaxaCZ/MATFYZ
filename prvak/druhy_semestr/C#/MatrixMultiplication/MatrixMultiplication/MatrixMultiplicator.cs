namespace MatrixMultiplication;

public class MatrixMultiplicator
{
    private int _matrixCount;
    private List<Matrix> _matrixList;
    private int[,] _operationsMatrix;
    private int[,] _indexMatrix;


    public MatrixMultiplicator(List<Matrix> matrixList)
    {
        _matrixList = matrixList;
        _matrixCount = matrixList.Count;
        _operationsMatrix = new int[_matrixCount, _matrixCount];
        _indexMatrix = new int[_matrixCount, _matrixCount];
    }

    public void MinOperations()
    {
        FillMatrices();
        string result = GetBestBracketing(0, _matrixCount - 1);
        Console.WriteLine(result);
        Console.WriteLine(_operationsMatrix[0,_matrixCount - 1]);
    }

    private void FillMatrices()
    {
        // fill up diagonal
        for (int i = 0; i < _matrixCount; i++) {
            _operationsMatrix[i, i] = 0;
            _indexMatrix[i, i] = -1;
        }
        
        for (int chainLength = 2; chainLength <= _matrixCount; chainLength++) 
        {
            for (int start = 0; start <= _matrixCount - chainLength; start++) 
            {
                int end = start + chainLength - 1; 
                
                _operationsMatrix[start, end] = int.MaxValue; 
                
                for (int pointOfDivision = start; pointOfDivision < end; pointOfDivision++) 
                {
                    int actualOperations = _operationsMatrix[start, pointOfDivision] 
                                       + _operationsMatrix[pointOfDivision + 1, end] 
                                       + _matrixList[start].RowsCount 
                                       * _matrixList[pointOfDivision].ColsCount 
                                       * _matrixList[end].ColsCount;
                    
                    if (actualOperations < _operationsMatrix[start, end]) 
                    {
                        _operationsMatrix[start, end] = actualOperations;
                        _indexMatrix[start, end] = pointOfDivision;
                    }
                }
            }
        }
    }
    
    private string GetBestBracketing(int start, int end)
    {
        if (start == end)
        {
            return "A" + (start + 1);
        }

        int cutIndex = _indexMatrix[start, end];

        string leftCase = GetBestBracketing(start, cutIndex);
        string rightCase = GetBestBracketing(cutIndex + 1, end);
        
        return "(" + leftCase + "*" + rightCase + ")";
    }
}
