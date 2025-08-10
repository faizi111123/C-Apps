#include <iostream>
#include <string.h>
#include<time.h>
#include<windows.h>
#include <fstream>
#include <cstdlib>


using namespace std;
#define N 9

//Functions
void displayscreen(int arr[N][N]);
void checkspace(int arr[N][N]);
void update(int arr[N][N]);
void newgame(int arr[N][N]);
void displaymain(int arr[N][N]);

//checker
bool checkrow(int arr[N][N], int a, int c)
{

    for (int j = 0; j < N; j++)
    {
        if (arr[a][j] == c)
            return true;

    }
    return false;

}
bool checkcol(int arr[N][N], int b, int c)
{
    for (int i = 0; i < N; i++)
    {
        if (arr[i][b] == c)
            return true;

    }
    return false;
}
bool checkbox(int arr[N][N], int str_row, int str_col, int c)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)

            if (arr[i + str_row][j + str_col] == c)
            {

                return true;
            }


    }return false;

}
bool isvalid(int arr[N][N], int a, int b, int c)
{
    int str_row = a - a % 3, str_col = b - b % 3;
    return !checkrow(arr, a, c) && !checkcol(arr, b, c) && !checkbox(arr, str_row, str_col, c);

}
//checker finished
//checker 2
bool checkrow1(int arr[N][N], int row, int n)
{

    for (int j = 0; j < N; j++)
    {
        if (arr[row][j] == n)
            return true;

    }
    return false;

}
bool checkcol1(int arr[N][N], int column, int n)
{
    for (int i = 0; i < N; i++)
    {
        if (arr[i][column] == n)
            return true;

    }
    return false;
}
bool checkbox1(int arr[N][N], int str_row, int str_col, int n)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)

            if (arr[i + str_row][j + str_col] == n)
            {

                return true;
            }


    }return false;

}


bool isvalid1(int arr[N][N], int row, int colomn, int n)
{
    int str_row = row - row % 3, str_col = colomn - colomn % 3;
    return !checkrow1(arr, row, n) && !checkcol1(arr, colomn, n) && !checkbox(arr, str_row, str_col, n);

}
//checker 2 finished

void sudokuGrid(int arr[N][N])
{ 

    for (int i = 0; i < 5; i++)
    {
        cout << "  __";

    }cout << endl;
    for (int row = 0; row < N; row++)
    {
       

      
        for (int col = 0; col < N; col++)
        {

            if (col==0||col == 3 || col == 6)
            {
                cout << "|";
                

            }
        
         


            cout << arr[row][col] << " ";

        }
        cout << "|";


       if (row == 2 || row == 5)
        {
           
           cout << endl;
            for (int i = 0; i < 5; i++)
            {
                cout << "  __";
            }
        }
        cout << endl;
    }
    for (int i = 0; i < 7; i++)
    {
        cout << " __";

    }
    cout << endl;
}

