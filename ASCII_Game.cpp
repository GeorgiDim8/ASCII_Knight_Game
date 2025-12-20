#include<iostream>
#include<ctime>
#include<conio.h>
#include<cstdlib>
#include<windows.h>


char input;
bool gameover = false;
bool levelwon = false;
const int A_HEIGHT = 20;
const int A_WIDTH = 70;
const int ENEMY_MAX = 10;
int EnemyCount = 0;
char arenaT[A_HEIGHT][A_WIDTH] = {};
char arena[A_HEIGHT][A_WIDTH] = {};
int PlatformISave[10] = {};
int PlatformJSave[10] = {};

int* EnemyX = new int[10];
int* EnemyY = new int[10];
int* EnemyVelX = new int[10];
int* EnemyVelY = new int[10];
char* EnemyType = new char[10];
int* EnemyHP = new int[10];

int* player = new int[10];
enum PlayerTypes { PX, PY, PHP, PFALL, PDOUBLEJ };

void InitializeArena();
void InitializePlayer();
void InitializeEnemies();
void AddEnemy(int, int, char, int, int, int);
void KillEnemy(int);
void UpdateEnemies();
bool EnemyCollisionCheck(int);

void UpdateWalker(int);
void UpdateJumper(int);
void UpdateFlier(int);
void UpdateCrawler(int);
//void UpdateBoss(int);

void CharacterControl();
void Graphics();
void RandomPlatforms();
void Arena_Template();
void ResetCursor();
void HideCursor();
void ColorCharacters();
void ColorAttribute(int x, int y, char sym);
void Check_Fall(int, int);
int  Jump_Max_Height(int, int, int);



void InitializeEnemies()
{
    EnemyX = new int[ENEMY_MAX];
    EnemyY = new int[ENEMY_MAX];
    EnemyVelX = new int[ENEMY_MAX];
    EnemyVelY = new int[ENEMY_MAX];
    EnemyType = new char[ENEMY_MAX];
    EnemyHP = new int[ENEMY_MAX];

    AddEnemy(A_HEIGHT - 2, A_WIDTH - 2, 'E', 1, 0, 1);

    for(int i = 0; i < 10; i++) {
        if (!PlatformISave[i])break;
        if(PlatformISave[i]-1>3)AddEnemy(PlatformISave[i] - 1, PlatformJSave[i]+1, 'E', 1, 0, 1);

    }

    AddEnemy(A_HEIGHT - 2, A_WIDTH - 2, 'C', 1, 1, 0);
    AddEnemy(A_HEIGHT - 3, 1, 'C', 1, 1, 0);

    AddEnemy(A_HEIGHT - 2, 4, 'J', 1, 0, 1);
    AddEnemy(2, 4, 'F', 1, 0, 1);

}

void AddEnemy(int x, int y, char T, int hp, int vx, int vy)
{
    if (EnemyCount > ENEMY_MAX)return;
    EnemyX[EnemyCount] = x;
    EnemyY[EnemyCount] = y;
    EnemyType[EnemyCount] = T;
    EnemyHP[EnemyCount] = hp;
    EnemyVelX[EnemyCount] = vx;
    EnemyVelY[EnemyCount] = vy;
    EnemyCount++;
}

void KillEnemy(int index)
{
    if (index >= ENEMY_MAX || index < 0)return;
    int last = EnemyCount - 1;
    EnemyX[index] = EnemyX[last];
    EnemyY[index] = EnemyX[last];
    EnemyType[index] = EnemyX[last];
    EnemyHP[index] = EnemyX[last];
    EnemyVelX[index] = EnemyVelX[last];
    EnemyVelY[index] = EnemyVelY[last];

    EnemyCount--;

}

void UpdateEnemies()
{
    for (int i = 0; i < EnemyCount; i++)
    {
        char x = EnemyType[i];
        if (x == 'E')UpdateWalker(i);
        else if (x == 'J')UpdateJumper(i);
        else if (x == 'F')UpdateFlier(i);
        else if (x == 'C')UpdateCrawler(i);
        //else if (x == 'B')UpdateBoss(i);
    }

}

