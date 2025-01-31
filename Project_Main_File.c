#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define Vertical_WALL '|'
#define UPPER_Horizontal_WALL '_'
#define LOWWER_Horizontal_WALL '-'
#define FLOOR '.'
#define ENCHANTED_FLOOR ','
#define DOOR '+'
#define CORRIDOR '#'
#define PILLAR 'O'
#define WINDOW '='
#define TRAP '^'
#define ENTRANCE_STAIRCASE '<'
#define EXIT_STAIRCASE '>'
#define TREASURE_ROOM_ENTRANCE 'T'
#define GOLD 'g'
#define BLACK_GOLD '$'
#define Terminal_Vertical_Lenght 25
#define Terminal_Horizontal_Lenght 45
#define VISIBLE 'v'
#define HIDE 'h'
#define FOOD '*'
#define MAX_ROOMS 6
#define MIN_ROOM_SIZE 4
#define MAX_NUM_WINDOWS 2
#define Marz_Show_Kardan 3
#define MESSAGE_Y 1
#define MESSAGE_X 60
#define MONSTERS_NUM 15
#define MONSTERS_DAMAGE 2
#define TREASURE_ROOM_MONSTERS_NUM 7 

#define MACE 'm'
#define DAGGER 'd'
#define MAGIC_WAND 'w'
#define NORMAL_ARROW 'a'
#define SWORD 's'

#define P_Fd 'p'
#define M_Fd 'm'
#define B_Fd 'b'




#define HEALTH_POTION '9'
#define SPEED_POTION '8'
#define DAMAGE_POTION '7'

#define DEMON 'D'
#define FIRE_BREATHING_MONSTER 'F'
#define GIANT 'G'
#define SNAKE 'S'
#define UNDEED 'U'


#define U_2692 "\xE2\x9A\x92"       // ⚒
#define U_1F5E1 "\xF0\x9F\x97\xA1" // 🗡
#define U_1FA84 "\xF0\x9F\xAA\x84" // 🪄
#define U_27B3 "\xE2\x9E\xB3"      // ➳
#define U_2694 "\xE2\x9A\x94"      // ⚔

#define U_HEART  "\xE2\x99\xA5"   // ❤️ (U+2764)
#define U_LIGHTNING "\xE2\xAB\xB8" // ⚡ (U+26A1)
#define U_EXPLOSION  "\xE2\x9A\xA0" // 💥 (U+1F4A5)

#define U_HORIZONTAL_WALL "\xE2\x8E\xAF"      // ⎯
#define U_LOWWER_HORIZONTAL_WALL "\xE2\x80\xBE" // ‾

#define U_GOLDEN_COIN "\xE2\x97\x8B"    // ●

#define U_1F4B0 "\xE2\x9C\xA6"  // ✦ (ستاره توپر)

#define U_P_Fd "\xE2\x99\xA6"  // ♦  (الماس، نشانه ارزشمندی)
#define U_M_Fd "\xE2\x9A\x9B"   // ⚛ (نماد جادویی)
#define U_B_Fd  "\xE2\x98\xA0"  // ☠ (جمجمه با استخوان، همچنان بزرگ)


#define U_1F969 "\xF0\x9F\xA5\xA9" // 🥩 (گوشت استیک بزرگ → نیاز به جایگزین)
#define U_1F356 "\xF0\x9F\x8D\x96" // 🍖 (استخوان با گوشت)

#define filePath "/home/mhbd/Desktop/Users/Users_Data.txt"

int difficulty_level;
int Hero_color;

typedef struct {
    char name[100];
    char password[100];
    char email[100];
    int sum_points ;
    int sum_golds ;
    int rank ;
    int num_games;
    int experience;
    int game_situation;
    //0 for no game in resume and 1 for having a game in resume and 2 for in game situation

} User_Data;


struct Player {
    int x,y;
    int health,point,level,food,hunger;
//normal perfect majical bad
    int n_fd,p_fd,m_fd,b_fd ;
    int DAGGER_num,MAGIC_WAND_num,NORMAL_ARROW_num,SWORD_num;
    int p_helth_num,p_speed_num,p_damage_num;
    int potion_lasting,damage_coefficient,speed_coefficient;
    char Namad,current_weapon,current_potion;
    
};

struct Monster {
    int x,y,health,room_num,level,followsteps;
    char namad;
};

struct Room{
    // x,y mokhtasat chap balla room
    int x,y,tool,arz;
    char type ;
    // t fot tresure
    // e fot enchat
    // n for normal
};

struct Monster Monsters[MONSTERS_NUM];

struct Player Hero;

int M_var = 0 ;
//char Game_Message[100];

//struct Room staircase_rooms[3];

//position ha az 0 ta 5 hastand!
//int staircase_rooms_positions[3];

struct food{
    char type;
    int recovering_num;
    int durability ;
};

int entrance_num;
int exit_num;
int x_exit_staircase;
int y_exit_staircase;

int aiming_direction = 0;








struct Room rooms[MAX_ROOMS+1];

char Map[Terminal_Vertical_Lenght][Terminal_Horizontal_Lenght];
char Showing_situation_Map[Terminal_Vertical_Lenght][Terminal_Horizontal_Lenght];
// yek map ke injorii tosho por mikonim ke hame VISIBLE bashan be joz TRAP
char Traps_situation_Map[Terminal_Vertical_Lenght][Terminal_Horizontal_Lenght];


User_Data Users [100] ;
int num_users=0;
//char current_user_name [50];
int current_user_index ;


void Initializing_Monsters(int num){
    int monsters_num ;
    
    if(num == 0){
        monsters_num = MONSTERS_NUM ;}
    else 
        monsters_num = TREASURE_ROOM_MONSTERS_NUM ;
    
    for(int i=0;i<monsters_num;i++){
        int m = rand() % 5;
        if(m == 0){
            Monsters[i].namad = DEMON;
            Monsters[i].health = 5 ;
            Monsters[i].followsteps = 1000;}
        else if(m == 1){
            Monsters[i].namad = FIRE_BREATHING_MONSTER;
            Monsters[i].health = 10 ;
            Monsters[i].followsteps = 1000;}
        else if(m == 2){
            Monsters[i].namad = GIANT;
            Monsters[i].health = 15 ;
            Monsters[i].followsteps = 5;}
        else if(m == 3){
            Monsters[i].namad = SNAKE;
            Monsters[i].health = 20 ;
            Monsters[i].followsteps = 1000;}
        else {
            Monsters[i].namad = UNDEED;
            Monsters[i].health = 30 ;
            Monsters[i].followsteps = 5;}
        
            Monsters[i].room_num = rand() % 24 ;
        
        if(num == 0)
            Monsters[i].level = ((Monsters[i].room_num) /6 ) + 1 ;
        else{
            Monsters[i].level = 5 ;
            Monsters[i].room_num = 0;}
}


        
}

void Initializing_Maps(){
    for(int i=0;i<Terminal_Vertical_Lenght;i++){
        for(int j=0;j<Terminal_Horizontal_Lenght;j++){
            Map[i][j] = ' ' ;
            Showing_situation_Map[i][j] = HIDE ;
            Traps_situation_Map[i][j] = VISIBLE ;
        }
    }
}

void Adding_Room(struct Room room){
    for (int i = room.y; i < room.y + room.arz; i++) {
        for (int j = room.x; j < room.x + room.tool; j++) {
            if (i == room.y )
                Map[i][j] = UPPER_Horizontal_WALL ;
            else if(i == room.y + room.arz - 1)
                Map[i][j] = LOWWER_Horizontal_WALL ;
            else if (j == room.x || j == room.x + room.tool - 1) 
                Map[i][j] = Vertical_WALL; 
            else {
                if(room.type == 'e')
                    Map[i][j] = ENCHANTED_FLOOR ;
                else
                    Map[i][j] = FLOOR; 
            }
        }
    }
}

int Adding_windows(struct Room room){
    int random = rand() % 2 ;
    if ( random == 1 )
        return 0;
    
    int num_windows = rand() % MAX_NUM_WINDOWS;
    for(int i=0;i<num_windows+1;i++){
    
    int wall = rand() % 4;
    int window_pos;

    switch (wall) {
        case 0:  // دیوار بالا
            window_pos = room.x + 1 + rand() % (room.tool - 2);
            Map[room.y][window_pos] = WINDOW;
            break;
        case 1:  // دیوار پایین
            window_pos = room.x + 1 + rand() % (room.tool - 2);
            Map[room.y + room.arz - 1][window_pos] = WINDOW;
            break;
        case 2:  // دیوار چپ
            window_pos = room.y + 1 + rand() % (room.arz - 2);
            Map[window_pos][room.x] = WINDOW;
            break;
        case 3:  // دیوار راست
            window_pos = room.y + 1 + rand() % (room.arz - 2);
            Map[window_pos][room.x + room.tool - 1] = WINDOW;
            break;
        }
    }
    return 1;
}

