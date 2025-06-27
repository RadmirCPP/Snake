// Snake.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>
#include <random>
#include <Windows.h>
enum Direction
{
    UP, DOWN, LEFT, RIGHT
    
};
void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; 
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x; coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void GreetingsMenu() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int col, row;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        int cenX = col / 2;
        int cenY = row / 2;
        gotoxy(cenX, cenY);
        std::cout << "Welcome to the Snake Game";
        gotoxy(cenX, cenY+1);
        std::cout << "press any key to start";
        std::cin.get();
        gotoxy(cenX, cenY);
        std::cout << "                          ";
        gotoxy(cenX, cenY + 1);
        std::cout << "                          ";
    }
    else { std::cout << "Error"; }
}
void static fieldDraw(char** field, int const Height, int const Width) {

    gotoxy(0, 0);  // Переместить курсор в левый верхний угол
    for (int i = 0; i < Height; ++i) {
        for (int j = 0; j < Width; ++j)
            std::cout << field[i][j];
        std::cout << '\n';
    }
}
std::pair<int, int> randEmptyCell(char** field, int const Height, int const Width) {
    
    srand(time(0));
    std::vector < std::pair <int, int > > AvaliableCells;
    for (int y = 1; y < Height - 1; y++) {
        for (int x = 1; x < Width - 1; x++) {
            if (field[y][x] == ' ')
                AvaliableCells.push_back({ x,y });
        }
    }
    if (AvaliableCells.empty()) { fieldDraw(field, Height, Width); gotoxy(Width / 2, Height + 1); std::cout << "You win!"; return { -1 , -1 }; }
    int randIndex = rand() % AvaliableCells.size();
  
    
    return AvaliableCells[randIndex];
}
void slowDown(int Mseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(Mseconds));
}
void fieldFill(char** field, int const Height, int const Width) {

    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            field[i][j] = ' ';

        }

    }
    for (int i = 0; i < Height; i++) {
        field[i][0] = '#';
        field[i][Width - 1] = '#';
        for (int j = 0; j < Width; j++) {


            field[0][j] = '#';
            field[Height - 1][j] = '#';
        }
    }
}
bool berriesSpawn(char** field, int const Height, int const Width) {
    char berry = '*'; 
    std::pair<int, int> CellLocation = randEmptyCell(field,Height,Width);
    if (CellLocation.first != -1)
    {
        
        field[CellLocation.second][CellLocation.first] = berry;
        return 0;
    }
    else {
        return 1;
    }
    
    

}
void SnakeMove(char** field, int const &Height, int const &Width, int &snakeX, int &snakeY, char snake) {
    int changeX = 1;
    int changeY = 1;
    int newX=snakeX;
    int newY=snakeY;
    char body = 'o';
    bool isBerryEaten = false;

    bool isEnd = false;
    static std::vector<std::pair<int, int>> bodyCord;
     std::pair<int, int> bodyTest;
    
    Direction dir = RIGHT;
        char key='d';
    while (true) {
    if (_kbhit()) {
        while (_kbhit()) key = _getch();
        if      (key == 'w' && dir!=DOWN)   dir = UP;
        else if (key == 's' && dir != UP) dir = DOWN;
        else if (key == 'a' && dir != RIGHT) dir = LEFT;
        else if (key == 'd' && dir != LEFT) dir = RIGHT;
    }
    if (dir == UP) newY--;
    else if (dir == DOWN) newY++;
    else if (dir == LEFT) newX--;
    else if (dir == RIGHT) newX++;
        if (field[newY][newX] == '#' || field[newY][newX]=='o'|| isEnd)
        {
            fieldDraw(field, Height, Width);
            break;
        }
        if (field[newY][newX] == '*') {
            
            bodyTest = { snakeY,snakeX };
            isBerryEaten = true;
            isEnd = berriesSpawn(field, Height, Width);
        }
     
            bodyCord.push_back({ snakeY, snakeX });
            for (int i = 0; bodyCord.size() > i; i++)
            {

                field[bodyCord[i].first][bodyCord[i].second] = body;

            }
            if (isBerryEaten) {
                
                for(int i=0; bodyCord.size() > i;i++)
                {

                    field[bodyCord[i].first][bodyCord[i].second] = body;
                    
                }
            
            
             }
        
            if (!isBerryEaten) {
                

                field[bodyCord.front().first][bodyCord.front().second] = ' ';
                bodyCord.erase(bodyCord.begin());
              }
        
        
           
        
        if (dir == UP) {//upKey
            
            snakeY = newY;

            field[snakeY][snakeX] = snake; 
        }
        else if (dir == DOWN) {//down
            
            snakeY = newY;
            field[snakeY][snakeX] = snake;
        }
        else if (dir == LEFT) {//left
            
            snakeX = newX;
            field[snakeY][snakeX] = snake;
        }
        else if (dir == RIGHT) {//right
            
            snakeX = newX;
        field[snakeY][snakeX] = snake;

        }
        isBerryEaten = false;
        fieldDraw(field, Height, Width);
        if (isEnd) {
            slowDown(500);
            break;
        }
        slowDown(200);
    }
}
int main()
{
    bool isStart = true;
    const int Width = 50;
    const int Height = 25;
    int snakeX = Width / 2;
    int snakeY = Height / 2;
    char** field = new char*[Height];
    
    for (int i = 0; i < Height; i++) {
        field[i] = new char[Width];
    }
    


        fieldFill(field, Height, Width);
    hideCursor();
    char snake = '@';
    field[snakeY][snakeX] = snake;
    GreetingsMenu();
    berriesSpawn(field, Height, Width);
    SnakeMove(field, Height, Width, snakeX, snakeY, snake);
    
    for (int i = 0; i < Height; i++) {
        delete[] field[i];
    }
    delete[] field;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
