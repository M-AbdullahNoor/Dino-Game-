#include <iostream>
#include <conio.h>   
#include <fstream>
#include <windows.h> 
#include <ctime>     

using namespace std;

const int width = 70;  
const int height = 25; 
int dinoY;             
bool onRoof = false;   
int floorObstacles[1]; 
int roofObstacles[1];  
int powerUpX = -1;  
int high=0;
int large=0;   
fstream score_data;
int score = 0;         
int speed = 200;       
int lives = 3;         
bool hasImmunity = false;
int immunityTimer = 0; 
bool gameOver = false; 

char dino[3][5] = {
    {' ', ' ', 'O', ' ', ' '},
    {' ', '/', '|', '\\', ' '},
    {' ', '/', ' ', '\\', ' '},
};


const int dinoHeight = 3;
const int dinoWidth = 5;


char obstacle[3][8] = {
    {' ', '+', '-', '-', '-', '-', ' ', '+'},
    {' ', '|', '#', '#', '#', '#', '#', '|'},
    {' ', '+', '-', '-', '-', '-', ' ', '+'},
};

const int obstacleHeight = 3;
const int obstacleWidth = 8;

void Setup() {
    dinoY = height - dinoHeight - 2; 
    onRoof = false;
    for (int i = 0; i < 1; i++) {
        floorObstacles[i] = width - (20 * i + 15); 
        roofObstacles[i] = width - (25 * i + 40);
    }
    powerUpX = -1; 
    score = 0;
    speed = 20;
    lives = 3;
    hasImmunity = false;
    immunityTimer = 0;
    gameOver = false;
    srand(time(0)); 
}

void Draw() {
    system("cls"); 
    
 
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            bool printed = false;

          
            if (i >= dinoY && i < dinoY + dinoHeight && j >= 2 && j < 2 + dinoWidth) {
                cout << dino[i - dinoY][j - 2]; // Green Dino
                printed = true;
            }
          
            for (int k = 0; k < 1; k++) {
                if (i >= height - obstacleHeight - 2 && i < height - 2 &&
                    j >= floorObstacles[k] && j < floorObstacles[k] + obstacleWidth) {
                    cout << obstacle[i - (height - obstacleHeight - 2)][j - floorObstacles[k]]; // Red obstacles
                    printed = true;
                }
            }

          
            for (int k = 0; k < 1; k++) {
                if (i >= 1 && i < 1 + obstacleHeight &&
                    j >= roofObstacles[k] && j < roofObstacles[k] + obstacleWidth) {
                    cout << obstacle[i - 1][j - roofObstacles[k]]; // Red obstacles
                    printed = true;
                }
            }

           
            if (powerUpX >= 0 && i == dinoY + 1 && j == powerUpX) {
                cout << 'P'; 
                printed = true;
            }

            if (!printed) {
                
                if (i == 0 || i == height - 1) {
                    cout << "#";
                    
                } else {
                    cout << " ";
                }
            }
        }
        cout << endl;
    }

    
    cout << "Score: " << score << "  Lives: " << lives<< " High Score: "<<high;
    if (hasImmunity)
        cout << "                    (IMMUNE)";
    cout << endl;
}

void Menu() {
    system("cls");
    cout << "========================" << endl;
    cout << "    DINO RUN GAME!    " << endl;
    cout << "========================" << endl;
    cout << "Press P to Play" << endl;
    cout << "Press E to Exit" << endl;

    while (true) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'p' || key == 'P') {
                return; 
            } else if (key == 'e' || key == 'E') {
                exit(0); 
            }
        }
    }
}

void GameOverScreen() {
    system("cls");
    cout << "========================" << endl;
    cout << "      GAME OVER!      " << endl;
    cout << "========================" << endl;
    cout << "Final Score: " << score << endl;
    Sleep(3000);
}

void Input() {
    if (_kbhit()) { 
        char key = _getch();
        if (key == 'w' && !onRoof) {
            onRoof = true;  
            dinoY = 1;      
        } else if (key == 's' && onRoof) {
            onRoof = false; 
            dinoY = height - dinoHeight - 2; 
        }
    }
}

void Logic() {
    
    for (int i = 0; i < 1; i++) {
        floorObstacles[i]--;
        if (floorObstacles[i] < 0) {
            floorObstacles[i] = width - 1;
            score++;
        }

        roofObstacles[i]--;
        if (roofObstacles[i] < 0) {
            roofObstacles[i] = width - 1;
            score++;
        }
    }

    
    if (powerUpX >= 0) {
        powerUpX--;
        if (powerUpX < 0 && rand() % 10 < 2) {
            powerUpX = width - 1; 
        }
    } else if (rand() % 20 == 0) { 
        powerUpX = width - 1;
    }

   
    if (!hasImmunity) {
        for (int i = 0; i < 1; i++) {
            if (!onRoof && floorObstacles[i] <= 6 && floorObstacles[i] + obstacleWidth >= 2) {
                lives--;
                hasImmunity = true; 
                immunityTimer = 5; 
            }
            if (onRoof && roofObstacles[i] <= 6 && roofObstacles[i] + obstacleWidth >= 2) {
                lives--;
                hasImmunity = true;
                immunityTimer = 5;
            }
        }
    }

    
    if (powerUpX >= 2 && powerUpX < 7 && ((onRoof && dinoY == 1) || (!onRoof && dinoY == height - dinoHeight - 2))) {
    score += 5; 
    hasImmunity = true;
    immunityTimer = 10; 
    powerUpX = -1;      
}

    
    if (hasImmunity) {
        immunityTimer--;
        if (immunityTimer <= 0) {
            hasImmunity = false;
        }
    }

    
    if (score % 10 == 0 && speed > 50) {
        speed -= 5; 
    }
      if (score>large)
      {
        large=score;
      }
      if(large >high )
      {
        high =large;
      }
    
    if (lives <= 0) {
        ofstream score_data("scorefile.txt");
        score_data<< high;
        score_data.close();

        gameOver = true;
    }
}
void read()
{
    score_data.open("scorefile.txt", ios ::in);
    score_data >> high;
    score_data.close();
}
int main() {
    read();
    Menu();
    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(speed); 
    }

    GameOverScreen();
    return 0;
}