void update(int arr[N][N], int arr1[N][N], int& count)
{
    int n, row = 1, colomn = 1;
   

    cout << "Enter Number : ";
    cin >> n;
  
    if (n == -1)
    {
        system("cls");
        int b;
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        cout << "Enter 1 to save to slot 1\nEnter 2 to save to slot 2\nEnter 3 to save to slot 3\nEnter your choice : ";
        SetConsoleTextAttribute(h, 9);
        cin >> b;
        if (b == 1)
        {
            fstream game1;
            game1.open("game1.txt", ios::out);
            if (game1.is_open())
            {
                for (int row = 0; row < N; row++)
                {
                    for (int col = 0; col < N; col++)
                    {
                        game1 << arr[row][col] << " ";
                    }
                    game1 << endl;
                }
            }
            game1.close();
            fstream grid1;
            grid1.open("grid1.txt", ios::out);
            if (grid1.is_open())
            {
                for (int row = 0; row < N; row++)
                {
                    for (int col = 0; col < N; col++)
                    {
                        grid1 << arr1[row][col] << " ";
                    }
                    grid1 << endl;
                }
            }
            grid1.close();
        }
        else if (b == 2)
        {
            fstream game2;
            game2.open("game2.txt", ios::out);
            if (game2.is_open())
            {
                for (int row = 0; row < N; row++)
                {
                    for (int col = 0; col < N; col++)
                    {
                        game2 << arr[row][col] << " ";
                    }
                    game2 << endl;
                }
            }
            game2.close();
            fstream grid2;
            grid2.open("grid2.txt", ios::out);
            if (grid2.is_open())
            {
                for (int row = 0; row < N; row++)
                {
                    for (int col = 0; col < N; col++)
                    {
                        grid2 << arr1[row][col] << " ";
                    }
                    grid2 << endl;
                }
            }
            grid2.close();
        }
        else if (b == 3)
        {
            fstream game3;
            game3.open("game3.txt", ios::out);
            if (game3.is_open())
            {
                for (int row = 0; row < N; row++)
                {
                    for (int col = 0; col < N; col++)
                    {
                        game3 << arr[row][col] << " ";
                    }
                    game3 << endl;
                }
            }
            game3.close();
            fstream grid3;
            grid3.open("grid1.txt", ios::out);
            if (grid3.is_open())
            {
                for (int row = 0; row < N; row++)
                {
                    for (int col = 0; col < N; col++)
                    {
                        grid3 << arr1[row][col] << " ";
                    }
                    grid3 << endl;
                }
            }
            grid3.close();
        }
        else
        {
            cout << "Incorrect slot entered\n";
        }
    }
    else if (n == -2)
    {
        count = -2;
    }
    else if (n > 0 && n < 10 && row > 0 && row < 10 && colomn < 10 && colomn > 0)
    {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(h, 8);
        cout << "Enter Position" << endl;
        SetConsoleTextAttribute(h, 10);
        cout << "Row : " << endl;
        cin >> row;
        SetConsoleTextAttribute(h, 10);
        cout << "Colomn : ";
        cin >> colomn;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                {
                    if (arr1[row][colomn] != arr[row][colomn])
                    {
                        if (isvalid(arr, row, colomn, n))
                            arr[row][colomn] = n;
                    }
                }
            }
        }
    }
}

void checkspace(int arr[N][N], int arr1[N][N], int count)
{
    count = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (arr[i][j] == 0)
            {
                count++;
            }
        }
    }
    cout << "Total Number of empty space : " << count << "\n";
}

void newgame()
{

    int arr[N][N] = { 0},
        arr1[N][N] = { 0 }, count = 1;
    srand(time(0));
   
    for (int i = 0; i < 150; i++)
    {
        int a = rand() % 10, b = rand() % 10, c = rand() % 10;

        if (arr[a][b] == 0 && a > 0 && b > 0)
        {
            if (isvalid(arr, a, b, c))
            {


                arr[a][b] = c;
            }
            
           
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            arr1[i][j] = arr[i][j];

            {
                if (arr1[i][j] == 0)
                {
                    arr1[i][j] = -1;
                }
            }
        }
    }
    while (count != 0)
    {
        system("cls");
        sudokuGrid(arr);
        checkspace(arr, arr1, count);
        update(arr, arr1, count);
        {
            if (count == -2)
            {
                break;
            }
        }
    }
    if (count == 0)
    {
        cout << "Congragulations! You have won the game\n";
    }
}