bool EnemyCollisionCheck(int index)
{
    char next = arena[EnemyX[index] + EnemyVelX[index]][EnemyY[index] + EnemyVelY[index]];
    if (next == '@') { player[PHP]--; return true; }
    if (next != ' ') return true;

    return false;
}

void UpdateWalker(int index)
{
    if (EnemyCollisionCheck(index) || 
        arena[EnemyX[index]+1][EnemyY[index]+EnemyVelY[index]]==' ')EnemyVelY[index] = -EnemyVelY[index];
    EnemyY[index] += EnemyVelY[index];
}

void UpdateCrawler(int index) 
{
    if (EnemyCollisionCheck(index))EnemyVelX[index] = -EnemyVelX[index];
    EnemyX[index] += EnemyVelX[index];
}

void UpdateJumper(int index) 
{
    if (EnemyVelX[index])
    {
        if (arena[EnemyX[index] + 1][EnemyY[index]] == '#' || arena[EnemyX[index] + 1][EnemyY[index]] == '=')
        {
            EnemyVelX[index] = 0;
            EnemyVelY[index] = 1;
        }

    }
   
        else
        {
          
         
            int r = rand() % 10;
            if (r % 10 == 1) {
                EnemyX[index] -= (Jump_Max_Height(EnemyX[index], EnemyY[index], 7)-1);
                EnemyVelX[index] = 1;
                EnemyVelY[index] = 0;
                return;
            }
            if (EnemyCollisionCheck(index) ||
                arena[EnemyX[index] + 1][EnemyY[index] + EnemyVelY[index]] == ' ') {
                 EnemyVelY[index] = -EnemyVelY[index];
            }
        }
            EnemyY[index] += EnemyVelY[index];
            EnemyX[index] += EnemyVelX[index];
    
}

void UpdateFlier(int index) 
{
    int r = rand() % 40;
    if (EnemyVelX[index])
    {
        if (arena[EnemyX[index] + 1][EnemyY[index] + EnemyVelY[index]] == '#' || arena[EnemyX[index] + 1][EnemyY[index] + EnemyVelY[index]] == '=')
        {
            EnemyVelX[index] = 0;
            EnemyVelY[index] = -1;
            return;
        }
        if (r % 4 == 0)EnemyVelX[index] = 0;
    }

    else
    { 
        if (r % 40 == 0) {
            EnemyVelX[index] = 1;
           
            return;
        }
        if (EnemyCollisionCheck(index)) EnemyVelY[index] = -EnemyVelY[index];

    }
    EnemyY[index] += EnemyVelY[index];
    EnemyX[index] += EnemyVelX[index];

}


int Jump_Max_Height(int start, int py, int span)
{
    if (arena[start - 1][py] != ' ')return 0;
    for (int i = start - 1; i >= start - span - 1; i--)
        if (arena[i][py] != ' ')return start - i;

    return span;
}

void Check_Fall(int px, int py)
{
    int fall = 0;
    if (arena[px + 1][py] != ' ')return;

    while (arena[px + 1][py] == ' ')
    {
        fall++;
        px++;
    }
    player[PFALL] = fall;
}

