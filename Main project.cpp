//  Project "Glich"

//119(120) - ширина ; 29(30) - высота
#include <ctime>
#include <conio.h>
#include <iostream>
#include <windows.h>
#include <cmath>
#include <fstream>

void SetColor(int text, int bg)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}
void gotoxy(int xpos, int ypos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = xpos; scrn.Y = ypos;
    SetConsoleCursorPosition(hOuput, scrn);
}
using namespace std;

struct person
{
    int x;
    int y;
    int hp;
    int dm; // damag
};
struct Player
{
    person me;
    short int moving_id = 0;
    int apple = 0;
    bool weapon = 0;
};
struct item
{
    int x;
    int y;
};
void n_o(int x)
{
    cout << x;
    if (x < 10) { cout << " "; }
}

item spawn_item(int fence_x, int fence_y, char who, bool** map)
{
    item apple;
    apple.x = rand() % (fence_x - 2) + 2;
    apple.y = rand() % (fence_y - 2) + 2;
    while (map[apple.x][apple.y] == 1)
    {
        apple.x = rand() % (fence_x - 2) + 2;
        apple.y = rand() % (fence_y - 2) + 2;
    };
    SetColor(10, 8);
    gotoxy(apple.x, apple.y); cout << who;
    return apple;
}
person spawn_person(int fence_x, int fence_y, bool** map)
{
    person z;
    z.x = rand() % (fence_x - 2) + 2;
    z.y = rand() % (fence_y - 2) + 2;
    while (map[z.x][z.y] == 1)
    {
        z.x = rand() % (fence_x - 2) + 2;
        z.y = rand() % (fence_y - 2) + 2;
    };
    z.hp = 10;
    z.dm = 4;
    SetColor(12, 8);
    gotoxy(z.x, z.y); cout << "$";
    return z;
}
person move(int were, person who, int fence_x, int fence_y, bool** map)
{
    switch (were)
    {
    case 75: // left
    {
        if (who.x - 1 != 0 && map[who.x - 1][who.y] != 1) {
            who.x--; gotoxy(who.x + 1, who.y); cout << " ";
        }
        break;
    }
    case 77: //right
    {
        if (who.x + 1 != fence_x && map[who.x + 1][who.y] != 1) {
            who.x++; gotoxy(who.x - 1, who.y); cout << " ";
        }
        break;
    }
    case 72: // up
    {
        if (who.y - 1 != 0 && map[who.x][who.y - 1] != 1) {
            who.y--; gotoxy(who.x, who.y + 1); cout << " ";
        }
        break;
    }
    case 80: // down
    {
        if (who.y + 1 != fence_y && map[who.x][who.y + 1] != 1) {
            who.y++; gotoxy(who.x, who.y - 1); cout << " ";
        }
        break;
    }
    default:
        break;
    }
    return who;
}
item item_in_hand(item sword, Player gamer)
{
    sword.x = gamer.me.x;
    sword.y = gamer.me.y;
    switch (gamer.moving_id)
    {
    case 1:
        sword.x = gamer.me.x - 1;
        break;
    case 2:
        sword.x = gamer.me.x + 1;
        break;
    case 3:
        sword.y = gamer.me.y - 1;
        break;
    case 4:
        sword.y = gamer.me.y + 1;
        break;
    default:
        break;
    }
    return sword;
}

 // запись из файла 
