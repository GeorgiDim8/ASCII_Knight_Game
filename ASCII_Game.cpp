#include<iostream>
#include<ctime>
#include<conio.h>
#include<cstdlib>
#include<windows.h>


const int A_HEIGHT = 20;
const int A_WIDTH = 70;
const int ENEMY_MAX = 10;
const int JUMP = 10;
const int D_JUMP = 7;
const int ATTACK_LENGTH = 3;
const int BOSS_SPAN = 2;
const int PLAYER_HP = 5;

char input;
bool gameover = false;
bool levelwon = false;
int EnemyCount = 0;
char arenaT[A_HEIGHT][A_WIDTH] = {};
char arena[A_HEIGHT][A_WIDTH] = {};
int PlatformISave[10] = {};
int PlatformJSave[10] = {};

int* EnemyX = new int[ENEMY_MAX];
int* EnemyY = new int[ENEMY_MAX];
int* EnemyVelX = new int[ENEMY_MAX];
int* EnemyVelY = new int[ENEMY_MAX];
char* EnemyType = new char[ENEMY_MAX];
int* EnemyHP = new int[ENEMY_MAX];

int* player = new int[10];
enum PlayerTypes { PX, PY, PHP, PFALL, PDOUBLEJ,PATTACK,PATTACKDUR };

void InitializeArena();
void InitializePlayer();
void AddEnemy(int, int, char, int, int, int);
int  DamageEnemy(int);
void UpdateEnemies();
bool EnemyCollisionCheck(int);
bool BossCollisionCheck(int);
void Cleanup(); 
void Wave1();
void Wave2();
void Wave3();
void BossWave();

void UpdateWalker(int);
void UpdateJumper(int);
void UpdateFlier(int);
void UpdateCrawler(int);
void UpdateBoss(int);

void CharacterControl();
void UpdatePlayer();
void VisualizeAttack();
bool CheckAttack();
void AttackUp();
void AttackLeft();
void AttackDown();
void AttackRight();
void Graphics();
void RandomPlatforms();
void Arena_Template();
void ResetCursor();
void HideCursor();
void ColorCharacters();
void ColorAttribute(int x, int y, char sym);
void Check_Fall(int, int);
int  Jump_Max_Height(int, int, int);
int  Check_Boss_X(int);
void Visualize_Boss(int);



/*void InitializeEnemies()
{
    EnemyX = new int[ENEMY_MAX];
    EnemyY = new int[ENEMY_MAX];
    EnemyVelX = new int[ENEMY_MAX];
    EnemyVelY = new int[ENEMY_MAX];
    EnemyType = new char[ENEMY_MAX];
    EnemyHP = new int[ENEMY_MAX];


}
*/
void Wave1()
{
    int count = rand() % 3 + 1;


    for (int i = 0; i < 10; i++) {
        if (!PlatformISave[i] || EnemyCount>2)break;
        if (PlatformISave[i] - 1 > 3 && PlatformISave[i] - 1 < 20)
            AddEnemy(PlatformISave[i] - 1, PlatformJSave[i] + 1, 'E', 1, 0, 1);
    }
    for(int i = 0; i< count;i++)
    AddEnemy(A_HEIGHT - rand() % 9 -3, 1, 'C', 1, 1, 0);
}


void Wave2()
{
    int count = rand() % 3 + 1;
    AddEnemy(A_HEIGHT - 2, A_WIDTH - 2, 'E', 1, 0, 1);

    for (int i = 0; i < 10; i++) {
        if (!PlatformISave[i] || EnemyCount>3)break;
        if (PlatformISave[i] - 1 > 3 && PlatformISave[i] - 1 < 20)
            AddEnemy(PlatformISave[i] - 1, PlatformJSave[i] + 1, 'E', 1, 0, 1);

    }

    for (int i = 0; i < count; i++)
    AddEnemy(A_HEIGHT - 2, 4+rand()%15, 'J', 1, 0, 1);
}


void Wave3() 
{
    int count = rand() % 3 + 1;
    AddEnemy(A_HEIGHT - 2, A_WIDTH - 2, 'E', 1, 0, 1);

    for (int i = 0; i < 10; i++) {
        if (!PlatformISave[i])break;
        if (PlatformISave[i] - 1 > 3 && PlatformISave[i] - 1 < 20)AddEnemy(PlatformISave[i] - 1, PlatformJSave[i] + 1, 'E', 1, 0, 1);

    }
    for (int i = 0; i < count; i++)
        AddEnemy(A_HEIGHT - 2 - rand() % 9, A_WIDTH - 2, 'C', 1, 1, 0);
   
    count = rand() % 3 + 1;
    for (int i = 0; i < count; i++)
    AddEnemy(A_HEIGHT - 3 - rand()%9, 1, 'C', 1, 1, 0);

    AddEnemy(A_HEIGHT - 2, 4, 'J', 1, 0, 1);

    count = rand() % 3 + 1;
    for (int i = 0; i < count; i++)
    AddEnemy(2, 4+rand()%30, 'F', 1, 0, 1);
}