void CharacterControl()
{
    if (!player[PHP]) { gameover = true; return; }

    if (_kbhit())
    {
        char key = _getch();
        int JumpSize;


        if (key == 'A' || key == 'a')
        {
            if (arena[player[PX]][player[PY] - 1] == ' ')player[PY]--;
            Check_Fall(player[PX], player[PY]);
        }

        else if (key == 'D' || key == 'd')
        {
            if (arena[player[PX]][player[PY] + 1] == ' ')player[PY]++;
            Check_Fall(player[PX], player[PY]);
        }
        else if (key == 'W' || key == 'w')
        {
            if (player[PFALL] == 0)
            {
                JumpSize = Jump_Max_Height(player[PX], player[PY], 10);
                player[PFALL] = JumpSize;
                player[PX] -= JumpSize;
            }

            else if (!player[PDOUBLEJ])
            {
                JumpSize = Jump_Max_Height(player[PX], player[PY], 7);
                player[PFALL] += JumpSize;
                player[PDOUBLEJ] = 1;
                player[PX] -= JumpSize;
            }


        }

    }

    if (player[PFALL])
    {
        if (arena[player[PX] + 1][player[PY]] != ' ')
        {
            player[PFALL] = 0;
            player[PDOUBLEJ] = 0;
        }
        else {
            player[PX]++;
            player[PFALL]--;
        }

        if (!player[PFALL])player[PDOUBLEJ] = 0;
    }

}

void InitializeArena()
{

    for (int j = 0; j < A_WIDTH; j++)
        for (int i = 1; i < A_HEIGHT; i++)
            arenaT[i][j] = ' ';

    for (int j = 0; j < A_WIDTH; j++)
    {
        arenaT[0][j] = '#';
        arenaT[A_HEIGHT - 1][j] = '#';
    }
    for (int i = 1; i < A_HEIGHT; i++)
    {
        arenaT[i][0] = '#';
        arenaT[i][A_WIDTH - 1] = '#';
    }

    RandomPlatforms();
}

void Arena_Template()
{
    for (int j = 0; j < A_WIDTH; j++)
        for (int i = 0; i < A_HEIGHT; i++)
            arena[i][j] = arenaT[i][j];
}


void RandomPlatforms()
{
    srand(time(0));
    int PlatformI, PlatformJ, PlatformLength;
    int PlatformCount = rand() % 6 + 2;
    for (int i = 0; i < PlatformCount; i++)
    {

        PlatformI = (rand() % 5) * 3 + 4;
        PlatformJ = rand() % 60;
        PlatformLength = min(rand() % 15 + 5, A_WIDTH - PlatformJ - 2);

        for (int s = 0; s < PlatformLength; s++)arenaT[PlatformI][PlatformJ + s] = '=';

        PlatformISave[i] = PlatformI;
        PlatformJSave[i] = PlatformJ;

    }
}

void Graphics()
{
    Arena_Template();
    std::cout << "HP: " << player[PHP] << "      " << "(a/ d move, w jump, double jump, i/j/k/l attack) PFALL - " << player[PFALL] << std::endl;

    arena[player[0]][player[1]] = '@';

    for (int i = 0; i < EnemyCount; i++)arena[EnemyX[i]][EnemyY[i]] = EnemyType[i];

    for (int i = 0; i < A_HEIGHT; i++)
    {
        for (int j = 0; j < A_WIDTH; j++)std::cout << arena[i][j];
        std::cout << std::endl;
    }

    //ColorCharacters();


}

void ResetCursor()
{
    COORD pos = { 0,0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void ColorAttribute(int x, int y, char sym)
{
    COORD pos = { x,y };
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(h, pos);
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << sym;
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void ColorCharacters()
{
    if (player[PHP])ColorAttribute(player[PY], player[PX] + 1, '@');
}

void HideCursor()
{
    HANDLE curs = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(curs, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(curs, &cursorInfo);
}

void InitializePlayer()
{
    player[PX] = A_HEIGHT - 2;
    player[PY] = A_WIDTH / 2;
    player[PHP] = 5;
    player[PFALL] = 0;
    player[PDOUBLEJ] = 0;
}

int main()
{
    int a = 0;
    InitializePlayer();
    InitializeArena();
    InitializeEnemies();
    HideCursor();
    while (!gameover && !levelwon)
    {
        
        UpdateEnemies();
        CharacterControl();
        Graphics();

        ResetCursor();
        Sleep(100);   

    }

    //Reset Cursor - Gives Rain Graphics
    if (gameover)std::cout << "You Lost" << std::endl;


    return 0;
}

