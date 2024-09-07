#include <iostream>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#include <cstdlib> 
#include <ctime> 
using namespace std;
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_RIGHT 77
#define KEY_LEFT 75
#pragma comment(lib,"winmm.lib")
bool gameOver;
const int width = 40;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail,sleep_value;
string username;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirecton dir;
char snakeDesign = 'o',Ans;

void PlayIntroSound() {
    PlaySound(TEXT("snake_game\\music\\intro.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void StopIntroSound() {
    PlaySound(NULL, NULL, SND_PURGE);
}
void eatSound() {
    PlaySound(TEXT("snake_game\\music\\eat.wav"), NULL, SND_FILENAME | SND_ASYNC);
}
void loseSound(){
	PlaySound(TEXT("snake_game\\music\\lose.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void showIntro() {
    PlayIntroSound();
    cout << "\n\n\n\n";
    cout << "\t\t\t\t#############################################" << endl;
    cout << "\t\t\t\t#                                           #" << endl;
    cout << "\t\t\t\t#           WELCOME TO SNAKE GAME           #" << endl;
    cout << "\t\t\t\t#                                           #" << endl;
    cout << "\t\t\t\t#############################################" << endl;
    cout << "\n\n";
    cout << "          Eat the fruit and grow your tail!" << endl;
    cout << "          Avoid running into the walls or yourself!" << endl;

    cout << "\n\n";
    cout << "          Controls:" << endl;
    cout << "          /\\ - Move Up" << endl;
    cout << "          \\/ - Move Down" << endl;
    cout << "          < - Move Left" << endl;
    cout << "          > - Move Right" << endl;

    cout << "\n\n";
    cout << "      Press any key to start the game...";
    getch();

}
void lose(){
		loseSound();
	cout << "\n\n";
    cout << "\t\t\t\t#############################################" << endl;
    cout << "\t\t\t\t#                                           #" << endl;
    cout << "\t\t\t\t#               You lost                    #" << endl;
    cout << "\t\t\t\t#                                           #" << endl;
    cout << "\t\t\t\t#############################################" << endl;
    cout<<username<<", Do you want to play again?(y/n):";
    cin>>Ans;
}
void DifficultyMenu() {
    system("cls");
    cout<<"Enter your Username:";
    cin>>username;
    int mode;
    cout << "\n\n\n\n\n\t\t\t\t\t+-------------------------------+" << endl;
    cout << "\t\t\t\t\t|        Select Difficulty      |" << endl;
    cout << "\t\t\t\t\t+-------------------------------+" << endl;
    cout << "\t\t\t\t\t|                               |" << endl;
    cout << "\t\t\t\t\t| 1. Easy      - Slow speed     |" << endl;
    cout << "\t\t\t\t\t| 2. Medium    - Medium speed   |" << endl;
    cout << "\t\t\t\t\t| 3. Hard      - Fast speed     |" << endl;
    cout << "\t\t\t\t\t|                               |" << endl;
    cout << "\t\t\t\t\t+-------------------------------+" << endl;
    cout << "\t\t\t\t\t| Enter your choice (1/2/3)     |" << endl;
    cout << "\t\t\t\t\t+-------------------------------+::" ;
    cin >> mode;
    switch (mode) {
        case 1:
            cout << "\n\n\n\n\n\t\t\t\t\tEasy mode selected." << endl;
            sleep_value=100;
            break;
        case 2:
            cout << "\n\n\n\n\n\t\t\t\t\tMedium mode selected." << endl;
            sleep_value=50;
            break;
        case 3:
            cout << "\n\n\n\n\n\t\t\t\t\tHard mode selected." << endl;
            sleep_value=10;
            break;
        default:
            cout << "\n\n\n\n\n\t\t\t\t\tInvalid choice." << endl;
            Sleep(2000);
            DifficultyMenu();
            return;
    }
    Sleep(1000);
}

void SnakeDesignMenu() {
    system("cls");
    int choice;
    cout << "\n\n\n\n\n\t\t\t\t\t+-------------------------------+" << endl;
    cout << "\t\t\t\t\t|        Select Snake Design    |" << endl;
    cout << "\t\t\t\t\t+-------------------------------+" << endl;
    cout << "\t\t\t\t\t|                               |" << endl;
    cout << "\t\t\t\t\t| 1. ooooO   - Classic Snake    |" << endl;
    cout << "\t\t\t\t\t| 2. *****O  - Star Snake       |" << endl;
    cout << "\t\t\t\t\t|                               |" << endl;
    cout << "\t\t\t\t\t+-------------------------------+" << endl;
    cout << "\t\t\t\t\t| Enter your choice (1/2):      |" << endl;
    cout << "\t\t\t\t\t+-------------------------------+::";
    cin >> choice;
    switch (choice) {
        case 1:
            snakeDesign = 'o'; 
            cout << "\n\n\n\n\n\t\t\t\t\tClassic Snake design selected." << endl;
            break;
        case 2:
            snakeDesign = '*'; 
            cout << "\n\n\n\n\n\t\t\t\t\tStar Snake design selected." << endl;
            break;
        default:
            cout << "\n\n\n\n\n\t\t\t\t\tInvalid choice." << endl;
            Sleep(2000);
            SnakeDesignMenu();
            return;
    }
    Sleep(1000);
    StopIntroSound();
}

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    srand(time(0));
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Draw() {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0});
    for (int i = 0; i < width + 2; i++)
        cout << ".";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << ".";
            if (i == y && j == x)
            cout<<'O';
            else if (i == fruitY && j == fruitX)
                cout << "@";
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << snakeDesign;
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }
            if (j == width - 1)
                cout << ".";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << ".";
    cout << endl;
    cout<<"Username:"<<username<<endl;
    cout << "Score:" << score << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case KEY_LEFT:
            dir = LEFT;
            break;
        case KEY_RIGHT:
            dir = RIGHT;
            break;
        case KEY_UP:
            dir = UP;
            break;
        case KEY_DOWN:
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        default:
            break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }
    if (x >= width || x < 0 || y >= height || y < 0)
        gameOver = true;
    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    if (x == fruitX && y == fruitY) {
    	eatSound();
    	Sleep(0);
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        if (nTail < 100) {
            nTail++;
        }
    }
}
   void HideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Set the cursor visibility to FALSE
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
void ShowCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE; // Set the cursor visibility to TRUE
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
void ResetGame() {
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
    gameOver = false;
}


int main() {
StartOver:
		ResetGame();
        system("cls");
        showIntro();
        DifficultyMenu();
        SnakeDesignMenu();
        system("cls");
        Setup();
        HideCursor();
        while (!gameOver) {
            Draw();
            Input();
            Logic();
            Sleep(sleep_value);
        }
        lose();
        if(Ans == 'y' || Ans == 'Y')
        goto StartOver;
        system("cls");
    ShowCursor();
    return 0;
}