void loadgame()
{
    system("cls");
    int x;
    int arr[N][N], arr1[N][N];
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    cout << "Enter 1 to load slot 1\nEnter 2 to load slot 2\nEnter 3 to load slot 3\nEnter your choice : ";
    SetConsoleTextAttribute(h, 7);
    cin >> x;
    if (x == 1)
    {
        fstream game1;
        game1.open("game1.txt", ios::in);
        if (game1.is_open())
        {
            for (int row = 0; row < N; row++)
            {
                for (int col = 0; col < N; col++)
                {
                    game1 >> arr[row][col];
                }
                game1 << endl;
            }
        }
        else
        {
            cout << "File not found";
        }
        game1.close();
        fstream grid1;
        grid1.open("grid1.txt", ios::in);
        if (grid1.is_open())
        {
            for (int row = 0; row < N; row++)
            {
                for (int col = 0; col < N; col++)
                {
                    grid1 >> arr1[row][col];
                }
                grid1 << endl;
            }
        }
        else
        {
            cout << "File not found";
        }
        grid1.close();
    }
    else if (x == 2)
    {
        fstream game2;
        game2.open("game2.txt", ios::in);
        if (game2.is_open())
        {
            for (int row = 0; row < N; row++)
            {
                for (int col = 0; col < N; col++)
                {
                    game2 >> arr[row][col];
                }
                game2 << endl;
            }
        }
        else
        {
            cout << "File not found";
        }
        game2.close();
        fstream grid2;
        grid2.open("grid2.txt", ios::in);
        if (grid2.is_open())
        {
            for (int row = 0; row < N; row++)
            {
                for (int col = 0; col < N; col++)
                {
                    grid2 >> arr1[row][col];
                }
                grid2 << endl;
            }
        }
        else
        {
            cout << "File not found";
        }
        grid2.close();
    }
    else if (x == 3)
    {
        fstream game3;
        game3.open("game3.txt", ios::in);
        if (game3.is_open())
        {
            for (int row = 0; row < N; row++)
            {
                for (int col = 0; col < N; col++)
                {
                    game3 >> arr[row][col];
                }
                game3 << endl;
            }
        }
        else
        {
            cout << "File not found";
        }
        game3.close();
        fstream grid3;
        grid3.open("grid3.txt", ios::in);
        if (grid3.is_open())
        {
            for (int row = 0; row < N; row++)
            {
                for (int col = 0; col < N; col++)
                {
                    grid3 >> arr1[row][col];
                }
                grid3 << endl;
            }
        }
        else
        {
            cout << "File not found";
        }
        grid3.close();
    }
    else
    {
        cout << "Game doesnt exist";
    }
    int count = 1;
    while (count != 0)
    {
        system("cls");
        sudokuGrid(arr);
        checkspace(arr, arr1, count);
        update(arr, arr1, count);
        {
            if (count == -2)
            {
                break;
            }
        }
    }
    if (count == 0)
    {
        cout << "Congragulations! You have won the game\n";
    }
}

void instructions()
{
    int y;

    system("cls");
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 9);
    cout << "$$$$$$$$$$$$$$$$ SUDOKU RULES $$$$$$$$$$$$$$$$" << endl;
    SetConsoleTextAttribute(h, 4);
    cout << "\nFill in the numbers 1 - 9\nNumber should not repeat in any row\nNumber should not repeat in any colomn\nNumber should not repeat in its 3x3 region\n";
    SetConsoleTextAttribute(h, 6);
    cout << "Enter '-1' at any point to save progress\nPress '-2' at any point to exit game\n";
    SetConsoleTextAttribute(h, 2);
    cout << "Enter the color in which  you want to play sudoku\n";
    cout << "0:Black  1:BlueBlue  2:Green  3:Aqua  4:Red  5:Purple  7:White  8:Gray  9:Light  10:LightGreen  11:Light Aqua  12:Light Red  14:Light Yellow  15:Bright White" << endl;
    cin >> y;

    SetConsoleTextAttribute(h, y);

}

void displaymain()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 11);
    cout << "----------------------------{WELCOME TO SUDOKU}---------------------------" << endl;
    cout << endl;


    SetConsoleTextAttribute(h, 8);
    char x = 'x';

    SetConsoleTextAttribute(h, 14);
    cout << "Enter 'c' to create new game\n";
    SetConsoleTextAttribute(h, 2);
    cout << "Enter 'l' to load previous game\n";
    SetConsoleTextAttribute(h, 12);
    cout << "Enter 'q' to quit game\n";
    while (x != 'c' || x != 'l' || x != 'q')
    {
        SetConsoleTextAttribute(h, 9);
        cout << "Enter your choice : ";
        SetConsoleTextAttribute(h, 7);
        cin >> x;
        if (x == 'c')
        {
            instructions();
            newgame();
            break;
        }
        else if (x == 'l')
        {
            instructions();
            loadgame();
            break;
        }
        else if (x == 'q')
        {
            cout << "Thank you ! Hope you enjoyed\n";
            break;
        }
    }
  
}

int main()
{
    displaymain();
}