bool FileInsert(const char* fileName, bool** map, int& GGx, int& GGy) // 
{
    std::ifstream file(fileName, std::ios::binary);

    if (!file.is_open())
    {
        std::cout << "Failed to open file" << std::endl;
        return false;
    }

    bool buffer[5000];
    file.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
    file.close();
    bool new_Gx = 0;
    int for_Gx = 0;
    int j = 0;
    int new_i = 0;
    for (int i = 0; i < 3600; i++)
    {
        if (buffer[i] != 204)
        {
            if (new_Gx == 0) { for_Gx++; }
            if (buffer[i] == 10) { j++; new_Gx = 1; new_i = 0; }
            if (buffer[i] == 49) { map[new_i][j] = 1;new_i++; }// возможно i и j надо поменять )))
            if (buffer[i] == 48) { map[new_i][j] = 0;new_i++; }// возможно i и j надо поменять )))
        }
        else { GGx = for_Gx-2; GGy = j; break; }
    }
    return true;
}
// создание - открытие
bool FileInput(const char* filenum, bool** arr, int GGx, int GGy)// путь файла, количество, числа
{
    ofstream fileNum;
    bool yes = 1;
    fileNum.open(filenum, ios::binary | fstream::out); //открытие потока на чтение в двоичном режиме и запись
    if (!fileNum.is_open()) //проверка на открытие файла
    {
        cout << "File opening error" << endl;
        return -1;
    }
    else
    {
        cout << "File is created and opened" << endl;
    }
    for (int i = 0; i < GGy + 1; i++)
    {
        for (int j = 0; j < GGx + 1; j++)
        {
            fileNum << arr[j][i];
        }
        if (i < GGy) { fileNum << endl; }
    }
    fileNum.close();
    return 1;
}

void dravv_map(int GGx, int GGy, bool** map)
{
    SetColor(8, 4);
    for (int i = 0; i < GGy + 1; i++)
    {
        for (int j = 0; j < GGx + 1; j++)
        {
            if (map[j][i] == 1) { gotoxy(j, i); cout << " "; } // рисуем . . . . .
        }
    }
}
// создание и редактирование карты 
void editor(int& GGx, int& GGy, bool** map)
{
    for (int i = 0; i < 119; i++) //перенастраиваем массив
    {
        for (int j = 0; j < 29; j++)
        {
            map[i][j] = 0;
            if (j == 0 || j == GGy || i == 0 || i == GGx) { map[i][j] = 1; } // подстраиваем под новые границы
        }
    }
    dravv_map(GGx, GGy, map);
    char* file_way = new char[30]; //путь файла 
    char* file_insert = new char[30]; // откуда брать зачения
    bool end = 0;
    int x = 1; // базовая позиция курсора 
    int y = 1; gotoxy(x, y);
    while (end == 0)
    {
        switch (_getch())
        {
        case 13: end = 1; break;
        case 122: map[x][y] = 1; break;
        case 120: map[x][y] = 0; break;
        case 72: if (y > 0) { y--; } break;
        case 80: if (y < GGy) { y++; } break;
        case 77: if (x < GGx) { x++; } break;
        case 75: if (x > 0) { x--; } break;
        case 59:
        {
            SetColor(12, 8); gotoxy(83, 6); cin >> file_way;
            FileInput(file_way, map, GGx, GGy); //запись в файл 
            break;
        }
        case 60: 
        {
            SetColor(12, 8); gotoxy(83, 8); cin >> file_insert;
            FileInsert(file_insert, map, GGx, GGy);
            system("cls");
            system("Color 08");
            dravv_map(GGx, GGy, map);
            break; 
        }
        default:
            break;
        }
        gotoxy(x, y);
        if (map[x][y] == 1) { SetColor(12, 4); cout << " "; }
        else { SetColor(0, 0); cout << " "; }
    }
}




