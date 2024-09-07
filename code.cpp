//Author:Niraj Thapa
//Code for snake game in C++
//If the music is not running click the project options and in the parameters add -lwinmm in linker section
#include <iostream>//for input output instructions
#include <fstream>//for file handling
#include <conio.h>//for getch instructions
#include <windows.h>//for music
#include <mmsystem.h>//for music
#include <cstdlib> //to use srand
#include <ctime> //to use time for random value generation
using namespace std;
#define KEY_UP 72//for using the arrow keys for snake movement
#define KEY_DOWN 80
#define KEY_RIGHT 77
#define KEY_LEFT 75
#pragma comment(lib,"winmm.lib")//for music files
const int width = 40;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail, sleep_value;
string username,uname;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirecton dir;
char snakeDesign = 'o', Ans;
bool gameOver;
int highscore;
//The game intro sound while it displays the instructions
void PlayIntroSound() {
    PlaySound(TEXT("music\\intro.wav"), NULL, SND_FILENAME | SND_ASYNC);
}
//To stop the intro sound during the game play
void StopIntroSound() {
    PlaySound(NULL, NULL, SND_PURGE);
}
//To produce eating sound when the snake eats the food
void eatSound() {
    PlaySound(TEXT("music\\eat.wav"), NULL, SND_FILENAME | SND_ASYNC);
}
// Sound when you lose
void loseSound(){
	PlaySound(TEXT("music\\lose.wav"), NULL, SND_FILENAME | SND_ASYNC);
}
//Sound for highscore
void HighscoreSound() {
    PlaySound(TEXT("music\\highscore.wav"), NULL, SND_FILENAME | SND_ASYNC);
}
//Load the highscore before gameplay
void loadHighscore() {
    ifstream in("highscore.txt");
    if (in.is_open()) {
        in >> highscore;
        in.ignore(); // Ignore the newline after the highscore
        getline(in, uname); // Read the entire line for uname
        in.close();
    } else {
        highscore = 0;
        uname = "none";
    }
}
//Save the highscore if achieved
void saveHighscore() {
	HighscoreSound();
	highscore=score;
	uname=username;
	system("cls");
    cout << "\t\t\t\t#############################################" << endl;
    cout << "\t\t\t\t#                                           #" << endl;
    cout << "\t\t\t\t#               NEW HIGHSCORE               #" << endl;
    cout << "\t\t\t\t#                                           #" << endl;
    cout << "\t\t\t\t#############################################" << endl;
    ofstream out("highscore.txt");
    if (out.is_open()) {
         out << highscore;
         out<<" ";
out << uname; // Write highscore and username on separate lines
        out.close();
    } else {
        cerr << "Error opening file for writing!" << endl;
    }
       cout<<username<<", Do you want to play again?(y/n):";
    cin>>Ans;
}
//To display the Introduction and instructions
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

//To display that you have lost the game
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
//To print the difficulty menu
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
//To ask the user for the type of snake
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
//To setup the game to initial value before starting
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
//To draw the snake and the board while the play is going on
void Draw() {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0});//To position the cursor to the top left of the console
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
    cout<<"Username:"<<username<<endl;//printing the username with the present score
    cout << "Score:" << score << endl;
    cout<<"Highscore:"<< highscore<<endl;
    	cout<<"Highscore by:"<<uname<<endl;
}
//Taking the keyboard arrow input
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
//The logic to play the game and make the snake tail follow the head
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
//To hide the cursor during gameplay
   void HideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Set the cursor visibility to FALSE
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
//To show the cursor after game finishes
void ShowCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE; // Set the cursor visibility to TRUE
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
//To reset the game if they want to play again
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
    loadHighscore();  
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
    if(score>highscore)
    saveHighscore();
    else
    lose();
    if (Ans == 'y' || Ans == 'Y')
        goto StartOver;
    system("cls");
    ShowCursor();
    return 0;
}