int second_largest(int a, int b, int c, int d) {
    int arr[4] = {a, b, c, d};


    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (arr[i] < arr[j]) { 
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    return arr[1];
}

int second_smallest(int a, int b, int c, int d) {
    int arr[4] = {a, b, c, d};


    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (arr[i] < arr[j]) { 
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    return arr[2];
}

void adjusting_no_monster_in_enchantrooms(){
     for(int i=0;i<MONSTERS_NUM ; i++){
        if(Monsters[i].level == Hero.level){
            if(rooms[Monsters[i].room_num % 6].type == 'e'){
                int attempts = 1000 ;
                while(attempts > 0){
                    Monsters[i].room_num = (Monsters[i].level - 1)*6 + rand() % 6 ;
                    if(rooms[Monsters[i].room_num % 6].type != 'e')
                        break;
                    attempts -- ;
                }
            }
        }
    }
}

void monster_spawner(int num) {
    int monsters_num;
    if(num == 0)
        monsters_num = MONSTERS_NUM ;
    else 
        monsters_num = TREASURE_ROOM_MONSTERS_NUM ;

    for(int i=0;i<monsters_num;i++){
        if((Monsters[i].level) == Hero.level){
            int attempts = 0 ;
    while (attempts < 1000) {
        Monsters[i].x = rooms[Monsters[i].room_num%6].x + (rand() % (rooms[Monsters[i].room_num%6].tool - 2)) + 1;
        Monsters[i].y = rooms[Monsters[i].room_num%6].y + (rand() % (rooms[Monsters[i].room_num%6].arz - 2)) + 1;
        if (Map[Monsters[i].y][Monsters[i].x] == FLOOR && Monsters[i].y != Hero.y && Monsters[i].x != Hero.x)
            break;
        attempts++;
            }
        }
    }
}

int Adding_doors_and_corridors_Horizontal(struct Room room0,struct Room room1){
    int T = second_largest(room0.y,room0.y+room0.arz-1,room1.y,room1.y+room1.arz-1);
    int t = second_smallest(room0.y,room0.y+room0.arz-1,room1.y,room1.y+room1.arz-1);
    
    if((T-t-1)<2){
        if(T-t-1 == 1){
            Map[T-1][room1.x] = DOOR ;
            Map[t+1][room0.x + room0.tool -1] = DOOR ;
            for(int i = room0.x + room0.tool ; i <= room1.x -1 ; i++)
                Map[t+1][i] = CORRIDOR ;
            return 1 ;
        }
        return 0;
    }
    int deltaY0 = ( rand() % (T-t-1) ) + 1 ;
    Map[t + deltaY0][room0.x + room0.tool -1] = DOOR ;
    
    int deltaY1;
    while(1){
        deltaY1 =( rand() % (T-t-1) ) + 1 ;
        if( (T - deltaY1 ) != ( t + deltaY0 ) ){
            Map[T - deltaY1][room1.x] = DOOR ;
            break ;
        }
    }

    int Horizontal_distance = room1.x - (room0.x + room0.tool -1) - 1 ;
    int R = (rand() % Horizontal_distance) + 1 ;
    int X_L_type = room0.x + room0.tool - 1 + R ;

    for(int i=room0.x + room0.tool;i<=X_L_type;i++)
        Map[t + deltaY0][i] = CORRIDOR ;

    for(int i=room1.x -1 ; i>=X_L_type ; i--)
        Map[T - deltaY1][i] = CORRIDOR ;

    if(t + deltaY0 < T - deltaY1){
        for(int i = t + deltaY0 ; i<=T - deltaY1 ;i++)
            Map[i][X_L_type] = CORRIDOR ;
    }
    else{
        for(int i = T - deltaY1 ; i<= t + deltaY0 ;i++)
            Map[i][X_L_type] = CORRIDOR ;

    }
    return 1;
}

int Adding_doors_and_corridors_Vertical(struct Room room0,struct Room room3){
    int T = second_largest(room0.x,room0.x+room0.tool-1,room3.x,room3.x+room3.tool-1);
    int t = second_smallest(room0.x,room0.x+room0.tool-1,room3.x,room3.x+room3.tool-1);
    
    if((T-t-1)<2){
        if(T-t-1 == 1){
            Map[room3.y][T-1] = DOOR ;
            Map[room0.y + room0.arz -1][t+1] = DOOR ;
            for(int i = room3.y - 1 ; i >= room0.y + room0.arz ; i--)
                Map[i][t+1] = CORRIDOR ;
            return 1 ;
        }
        return 0;
    }
    int deltaX0 = ( rand() % (T-t-1) ) + 1 ;
    Map[room0.y + room0.arz -1][t + deltaX0] = DOOR ;
   
    int deltaX3;
    while(1){
        deltaX3 =( rand() % (T-t-1) ) + 1 ;
        if( (T - deltaX3 ) != ( t + deltaX0 ) ){
            Map[room3.y][T - deltaX3] = DOOR ;
            break ;
        }
    }

    int Vertical_distance = room3.y - (room0.y + room0.arz -1) - 1 ;
    int R = (rand() % Vertical_distance) + 1 ;
    int X_L_type = room0.y + room0.arz - 1 + R ;

    for(int i=room0.y + room0.arz;i<=X_L_type;i++)
        Map[i][t + deltaX0] = CORRIDOR ;

    for(int i=room3.y -1 ; i>=X_L_type ; i--)
        Map[i][T - deltaX3] = CORRIDOR ;

    if(t + deltaX0 < T - deltaX3){
        for(int i = t + deltaX0 ; i<=T - deltaX3 ;i++)
            Map[X_L_type][i] = CORRIDOR ;
    }
    else{
        for(int i = T - deltaX3 ; i<= t + deltaX0 ;i++)
            Map[X_L_type][i] = CORRIDOR ;

    }
    return 1;
}

int Adding_pillar(struct Room room){
    int m = rand() % 3 ;
    if(m == 0 || m == 1)
        return 0;
    int i = room.x + (rand() % (room.tool - 2)) + 1 ;
    int j = room.y + (rand() % (room.arz -2)) + 1 ;
    Map[j][i] = PILLAR ;
    return 1 ;
}

int Adding_gold(struct Room room){
    int m = rand() % 10 ;
    if(m == 0 || m == 1 || m == 2){
        int i = room.x + (rand() % (room.tool - 2)) + 1 ;
        int j = room.y + (rand() % (room.arz -2)) + 1 ;
        Map[j][i] = GOLD ;
        return 1;} 
    return 0 ;
}

int Adding_black_gold(struct Room room){
    int m = rand() % 10 ;
    if(m == 0){
        int i = room.x + (rand() % (room.tool - 2)) + 1 ;
        int j = room.y + (rand() % (room.arz -2)) + 1 ;
        Map[j][i] = BLACK_GOLD ;
        return 1;} 
    return 0 ;
}

int Adding_food(struct Room room){
      int m = rand() % 5 ;
    if(m == 0 || m == 1 || m==2){
        int i = room.x + (rand() % (room.tool - 2)) + 1 ;
        int j = room.y + (rand() % (room.arz -2)) + 1 ;
        int n = rand() % 4 ;
        if(n==0)
            Map[j][i] = FOOD ;
        else if(n == 1)
            Map[j][i] = P_Fd ;
        else if(n == 2)
            Map[j][i] = B_Fd ;
        else 
            Map[j][i] = M_Fd ;
        return 1;} 
    return 0 ;
}

void Adding_traps(struct Room room){
    for(int i=0 ; i<difficulty_level ; i++){
        int m = rand() % 2 ;
        if(m == 0)
            continue;;
        int i = room.x + (rand() % (room.tool - 2)) + 1 ;
        int j = room.y + (rand() % (room.arz -2)) + 1 ;
        Map[j][i] = TRAP ;}
}

int Adding_dagger(struct Room room){
      int m = rand() % 5 ;
    if(m == 0){
        int i = room.x + (rand() % (room.tool - 2)) + 1 ;
        int j = room.y + (rand() % (room.arz -2)) + 1 ;
        Map[j][i] = DAGGER ;
        return 1;} 
    return 0 ;
}

int Adding_magic_wand(struct Room room){
      int m = rand() % 10 ;
    if(m == 0){
        int i = room.x + (rand() % (room.tool - 2)) + 1 ;
        int j = room.y + (rand() % (room.arz -2)) + 1 ;
        Map[j][i] = MAGIC_WAND ;
        return 1;} 
    return 0 ;
}

int Adding_normal_arrow(struct Room room){
      int m = rand() % 5 ;
    if(m == 0 || m == 1 || m == 2){
        int i = room.x + (rand() % (room.tool - 2)) + 1 ;
        int j = room.y + (rand() % (room.arz -2)) + 1 ;
        Map[j][i] = NORMAL_ARROW ;
        return 1;} 
    return 0 ;
}

int Adding_sword(struct Room room){
      int m = rand() % 5 ;
    if(m == 0){
        int i = room.x + (rand() % (room.tool - 2)) + 1 ;
        int j = room.y + (rand() % (room.arz -2)) + 1 ;
        Map[j][i] = SWORD ;
        return 1;} 
    return 0 ;
}

int Adding_potion(struct Room room){
    int m = rand() % 5 ;
    if(room.type != 'e'){
        if(m == 0 || m==1){
            int i = room.x + (rand() % (room.tool - 2)) + 1 ;
            int j = room.y + (rand() % (room.arz -2)) + 1 ;
            int n = rand() % 3 ;
            if(n == 0)
                Map[j][i] = HEALTH_POTION ;
            else if(n == 1)
                Map[j][i] = SPEED_POTION ;
            else 
                Map[j][i] = DAMAGE_POTION ;
            return 1;}}
    else{
        for(int i=0;i<3;i++){
            int i = room.x + (rand() % (room.tool - 2)) + 1 ;
            int j = room.y + (rand() % (room.arz -2)) + 1 ;
            int n = rand() % 3 ;
            if(n == 0)
                Map[j][i] = HEALTH_POTION ;
            else if(n == 1)
                Map[j][i] = SPEED_POTION ;
            else 
                Map[j][i] = DAMAGE_POTION ;
        }
    }
    return 0 ;
}

void Adding_exit_staircase(struct Room room){
    int i = room.x + (rand() % (room.tool - 2)) + 1 ;
    int j = room.y + (rand() % (room.arz -2)) + 1 ;
    x_exit_staircase = i;
    y_exit_staircase = j;
    if(Hero.level != 4)
        Map[j][i] = EXIT_STAIRCASE ;
    else    
        Map[j][i] = TREASURE_ROOM_ENTRANCE ;
}

void Adding_hero(struct Room room){
    while(1){
    int i = room.x + (rand() % (room.tool - 2)) + 1 ;
    int j = room.y + (rand() % (room.arz -2)) + 1 ;
    if(Map[j][i] != EXIT_STAIRCASE ){
        Hero.y = j;
        Hero.x = i;
        Showing_situation_Map[j][i] = VISIBLE ;
        break ;}
    }

}

void Generate_map() {
    // srand(time(NULL));
    if(Hero.level == 1)
        entrance_num = rand() % 6 ;
    else
        entrance_num = exit_num ;    

    
    while(1){
        exit_num = (rand() % 6) ;
        if(exit_num != entrance_num)
            break;
    }
    //Initializing_Monsters();
    
    Initializing_Maps();
    rooms[0].y = (rand() % 4)    ; rooms[0].x = (rand() % 4)    ;
    rooms[1].y = (rand() % 4)    ; rooms[1].x = (rand() % 4)+15 ;
    rooms[2].y = (rand() % 4)    ; rooms[2].x = (rand() % 4)+30 ;
    rooms[3].y = (rand() % 4)+13 ; rooms[3].x = (rand() % 4)    ;
    rooms[4].y = (rand() % 4)+13 ; rooms[4].x = (rand() % 4)+15 ;
    rooms[5].y = (rand() % 4)+13 ; rooms[5].x = (rand() % 4)+30 ;

    for(int i=0;i<MAX_ROOMS;i++){
        rooms[i].arz = rand() % 11 ; rooms[i].tool = rand() % 11 ;
        if(rooms[i].arz<4)
            rooms[i].arz+=6;
        
        if(rooms[i].tool<4)
            rooms[i].tool+=6;

        rooms[i].type = 'n' ;
        
    }
    rooms[6].type = 'n';
    if(Hero.level != 1){
        rooms[entrance_num].x = rooms[6].x ;
        rooms[entrance_num].y = rooms[6].y ;
        rooms[entrance_num].tool = rooms[6].tool ;
        rooms[entrance_num].arz = rooms[6].arz ;
        int g = rand() % 10 ;
        if(g>=0 && g<=5)
            rooms[entrance_num].type = 'e';
    }





    rooms[6].x = rooms[exit_num].x ;
    rooms[6].y = rooms[exit_num].y ;
    rooms[6].arz = rooms[exit_num].arz ;
    rooms[6].tool = rooms[exit_num].tool ;

    adjusting_no_monster_in_enchantrooms();

    monster_spawner(0);



    for(int i=0 ; i<MAX_ROOMS ; i++){
        Adding_Room(rooms[i]);
        Adding_windows(rooms[i]);
        Adding_pillar(rooms[i]);
        Adding_traps(rooms[i]);
        Adding_gold(rooms[i]);
        Adding_black_gold(rooms[i]);
        Adding_food(rooms[i]);
        Adding_dagger(rooms[i]);
        Adding_magic_wand(rooms[i]);
        Adding_normal_arrow(rooms[i]);
        Adding_sword(rooms[i]);
        Adding_potion(rooms[i]);
    }

    if(Hero.level == 1){
        Adding_exit_staircase(rooms[exit_num]) ;
        Adding_hero(rooms[(rand() % 6)]) ;}
    else {
        Map[y_exit_staircase][x_exit_staircase] = ENTRANCE_STAIRCASE ;
        Adding_exit_staircase(rooms[exit_num]);
    }
    

    Adding_doors_and_corridors_Horizontal(rooms[0],rooms[1]);Adding_doors_and_corridors_Horizontal(rooms[1],rooms[2]);
    Adding_doors_and_corridors_Horizontal(rooms[3],rooms[4]);Adding_doors_and_corridors_Horizontal(rooms[4],rooms[5]);
    
    Adding_doors_and_corridors_Vertical(rooms[0],rooms[3]);
    Adding_doors_and_corridors_Vertical(rooms[1],rooms[4]);
    Adding_doors_and_corridors_Vertical(rooms[2],rooms[5]);

     for(int i=0;i<Terminal_Vertical_Lenght;i++){
        for(int j=0;j<Terminal_Horizontal_Lenght;j++){
            if(Map[i][j] == TRAP)
            Traps_situation_Map[i][j] = HIDE ;
        }
    }
    
}

int Marking_rooms_VISIBLE (struct Room room) {
    for(int i = room.y ; i <= room.y+room.arz-1 ; i++){
        for(int j = room.x ; j<= room.x+room.tool-1 ; j++){
            if(Showing_situation_Map[i][j] == VISIBLE ){
                for(int k = room.y ; k <= room.y+room.arz-1 ; k++){
                    for(int m = room.x ; m<= room.x+room.tool-1 ; m++)
                        Showing_situation_Map[k][m] = VISIBLE ;

                }
                return 1;
            }
        }
    }
return 0;
}

int current_room_num(int x,int y,struct Room rooms[7]){
    for(int i=0;i<6;i++){
        if(x>rooms[i].x && x<rooms[i].x+rooms[i].tool-1 &&  y>rooms[i].y && y<rooms[i].y+rooms[i].arz-1 )
            return i;
    }
    return 6;

}

void preparing_treasure_room(){
    Initializing_Maps() ;
    rooms[0].x = 15 ;
    rooms[0].y = 0 ;
    rooms[0].tool = 20 ;
    rooms[0].arz = 10 ;
    rooms[0].type = 't' ;
    Adding_Room(rooms[0]);
    Adding_hero(rooms[0]);
    Initializing_Monsters(1) ;
    for(int i=0;i<30;i++)
        Adding_traps(rooms[0]);
    
    for(int i=0;i<10;i++){
        Adding_gold(rooms[0]);
        Adding_black_gold(rooms[0]);
    }
    monster_spawner(1) ;
}


//Print Haye Tabe haye 1-Print_all 2-Showing_Map 3-clearing_map_area  ba ham hamaang ast !!

// int Game_Condition();
// void Handeling_input();
// void Printing_Map();
// void Updating_Map();
// void Updating_Player_Data();

void Print_all(){

    start_color();
    init_pair(1,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);
    init_pair(3,COLOR_BLUE,COLOR_WHITE);
    init_pair(4,COLOR_WHITE,COLOR_BLACK);
    init_pair(5,COLOR_RED,COLOR_BLACK);
    init_pair(6,COLOR_BLUE,COLOR_BLACK);
    init_pair(7,COLOR_YELLOW,COLOR_BLACK);
    attron(A_BOLD);


    
    for(int i=0;i<Terminal_Vertical_Lenght;i++){
        for(int j=0;j<Terminal_Horizontal_Lenght;j++){
            int n=0;
            int monsters_num ;
            if(Hero.level == 5)
                monsters_num = TREASURE_ROOM_MONSTERS_NUM ;
            else 
                monsters_num = MONSTERS_NUM ;
            for(int k=0;k<monsters_num;k++){
                if(i == Monsters[k].y && j == Monsters[k].x && Monsters[k].level == Hero.level){
                    attron(COLOR_PAIR(5));
                    mvprintw(i+4,j+50,"%c",Monsters[k].namad);
                    n++;
                    break;
                }
            }
            if(n != 0)
                continue;
            

            // for(int k=0;k<30;k++){
            //     if(i == Monsters[k].y && j == Monsters[k].x && Monsters[k].level == Hero.level){
            //         attron(COLOR_PAIR(5));
            //         mvprintw(i+4,j+50,"%c",Monsters[k].namad);
            //         continue;
            //     }
            // }

            if(i == Hero.y && j == Hero.x){
                if(Hero_color == 1)
                    attron(COLOR_PAIR(6));
                else if(Hero_color == 2)
                    attron(COLOR_PAIR(2));
                else if(Hero_color == 3)
                    attron(COLOR_PAIR(7));
                else 
                    attron(COLOR_PAIR(4));
                mvprintw(i+4,j+50,"%c",Hero.Namad);
                continue;
            }
            else if(Map[i][j] == FLOOR){
                attroff(A_BOLD);
                attron(COLOR_PAIR(2));
                mvprintw(i+4,j+50,"%c",Map[i][j]);
                attron(A_BOLD);
                continue;
            }
            else if(Map[i][j] == ENCHANTED_FLOOR){
                attroff(A_BOLD);
                attron(COLOR_PAIR(5));
                mvprintw(i+4,j+50,"%c",FLOOR);
                attron(A_BOLD);
                continue;
            }
            else if(Map[i][j] == DAGGER){
                attron(COLOR_PAIR(4));
                mvaddstr(i+4,j+50,U_1F5E1);
                attroff(COLOR_PAIR(4));
                continue;
            }
            else if(Map[i][j] == '1'){
                attron(COLOR_PAIR(4));
                mvaddstr(i+4,j+50,U_1F5E1);
                attroff(COLOR_PAIR(4));
                continue;
            }
            else if(Map[i][j] == MAGIC_WAND){
                attron(COLOR_PAIR(4));
                mvaddstr(i+4,j+50,U_1FA84);
                attroff(COLOR_PAIR(4));
                continue;
            }
            else if(Map[i][j] == '2'){
                attron(COLOR_PAIR(4));
                mvaddstr(i+4,j+50,U_1FA84);
                attroff(COLOR_PAIR(4));
                continue;
            }
            else if(Map[i][j] == NORMAL_ARROW){
                attron(COLOR_PAIR(4));
                mvaddstr(i+4,j+50,U_27B3);
                attroff(COLOR_PAIR(4));
                continue;
            }
            else if(Map[i][j] == '3'){
                attron(COLOR_PAIR(4));
                mvaddstr(i+4,j+50,U_27B3);
                attroff(COLOR_PAIR(4));
                continue;
            }
            else if(Map[i][j] == SWORD){
                attron(COLOR_PAIR(4));
                mvaddstr(i+4,j+50,U_2694);
                attroff(COLOR_PAIR(4));
                continue;
            }
            else if(Map[i][j] == HEALTH_POTION){
                attron(COLOR_PAIR(5));
                mvaddstr(i+4,j+50,U_HEART);
                attroff(COLOR_PAIR(5));
                continue;
            }
            else if(Map[i][j] == SPEED_POTION){
                //attron(A_BOLD);
                attron(COLOR_PAIR(6));
                mvaddstr(i+4,j+50,U_LIGHTNING);
                attroff(COLOR_PAIR(6));
                //attroff(A_BOLD);
                continue;
            }
            else if(Map[i][j] == DAMAGE_POTION){
                attron(COLOR_PAIR(5));
                mvaddstr(i+4,j+50,U_EXPLOSION);
                attroff(COLOR_PAIR(5));
                continue;
            }
            else if(Map[i][j] == LOWWER_Horizontal_WALL){
                //attron(A_BOLD);
                attron(COLOR_PAIR(1));
                mvaddstr(i+4,j+50,U_LOWWER_HORIZONTAL_WALL);
                attroff(COLOR_PAIR(1));
                //attroff(A_BOLD);
                continue;
            }
            else if(Map[i][j] == GOLD){
                //attron(A_BOLD);
                attron(COLOR_PAIR(7));
                mvaddstr(i+4,j+50,U_GOLDEN_COIN);
                attroff(COLOR_PAIR(7));
                //attroff(A_BOLD);
                continue;
            }
            else if(Map[i][j] == TREASURE_ROOM_ENTRANCE){
                attron(COLOR_PAIR(7));
                mvaddstr(i+4,j+50,U_1F4B0);
                attroff(COLOR_PAIR(7));
                continue;
            }
            else if(Map[i][j] == B_Fd){
                attron(COLOR_PAIR(2));
                mvaddstr(i+4,j+50,U_B_Fd);
                attroff(COLOR_PAIR(2));
                continue;
            }
            else if(Map[i][j] == P_Fd){
                attron(COLOR_PAIR(2));
                mvaddstr(i+4,j+50,U_P_Fd);
                attroff(COLOR_PAIR(2));
                continue;
            }
            else if(Map[i][j] == M_Fd){
                attron(COLOR_PAIR(2));
                mvaddstr(i+4,j+50,U_M_Fd);
                attroff(COLOR_PAIR(2));
                continue;
            }
            




            else if(Map[i][j] == CORRIDOR)
                attron(COLOR_PAIR(4));
            else if(Map[i][j] == TRAP)
                attron(COLOR_PAIR(5));
            else if(Map[i][j] == BLACK_GOLD || Map[i][j] == FOOD)
                attron(COLOR_PAIR(2));
            else
                attron(COLOR_PAIR(1));
            
            mvprintw(i+4,j+50,"%c",Map[i][j]);
        }
    }





}

void Showing_Map(){
    start_color();
    init_pair(1,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);
    init_pair(3,COLOR_BLUE,COLOR_WHITE);
    init_pair(4,COLOR_WHITE,COLOR_BLACK);
    init_pair(5,COLOR_RED,COLOR_BLACK);
    init_pair(6,COLOR_BLUE,COLOR_BLACK);
    init_pair(7,COLOR_YELLOW,COLOR_BLACK);
    
    attron(A_BOLD);
    
     for(int i=0 ; i<Terminal_Vertical_Lenght ; i++){
        for(int j=0 ; j<Terminal_Horizontal_Lenght ; j++){
            if(Showing_situation_Map[i][j] == VISIBLE){
                int n=0;
                int monsters_num ;
                if(Hero.level == 5)
                    monsters_num = TREASURE_ROOM_MONSTERS_NUM ;
                else 
                    monsters_num = MONSTERS_NUM ;
                for(int k=0;k<monsters_num;k++){
                    if(i == Monsters[k].y && j == Monsters[k].x && Monsters[k].level == Hero.level){
                        attron(COLOR_PAIR(5));
                        mvprintw(i+4,j+50,"%c",Monsters[k].namad);
                    n++;
                    break;
                    }
                }
                if(n!=0)
                    continue;                
                // for(int k=0;k<30;k++){
                //     if(i == Monsters[k].y && j == Monsters[k].x && Monsters[k].level == Hero.level){
                //         attron(COLOR_PAIR(5));
                //         mvprintw(i+4,j+50,"%c",Monsters[k].namad);
                //         continue;
                //     }
                // }
                if(i == Hero.y && j == Hero.x){
                if(Hero_color == 1)
                    attron(COLOR_PAIR(6));
                else if(Hero_color == 2)
                    attron(COLOR_PAIR(2));
                else if(Hero_color == 3)
                    attron(COLOR_PAIR(7));
                else 
                    attron(COLOR_PAIR(4));
                mvprintw(i+4,j+50,"%c",Hero.Namad);
                continue;
                }
                else if(Map[i][j] == FLOOR){
                    attroff(A_BOLD);
                    attron(COLOR_PAIR(2));
                    mvprintw(i+4,j+50,"%c",Map[i][j]);
                    attron(A_BOLD);
                    continue;
                }
                else if(Map[i][j] == ENCHANTED_FLOOR){
                attroff(A_BOLD);
                attron(COLOR_PAIR(5));
                mvprintw(i+4,j+50,"%c",FLOOR);
                attron(A_BOLD);
                continue;
                }
                else if(Map[i][j] == DAGGER){
                    attron(COLOR_PAIR(4));
                    mvaddstr(i+4,j+50,U_1F5E1);
                    attroff(COLOR_PAIR(4));
                    continue;
                }
                else if(Map[i][j] == '1'){
                    attron(COLOR_PAIR(4));
                    mvaddstr(i+4,j+50,U_1F5E1);
                    attroff(COLOR_PAIR(4));
                    continue;
                }
                else if(Map[i][j] == MAGIC_WAND){
                    attron(COLOR_PAIR(4));
                    mvaddstr(i+4,j+50,U_1FA84);
                    attroff(COLOR_PAIR(4));
                    continue;
                }
                else if(Map[i][j] == '2'){
                    attron(COLOR_PAIR(4));
                    mvaddstr(i+4,j+50,U_1FA84);
                    attroff(COLOR_PAIR(4));
                    continue;
                }
                else if(Map[i][j] == NORMAL_ARROW){
                    attron(COLOR_PAIR(4));
                    mvaddstr(i+4,j+50,U_27B3);
                    attroff(COLOR_PAIR(4));
                    continue;
                }
                else if(Map[i][j] == '3'){
                    attron(COLOR_PAIR(4));
                    mvaddstr(i+4,j+50,U_27B3);
                    attroff(COLOR_PAIR(4));
                    continue;
                }
                else if(Map[i][j] == SWORD){
                    attron(COLOR_PAIR(4));
                    mvaddstr(i+4,j+50,U_2694);
                    attroff(COLOR_PAIR(4));
                    continue;
                }
                else if(Map[i][j] == HEALTH_POTION){
                    attron(COLOR_PAIR(5));
                    mvaddstr(i+4,j+50,U_HEART);
                    attroff(COLOR_PAIR(5));
                    continue;
                }
                else if(Map[i][j] == SPEED_POTION){
                    //attron(A_BOLD);
                    attron(COLOR_PAIR(6));
                    mvaddstr(i+4,j+50,U_LIGHTNING);
                    attroff(COLOR_PAIR(6));
                    //attroff(A_BOLD);
                    continue;
                }
                else if(Map[i][j] == DAMAGE_POTION){
                    attron(COLOR_PAIR(5));
                    mvaddstr(i+4,j+50,U_EXPLOSION);
                    attroff(COLOR_PAIR(5));
                    continue;
                }
                else if(Map[i][j] == LOWWER_Horizontal_WALL){
                    //attron(A_BOLD);
                    attron(COLOR_PAIR(1));
                    mvaddstr(i+4,j+50,U_LOWWER_HORIZONTAL_WALL);
                    attroff(COLOR_PAIR(1));
                    //attroff(A_BOLD);
                    continue;
                }
                else if(Map[i][j] == GOLD){
                    //attron(A_BOLD);
                    attron(COLOR_PAIR(7));
                    mvaddstr(i+4,j+50,U_GOLDEN_COIN);
                    attroff(COLOR_PAIR(7));
                    //attroff(A_BOLD);
                    continue;
                }
                else if(Map[i][j] == TREASURE_ROOM_ENTRANCE){
                    attron(COLOR_PAIR(7));
                    mvaddstr(i+4,j+50,U_1F4B0);
                    attroff(COLOR_PAIR(7));
                    continue;
                }
                else if(Map[i][j] == B_Fd){
                    attron(COLOR_PAIR(2));
                    mvaddstr(i+4,j+50,U_B_Fd);
                    attroff(COLOR_PAIR(2));
                    continue;
                }
                else if(Map[i][j] == P_Fd){
                    attron(COLOR_PAIR(2));
                    mvaddstr(i+4,j+50,U_P_Fd);
                    attroff(COLOR_PAIR(2));
                    continue;
                }
                else if(Map[i][j] == M_Fd){
                    attron(COLOR_PAIR(2));
                    mvaddstr(i+4,j+50,U_M_Fd);
                    attroff(COLOR_PAIR(2));
                    continue;
                }
            
                
                else if(Map[i][j] == CORRIDOR)
                    attron(COLOR_PAIR(4));
                
                else if(Map[i][j] == TRAP){
                    if(Traps_situation_Map[i][j] == HIDE){
                        attroff(A_BOLD);
                        attron(COLOR_PAIR(2));
                        mvprintw(i+4,j+50,"%c",FLOOR);
                        attron(A_BOLD);
                        continue;
                        }
                    else
                        attron(COLOR_PAIR(5));
                }
                
                else if(Map[i][j] == BLACK_GOLD || Map[i][j] == FOOD)
                    attron(COLOR_PAIR(2));
                else
                    attron(COLOR_PAIR(1));
                
                mvprintw(i+4,j+50,"%c",Map[i][j]);
                }
            }
        }       
    }

int Emkan_Harekat(int y,int x){
    if(Map[y][x] == WINDOW || Map[y][x] == UPPER_Horizontal_WALL || Map[y][x] == LOWWER_Horizontal_WALL || Map[y][x] == Vertical_WALL || Map[y][x] == ' ' || Map[y][x] == PILLAR)
        return 0;
    return 1;
}

void clearing_message_section(){
    noecho();
    for(int i=0; i<170 ; i++)
        mvprintw(1,i," ");
}

void clearing_damage_message_section(){
    noecho();
    for(int i=0; i<170 ; i++)
        mvprintw(2,i," ");
}

void clearing_player_data_section(){
    for(int i=0; i<150 ; i++)
        mvprintw(32,i," ");
}

void uploading_player_data_on_screen(){
    attron(COLOR_PAIR(7));
    mvprintw(32,30,"Level : %d",Hero.level);
    mvprintw(32,50,"Point : %d",Hero.point);
    mvprintw(32,70,"Health : %d",Hero.health);
    mvprintw(32,90,"Food number : %d",Hero.food);
    mvprintw(32,110,"Hunger : %d",Hero.hunger);

}

void clearing_map_area(){
    for(int i=0;i<Terminal_Vertical_Lenght+4;i++){
        for(int j=0;j<Terminal_Horizontal_Lenght+130;j++)
            mvprintw(i+2,j," ");
        }
}

void clearing_messagebar_weapon_menu(){
    for(int i=0;i<150;i++)
        mvprintw(24,i," ");
}

int shotting(int aiming_direction){
    int range,weapon_damage,weapon_x,weapon_y;
    int m = 0 ;
    
    char weapon;
    weapon_x = Hero.x ;
    weapon_y = Hero.y ;

    if(Hero.current_weapon == DAGGER){
        range = 5 ;
        weapon_damage = 12*Hero.damage_coefficient;
        weapon = '1';
        Hero.DAGGER_num--;
        if(Hero.DAGGER_num <= -1){
            m++;
            Hero.DAGGER_num = 0;}
    }
    else if(Hero.current_weapon == MAGIC_WAND){
        range = 10 ;
        weapon_damage = 15*Hero.damage_coefficient;
        weapon = '2';
        Hero.MAGIC_WAND_num--;
        if(Hero.MAGIC_WAND_num <= -1){
            m++;
            Hero.MAGIC_WAND_num = 0;}
    }
    else if(Hero.current_weapon == NORMAL_ARROW){
        range = 5 ;
        weapon_damage = 5*Hero.damage_coefficient;
        weapon = '3';
        Hero.NORMAL_ARROW_num--;
        if(Hero.NORMAL_ARROW_num <= -1){
            m++;
            Hero.NORMAL_ARROW_num = 0;}
    }
    if(m>0){
        clearing_message_section();
        attron(COLOR_PAIR(4));
        mvprintw(MESSAGE_Y,MESSAGE_X,"You are out of ammo! Change your weapon or refeel you");
        return 0;
    }


    int q = 0;
    while(range>0){
        int p = 0 ;
        for(int i=0;i<MONSTERS_NUM;i++){
            if(Monsters[i].level == Hero.level && Monsters[i].health > 0 )
                if(weapon_x == Monsters[i].x && weapon_y == Monsters[i].y){
                    
                    if(Hero.current_weapon == MAGIC_WAND){
                        Monsters[i].followsteps = 0;}
                    
                Monsters[i].health-= weapon_damage ;
                clearing_damage_message_section();
                attron(COLOR_PAIR(4));
                mvprintw(MESSAGE_Y+1,MESSAGE_X,"%c was attacked by your hit! %c's health is %d",Monsters[i].namad,Monsters[i].namad,Monsters[i].health);
                p++ ;
                break;
                } 
        }
        if(p>0){
            q++; 
            break;}
        
        if(aiming_direction == 1){
            if(Emkan_Harekat(weapon_y+1,weapon_x-1)){
                weapon_y++;
                weapon_x--;
            }
            else{
                Map[weapon_y][weapon_x] = weapon ;
                break;}
        }
        else if(aiming_direction == 2){
            if(Emkan_Harekat(weapon_y+1,weapon_x)){
                weapon_y++;
            }
            else{
                Map[weapon_y][weapon_x] = weapon ;
                break;}
        }
        else if(aiming_direction == 3){
            if(Emkan_Harekat(weapon_y+1,weapon_x+1)){
                weapon_y++;
                weapon_x++;
            }
            else{
                Map[weapon_y][weapon_x] = weapon ;
                break;}            
        }
        else if(aiming_direction == 4){
            if(Emkan_Harekat(weapon_y,weapon_x-1)){
                weapon_x--;
            }
            else{
                Map[weapon_y][weapon_x] = weapon ;
                break;}            
        }
        else if(aiming_direction == 6){
            if(Emkan_Harekat(weapon_y,weapon_x+1)){
                weapon_x++;
            }
            else{
                Map[weapon_y][weapon_x] = weapon ;
                break;}            
        }
        else if(aiming_direction == 7){
            if(Emkan_Harekat(weapon_y-1,weapon_x-1)){
                weapon_y--;
                weapon_x--;
            }
            else{
                Map[weapon_y][weapon_x] = weapon ;
                break;}            
        }
        else if(aiming_direction == 8){
            if(Emkan_Harekat(weapon_y-1,weapon_x)){
                weapon_y--;
            }
            else{
                Map[weapon_y][weapon_x] = weapon ;
                break;}            
        }
        else if(aiming_direction == 9){
            if(Emkan_Harekat(weapon_y-1,weapon_x+1)){
                weapon_y--;
                weapon_x++;
            }
            else{
                Map[weapon_y][weapon_x] = weapon ;
                break;}            
        }
        range--;
    }
if(q == 0)
    Map[weapon_y][weapon_x] = weapon ;
return 0;
}

void Updating_by_input(char ch){
    int n = Hero.speed_coefficient ;
    clearing_message_section();
    clearing_damage_message_section();
    // clearing_player_data_section();
    // uploading_player_data_on_screen();
    int p = 0 ;
    if(ch == '1'){
        if(Emkan_Harekat(Hero.y+n,Hero.x-n)){
            Hero.y+=n;
            Hero.x-=n;
        }}
    else if(ch == '2'){
        if(Emkan_Harekat(Hero.y+n,Hero.x)){
            Hero.y+=n;
        }}
    else if(ch == '3'){
        if(Emkan_Harekat(Hero.y+n,Hero.x+n)){
            Hero.y+=n;
            Hero.x+=n;
        }}
    else if(ch == '4'){
        if(Emkan_Harekat(Hero.y,Hero.x-n)){
            Hero.x-=n;
        }   }
    else if(ch == '6'){
        if(Emkan_Harekat(Hero.y,Hero.x+n)){
            Hero.x+=n;
        }}
    else if(ch == '7'){
        if(Emkan_Harekat(Hero.y-n,Hero.x-n)){
            Hero.y-=n;
            Hero.x-=n;
        }}
    else if(ch == '8'){
        if(Emkan_Harekat(Hero.y-n,Hero.x)){
            Hero.y-=n;
        }}
    else if(ch == '9'){
        if(Emkan_Harekat(Hero.y-n,Hero.x+n)){
            Hero.y-=n;
            Hero.x+=n;
        }}
    else if(ch == 'M'){
        M_var ++ ;}
    else if(ch == 's'){
        Traps_situation_Map[Hero.y+1][Hero.x+1] = VISIBLE; Traps_situation_Map[Hero.y+1][Hero.x] = VISIBLE; Traps_situation_Map[Hero.y+1][Hero.x-1] = VISIBLE;
        Traps_situation_Map[Hero.y][Hero.x+1] = VISIBLE; Traps_situation_Map[Hero.y][Hero.x] = VISIBLE; Traps_situation_Map[Hero.y][Hero.x-1] = VISIBLE;
        Traps_situation_Map[Hero.y-1][Hero.x+1] = VISIBLE; Traps_situation_Map[Hero.y-1][Hero.x] = VISIBLE; Traps_situation_Map[Hero.y-1][Hero.x-1] = VISIBLE;
    }
    else if(ch == 'f'){
        ch = getch();
        if( ch == '1'){
            while(Emkan_Harekat(Hero.y+1,Hero.x-1)){
            Hero.y++;
            Hero.x--;}}
        else if( ch == '2'){
            while(Emkan_Harekat(Hero.y+1,Hero.x)){
                Hero.y++;
                }}
        else if( ch == '3'){
            while(Emkan_Harekat(Hero.y+1,Hero.x+1)){
                Hero.y++;
                Hero.x++;}}
        else if( ch == '4'){
            while(Emkan_Harekat(Hero.y,Hero.x-1)){
                Hero.x--;}}
        else if( ch == '6'){
            while(Emkan_Harekat(Hero.y,Hero.x+1)){
                Hero.x++;}}
        else if( ch == '7'){
            while(Emkan_Harekat(Hero.y-1,Hero.x-1)){
                Hero.y--;
                Hero.x--;}}
        else if( ch == '8'){
            while(Emkan_Harekat(Hero.y-1,Hero.x)){
                Hero.y--;
                }}
        else if( ch == '9'){
            while(Emkan_Harekat(Hero.y-1,Hero.x+1)){
                Hero.y--;
                Hero.x++;}}
    }
    else if(ch == 'g'){
        p++;
        ch = getch();
        if( ch == '1'){
            if(Emkan_Harekat(Hero.y+1,Hero.x-1)){
            Hero.y++;
            Hero.x--;}}
        else if( ch == '2'){
            if(Emkan_Harekat(Hero.y+1,Hero.x)){
                Hero.y++;
                }}
        else if( ch == '3'){
            if(Emkan_Harekat(Hero.y+1,Hero.x+1)){
                Hero.y++;
                Hero.x++;}}
        else if( ch == '4'){
            if(Emkan_Harekat(Hero.y,Hero.x-1)){
                Hero.x--;}}
        else if( ch == '6'){
            if(Emkan_Harekat(Hero.y,Hero.x+1)){
                Hero.x++;}}
        else if( ch == '7'){
            if(Emkan_Harekat(Hero.y-1,Hero.x-1)){
                Hero.y--;
                Hero.x--;}}
        else if( ch == '8'){
            if(Emkan_Harekat(Hero.y-1,Hero.x)){
                Hero.y--;
                }}
        else if( ch == '9'){
            if(Emkan_Harekat(Hero.y-1,Hero.x+1)){
                Hero.y--;
                Hero.x++;}}
    }
    else if(ch == 'E'){
        if(Hero.food>0){
            int n ;
            int attempts = 1000 ;
            char a[10];
            while(attempts>0){
                n = rand() % 4 ;
                if( n == 0 ){
                    if(Hero.n_fd > 0){
                        Hero.n_fd -- ;
                        strcpy(a,"normal");
                        Hero.health += 10 ;
                        break;
                    }
                }
                else if(n == 1){
                    if(Hero.p_fd > 0){
                        Hero.p_fd -- ;
                        strcpy(a,"perfect");
                        Hero.damage_coefficient = 2 ;
                        Hero.potion_lasting = 10 ;
                        Hero.health += 10 ;
                        break;
                    }
                }
                else if(n == 2){
                    if(Hero.m_fd > 0){
                        Hero.m_fd -- ;
                        strcpy(a,"magic");
                        Hero.speed_coefficient = 2 ;
                        Hero.potion_lasting = 10 ;
                        Hero.health += 10 ;
                        break;
                    }
                }
                else{
                    if(Hero.b_fd > 0){
                        Hero.b_fd -- ;
                        strcpy(a,"bad");
                        Hero.health -= 10 ;
                        break;
                    }
                }
            attempts -- ;
            }
            attron(COLOR_PAIR(4));
            mvprintw(MESSAGE_Y,MESSAGE_X,"You used one of you %s foods",a);
            attroff(COLOR_PAIR(4));
            Hero.hunger = 0;
            Hero.food-- ;}
        else {
            attron(COLOR_PAIR(4));
            mvprintw(MESSAGE_Y,MESSAGE_X,"You dont have any food!");
        }
    }
    else if(ch == 'i'){
        clearing_map_area();
        
        attron(COLOR_PAIR(4));
        mvprintw(1,60,"This menu shows your weapon inventory");
        for(int i=0;i<10;i++)
            mvprintw(i+3,75,"|");
        mvprintw(3,35,"Short range weapons:");
        mvprintw(3,100,"Long range weapons:");

        attron(COLOR_PAIR(5));
        mvprintw(5,80,"Name");
        mvprintw(5,95,"Symbol");
        mvprintw(5,110,"Number");
        mvprintw(5,125,"Range");
        mvprintw(5,140,"Power");

        mvprintw(5,15,"Name");
        mvprintw(5,30,"Symbol");
        mvprintw(5,45,"Availability");
        mvprintw(5,60,"Power");

        attron(COLOR_PAIR(7));
        mvprintw(7,80,"Dagger");
        mvprintw(7,95,"%c",DAGGER);
        mvprintw(7,110,"%d",Hero.DAGGER_num);
        mvprintw(7,125,"5");
        mvprintw(7,140,"12");

        mvprintw(7,15,"Mace");
        mvprintw(7,30,"%c",MACE);
        mvprintw(7,45,"Y");
        mvprintw(7,60,"5");

        mvprintw(9,80,"Magic Wand");
        mvprintw(9,95,"%c",MAGIC_WAND);
        mvprintw(9,110,"%d",Hero.MAGIC_WAND_num);
        mvprintw(9,125,"10");
        mvprintw(9,140,"15");

        mvprintw(9,15,"Sword");
        mvprintw(9,30,"%c",SWORD);
        if(Hero.SWORD_num == 1)
            mvprintw(9,45,"Y");
        else    
            mvprintw(9,45,"N");
        mvprintw(9,60,"10");

        mvprintw(11,80,"Normal Arrow");
        mvprintw(11,95,"%c",NORMAL_ARROW);
        mvprintw(11,110,"%d",Hero.NORMAL_ARROW_num);
        mvprintw(11,125,"5");
        mvprintw(11,140,"10");


        attron(COLOR_PAIR(4));
        mvprintw(14,60,"Press q to quit this menu");
        mvprintw(16,60,"You can press p to put your current weapon in the bag");
        mvprintw(18,60,"You can change your current weapon by pressing related symbol");


    while(1){
        char ch2 = getch();
        if(ch2 == 'q')
            break;    
        else if(ch2 == 'p')
            Hero.current_weapon =' ';
        else if(ch2 == 'd'){
            clearing_messagebar_weapon_menu();
            attron(COLOR_PAIR(7));
            if(Hero.current_weapon == ' '){
                if(Hero.DAGGER_num != 0){
                    Hero.current_weapon = DAGGER ;
                    
                    mvprintw(24,60,"Your current weapon has been changed to dagger");
                }
                else 
                    mvprintw(24,60,"You don't have any of the requested weapon!");
            }
            else {
                if(Hero.current_weapon == 'd')
                    mvprintw(24,60,"The requested weapon is your current one!");
                else     
                    mvprintw(24,60,"First put you current weapon in the bag");
                }
            attroff(COLOR_PAIR(7));
        }

        else if(ch2 == 'w'){
            clearing_messagebar_weapon_menu();
            attron(COLOR_PAIR(7));
            if(Hero.current_weapon == ' '){
                if(Hero.MAGIC_WAND_num != 0){
                    Hero.current_weapon = MAGIC_WAND ;
                    
                    mvprintw(24,60,"Your current weapon has been changed to magic wand");
                }
                else 
                    mvprintw(24,60,"You don't have any of the requested weapon!");
            }
            else {
                if(Hero.current_weapon == 'w')
                    mvprintw(24,60,"The requested weapon is your current one!");
                else     
                    mvprintw(24,60,"First put you current weapon in the bag");
                }
            attroff(COLOR_PAIR(7));
        }

        else if(ch2 == 's'){
            clearing_messagebar_weapon_menu();
            attron(COLOR_PAIR(7));
            if(Hero.current_weapon == ' '){
                if(Hero.SWORD_num != 0){
                    Hero.current_weapon = SWORD ;
                    
                    mvprintw(24,60,"Your current weapon has been changed to sword");
                }
                else 
                    mvprintw(24,60,"You don't have any of the requested weapon!");
            }
           else {
                if(Hero.current_weapon == 's')
                    mvprintw(24,60,"The requested weapon is your current one!");
                else     
                    mvprintw(24,60,"First put you current weapon in the bag");
                }
            attroff(COLOR_PAIR(7));
        }

        else if(ch2 == 'a'){
            clearing_messagebar_weapon_menu();
            attron(COLOR_PAIR(7));
            if(Hero.current_weapon == ' '){
                if(Hero.NORMAL_ARROW_num != 0){
                    Hero.current_weapon = NORMAL_ARROW ;
                    
                    mvprintw(24,60,"Your current weapon has been changed to normal arrow");
                }
                else 
                    mvprintw(24,60,"You don't have any of the requested weapon!");
            }
            else {
                if(Hero.current_weapon == 'a')
                    mvprintw(24,60,"The requested weapon is your current one!");
                else     
                    mvprintw(24,60,"First put you current weapon in the bag");
                }
            attroff(COLOR_PAIR(7));
        }

        else if(ch2 == 'm'){
            clearing_messagebar_weapon_menu();
            attron(COLOR_PAIR(7));
            if(Hero.current_weapon == ' '){
                Hero.current_weapon = MACE ;
                mvprintw(24,60,"Your current weapon has been changed to mace");
            }
            else {
                if(Hero.current_weapon == 'm')
                    mvprintw(24,60,"The requested weapon is your current one!");
                else     
                    mvprintw(24,60,"First put you current weapon in the bag");
                }
            attroff(COLOR_PAIR(7));
        }
    }
    clearing_map_area();
          
    }
    else if(ch == ' '){
        if(Hero.current_weapon == ' '){
        attron(COLOR_PAIR(4));
        mvprintw(MESSAGE_Y,MESSAGE_X,"First pick a weapon");
        }
        else{
            if(Hero.current_weapon == MACE || Hero.current_weapon == SWORD){
                int p=0;
                for(int i=0;i<MONSTERS_NUM;i++){
                    if(Monsters[i].level == Hero.level && Monsters[i].health > 0 ){
                        int dx = Hero.x - Monsters[i].x ;
                        int dy = Hero.y - Monsters[i].y ;
                        if(dx>=-1 && dx<= 1 && dy>=-1 && dy <=1){
                            if(Hero.current_weapon == MACE)
                                Monsters[i].health -= 5;
                            else 
                                Monsters[i].health -= 10;    
                            p++;
                                clearing_damage_message_section();   
                                attron(COLOR_PAIR(4));
                                mvprintw(MESSAGE_Y+1,MESSAGE_X,"%c was attacked by your hit! %c's health is %d",Monsters[i].namad,Monsters[i].namad,Monsters[i].health);
                    }
                }
            }
                    if(p==0){
                        clearing_damage_message_section();   
                        attron(COLOR_PAIR(4));
                        mvprintw(MESSAGE_Y+1,MESSAGE_X,"Nobody was attacked by your hit!");}
            }
            else{
                char ch3 = getch();
                if(ch3 == '1')
                    aiming_direction = 1;
                else if(ch3 == '2')
                    aiming_direction = 2;
                else if(ch3 == '3')
                    aiming_direction = 3;
                else if(ch3 == '4')
                    aiming_direction = 4;
                else if(ch3 == '6')
                    aiming_direction = 6;
                else if(ch3 == '7')
                    aiming_direction = 7;
                else if(ch3 == '8')
                    aiming_direction = 8;
                else if(ch3 == '9')
                    aiming_direction = 9;

                shotting(aiming_direction);

            }

        }

    }
    else if(ch == 'a'){
        if(Hero.current_weapon != DAGGER && Hero.current_weapon != MAGIC_WAND && Hero.current_weapon != NORMAL_ARROW){
            clearing_message_section();
            attron(COLOR_PAIR(4));
            mvprintw(MESSAGE_Y,MESSAGE_X,"First pick a long ranged weapon");
        }
        else{
            if(aiming_direction == 0){
            clearing_message_section();
            attron(COLOR_PAIR(4));
            mvprintw(MESSAGE_Y,MESSAGE_X,"You don't have any defualt aiming direction!");
            }
            else 
                shotting(aiming_direction);
        }
    }
    else if(ch == 'p'){
        clearing_map_area();
        
        attron(COLOR_PAIR(4));
        mvprintw(1,60,"This menu shows your potion inventory");

        attron(COLOR_PAIR(5));
        mvprintw(3,40,"Name");
        mvprintw(3,55,"Symbol");
        mvprintw(3,70,"Number");
        mvprintw(3,85,"Effect");
        
        attron(COLOR_PAIR(7));
        mvprintw(6,40,"Health");
        mvprintw(6,55,"h");
        mvprintw(6,70,"%d",Hero.p_helth_num);
        mvprintw(6,85,"It boosts your healing up to twice the normal speed");

        mvprintw(8,40,"Speed");
        mvprintw(8,55,"s");
        mvprintw(8,70,"%d",Hero.p_speed_num);
        mvprintw(8,85,"It boosts your movment up to twice the normal speed");

        mvprintw(10,40,"Damage");
        mvprintw(10,55,"d");
        mvprintw(10,70,"%d",Hero.p_damage_num);
        mvprintw(10,85,"It increases your weapons powers");

        attron(COLOR_PAIR(4));
        mvprintw(14,60,"Press q to quit this menu");
        mvprintw(16,60,"You can pick any potion by pressing related symbol");

        while(1){
        char ch2 = getch();
        if(ch2 == 'q')
            break;
        else if(ch2 == 'h'){
            if(Hero.p_helth_num > 0){
                Hero.potion_lasting = 10 ;
                Hero.p_helth_num -- ;
                Hero.current_potion = 'h';
                mvprintw(18,60,"You used one of your health potions");
            }
            else 
                mvprintw(18,60,"You don't have any health potion!");
        }
        else if(ch2 == 's'){
            if(Hero.p_speed_num > 0){
                Hero.potion_lasting = 10 ;
                Hero.p_speed_num -- ;
                Hero.current_potion = 's';
                mvprintw(18,60,"You used one of your speed potions");
            }
            else 
                mvprintw(18,60,"You don't have any speed potion!");
        }
        else if(ch2 == 'd'){
            if(Hero.p_damage_num > 0){
                Hero.potion_lasting = 10 ;
                Hero.p_damage_num -- ;
                Hero.current_potion = 'd';
                mvprintw(18,60,"You used one of your damage potions");
            }
            else 
                mvprintw(18,60,"You don't have any damage potion!");
        }
    }
        

        
    
    clearing_map_area();

    }
    
    
    int Dastavard;

    if(Map[Hero.y][Hero.x] == TRAP){
        Hero.health -= 5;
        Traps_situation_Map[Hero.y][Hero.x] = VISIBLE ;
        clearing_message_section();
        attron(COLOR_PAIR(4));
        mvprintw(MESSAGE_Y,MESSAGE_X,"You were hit by a trap!");}

    if(Map[Hero.y][Hero.x] == GOLD && p == 0){
        Dastavard = rand() % 4 ;
        Dastavard++;
        Dastavard = 5*Dastavard;
        Hero.point += Dastavard;
        if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e')
            Map[Hero.y][Hero.x] = ENCHANTED_FLOOR ;
        else 
            Map[Hero.y][Hero.x] = FLOOR ;
        clearing_message_section();
        attron(COLOR_PAIR(4));
        mvprintw(MESSAGE_Y,MESSAGE_X,"You gained %d points from gold!",Dastavard);}

    if(Map[Hero.y][Hero.x] == BLACK_GOLD && p == 0){
        Dastavard = rand() % 3 ;
        Dastavard += 2;
        Dastavard = 25*Dastavard;
        Hero.point += Dastavard;
        if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e')
            Map[Hero.y][Hero.x] = ENCHANTED_FLOOR ;
        else 
            Map[Hero.y][Hero.x] = FLOOR ;
        clearing_message_section();
        attron(COLOR_PAIR(4));
        mvprintw(MESSAGE_Y,MESSAGE_X,"You gained %d points from black gold!",Dastavard);}

    if((Map[Hero.y][Hero.x] == FOOD || Map[Hero.y][Hero.x] == P_Fd || Map[Hero.y][Hero.x] == M_Fd || Map[Hero.y][Hero.x] == B_Fd )&& p == 0){
        if(Hero.food<6){
            if(Map[Hero.y][Hero.x] == FOOD)
            Hero.n_fd ++ ;
            else if(Map[Hero.y][Hero.x] == P_Fd)
            Hero.p_fd ++ ;
            else if(Map[Hero.y][Hero.x] == M_Fd)
            Hero.m_fd ++ ;
            else if(Map[Hero.y][Hero.x] == B_Fd)
            Hero.b_fd ++ ;
            
            Hero.food++;
            clearing_message_section();
            attron(COLOR_PAIR(4));
            mvprintw(MESSAGE_Y,MESSAGE_X,"You gained a food!");
            if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e')
                Map[Hero.y][Hero.x] = ENCHANTED_FLOOR ;
            else 
                Map[Hero.y][Hero.x] = FLOOR ;}
        else{
            clearing_message_section();
            attron(COLOR_PAIR(4));
            mvprintw(MESSAGE_Y,MESSAGE_X,"Your capacity is full!");}}

    if(Map[Hero.y][Hero.x] == DAGGER && p == 0){
        Hero.DAGGER_num += 10;
        if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e')
            Map[Hero.y][Hero.x] = ENCHANTED_FLOOR ;
        else 
            Map[Hero.y][Hero.x] = FLOOR ;
        clearing_message_section();
        attron(COLOR_PAIR(4));
        mvprintw(MESSAGE_Y,MESSAGE_X,"You collected 10 daggers!");}

    if(Map[Hero.y][Hero.x] == SWORD && p == 0){
        if(Hero.SWORD_num == 0){
            Hero.SWORD_num++;
            if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e')
                Map[Hero.y][Hero.x] = ENCHANTED_FLOOR ;
            else 
                Map[Hero.y][Hero.x] = FLOOR ;
            clearing_message_section();
            attron(COLOR_PAIR(4));
            mvprintw(MESSAGE_Y,MESSAGE_X,"You collected a sword!");}
        else{
            if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e')
                Map[Hero.y][Hero.x] = ENCHANTED_FLOOR ;
            else 
                Map[Hero.y][Hero.x] = FLOOR ;
            clearing_message_section();
            attron(COLOR_PAIR(4));
            mvprintw(MESSAGE_Y,MESSAGE_X,"You already have collected the sword!");
        }
    }

    if(Map[Hero.y][Hero.x] == MAGIC_WAND && p == 0){
        Hero.MAGIC_WAND_num += 8;
        if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e')
            Map[Hero.y][Hero.x] = ENCHANTED_FLOOR ;
        else 
            Map[Hero.y][Hero.x] = FLOOR ;
        clearing_message_section();
        attron(COLOR_PAIR(4));
        mvprintw(MESSAGE_Y,MESSAGE_X,"You collected 8 magic wands!");}

    if(Map[Hero.y][Hero.x] == NORMAL_ARROW && p == 0){
        Hero.NORMAL_ARROW_num += 20;
        if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e')
            Map[Hero.y][Hero.x] = ENCHANTED_FLOOR ;
        else 
            Map[Hero.y][Hero.x] = FLOOR ;
        clearing_message_section();
        attron(COLOR_PAIR(4));
        mvprintw(MESSAGE_Y,MESSAGE_X,"You collected 20 normal arrows!");}

    if(Map[Hero.y][Hero.x] == '1' && p == 0){
        Hero.DAGGER_num += 1;
        if(current_room_num(Hero.x,Hero.y,rooms) == 6)
            Map[Hero.y][Hero.x] = CORRIDOR ;
        else{
            if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e')
                Map[Hero.y][Hero.x] = ENCHANTED_FLOOR ;
            else 
                Map[Hero.y][Hero.x] = FLOOR ;
            }
        clearing_message_section();
        attron(COLOR_PAIR(4));
        mvprintw(MESSAGE_Y,MESSAGE_X,"You recollected a dagger!");}
    
    if(Map[Hero.y][Hero.x] == '2' && p == 0){
        Hero.MAGIC_WAND_num += 1;
        if(current_room_num(Hero.x,Hero.y,rooms) == 6)
            Map[Hero.y][Hero.x] = CORRIDOR ;
        else{
            if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e')
                Map[Hero.y][Hero.x] = ENCHANTED_FLOOR ;
            else 
                Map[Hero.y][Hero.x] = FLOOR ;
            }
        clearing_message_section();
        attron(COLOR_PAIR(4));
        mvprintw(MESSAGE_Y,MESSAGE_X,"You recollected a magic wand!");}

    if(Map[Hero.y][Hero.x] == '3' && p == 0){
        Hero.NORMAL_ARROW_num += 1;
        if(current_room_num(Hero.x,Hero.y,rooms) == 6)
            Map[Hero.y][Hero.x] = CORRIDOR ;
        else{
            if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e')
                Map[Hero.y][Hero.x] = ENCHANTED_FLOOR ;
            else 
                Map[Hero.y][Hero.x] = FLOOR ;
            }
        clearing_message_section();
        attron(COLOR_PAIR(4));
        mvprintw(MESSAGE_Y,MESSAGE_X,"You recollected a normal arrow!");}
    
    if(Map[Hero.y][Hero.x] == HEALTH_POTION && p == 0){
        Hero.p_helth_num += 1;
        if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e')
            Map[Hero.y][Hero.x] = ENCHANTED_FLOOR ;
        else 
            Map[Hero.y][Hero.x] = FLOOR ;       
        clearing_message_section();
        attron(COLOR_PAIR(4));
        mvprintw(MESSAGE_Y,MESSAGE_X,"You collected a health potion!");}
    
    if(Map[Hero.y][Hero.x] == SPEED_POTION && p == 0){
        Hero.p_speed_num += 1;
        if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e')
            Map[Hero.y][Hero.x] = ENCHANTED_FLOOR ;
        else 
            Map[Hero.y][Hero.x] = FLOOR ;     
        clearing_message_section();
        attron(COLOR_PAIR(4));
        mvprintw(MESSAGE_Y,MESSAGE_X,"You collected a speed potion!");}
    
    if(Map[Hero.y][Hero.x] == DAMAGE_POTION && p == 0){
        Hero.p_damage_num += 1;
        if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e')
            Map[Hero.y][Hero.x] = ENCHANTED_FLOOR ;
        else 
            Map[Hero.y][Hero.x] = FLOOR ;     
        clearing_message_section();
        attron(COLOR_PAIR(4));
        mvprintw(MESSAGE_Y,MESSAGE_X,"You collected a damage potion!");}
      

    
    
    
    for(int i=0;i<MONSTERS_NUM;i++){
        if(Monsters[i].level == Hero.level && Monsters[i].health > 0 ){
            int dx = Hero.x - Monsters[i].x ;
            int dy = Hero.y - Monsters[i].y ;
            if(dx>=-1 && dx<= 1 && dy>=-1 && dy <=1){
                Hero.health -= MONSTERS_DAMAGE ;
                clearing_message_section();
                attron(COLOR_PAIR(4));
                mvprintw(MESSAGE_Y,MESSAGE_X,"You were attacked by %c !",Monsters[i].namad);
            }
        }
    }
    
    if(M_var % 2 == 0){
        for(int i = -Marz_Show_Kardan ; i <= Marz_Show_Kardan ; i++){
            for(int j = -Marz_Show_Kardan ; j <= Marz_Show_Kardan ; j++)
                Showing_situation_Map[Hero.y + i][Hero.x + j] = VISIBLE ;
            }
        }
    if(Map[Hero.y][Hero.x] == EXIT_STAIRCASE)
        Hero.level++;

    if(Map[Hero.y][Hero.x] == TREASURE_ROOM_ENTRANCE){
        Hero.level++;
        preparing_treasure_room();}
    
        
    if(ch >= '1' && ch <= '9'){
        if(Hero.potion_lasting != 0)
            Hero.potion_lasting -- ;
        else {
            Hero.current_potion = ' ';
            Hero.damage_coefficient = 1;
            Hero.speed_coefficient = 1;}
    }
    
    if(rooms[current_room_num(Hero.x,Hero.y,rooms)].type == 'e'){
        Hero.health -= 2 ;
        attron(COLOR_PAIR(5));
        mvprintw(MESSAGE_Y,MESSAGE_X-20,"You are getting hurt becuse of the enchant room! Get out quickly");
        attroff(COLOR_PAIR(5));
    }
}

void updating_monsters_position(){
    for(int i=0;i<MONSTERS_NUM;i++){
        if(Monsters[i].health > 0 && (Monsters[i].room_num % 6) == current_room_num(Hero.x,Hero.y,rooms) && Monsters[i].level == Hero.level && Monsters[i].followsteps > 0){
            
            if(Hero.x > Monsters[i].x && Emkan_Harekat(Monsters[i].y,Monsters[i].x+1) == 1)
                Monsters[i].x++;
            else if (Hero.x < Monsters[i].x && Emkan_Harekat(Monsters[i].y,Monsters[i].x-1) == 1)
                Monsters[i].x--;

            if(Hero.y > Monsters[i].y && Emkan_Harekat(Monsters[i].y+1,Monsters[i].x) == 1)
                Monsters[i].y++;
            else if (Hero.y < Monsters[i].y && Emkan_Harekat(Monsters[i].y-1,Monsters[i].x) == 1)
                Monsters[i].y--;
            Monsters[i].followsteps -- ;
        }
    }
}

int monsters_situation(){
    for(int i=0;i<TREASURE_ROOM_MONSTERS_NUM;i++){
        if(Monsters[i].health > 0)
            return 1;
    }
    return 0;
}

void endgame_menu(int endgame_num){
    clearing_map_area();
    attron(COLOR_PAIR(4));
    if(endgame_num == 1){
        mvprintw(MESSAGE_Y+5,MESSAGE_X,"Congratulation! Becuse of passing the treasure room you also gain 200 points!");
        Hero.point += 200 ;
        mvprintw(MESSAGE_Y+7,MESSAGE_X,"You passed the game by %d points!",Hero.point);
    }
    else if(endgame_num == 0){
        mvprintw(MESSAGE_Y+5,MESSAGE_X,"You lost!");
        mvprintw(MESSAGE_Y+7,MESSAGE_X,"Try again!");
    }
    else if(endgame_num == -1){
        mvprintw(MESSAGE_Y+5,MESSAGE_X,"You left the game!");
        mvprintw(MESSAGE_Y+7,MESSAGE_X,"Your game will be saved.");        
    }
    mvprintw(MESSAGE_Y+8,MESSAGE_X,"Press q to get back to the pre_game menu");
    attroff(COLOR_PAIR(4));
    char ch;
    ch = getch();
    while(ch != 'q')
        ch = getch();
}

// void Blacking_terminal(){
//     // for(int i=0;i<200;i++){
//     //     for(int j=0;j<50;j++){
//     //         //attron(8);
//     //         mvprintw(j,i," ");
//     //         //attroff(8);
//     //         }
//     // }
//         clear();
//     refresh();
// }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//login menu Func :


void Saving_User_Data(User_Data user) {
    user.num_games = 0;
    user.sum_points = 0;
    user.sum_points = 0;
    user.experience = 0;
    user.game_situation = 0;
    FILE *file = fopen(filePath, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "%s %s %s %d %d %d %d\n", user.name, user.password,user.email,user.num_games,user.sum_points,user.sum_golds,user.experience); 
    fclose(file);
}

int Name_Validation(char name[100]){
    FILE *file = fopen(filePath, "r");
    char line[300];
    while (fgets(line, sizeof(line), file)) {
    char temp1[100];char temp2[100];char temp3[100];
        if (sscanf(line, "%s %s %s", temp1, temp2, temp3) == 3) {
            if (strcmp(name, temp1) == 0) {
                fclose(file);
                return 0; 
            }
        }
       
    
    }

    fclose(file); 
    return 1; 
}
int Password_Validation(char password[100]){
    int lenght=strlen(password);
    if(lenght<7)
        return 0;
    int sum=0;
    for(int i=0;i<lenght;i++){
        if(password[i]>=48 && password[i]<=57){
            sum++;
            break;}
    }
    for(int i=0;i<lenght;i++){
        if(password[i]>=65 && password[i]<=90){
            sum++;
            break;
        }
    }
    for(int i=0;i<lenght;i++){
        if(password[i]>=97 && password[i]<=122){
            sum++;
            break;
        }
    }
    if(sum==3)
        return 1;
    return 0;
    
    
}
int Email_Validation(char email[100]){
    int lenght=strlen(email);
    int index_adsign=-1;
    int index_dot=-1;
    for(int i=0;i<lenght;i++){
        if(email[i]=='.'){
            index_dot=i;
            break;}
    }
    for(int i=0;i<lenght;i++){
        if(email[i]=='@'){
            index_adsign=i;
            break;}
    }
    if(index_adsign!=-1 && index_dot-1 && index_adsign<index_dot)
        return 1;
    return 0;
}

void Uploading_New_User_Menu(){
    start_color();
    init_pair(1,COLOR_BLUE,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_BLACK);
    curs_set(0);
    attron(COLOR_PAIR(1) | A_BOLD);

    mvprintw(1,50,"Please fill the informations.You can get back to the login menu by pressing Esc.");
    mvprintw(3,50,"Name:");
    mvprintw(5,50,"password:");
    mvprintw(7,50,"email:");
    refresh();

    attroff(COLOR_PAIR(1) | A_BOLD);
}

void Clicking_New_User_Option(){
    initscr();
    User_Data User1;
    do{
    Uploading_New_User_Menu();
    move(3,56);
    scanw("%s",User1.name);
    if(Name_Validation(User1.name))
        break;
    else{
        clear();
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(5,50,"Name %s is already there!",User1.name);
        mvprintw(7,50,"Press any key to exit this menu");
        char a=getch();
        clear();
    }
    }while(1);

    do{
    Uploading_New_User_Menu();
    move(3,56);
    printw("%s",User1.name);
    move(5,60);
    scanw("%s",User1.password);
    if(Password_Validation(User1.password))
        break;
    else{
        clear();
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(5,50,"Password %s is not valid!",User1.password);
        mvprintw(7,50,"Press any key to exit this menu");
        char a=getch();
        clear();
    }
    }while(1);

     do{
    Uploading_New_User_Menu();
    move(3,56);
    printw("%s",User1.name);
    move(5,60);
    printw("%s",User1.password);
    move(7,57);
    scanw("%s",User1.email);
    if(Email_Validation(User1.email))
        break;
    else{
        clear();
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(5,50,"Email %s is not valid!",User1.email);
        mvprintw(7,50,"Press any key to exit this menu");
        char a=getch();
        clear();}
    }while(1);

Saving_User_Data(User1);
refresh();
char a=getch();
clear();  
    endwin();           
}

void getting_last_line(char* lastline){
    FILE *file = fopen(filePath, "r");
    char*temp=(char*)malloc(100*sizeof(char));

    while(fgets(temp,100,file))
        strcpy(lastline,temp);
    free(temp);
    fclose(file);
}



int Users_menu(int* num_users){
    //Safheye avaliye menu karbaran
    //initscr();
    start_color();
    init_pair(1,COLOR_BLUE,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_BLACK);
    init_pair(3,COLOR_BLUE,COLOR_WHITE);
    init_pair(4,COLOR_WHITE,COLOR_BLACK);

    attron(COLOR_PAIR(4));
    mvprintw(1,60,"Pick a user or sign a new one");
    attroff(COLOR_PAIR(4));




    attron(COLOR_PAIR(1)|A_BOLD);
    for(int j=0;j<*num_users;j++)
        mvprintw(2+j,60,"%s",Users[j].name);
    mvprintw(2+*num_users,60,"New_User");
    
    move(2,60);
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(3));
    printw("%s",Users[0].name);
    attroff(COLOR_PAIR(3));
    noecho();
    keypad(stdscr, TRUE);
    
    int p=0;
    char input;
    int Current_index=0;
    
    while((input=getch())!='q'){
        
        if(p==1){
        clear();
        //Bedast avardan line akhar file bazikon ha

        char* updated_lastline=(char*)malloc(100*sizeof(char));
        getting_last_line(updated_lastline);
        char temp1[100];char temp2[100];char temp3[100];

        sscanf(updated_lastline,"%s %s %s",temp1,temp2,temp3);
        
        strcpy(Users[*num_users-1].name,temp1);
        strcpy(Users[*num_users-1].password,temp2);

        attron(COLOR_PAIR(1));
        for(int j=0;j<*num_users;j++)
            mvprintw(2+j,50,"%s",Users[j].name);
        mvprintw(2+*num_users,60,"New_User");



        
        move(2,60);
        attroff(COLOR_PAIR(1));
        attron(COLOR_PAIR(3));
        printw("%s",Users[0].name);
        attroff(COLOR_PAIR(3));
        noecho();

        p=0;
        Current_index=0;
        }
        
        

        

        
        attron(COLOR_PAIR(1)|A_BOLD);
        if(Current_index==*num_users)
            mvprintw(2+*num_users,60,"New_User");
        else 
            mvprintw(2 + Current_index, 60, "%s", Users[Current_index].name);
        attroff(COLOR_PAIR(1));

        if (input == '8') {
            Current_index--;
            if (Current_index < 0)
                Current_index = *num_users ; 
        } else if (input == '2') {
            Current_index++;
            if (Current_index >= *num_users+1)
                Current_index = 0;    
        } else if (input =='5'){
            
            if(Current_index==*num_users){
                //menu new user
                clear();
                echo();
                Clicking_New_User_Option();
                (*num_users)++;
                clear();
                noecho();
                p++;

            }
            
            else{
            do{
            clear();
            mvprintw(2,60,"Enter the password: ");
            char temp5[100];
            echo();
            scanw("%s",temp5);
            if(strcmp(temp5,Users[Current_index].password)==0){
                //menu voorod be bazi
                strcpy(Users[current_user_index].name,Users[Current_index].name);
                return 0;
            }
            else{
                clear();
                mvprintw(2,60,"%s in not the password",temp5);
                mvprintw(4,60,"Press any key to exit thie menu");
                char a=getch();
                clear();
            }
            }while(1);
            }

        }

        attron(COLOR_PAIR(3)|A_BOLD);
        if(Current_index==*num_users)
            mvprintw(2+*num_users,60,"New_User");
        else 
            mvprintw(2 + Current_index, 60, "%s", Users[Current_index].name);
        attroff(COLOR_PAIR(3));

        

    }
    //endwin();
}

void Loading_users_information(){
    FILE *file = fopen(filePath, "r");
    char line[300];

    while (fgets(line, sizeof(line), file)) {
    char temp1[100];char temp2[100];char temp3[100];
        if (sscanf(line, "%s %s %s", temp1, temp2, temp3) == 3) {
            strcpy(Users[num_users].name,temp1);
            strcpy(Users[num_users].password,temp2);
            strcpy(Users[num_users].email,temp3);
            num_users++;
        }
        else{
            break;
        }
    }
    fclose(file);
}

void setting_menu(){
    char hero_colors_options[4][50] = {"Blue","Green","Yellow","White"} ;
    char game_difficulty_options[4][50] = {"Easy","Normal","Hard","Imposibale"};
      
    start_color();
    init_pair(1,COLOR_BLUE,COLOR_BLACK);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);
    init_pair(3,COLOR_YELLOW,COLOR_BLACK);
    init_pair(4,COLOR_WHITE,COLOR_BLACK);
    init_pair(5,COLOR_RED,COLOR_BLACK);
    curs_set(0);
    char ch;
    int m = 0;
    int n = 0;
    while((ch = getch()) != 'q'){
        clear();
        attron(COLOR_PAIR(4));
        //mvprintw(1,60,"Wellcome back ! Pick an option.");
        mvprintw(1,50,"You can pick the game difficulty level and your Hero color:");
        //for(int i=0;i<25;i++)
            //mvprintw(i+2,650,"|");
        mvprintw(3,50,"Game difficulty level:");
        mvprintw(3,90,"Hero color:");
        attroff(COLOR_PAIR(4));
        
        for(int i=0;i<4;i++)
            mvprintw(i+5,50,"%s",game_difficulty_options[i]);
        
        attroff(COLOR_PAIR(4));

        for(int i=0;i<4;i++){
            attron(COLOR_PAIR(i+1));
            mvprintw(i+5,90,"%s",hero_colors_options[i]);
            attron(COLOR_PAIR(i+1));
            }

        
        if(ch == '2'){
            m++;
            m = m%4;
            }
        else if( ch == '8'){
            m--;
            if(m<0)
                m = 3;
            m = m%4;
        }
        else if( ch == '6'){
            n++;
            n = n%2;
        }
        else if( ch == '4'){
            n--;
            if(n<0)
                n = 1;
            n = n%2;
        }
        else if (ch == '5'){
            if(n == 0){
                difficulty_level = m+1 ;
                attron(COLOR_PAIR(30));
                mvprintw(8,50,"The difficulty level is set !");
                attroff(COLOR_PAIR(30));
            }
            else{
                Hero_color = m+1;
                attron(COLOR_PAIR(30));
                mvprintw(8,50,"The color is set !");
                attroff(COLOR_PAIR(30)); 
            }
        }
        int i = 48 + n*40 ;
        int j = 5 + m ;
        attron(COLOR_PAIR(4));
        mvprintw(j,i,"*");
        attroff(COLOR_PAIR(4));
}
}