int main()
{
    setlocale(0, "");
    bool programm_work = 1;
    int Gx = 80; /* границы */
    int Gy = 27; /* границы */
    int potencial_speed = 400; // стандартная скорость зомби
    int zombie_radius = 10; // стандартная область зрения зомби
    bool** map_me; // двумерный динамический массив с 1 и 0 (для внутренних границ карты);
    map_me = new bool* [119]; // 119 - максимальный размер карты по x
    for (int i = 0; i < 119; i++)
    {
        map_me[i] = new bool[29]; // 29 - максимальный размер карты по y
    }
    for (int i = 0; i < 119; i++)
    {
        for (int j = 0; j < 29; j++)
        {
            map_me[i][j] = 0;
            if (j == 0 || j == Gy || i == 0 || i == Gx) { map_me[i][j] = 1; } // просто строим стандартные границы 
        }
    }
    while (programm_work == 1) {
        system("cls");
        bool end_of_menu = 0;
        int  choose = 7;  // переменная для стрелочки
        bool menu_choose = 0; //что там с основным меню?
        bool next_menu_choose = 0; // что там со следующим меню ?
        int next_choose = choose + 1; // следующая стрелочка
        gotoxy(50, choose); cout << "--->";
        system("Color 03");
        gotoxy(55, 5); cout << "--WELCOME--\n";
        gotoxy(56, 7); cout << "ИГРАТЬ";
        gotoxy(56, 8); cout << "Cложность";
        gotoxy(56, 9); cout << "Изменить карту";
        gotoxy(56, 10); cout << "Выход";
        while (menu_choose == 0)
        {
            switch (_getch())
            {
            case 13: //Enter
                switch (choose)
                {
                case 7:
                    menu_choose = 1; end_of_menu = 1;
                    break;
                case 8:
                    next_menu_choose = 0;
                    gotoxy(40, choose); cout << "Сложности: ";
                    gotoxy(40, choose + 1); cout << "Лёгкая ";
                    gotoxy(40, choose + 2); cout << "Нормальная ";
                    gotoxy(40, choose + 3); cout << "Сложная ";
                    gotoxy(40, choose + 4); cout << "Невозможная ";
                    gotoxy(36, choose + 1); cout << "--->";
                    next_choose = 9;
                    while (next_menu_choose == 0)
                    {
                        switch (_getch())
                        {
                        case 13: next_menu_choose = 1; break;
                        case 72:
                            if (next_choose != 9) { gotoxy(36, next_choose); cout << "    "; next_choose--; }
                            break;
                        case 80:
                            if (next_choose != 12) { gotoxy(36, next_choose); cout << "    "; next_choose++; }
                            break;
                        default:
                            break;
                        }
                        gotoxy(36, next_choose); cout << "--->";
                        switch (next_choose)
                        {
                        case 9:
                            potencial_speed = 400;
                            zombie_radius = 10;
                            break;
                        case 10:
                            potencial_speed = 300;
                            zombie_radius = 20;
                            break;
                        case 11:
                            potencial_speed = 200;
                            zombie_radius = 30;
                            break;
                        case 12:
                            potencial_speed = 80;
                            zombie_radius = 40;
                            break;
                        default:
                            break;
                        }

                    }
                    for (int i = 8; i < 14; i++) { gotoxy(36, i); cout << "                    "; }
                    break;
                case 9:
                    next_menu_choose = 0;
                    gotoxy(38, choose); cout << "Изменить карту:<--";
                    gotoxy(38, choose + 2); cout << "X:" << Gx;
                    gotoxy(38, choose + 3); cout << "Y:" << Gy;
                    gotoxy(46, choose + 2); cout << "new_X:"; cin >> Gx; // минимально допустимые границы - 3 x 3 
                    while (Gx < 3 || Gx>119) { cout << "Измените еще раз:"; cin >> Gx; }
                    gotoxy(46, choose + 3); cout << "new_Y:"; cin >> Gy;
                    while (Gy < 3 || Gy>28) { cout << "Измените еще раз:"; cin >> Gy; }
                    system("cls");
                    gotoxy(83, 5); SetColor(3, 0); cout << "Сохранить карту в файл [F1]";
                    gotoxy(83, 7); cout << "Загрузить карту с файла [F2]";
                    gotoxy(83, 9); SetColor(3, 0); cout << "z - поставить границу";
                    gotoxy(83, 10); cout << "x - убрать границу";
                    gotoxy(83, 11); cout << "Enter - подтвердить изменения";
                    editor(Gx, Gy, map_me);
                    system("cls");
                    system("Color 03");
                    gotoxy(55, 5); cout << "--WELCOME--\n"; //мне лень было создавать дополнительный цикл и всё такое, в общем вот....
                    gotoxy(56, 7); cout << "ИГРАТЬ";
                    gotoxy(56, 8); cout << "Cложность (в разработке)";
                    gotoxy(56, 9); cout << "Изменить карту (в еще большей разработке)";
                    gotoxy(56, 10); cout << "Выход";
                    break;
                case 10:
                    return 0;
                    break;
                default:
                    break;
                }
                break;
            case 72: //up
                if (choose != 7) { gotoxy(50, choose); cout << "    "; choose--; }
                break;
            case 80: //down
                if (choose != 10) { gotoxy(50, choose); cout << "    "; choose++; }
                break;
            default:
                break;
            }
            gotoxy(50, choose); cout << "--->";
        }
        system("cls");
        system("Color 84");
        //рисуем всё что надо(по карте)
        dravv_map(Gx, Gy, map_me);
        //таймер и основные "персонажи"
        int minute = 0;
        Player Glicher;
        Glicher.me = spawn_person(Gx, Gy, map_me);
        Glicher.me.hp = 20; Glicher.me.dm = 0;

        item sword = spawn_item(Gx, Gy, '/', map_me);
        bool GW = 0; // GW - game work ? 
        item apple = spawn_item(Gx, Gy, '0', map_me);
        person zombie = spawn_person(Gx, Gy, map_me);
        gotoxy(90, 10);
        int A_T = clock() / potencial_speed; //another tact - переменная для скорости зомби;
        int t_key;
        while (GW == 0)
        { //интерфейс:
            SetColor(11, 8);
            gotoxy(88, 2); cout << "hp: "; n_o(Glicher.me.hp);
            gotoxy(88, 4); cout << "apple: "; n_o(Glicher.apple);
            gotoxy(88, 5); cout << "sword: "; n_o(Glicher.weapon);
            SetColor(12, 8);
            gotoxy(94, 7); cout << "zombie hp: "; n_o(zombie.hp);
            gotoxy(88, 12); cout << "z - удар мечем";
            gotoxy(88, 14); cout << "x - съесть яблоко";
            SetColor(14, 8);
            gotoxy(88, 0);
            int sec = clock() / 1000 - minute * 60;
            bool esc_menu = 0;
            if (sec == 60)
            {
                sec = 0;
                minute++;
            }
            SetColor(12, 8);
            cout << minute << ":"; n_o(sec); cout << "  " << clock();
            // управление :
            switch (_kbhit()) // нажата ли клавиша ?
            {
            case 1:
                t_key = _getch();
                switch (t_key)
                {
                case 122: //z
                    if (Glicher.weapon == 1)
                    {
                        for (int i = -1; i < 2; i++)
                        { // боевая система  (просто обозначения радуса действия меча при разных условиях)
                            if (Glicher.moving_id == 1 && zombie.x + 1 == sword.x && zombie.y == sword.y + i) { zombie.hp = zombie.hp - Glicher.me.dm; }
                            if (Glicher.moving_id == 2 && zombie.x - 1 == sword.x && zombie.y == sword.y + i) { zombie.hp = zombie.hp - Glicher.me.dm; }
                            if (Glicher.moving_id == 3 && zombie.y + 1 == sword.y && zombie.x == sword.x + i) { zombie.hp = zombie.hp - Glicher.me.dm; }
                            if (Glicher.moving_id == 4 && zombie.x - 1 == sword.x && zombie.y == sword.y + i) { zombie.hp = zombie.hp - Glicher.me.dm; }
                        }
                    }
                    break;
                case 120: //x
                {
                    if (Glicher.apple > 0)
                    {
                        Glicher.me.hp += 5;
                        Glicher.apple--;
                    }
                    break;
                }
                case 99: // c - телепортация на 6 блоков
                    gotoxy(Glicher.me.x, Glicher.me.y); cout << " ";
                    switch (Glicher.moving_id)
                    {
                    case 1: //left
                        if (Glicher.me.x < 7) { Glicher.me.x = 1; }
                        else { Glicher.me.x -= 6; }
                        break;
                    case 2: //right
                        if (Glicher.me.x > Gx - 7) { Glicher.me.x = Gx - 1; }
                        else { Glicher.me.x += 6; }
                        break;
                    case 3: //up
                        if (Glicher.me.y < 7) { Glicher.me.y = 1; }
                        else { Glicher.me.y -= 6; }
                        break;
                    case 4: //down
                        if (Glicher.me.y > Gy - 7) { Glicher.me.y = Gy - 1; }
                        else { Glicher.me.y += 6; }
                        break;
                    default:
                        break;
                    }
                    break;
                case 27:
                    while (esc_menu == 0) 
                    {
                        gotoxy(Gx/2, Gy/2); cout << "Enter для продолжеия";
                        gotoxy(Gx/2, (Gy/2)+1); cout << "Esc чтобы выйти";
                        switch (_getch())
                        {
                        case 13:
                            esc_menu = 1;
                            break;
                        case 27:
                            Glicher.me.hp = 0;
                            esc_menu = 1;
                            break;
                        default:
                            break;
                        }
                    }
                    A_T = clock() / potencial_speed;
                    gotoxy(Gx/2, Gy/2); cout << "                     ";
                    gotoxy(Gx/2, (Gy/2)+1); cout << "                ";
                    break;
                case 75: // лева
                    if (t_key == 75) { Glicher.moving_id = 1; }
                case 77: // права
                    if (t_key == 77) { Glicher.moving_id = 2; }
                case 72: // верх
                    if (t_key == 72) { Glicher.moving_id = 3; }
                case 80: // низ
                    if (t_key == 80) { Glicher.moving_id = 4; }
                    Glicher.me = move(t_key, Glicher.me, Gx, Gy, map_me);
                    break;
                default:
                    break;
                }
            default:
                break;
            }
            //рисуем игрока
            gotoxy(Glicher.me.x, Glicher.me.y);
            SetColor(11, 8); cout << "*";
            // ёбля с мечём: 
            if (Glicher.weapon == 1)
            {
                gotoxy(sword.x, sword.y);
                if (sword.y == Gy || sword.x == Gx || sword.x == 0 || sword.y == 0 || map_me[sword.x][sword.y] == 1) { SetColor(0, 4); cout << " "; }
                else { cout << " "; }
                sword = item_in_hand(sword, Glicher);
            }
            if (sword.x == Gx || sword.y == Gy)
            {
                SetColor(11, 4);
                gotoxy(sword.x, sword.y); cout << "/";
            }
            else {
                gotoxy(sword.x, sword.y); cout << "/";
            }
            //рисуем яблоко
            gotoxy(apple.x, apple.y); SetColor(10, 8); cout << "0";
            //рисуем зомби 
            if (zombie.hp > 0)
            {
                gotoxy(zombie.x, zombie.y); SetColor(12, 8); cout << "$";
            }
            else
            {
                gotoxy(zombie.x, zombie.y); SetColor(12, 12); cout << " ";
            }
            // пространство для механик 
            if (A_T + 1 == clock() / potencial_speed) // тут определяется скорость зомби - скорость кусания и скорость передвижения
            {
                A_T = clock() / potencial_speed;
                // пространство для зоооомбиии
                if (zombie.hp > 0) {
                    int d_t_z = int(sqrt(pow((Glicher.me.x - zombie.x), 2) + pow((Glicher.me.y - zombie.y), 2))); //distance to zombie
                    for (int j = 0; j < zombie_radius; j++)
                    {
                        if (d_t_z == j)
                        {
                            if (abs(Glicher.me.x - zombie.x) > abs(Glicher.me.y - zombie.y))
                            {
                                if (Glicher.me.x - zombie.x > 0)
                                {
                                    zombie = move(77, zombie, Gx, Gy, map_me);
                                }
                                else { zombie = move(75, zombie, Gx, Gy, map_me); }
                            }
                            else
                            {
                                if (Glicher.me.y - zombie.y > 0)
                                {
                                    zombie = move(80, zombie, Gx, Gy, map_me);
                                }
                                else { zombie = move(72, zombie, Gx, Gy, map_me); }
                            }
                        }
                    }
                    if (d_t_z == 1 || d_t_z == 0) { Glicher.me.hp -= zombie.dm; }
                    gotoxy(88, 9); n_o(d_t_z);
                }
                else { zombie = spawn_person(Gx, Gy, map_me); }
            }
            if (sword.x == Glicher.me.x && Glicher.me.y == sword.y) { Glicher.weapon = 1; Glicher.me.dm = 8; }
            if (apple.x == Glicher.me.x && apple.y == Glicher.me.y)
            {
                Glicher.apple++;
                apple = spawn_item(Gx, Gy, '0', map_me);
            }
            if (Glicher.me.hp < 1) { system("cls"); gotoxy(50, 10); system("Color CF"); cout << "ВАС СЪЕЛИ\n\n\n"; GW = 1; system("pause"); menu_choose = 0; } //чуть позже изменю;
        }
    }
}