#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

void printBoard(const vector<vector<int>>& board)
{
    // This function prints out the board given in argument

    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            cout << board[x][y] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void initializeBoard(vector<vector<int>>& board)
{
    /**
     * This initializes an empty board consisting of 0's
     */
    for(int x = 0; x < 9; ++x)
    {
        vector<int> temp;
        for(int y = 0; y < 9; ++y)
        {
             temp.push_back(0);
        }
        board.push_back(temp);
    }
}

vector<vector<int>> findLowestMRV(const vector<vector<int>>& MRVboard)
{
    /**
     * This will return a list of locations in the form of (row, column) that have the lowest
     * minimum remaining value heuristic values
     */
    vector<vector<int>> MRVs;

    int currMin = 9;

    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            if(MRVboard[row][col] != -1 && MRVboard[row][col] < currMin)
            {
                currMin = MRVboard[row][col];
            }
        }
    }

    for (int row = 0; row < 9; ++row)
    {
        for(int col = 0; col < 9; ++col)
        {
            if(MRVboard[row][col] == currMin)
            {
                vector<int> coordinates;
                coordinates.push_back(row);
                coordinates.push_back(col);
                MRVs.push_back(coordinates);
            }
        }
    }
    return MRVs;
}

template <class ForwardIterator, class T>
ForwardIterator remove (ForwardIterator first, ForwardIterator last, const T& val)
{
    ForwardIterator result = first;
    while (first!=last) {
        if (!(*first == val)) {
            if (result!=first)
                *result = move(*first);
            ++result;
        }
        ++first;
    }
    return result;
}

vector<int> findDomain(const vector<vector<int>>& board, int row, int col)
{
    /*
     * This function returns the domain of an entry within the sudoku board.
     */
    // This will find the element in board with the minimum remaining value

    vector<int> domain = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int colT = 0; colT < 9; colT++)     // Check horizontal
    {
        for (int i = 0; i < domain.size(); i++)
        {
            int currentValue = board[row][colT];
            if (currentValue == domain[i])
            {
                domain.erase(remove(domain.begin(), domain.end(), currentValue), domain.end());
                // Erases the possible value from the domain
            }
        }
    }

    for (int rowT = 0; rowT < 9; rowT++)     // Check row
    {
        for (int i = 0; i < domain.size(); i++)
        {
            int currentValue = board[rowT][col];
            if (currentValue == domain[i])
            {
                domain.erase(remove(domain.begin(), domain.end(), currentValue), domain.end());
                // Erases the possible value from the domain
            }
        }
    }

    // Calculate location of 3x3 box
    int locRow, locCol;

    // Row position
    if (row < 3)
    {
        locRow = 0;
    }
    else if (row < 6)
    {
        locRow = 3;
    }
    else
    {
        locRow = 6;
    }

    // Column position
    if (col < 3)
    {
        locCol = 0;
    }
    else if (col < 6)
    {
        locCol = 3;
    }
    else
    {
        locCol = 6;
    }

    // loop through 3x3 box
    for (int rowT = locRow; rowT < locRow + 3; rowT++)
    {
        for (int colT = locCol; colT < locCol + 3; colT++)
        {
            for (int i = 0; i < domain.size(); i++)
            {
                int currentValue = board[rowT][colT];
                if (currentValue == domain[i])
                {
                    domain.erase(remove(domain.begin(), domain.end(), currentValue), domain.end());
                    // Erases the possible value from the domain
                }
            }
        }
    }

    // size of remaining domain is the MRV
    return domain;
}

vector<vector<int>> generateMRVboard(const vector<vector<int>>& board)
{
    /**
     * This function generates a board, consisting of the values of the minimum remaining value heuristics.
     * If the value is -1, then there is already a non-empty value in the actual board.
     */
    vector<vector<int>> boardMRV;
    initializeBoard(boardMRV);

    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            vector<int> remainingPossibleValues = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            if (board[row][col] != 0)
            {
                boardMRV[row][col] = -1;
                continue;
            }
            else
            {
                vector<int> location = {row, col};
                boardMRV[row][col] = findDomain(board, location[0], location[1]).size();
            }
        }
    }

    return boardMRV;
}

int findSingleDegree(const vector<vector<int>>& board, const vector<int>& coordinates)
{
    /**
     * This function will find the degree of a single entry of the board
     */

    // coordinates[0] = row coordinate, coordinates[1] = col coordinate
    // calculate location of 3x3 box
    int locRow, locCol;

    // Horizontal position
    if (coordinates[0] < 3)
    {
        locRow = 0;
    }
    else if (coordinates[0] < 6)
    {
        locRow = 3;
    }
    else
    {
        locRow = 6;
    }

    // Calculate vertical position
    if (coordinates[1] < 3)
    {
        locCol = 0;
    }
    else if (coordinates[1] < 6)
    {
        locCol = 3;
    }
    else
    {
        locCol = 6;
    }

    int degree = 0;

    // Check 3x3 box
    for (int row = locRow; row < locRow + 3; row++)
    {
        for (int col = locCol; col < locCol + 3; col++)
        {
            if (coordinates[0] == row && coordinates[1] == col)
            {
                continue;
            }

            if (board[row][col] == 0)
            {
                degree ++;
            }
        }
    }

    // Check for row & col
    for (int row = 0; row < 9; row++)
    {
        if (board[row][coordinates[1]] == 0)
        {
            if (row < locRow || row >= locRow + 3)
            {
                degree ++;
            }
        }
    }

    for (int col = 0; col < 9; col++)
    {
        if (board[coordinates[0]][col] == 0)
        {
            if (col < locCol || col >= locCol + 3)
            {
                degree ++;
            }
        }
    }

    return degree;
}