int PreGame_menu(){
    char options[4][50] = {"New game","Resume last game","Score table","Settings"} ;
    

    char ch;
    attron(A_BOLD);
    int m = 0 ;
    while((ch = getch()) != 'q'){
        start_color();
        init_pair(1,COLOR_WHITE,COLOR_BLACK);
        init_pair(2,COLOR_BLUE,COLOR_BLACK);
        init_pair(3,COLOR_BLUE,COLOR_WHITE);
        curs_set(0);
        clear();
        attron(COLOR_PAIR(1));
        //mvprintw(1,60,"Wellcome back ! Pick an option.");
        mvprintw(1,60,"Wellcome back %s ! Pick an option.",Users[current_user_index].name);
        attroff(COLOR_PAIR(1));
        for(int i=0;i<4;i++){
            attron(COLOR_PAIR(2));
            mvprintw(i+3,60,"%s",options[i]);
        }
        attroff(COLOR_PAIR(2));
        
        if(ch == '2'){
            m++;
            m = m%4;
            }
        else if( ch == '8'){
            m--;
            if(m<0)
                m = 3;
            m = m%4;
        }
        else if( ch == '5'){
            if(strcmp(options[m],"New game") == 0){
                Users[current_user_index].game_situation = 0;
                Users[current_user_index].sum_points = 0;
                clear();
                return 0;
            }
            if(strcmp(options[m],"Resume last game") == 0){
                if(Users[current_user_index].game_situation == 0){
                    attron(COLOR_PAIR(1));
                    mvprintw(1,60,"You dont have any game in resume!");
                    attroff(COLOR_PAIR(1));
                    }
                else{
                    clear();
                    return 0;
                    }
            }
            if(strcmp(options[m],"Score table") == 0){

            }           
            if(strcmp(options[m],"Settings") == 0){
                setting_menu();
            } 
        }

        attron(COLOR_PAIR(3));
        mvprintw(m+3,60,"%s",options[m]);
        attroff(COLOR_PAIR(3));

    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////

//Saving & Loading game func :

void Save_Game() {
    char filepath[256];
    sprintf(filepath, "/home/mhbd/Desktop/Users/%s.txt", Users[current_user_index].name);
    FILE *file = fopen(filepath, "w");
    // if (file == NULL) {
    //     printf("Error opening file for writing!\n");
    //     return;
    // }

    // Hero:
    fprintf(file,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %c %c %c\n",
        Hero.x, Hero.y, Hero.health, Hero.point, Hero.level, Hero.food,
        Hero.hunger, Hero.n_fd, Hero.p_fd, Hero.m_fd, Hero.b_fd,
        Hero.DAGGER_num, Hero.MAGIC_WAND_num, Hero.NORMAL_ARROW_num,
        Hero.SWORD_num, Hero.p_helth_num, Hero.p_speed_num, Hero.p_damage_num,
        Hero.potion_lasting, Hero.damage_coefficient, Hero.speed_coefficient,
        Hero.Namad, Hero.current_weapon, Hero.current_potion);

    // Monsters:
    for (int i = 0; i < MONSTERS_NUM; i++) {
        fprintf(file, "%d %d %d %d %d %d %c\n",
            Monsters[i].x, Monsters[i].y, Monsters[i].health, Monsters[i].room_num,
            Monsters[i].level, Monsters[i].followsteps, Monsters[i].namad);
    }

    // Map:
    for (int i = 0; i < Terminal_Vertical_Lenght; i++) {
        for (int j = 0; j < Terminal_Horizontal_Lenght; j++) {
            fprintf(file, "%c", Map[i][j]);
        }
        fprintf(file, "\n");
    }
        //fprintf(file, "\n");

    // rooms
    for (int i = 0; i < MAX_ROOMS + 1; i++) {
        fprintf(file, "%d %d %d %d %c\n",
            rooms[i].x, rooms[i].y, rooms[i].tool, rooms[i].arz, rooms[i].type);
    }

    // Entrance & Exit
    fprintf(file, "%d %d %d %d %d\n",
        entrance_num, exit_num, x_exit_staircase, y_exit_staircase, aiming_direction);

    fclose(file);
}

void Load_Game() {
    char filepath[256];
    sprintf(filepath, "/home/mhbd/Desktop/Users/%s.txt", Users[current_user_index].name);
    FILE *file = fopen(filepath, "r");
    // if (file == NULL) {
    //     printf("Error opening file for reading!\n");
    //     return;
    // }

    // Hero:
    fscanf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %c %c %c\n",
        &Hero.x, &Hero.y, &Hero.health, &Hero.point, &Hero.level, &Hero.food,
        &Hero.hunger, &Hero.n_fd, &Hero.p_fd, &Hero.m_fd, &Hero.b_fd,
        &Hero.DAGGER_num, &Hero.MAGIC_WAND_num, &Hero.NORMAL_ARROW_num,
        &Hero.SWORD_num, &Hero.p_helth_num, &Hero.p_speed_num, &Hero.p_damage_num,
        &Hero.potion_lasting, &Hero.damage_coefficient, &Hero.speed_coefficient,
        &Hero.Namad,&Hero.current_weapon,&Hero.current_potion);


    // Monsters:
    for (int i = 0; i < MONSTERS_NUM; i++) {
        fscanf(file, "%d %d %d %d %d %d %c\n",
            &Monsters[i].x, &Monsters[i].y, &Monsters[i].health, &Monsters[i].room_num,
            &Monsters[i].level, &Monsters[i].followsteps, &Monsters[i].namad);
    }

    // Map:
    for (int i = 0; i < Terminal_Vertical_Lenght; i++) {
            for (int j = 0; j < Terminal_Horizontal_Lenght; j++) {
                Map[i][j] = fgetc(file);
            }
            fgetc(file);
        }

    // Rooms:
    for (int i = 0; i < MAX_ROOMS + 1; i++) {
        fscanf(file, "%d %d %d %d %c\n",
            &rooms[i].x, &rooms[i].y, &rooms[i].tool, &rooms[i].arz, &rooms[i].type);
    }

    // Entrance & Exit
    fscanf(file, "%d %d %d %d %d\n",
        &entrance_num, &exit_num, &x_exit_staircase, &y_exit_staircase, &aiming_direction);

    fclose(file);
}

///////////////////////////////////////////////////////////////

void Play_Game(){
    //initscr();
    // Blacking_terminal();
    clear();
    Hero.Namad = 'H';
    Hero.health = 100 ;
    Hero.level = 1 ;
    Hero.point = 0 ;
    Hero.food = 0 ;
    Hero.hunger = 0 ;
    Hero.current_weapon = ' ';
    Hero.potion_lasting = 0 ;
    Hero.current_potion = ' ';
    Hero.damage_coefficient = 1 ;
    Hero.speed_coefficient = 1 ;

    int Hero_level_cpy = 1;

    int recovering_rate = 1;
    
if(Users[current_user_index].game_situation != 1)
    Initializing_Monsters(0);

    int endgame_num = -1;
    
while(1){
   
    if(Hero.level != 5 && Users[current_user_index].game_situation != 1)
        Generate_map();
    if(Users[current_user_index].game_situation == 1){
        Users[current_user_index].game_situation = 2;
        Load_Game();
        }
    
    int exit = 0;
    noecho();
    curs_set(0);
    char ch;
    time_t last_update = time(NULL);  //zakhire kardan time start bazi
do{
    if(Hero.health <= 0){
        Hero.health = 0;
        endgame_num = 0;
        break ;
    }
    if(Hero.level == 5){
        if(monsters_situation()== 0){
            endgame_num = 1;
            break ;
        }
    }
    
    ch = getch();
    Updating_by_input(ch);
    
    // if(Hero.potion_lasting != 0 && Hero.current_potion == 'h')
    //     recovering_rate = 2;

    // if(Hero.potion_lasting != 0 && Hero.current_potion == 'd' )
    //     Hero.damage_coefficient = 2 ;

    if(Hero.potion_lasting != 0){
        if(Hero.current_potion == 'h')
            recovering_rate = 2 ;
        else if(Hero.current_potion == 'd')
            Hero.damage_coefficient = 2 ;
        else if(Hero.current_potion == 's')
            Hero.speed_coefficient = 2 ;
    }

    for(int i=0;i<6;i++)
            Marking_rooms_VISIBLE(rooms[i]);
    
    if (difftime(time(NULL), last_update) >= 5) {  
            if(Hero.hunger >= 50)
                Hero.health -= 3 ;
            else if(Hero.hunger < 50){
                if(Hero.health<100)
                    Hero.health += 3*recovering_rate;
            }

    if (difftime(time(NULL), last_update) >= 10) {  
            if(Hero.n_fd > 0){
                Hero.n_fd --;
                Hero.b_fd ++;}
            if(Hero.p_fd > 0){
                Hero.p_fd --;
                Hero.n_fd ++;}
            if(Hero.m_fd > 0){
                Hero.m_fd --;
                Hero.n_fd ++;}    
            }
            
            Hero.hunger += 2 ;  
            last_update = time(NULL);  
        }
    if(Hero.health > 100)
        Hero.health = 100 ;
    clearing_player_data_section();
    uploading_player_data_on_screen();
    if(M_var % 2 == 0){
        //clear();
        clearing_map_area();
        Showing_Map();
    }
    else
        Print_all();

    if(Hero.level != Hero_level_cpy){
        Hero_level_cpy = Hero.level ;
        exit++;
        break ;
    }
    updating_monsters_position();
    } while(ch != 'q') ;
    
    

    if(exit == 1)
        continue;
    else    
        break;
}
//    endwin();
    if(endgame_num == 0){
        endgame_menu(0);
        Users[current_user_index].game_situation = 0;
        Users[current_user_index].sum_points += Hero.point ;
        Users[current_user_index].num_games += 1;
        }
    else if(endgame_num == 1){
        endgame_menu(1);
        Users[current_user_index].game_situation = 0;
        Users[current_user_index].sum_points += Hero.point ;
        Users[current_user_index].num_games += 1;
        }
    else{   
        endgame_menu(-1);
        Users[current_user_index].game_situation = 1;
        Save_Game();
    }

}




int main(){
    setlocale(LC_ALL, "");
    srand(time(NULL));
    
    start_color();
    init_pair(1,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);
    init_pair(3,COLOR_BLUE,COLOR_WHITE);
    init_pair(4,COLOR_WHITE,COLOR_BLACK);
    init_pair(5,COLOR_RED,COLOR_BLACK);
    init_pair(6,COLOR_BLUE,COLOR_BLACK);
    init_pair(7,COLOR_YELLOW,COLOR_BLACK);
    init_pair(8,COLOR_RED,COLOR_RED);
    
    curs_set(0);
    initscr();
    difficulty_level = 1;
    Hero_color = 1;
    attron(COLOR_PAIR(1) | A_BOLD);
//Tarif Araye haye users_name va usres_password va por kardan anha


    Loading_users_information();
    Users_menu(&num_users);


    PreGame_menu();


    Play_Game();


    PreGame_menu();

    Play_Game();

// char ch ;
//     while((ch = getch())!= 'j'){
//         PreGame_menu();
//         Play_Game();
//     }

endwin();
}