void BossWave() 
{
    AddEnemy(A_HEIGHT - 3, 30, 'B', 5, 0, 0);
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

int DamageEnemy(int index)
{
    if (index >= ENEMY_MAX || index < 0)return index;
    EnemyHP[index]--;
    if (EnemyHP[index])return index;
    int last = EnemyCount - 1;
    EnemyX[index] = EnemyX[last];
    EnemyY[index] = EnemyY[last];
    EnemyType[index] = EnemyType[last];
    EnemyHP[index] = EnemyHP[last];
    EnemyVelX[index] = EnemyVelX[last];
    EnemyVelY[index] = EnemyVelY[last];

    EnemyCount--;
    if (EnemyCount == 0)levelwon = true;
    return index - 1;
}

void UpdateEnemies()
{
    for (int i = 0; i < EnemyCount; i++)
    {
        char x = EnemyType[i];

        switch (x)
        {
        case 'E': { UpdateWalker(i); break; }
        case 'J': { UpdateJumper(i); break; }
        case 'F': { UpdateFlier(i); break; }
        case 'C': { UpdateCrawler(i); break; }
        case 'B': { UpdateBoss(i); break; }
        }
    }

}

bool EnemyCollisionCheck(int index)
{
    char next = arena[EnemyX[index] + EnemyVelX[index]][EnemyY[index] + EnemyVelY[index]];

    if (EnemyX[index] == player[PX] &&
        EnemyY[index] == player[PY] || next == '@')
    {
        player[PHP]--;
        return true;
    }

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
                EnemyX[index] -= (Jump_Max_Height(EnemyX[index], EnemyY[index], JUMP-3)-1);
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
void UpdateBoss(int index) 
{
    if (!EnemyCount)return;
    if (EnemyVelX[index])
    {
        if (BossCollisionCheck(index) || arena[EnemyX[index]+1][EnemyY[index]+1] == '#' || arena[EnemyX[index]+1][EnemyY[index]+1] == '=' ||
            arena[EnemyX[index] + 2][EnemyY[index] + 1] == '=' || arena[EnemyX[index]][EnemyY[index] + 1] == '=')
        {
            
            EnemyVelX[index] = 0;
        }
    }
     else 
        {   
            EnemyY[index] = player[PY]+rand() % 3;
            EnemyX[index] = Check_Boss_X(EnemyY[index]);
            EnemyVelX[index] = 1;
            if (EnemyY[index] == 0) {
                std::cout << "Warning" << std::endl; return;}
            
       }
        Visualize_Boss(index);
        EnemyX[index] += EnemyVelX[index];


}

bool BossCollisionCheck(int index) 
{
    char next1 = arena[EnemyX[index] + EnemyVelX[index]][EnemyY[index]];
    char next2 = arena[EnemyX[index] + EnemyVelX[index]][EnemyY[index]+1];
    char next3 = arena[EnemyX[index] + EnemyVelX[index]][EnemyY[index]-1];

    if ((EnemyX[index]+1) == player[PX] &&
        EnemyY[index] == player[PY] ||
        (EnemyY[index]+1)==player[PY] && (EnemyX[index]+1) == player[PX] || 
        (EnemyY[index]-1) == player[PY] && (EnemyX[index]+1) == player[PX] )
    {
        player[PHP]-=2;
        return true;
    }

    if (next1 != ' ' && next2 != ' ' && next3 != ' ') return true;

    return false;
}


int Check_Boss_X(int y)
{
    int x = player[PX]-10;
    bool possible = false;

    while (!possible && x<A_HEIGHT-3) 
    {
        if (arena[x - 1][y - 1] == ' ' && arena[x - 1][y] == ' ' && arena[x - 1][y + 1] == ' ' &&
            arena[x][y - 1] == ' ' && arena[x][y] == ' ' && arena[x][y + 1] == ' ' &&
            arena[x + 1][y - 1] == ' ' && arena[x][y + 1] == ' ' && arena[x + 1][y + 1] == ' ')possible = true;
        else x++;
    }
    if (x == A_HEIGHT-3) return 2;
    return x;
}

void Visualize_Boss(int i) 
{
    if (!EnemyCount)return;
    int x = EnemyX[i];
    int y = EnemyY[i];
    if (x <= 1)x = 2;
    else if (x >= A_HEIGHT - 1)x = A_HEIGHT - 2;
    if (y <= 1)y = 2;
    else if (y >= A_WIDTH - 1)y = A_WIDTH - 2;
    arena[x-1][y-1] = 'B';
    arena[x-1][y] = 'B';
    arena[x-1][y+1] = 'B';
    arena[x][y-1] = 'B';
    arena[x][y] = 'B';
    arena[x][y+1] = 'B';
    arena[x+1][y-1] = 'B';
    arena[x+1][y] = 'B';
    arena[x+1][y+1] = 'B';
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

    if (_kbhit())
    {
        char key = _getch();
        int JumpSize;

        switch (key) 
        {
        case 'A':
        case 'a': 
            {
                if (arena[player[PX]][player[PY] - 1] == ' ')player[PY]--;
                Check_Fall(player[PX], player[PY]);
                break;
            }
        case 'D':
        case 'd':
            {
                if (arena[player[PX]][player[PY] + 1] == ' ')player[PY]++;
                Check_Fall(player[PX], player[PY]);
                break;
            }
        case 'W':
        case 'w':
            {
                if (player[PFALL] == 0)
                {
                JumpSize = Jump_Max_Height(player[PX], player[PY], JUMP);
                player[PFALL] = JumpSize;
                player[PX] -= JumpSize;
                }

                else if (!player[PDOUBLEJ])
                {
                JumpSize = Jump_Max_Height(player[PX], player[PY], D_JUMP);
                player[PFALL] += JumpSize;
                player[PDOUBLEJ] = 1;
                player[PX] -= JumpSize;
                }
                break;
            }
        case 'I':
        case 'i':
            {
                if (!player[PATTACK]) {
                player[PATTACK] = 1;
                player[PATTACKDUR] = ATTACK_LENGTH;
                }
                break;
            }
        case 'J':
        case 'j':
            {
                if (!player[PATTACK]) {
                player[PATTACK] = 2;
                player[PATTACKDUR] = ATTACK_LENGTH;
                }
                break;
            }
        case 'K':
        case 'k':
            {
                if (!player[PATTACK]) {
                player[PATTACK] = 3;
                player[PATTACKDUR] = ATTACK_LENGTH;
                }
                break;
            }
        case 'L':
        case 'l':
            {
                if (!player[PATTACK]) {
                player[PATTACK] = 4;
                player[PATTACKDUR] = ATTACK_LENGTH;
                }
                break;
            }

        }

    }
    
    
}
void UpdatePlayer() 
{
    if (!player[PHP]) { gameover = true; return; }

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
    if (player[PATTACK]) 
    {
        VisualizeAttack();
    }
    

    

}

void VisualizeAttack()
{
    int at = player[PATTACK];
    if (player[PATTACKDUR] && CheckAttack()) 
    {
        if (at == 1)AttackUp();
        else if (at == 2)AttackLeft();
        else if (at == 3)AttackDown();
        else if (at == 4)AttackRight();

         player[PATTACKDUR]--;
    }
    else 
    {
        player[PATTACK] = 0;
        player[PATTACKDUR] = 0;
    }

}

bool CheckAttack() 
{
    int at = player[PATTACK];
    if (at == 1 && arena[player[PX] - 1][player[PY]] != '=')return true;

    if (at == 2 && arena[player[PX]][player[PY] - 1] != '#')return true;
   
    if(at == 3 && player[PFALL])return true;
    
    if (at == 4 && arena[player[PX]][player[PY] +1] != '#')return true;

    return false;
}
void AttackLeft()
{
    
    arena[player[PX]][player[PY] - 1] = '|';
    arena[player[PX]-1][player[PY] - 1] = '/';
    if(arena[player[PX]+1][player[PY]-1]==' ')arena[player[PX] + 1][player[PY] - 1] = '\\';

    int span = 1;
    if (EnemyType[0] == 'B')span = BOSS_SPAN;


    for (int i = 0; i < EnemyCount; i++) 
    {
        int ex = EnemyX[i];
        int ey = EnemyY[i];
        if (ey == player[PY] - span) 
        {
            if (ex == player[PX] || ex == (player[PX] - 1) || ex == (player[PX] + 1)) {
                i = DamageEnemy(i);
                
            }
        } 

    }


}
void AttackRight()
{

    arena[player[PX]][player[PY] + 1] = '|';
    arena[player[PX] - 1][player[PY] + 1] = '\\';
    if (arena[player[PX] + 1][player[PY] + 1] == ' ')arena[player[PX] + 1][player[PY] + 1] = '/';

    int span = 1;
    if (EnemyType[0] == 'B')span = BOSS_SPAN;

    for (int i = 0; i < EnemyCount; i++)
    {
        int ex = EnemyX[i];
        int ey = EnemyY[i];
        if (ey == player[PY] + span)
        {
            if (ex == player[PX] || ex == (player[PX] - 1) || ex == (player[PX] + 1))i=DamageEnemy(i);
        }

    }

}
void AttackUp()
{

    arena[player[PX]-1][player[PY]] = '-';
    if (arena[player[PX] - 1][player[PY] + 1] == ' ')arena[player[PX] - 1][player[PY] + 1] = '\\';
    if (arena[player[PX] - 1][player[PY] - 1] == ' ')arena[player[PX] - 1][player[PY] - 1] = '/';

    int span = 1;
    if (EnemyType[0] == 'B')span = BOSS_SPAN;

    for (int i = 0; i < EnemyCount; i++)
    {
        int ex = EnemyX[i];
        int ey = EnemyY[i];
        if (ex == player[PX] - span)
        {
            if (ey == player[PY] || ey == (player[PY] - 1) || ey == (player[PY] + 1))i=DamageEnemy(i);
        }

    }

}
void AttackDown()
{

    arena[player[PX] + 1][player[PY]] = '_';
    if (arena[player[PX] + 1][player[PY] + 1] == ' ')arena[player[PX] + 1][player[PY] + 1] = '/';
    if (arena[player[PX] + 1][player[PY] - 1] == ' ')arena[player[PX] + 1][player[PY] - 1] = '\\';

    int span = 1;
    if (EnemyType[0] == 'B')span = BOSS_SPAN;

    for (int i = 0; i < EnemyCount; i++)
    {
        int ex = EnemyX[i];
        int ey = EnemyY[i];
        if (ex == player[PX] + span)
        {
            if (ey == player[PY] || ey == (player[PY] - 1) || ey == (player[PY] + 1))i=DamageEnemy(i);
        }

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
    for (int i = 0; i < 10; i++) 
    {
        PlatformISave[i] = 0;
        PlatformJSave[i] = 0;
    }

    for (int i = 0; i < PlatformCount; i++)
    {

        PlatformI = (rand() % 5) * 3 + 4;
        PlatformJ = rand() % (A_WIDTH-10);
        PlatformLength = min(rand() % (A_HEIGHT-5) + 5, A_WIDTH - PlatformJ - 2);

        for (int s = 0; s < PlatformLength; s++)arenaT[PlatformI][PlatformJ + s] = '=';

        PlatformISave[i] = PlatformI;
        PlatformJSave[i] = PlatformJ;

    }
}

void Graphics()
{
    
    std::cout << "HP: " << player[PHP] << "      " << "(a/ d move, w jump, double jump, i/j/k/l attack)" << std::endl;

    arena[player[0]][player[1]] = '@';

    
    for (int i = 0; i < EnemyCount; i++)arena[EnemyX[i]][EnemyY[i]] = EnemyType[i];

    for (int i = 0; i < A_HEIGHT; i++)
    {
        for (int j = 0; j < A_WIDTH; j++)std::cout << arena[i][j];
        std::cout << std::endl;
    }

    //ColorCharacters();
    Arena_Template();

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
    player[PHP] += 1;
    player[PFALL] = 0;
    player[PDOUBLEJ] = 0;
    player[PATTACK] = 0;
    player[PATTACKDUR] = 0;
}

void Cleanup()
{
    delete[] EnemyX;
    delete[] EnemyY;
    delete[] EnemyVelX;
    delete[] EnemyVelY;
    delete[] EnemyType;
    delete[] EnemyHP;

    delete[] player;

}
void Loss() 
{
    InitializeArena();
    char end[10] = "You lost ";
    for (int z = 0; z < 8; z++)arena[10][30 + z] = end[z];

    for (int i = 0; i < A_HEIGHT; i++)
    {
        for (int j = 0; j < A_WIDTH; j++)std::cout << arena[i][j];
        std::cout << std::endl;
    }
}

void Initialize() 
{
    InitializePlayer();
    InitializeArena();
   // InitializeEnemies();
    gameover = false;
    levelwon = false;
}

void GameCycle() 
{
    while (!gameover && !levelwon)
    {

        UpdateEnemies();
        CharacterControl();
        UpdatePlayer();
        Graphics();

        ResetCursor();
        Sleep(100);

    }
    if (gameover)
    {
        Loss();
        std::exit(0);
    }
}

int main()
{
    int a = 0;
    player[PHP] = PLAYER_HP-1;
    Initialize();
    HideCursor();
    Wave1();
    std::cout << "WAVE 1" << std::endl;
    Sleep(1000);
    GameCycle();

    std::cout << "WAVE 2" << std::endl;
    Sleep(1000);
    Initialize();
    ResetCursor();
    Wave2();
    GameCycle();
    
    std::cout << "WAVE 3" << std::endl;
    Sleep(1000);
    Initialize();
    Wave3();
    GameCycle();
   


    std::cout << "FINAL BOSS" << std::endl;
    Sleep(1000);
    Initialize();
    BossWave();
    GameCycle();

    //ResetCursor();
    if (gameover)std::cout << "You Lost" << std::endl;
    else std::cout << "YOU WON" << std::endl;
    Cleanup();


    return 0;
}