int findDegree(const vector<vector<int>>& board, int row, int col)
{
    /**
     * This will find the degree of given row and column
     */
    int degree = 0;
    vector<int> coords = {row, col};
    degree = findSingleDegree(board, coords);
    return degree;
}

vector<int> degree(const vector<vector<int>>& board, const vector<vector<int>>& pairs)
{
    /**
     * This will find the first location of the minimum degree.
     */

    vector<vector<int>> degreeCoords;
    for(int pair = 0; pair < pairs.size(); ++pair){
        int row = pairs[pair][0];
        int col = pairs[pair][1];
        int degreeValue = findDegree(board, row, col);
        vector<int> temp;
        temp.push_back(row);
        temp.push_back(col);
        temp.push_back(degreeValue);
        degreeCoords.push_back(temp);
    }
    int minDegree = 64;
    int minDegreeLoc = 0;
    for(int y = 0; y < degreeCoords.size(); ++y)
    {
        if(degreeCoords[y][2] < minDegree)
        {
            minDegree = degreeCoords[y][2];
            minDegreeLoc = y;
        }
    }

    return degreeCoords[minDegreeLoc];
}

vector<int> getNextCoordinate(vector<vector<int>>& board)
{
    // This function will create a new board of MRV values, find the lowest of them
    // and return the coordinates of the lowest degree

    vector<vector<int>> MRVboard = generateMRVboard(board);
    vector<vector<int>> MRVPairs = findLowestMRV(MRVboard);
    vector<int> coordinateReturn = degree(board, MRVPairs);
    return coordinateReturn;
}

bool boardIsFilled(const vector<vector<int>>& board)
{
    // This function checks if every slot in the board
    // is filled, returns true if so, false otherwise

    for(int row = 0; row < board.size(); ++row)
    {
        for(int col = 0; col < board.size(); ++col)
        {
            if(board[row][col] == 0){
                return false;
            }
        }
    }
    return true;
}

bool checkNeighbors(const vector<vector<int>>& board, const vector<int>& location)
{
    // This function checks the validity fo the neighbors of the location
    // This is done by looking at the domains of the row, column, and box of location
    // If the size of a domain is 0, and hasn't been filled, it indicates failure and returns false

    // Check box neighbor's domain
    int locRow, locCol;

    // Horizontal position
    if (location[0] < 3)
    {
        locRow = 0;
    }
    else if (location[0] < 6)
    {
        locRow = 3;
    }
    else
    {
        locRow = 6;
    }

    // Calculate vertical position
    if (location[1] < 3)
    {
        locCol = 0;
    }
    else if (location[1] < 6)
    {
        locCol = 3;
    }
    else
    {
        locCol = 6;
    }

    // check 3x3 box
    for (int rowT = locRow; rowT < locRow + 3; rowT++)
    {
        for (int colT = locCol; colT < locCol + 3; colT++)
        {
            if (rowT == location[0] && colT == location[1]) // skip self
            {
                continue;
            }

            if (board[rowT][colT] == 0 && findDomain(board, rowT, colT).empty())
            {
                return false;
            }
        }
    }

    // Check for row & col
    for (int rowT = 0; rowT < 9; rowT++)
    {
        if (board[rowT][location[1]] == 0)
        {
            if (rowT < locRow || rowT >= locRow + 3)
            {
                if (board[rowT][location[1]] == 0 && findDomain(board, rowT, location[1]).empty())
                {
                    return false;
                }
            }
        }
    }

    for (int colT = 0; colT < 9; colT++)
    {
        if (board[location[0]][colT] == 0)
        {
            if (colT < locCol || colT >= locCol + 3)
            {
                if (board[location[0]][colT] == 0 && findDomain(board, location[0], colT).empty())
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool doSudoku(vector<vector<int>>& board)
{
    // This function recurses the board through the conditions and fills up the board

    if(boardIsFilled(board))            // End condition, if entire board is filled
    {
        return true;
    }

    vector<int> location = getNextCoordinate(board);        // Finds next location to fill in
    vector<int> domain = findDomain(board, location[0], location[1]);       // Finds domain of location just found


    for(int element = 0; element < domain.size(); ++element)
    {
        vector<vector<int>> newBoard = board;       // newBoard to check later if neighbors are valid

        newBoard[location[0]][location[1]] = domain[element];

        if(checkNeighbors(newBoard, location))   // Checks if neighbors of the current location is valid
        {
            board = newBoard;       // If neighbors are valid, go through and copy

            if(doSudoku(board))
            {
                return true;
            }

            board[location[0]][location[1]] = 0;
        }
    }

    return false;
}

void doFile(const string &name)
{
    // This function reads in a filename and passes it accordingly to their functions to process the board

    // ifstream to read in filename
    ifstream ifs(name);

    vector<vector<int>> board;

    // Fill board with 9x9 vector with 0's
    initializeBoard(board);

    string n;

    for (int i = 0; i < 9; i++)      // Loop to read in initial board
    {
        for (int j = 0; j < 9; j++)
        {
            ifs >> n;

            board[i][j] = stoi(n);
        }
    }

    doSudoku(board);
    printBoard(board);
}

int main()
{
    cout << "Input1.txt:" << endl;
    doFile("Input1.txt");

//    cout << "==================" << endl;
//    cout << "Input2.txt:" << endl;
//    doFile("Input2.txt");
//
//    cout << "==================" << endl;
//    cout << "Input3.txt:" << endl;
//    doFile("Input3.txt");
    return 0;
}
