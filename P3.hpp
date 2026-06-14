#include <cfloat>
#include "P1-SD.hpp"
#define czlonek_value 8
#define ruby_value 20
#define upgrade_value 6
#define tree strategie_tree[state.number]
#define MAX_DEPTH 7
#define MONEY_BALANCER 0.25
const short small_targ[5]={2,5,9,14,20};
const short grand_targ[5]={3,7,12,18,25};
int boty, gracze, ludzie;
std::mutex tree_block[5];
struct tree_node{
    uint level;
    short i_pole;
    bool path=false;
    uint parent;
    dynamic_array<std::pair<float, uint>> childrens;
};
dynamic_array<tree_node> strategie_tree[5];
dynamic_array<short> sciezka[5];
void print_drzewo(short index, int first_n = -1){
    int n = strategie_tree[index].size;
    if(first_n != -1 && first_n<n){
        n = first_n;
    }
    for(int i=0; i<n; i++){
        printf("Wezel o indeksie %d powiazany z polem %d, %3s na sciezce i ma dzieci o indeksach\n", i, strategie_tree[index].start[i].i_pole, strategie_tree[index].start[i].path?" ":"nie");
        for(int j=0; j<strategie_tree[index].start[i].childrens.size; j++){
            printf("%d powiazanych z polem %d o wartosci heurystyki %f\n", strategie_tree[index].start[i].childrens.start[j].second, strategie_tree[index].start[strategie_tree[index].start[i].childrens.start[j].second].i_pole, strategie_tree[index].start[i].childrens.start[j].first);
        }
    }
}
struct decision{
    short i_pole=7;
    bool logiczna=false;
    short produkt=0;
    short liczbowa=0;
};
std::map<int, std::string> kolory{
    {0, "Czerwony"},
    {1, "Bialy"},
    {2, "Zielony"},
    {3, "Niebieski"},
    {4, "Zolty"}
};
std::map<int, std::string> towary = {
    {0, "Owoce"},
    {1, "Przyprawy"},
    {2, "Tkaniny"},
    {3, "Pierscienie"}
};
std::map<int, std::string> nazwy_pol = {
    {1, "Kolodziej"},
    {2, "Sklad tkanin"},
    {3, "Sklad przypraw"},
    {4, "Sklad owocow"},
    {5, "Poczta"},
    {6, "Karawanseraj"},
    {7, "Fontanna"},
    {8, "Czarny rynek"},
    {9, "Herbaciarnia"},
    {10, "Maly targ"},
    {11, "Duzy targ"},
    {12, "Posterunek"},
    {13, "Palac sultana"},
    {14, "Maly meczet"},
    {15, "Duzy meczet"},
    {16, "Jubiler"}
};
struct player_state{
    int money;
    short position = 7;
    short things[4]={0,0,0,0};
    bool upgrades[4]={0,0,0,0};
    short rubies=0;
    short cart_upgrades=0; 
    short workers=4;
    short number=0;
    short player_color;
};
struct pole{
    short number;
    bool player_marker[5] = {0,0,0,0,0};
    bool player[5] = {0,0,0,0,0};
    short monety[5] = {0,0,0,0,0};
    bool player_family[5] = {0,0,0,0,0};
    bool sprzedawca = 0;
};
struct plansza{
    pole pola[4][4];
    short avaliable_rubies_sultan=7;
    short avaliable_rubies_jeweler=12;
    short grand_targ[4]={0,0,0,0}; 
    short small_targ[4]={0,0,0,0};
    short upgrade_cost[4]={2,2,2,2};
    float sultan[4]={1,1,1,1};
    short post_state=0;
    uint move=0;
};
const short small_targ_states[6][4]={
    {1,2,1,0},
    {2,2,0,1},
    {2,2,1,0},
    {2,1,1,1},
    {1,3,1,0},
    {3,1,1,0}
};
const short grand_targ_states[6][4]={
    {0,1,2,2},
    {1,0,1,3},
    {1,0,2,2},
    {0,1,1,3},
    {1,1,1,2},
    {0,0,3,2}
};
const short small_targ_return[5]={2,5,9,14,20};
const short grand_targ_return[5]={3,7,12,18,25};
const short post_states[5][3]={
    {0,1,2},
    {2,1,2},
    {2,1,3},
    {2,3,3},
    {2,3,4}
};
void print_path(uint id){
    printf("Sciezka gracza %d: ", id);
    for(uint i=0; i<sciezka[id].size; i++){
        printf("%d ",sciezka[id].start[i]);
    }
    printf("\n");
}
void print_board(plansza &board){
    for(short i=0; i<4; i++){
        for(short j=0; j<4; j++){
            printf("%15s(%2d) %c %c %c %c %c", nazwy_pol[board.pola[i][j].number].c_str(), board.pola[i][j].number, board.pola[i][j].player[0]?'1':' ',board.pola[i][j].player[1]?'2':' ',board.pola[i][j].player[2]?'3':' ',board.pola[i][j].player[3]?'4':' ',board.pola[i][j].player[4]?'5':' ');
        }
        printf("\n");
    }
}
void print_targ(plansza &board, bool grand=false){
    if(grand){
        printf("Towary na wielkim targze: \n Owoce: %d\n przyprawy: %d\n Tkaniny: %d\n Pierscienie: %d\n", board.grand_targ[0], board.grand_targ[1], board.grand_targ[2], board.grand_targ[3]);
    }else{
        printf("Towary na malym targze: \n Owoce: %d\n przyprawy: %d\n Tkaniny: %d\n Pierscienie: %d\n", board.small_targ[0], board.small_targ[1], board.small_targ[2], board.small_targ[3]);
    }
}
void print_meczets(plansza &board){
    printf("Koszt ulepszenia:\n Owoce: %d\n przyprawy: %d\n Tkaniny: %d\n Pierscienie: %d\n", board.upgrade_cost[0], board.upgrade_cost[1], board.upgrade_cost[2], board.upgrade_cost[3]);
}
void print_player_state(player_state &state){
    printf("Stan gracza %s:\n Pieniadze: %d\n Towary: Owoce: %d, przyprawy: %d, Tkaniny: %d, Pierscienie: %d\n Ulepszenia: Owoce: %d, przyprawy: %d, Tkaniny: %d, Pierscienie: %d\n Rubiny: %d\n Ulepszenia wozka: %d\n Pracownicy w stosie: %d\n", kolory[state.player_color].c_str(), state.money, state.things[0], state.things[1], state.things[2], state.things[3], state.upgrades[0], state.upgrades[1], state.upgrades[2], state.upgrades[3], state.rubies, state.cart_upgrades, state.workers);
}
std::pair<short, short> find_pole(plansza &board, short i_pole){
    for(short i=0; i<4; i++){
        for(short j=0; j<4; j++){
            if(board.pola[i][j].number == i_pole){
                return std::make_pair(i,j);
            }
        }
    }
    return std::make_pair(-1,-1);
}
int distance(plansza &board, int pole1, int pole2){
    std::pair<short, short> p1 = find_pole(board, pole1);
    std::pair<short, short> p2 = find_pole(board, pole2);
    if(p1.first == -1 || p2.first == -1){
        throw std::invalid_argument("Pole not found");
    }
    return abs(p1.first-p2.first)+abs(p1.second-p2.second);
}
int distance(plansza &board, int pole1, std::pair<short, short> pole2){
    std::pair<short, short> p1 = find_pole(board, pole1);
    if(p1.first == -1 || pole2.first == -1){
        throw std::invalid_argument("Pole not found");
    }
    return abs(p1.first-pole2.first)+abs(p1.second-pole2.second);
}
void make_game(plansza &board, player_state *states, short player_number){
    dynamic_array<short> *pola = new dynamic_array<short>();
    for(short i=1; i<=16; i++){
        push_back(pola, i);
    }
    pop_at(pola, 6);
    dynamic_array<std::pair<short, short>> *ava_places = new dynamic_array<std::pair<short, short>>();
    for(short i=0; i<4; i++){
        for(short j=0; j<4; j++){
            push_back(ava_places,std::make_pair(i,j));
        }
    }
    memcpy(board.grand_targ,grand_targ_states[rand()%6],sizeof(board.grand_targ));
    memcpy(board.small_targ,small_targ_states[rand()%6],sizeof(board.small_targ));
    if(player_number <4){
        board.avaliable_rubies_jeweler = 7+player_number;
        board.avaliable_rubies_sultan--;
        for(short i=0; i<4; i++){
            board.sultan[i] += 0.25;
        }
    }
    short x = rand()%2+1;
    short y = rand()%2+1;
    board.pola[x][y].number = 7;
    pop_at(ava_places,find(ava_places, std::make_pair(x,y)));
    while(pola->size!=0){
        short pole_num = pop_back(pola);
        std::pair<short, short> index = pop_at(ava_places, rand()%ava_places->size);
        if (pole_num == 8){
            while(distance(board, 7, index)<3){
                push_back(ava_places, index);
                index = pop_at(ava_places, rand()%ava_places->size);
            }
        }
        board.pola[index.first][index.second].number = pole_num;
    }
    dynamic_array<short>* colors = new dynamic_array<short>();
    for(short i=0; i<5; i++){
        push_back(colors, i);
    }
    for(short i=0; i<player_number; i++){
        states[i].number = i;
        states[i].money = 2+i;
        states[i].player_color = pop_at(colors, rand()%colors->size);
    }
}
void do_action(plansza &board, player_state &player, short i_pole, decision decyzja){
    int rzut;
    std::pair<short, short> index;
    short sprzedane_towary = 0;
    switch (i_pole){
        case 1:
            if(player.money >= 7){
                player.money -= 7;
                player.cart_upgrades++;
                if(player.cart_upgrades==3){
                    player.rubies++;
                }
            }
        break;
        case 2:
            player.things[2]=2+player.cart_upgrades;
        break;
        case 3:
            player.things[1]=2+player.cart_upgrades;
        break;
        case 4:
            player.things[0]=2+player.cart_upgrades;
        break;
        case 5:
            player.things[post_states[board.post_state][0]]= min(player.things[post_states[board.post_state][0]]+1, 2+player.cart_upgrades);;
            player.things[post_states[board.post_state][1]]= min(player.things[post_states[board.post_state][1]]+1, 2+player.cart_upgrades);;
            player.money+=post_states[board.post_state][2];
            board.post_state = (board.post_state+1)%5;
        break;
        case 6:
            if(decyzja.logiczna){
                player.money+=5;
            }else{
                player.things[decyzja.produkt]= min(player.things[decyzja.produkt]+2, 2+player.cart_upgrades);;
            }
            break;
        case 7:
            player.workers = 4+player.upgrades[3];
            for(short i=0;i<4;i++){
                for(short j=0;j<4;j++){
                    board.pola[i][j].player_marker[player.number] = 0;
                }
            }
        break;
        case 8:
            if(decyzja.produkt==3){
                throw std::invalid_argument("Invalid product choice");
            }else{
                player.things[decyzja.produkt] = min(player.things[decyzja.produkt]+1, 2+player.cart_upgrades);
            }
            rzut= rand()%6+rand()%6+2;
            if(rzut==7||rzut==8){
                player.things[3]= min(player.things[3]+1, 2+player.cart_upgrades);
            }else if(rzut==9||rzut==10){
                player.things[3] = min(player.things[3]+2, 2+player.cart_upgrades);
            }else if(rzut==11||rzut==12){
                player.things[3] = min(player.things[3]+3, 2+player.cart_upgrades);;
            }
        break;
        case 9:
            rzut = rand()%6+rand()%6+2;
            if(decyzja.liczbowa>=rzut){
                player.money+=decyzja.liczbowa;
            }else{
                player.money+=2;
            }
        break;
        case 10:
            for(short i=0; i<4; i++){
                if(player.things[i]>board.small_targ[i]){
                   sprzedane_towary += min(player.things[i],board.small_targ[i]);
                }
            }
            if(sprzedane_towary >0){
                player.money+=small_targ_return[sprzedane_towary-1];
            }
        break;
        case 11:
            for(short i=0; i<4; i++){
                if(player.things[i]>board.grand_targ[i]){
                   sprzedane_towary += min(player.things[i],board.grand_targ[i]);
                }
            }
            if(sprzedane_towary > 0){
                player.money+=grand_targ_return[sprzedane_towary-1];
            }
        break;
        case 12:
            index = find_pole(board, 12);
            if(board.pola[index.first][index.second].player_family[player.number]){
                do_action(board, player, decyzja.i_pole, decyzja);
            }
        break;
        case 13:
            if(player.things[0]>=board.sultan[0] && player.things[1]>=board.sultan[1] && player.things[2]>=board.sultan[2] && player.things[3]>=board.sultan[3]){
                for(short i=0; i<4; i++){
                    player.things[i]-=(int)board.sultan[i];
                }
                float cost = board.sultan[0];
                while(cost-(int)board.sultan[0]>0){
                    if(player.things[decyzja.produkt]>0){
                        player.things[decyzja.produkt]--;
                        cost -= 0.25;
                    }
                    else{
                        throw std::invalid_argument("Not enough products to buy from sultan");
                    }
                }
                player.rubies++;
                board.avaliable_rubies_sultan--;
                if((7-board.avaliable_rubies_sultan)%5==0){
                    for(short i=0; i<4; i++){
                        board.sultan[i] += 0.25;
                    }
                }else{
                    board.sultan[4-(7-board.avaliable_rubies_sultan)%5]++;
                }
            }
        break;
        case 14:
            if(decyzja.produkt==1){
                if(!player.upgrades[1]&&player.things[1]>=board.upgrade_cost[1]){
                    player.things[1]--;
                    player.upgrades[1] = 1;
                    if(player.upgrades[2]){
                        player.rubies++;
                    }
                }else{
                    printf("Produkt 1: %d, Upgrade: %d\n", player.things[1], player.upgrades[1]);
                    throw std::invalid_argument("Not enough products to buy upgrade or upgrade already bought");
                }
            }else if(decyzja.produkt==2){
                if(!player.upgrades[2]&&player.things[2]>=board.upgrade_cost[2]){
                    player.things[2]--;
                    player.upgrades[2] = 1;
                    if(player.upgrades[1]){
                        player.rubies++;
                    }
                }else{
                    printf("Produkt 2: %d, Upgrade: %d\n", player.things[2], player.upgrades[2]);
                    throw std::invalid_argument("Not enough products to buy upgrade or upgrade already bought");
                }
            }
            else{
                throw std::invalid_argument("Invalid product choice");
            }            
        break;
        case 15:
            if(decyzja.produkt==0){
                if(!player.upgrades[0]&&player.things[0]>=board.upgrade_cost[0]){
                    player.things[0]--;
                    player.upgrades[0] = 1;
                    if(player.upgrades[3]){
                        player.rubies++;
                    }
                }else{
                    printf("Produkt 0: %d, Upgrade cost: %d\n", player.things[0], board.upgrade_cost[0]);
                    throw std::invalid_argument("Not enough products to buy upgrade or upgrade already bought");
                }
            }else if(decyzja.produkt==3){
                if(!player.upgrades[3]&&player.things[3]>=board.upgrade_cost[3]){
                    player.things[3]--;
                    player.upgrades[3] = 1;
                    if(player.upgrades[0]){
                        player.rubies++;
                    }
                }else{
                    printf("Produkt 3: %d, Upgrade: %d\n", player.things[3], player.upgrades[3]);
                    throw std::invalid_argument("Not enough products to buy upgrade or upgrade already bought");
                }
            }
            else{
                throw std::invalid_argument("Invalid product choice");
            }
        break;
        case 16:
            if(player.money >= 24-board.avaliable_rubies_jeweler){
                player.money -= 24-board.avaliable_rubies_jeweler;
                player.rubies++;
                board.avaliable_rubies_jeweler--;
            }
        break;
        default:
            break;
        }
    }
void move_player(plansza &board, player_state &player, short i_pole, decision decyzja){
    std::pair<short, short> index = find_pole(board, player.position);
    if(distance(board, player.position, i_pole)<3 && player.position != i_pole){
        board.pola[index.first][index.second].player[player.number] = 0;
        player.position = i_pole;
        push_back(&sciezka[player.number], i_pole);
        index = find_pole(board, i_pole);
        for(short i=0; i<5; i++){
            if(board.pola[index.first][index.second].player[i] == 1){
                player.money -= 2;
            }
        }
        board.pola[index.first][index.second].player[player.number] = 1;
        if(board.pola[index.first][index.second].player_marker[player.number]){
            board.pola[index.first][index.second].player_marker[player.number] = 0;
            player.workers++;
            do_action(board, player, i_pole, decyzja);
        }
        else{
            if(player.workers!=0){
                board.pola[index.first][index.second].player_marker[player.number] = 1;
                player.workers--;
                do_action(board, player, i_pole, decyzja);
            }
        }

    }
}
float heu_moja(plansza &board, player_state &state, short i_pole){
    float things_value[4] = {0};
    float dice;
    short sprzedane_towary = 0;
    std::pair<short, short> index;
    for(short i=0; i<4; i++){
        things_value[i] = board.grand_targ[i]+board.small_targ[i]+(board.upgrade_cost[i]*state.upgrades[i])+board.sultan[i];
    }
    float money_value = ((24-board.avaliable_rubies_jeweler+(3-state.cart_upgrades)*7)-state.money)*MONEY_BALANCER;
    //printf("Wartosc produktow %f %f %f %f i monet %f\n", things_value[0],things_value[1],things_value[2],things_value[3],money_value);
    switch(i_pole){
        case 1:
            if(state.money >= 7){
                return things_value[0]+things_value[1]+things_value[2]+things_value[3]+ruby_value*(state.cart_upgrades/3);
            }else{
                return -1;
            }
        case 2:
            return (2+state.cart_upgrades-state.things[2])*things_value[2];
        case 3:
            return (2+state.cart_upgrades-state.things[1])*things_value[1];
        case 4:
            return (2+state.cart_upgrades-state.things[0])*things_value[0];            
        case 5:
            return things_value[post_states[board.post_state][0]]+things_value[post_states[board.post_state][1]]+post_states[board.post_state][2]*money_value;
        case 6:
            return money_value*5;
        case 7:
            return 2*((4+state.upgrades[3])-state.workers);
        case 8:
            if(state.upgrades[2]){
                dice = 0.3*(2+state.cart_upgrades-state.things[3])+0.3*(2+state.cart_upgrades-state.things[3])*2+0.1666*(2+state.cart_upgrades-state.things[3])*3;
            }else{
                dice = 0.3056*(2+state.cart_upgrades-state.things[3])+0.1944*(2+state.cart_upgrades-state.things[3])*2+0.0833*(2+state.cart_upgrades-state.things[3])*3;
            }
            return (2+state.cart_upgrades-state.things[0])*0.3+(2+state.cart_upgrades-state.things[1])*0.3+(2+state.cart_upgrades-state.things[2])+dice;
        case 9:
            if(state.upgrades[2])
                return 0.0555*money_value*12+0.1666*money_value*11+0.3333*money_value*10+0.5*money_value*9+0.6665*money_value*8+0.8331*money_value*7+0.9*money_value*6;
            else
                return 0.0277*money_value*12+0.0833*money_value*11+0.1666*money_value*10+0.2777*money_value*9+0.4166*money_value*8+0.5827*money_value*7+0.6944*money_value*6+0.7777*money_value*5+0.8611*money_value*4+0.9166*money_value*3;
        case 10:
            if((state.things[0]-board.small_targ[0]+state.things[1]-board.small_targ[1]+state.things[2]-board.small_targ[2]+state.things[3]-board.small_targ[3])>0){
                for(short i=0; i<4; i++){
                    if(state.things[i]>board.small_targ[i]){
                        sprzedane_towary += min(state.things[i],board.small_targ[i]);
                    }
                }
                //printf("%d \n", sprzedane_towary);
                if(sprzedane_towary > 0){
                    return money_value*small_targ_return[sprzedane_towary-1];
                }
            }
            else{
                return -1;
            }
        case 11:
            if((state.things[0]-board.grand_targ[0]+state.things[1]-board.grand_targ[1]+state.things[2]-board.grand_targ[2]+state.things[3]-board.grand_targ[3])>0){
                for(short i=0; i<4; i++){
                    if(state.things[i]>board.grand_targ[i]){
                       sprzedane_towary += min(state.things[i],board.grand_targ[i]);
                    }
                }
                //printf("%d \n", sprzedane_towary);
                if(sprzedane_towary > 0){
                    return money_value*grand_targ_return[sprzedane_towary-1];
                }
            }
            else{
                return -1;
            }
        case 12:
            index = find_pole(board, 12);
            if(board.pola[index.first][index.second].player_family[state.number]){
                return czlonek_value;
            }
            else{
                return -1;
            }
        case 13:
            if(state.things[0]>=board.sultan[0] && state.things[1]>=board.sultan[1] && state.things[2]>=board.sultan[2] && state.things[3]>=board.sultan[3]){
                return ruby_value*(6-state.rubies);
            }
            else{
                return -1;
            }
        case 14:
            if((state.things[1]>=board.upgrade_cost[1]&&!state.upgrades[1]) || (state.things[2]>=board.upgrade_cost[2]&&!state.upgrades[2])){
                return 0.5*ruby_value*(6-state.rubies)+state.upgrades[1]*upgrade_value+state.upgrades[2]*upgrade_value;
            }else{
                return -1;
            }
        case 15:
            if((state.things[0]>=board.upgrade_cost[0]&&!state.upgrades[0]) || (state.things[3]>=board.upgrade_cost[3]&&!state.upgrades[3])){
                return 0.5*ruby_value*(6-state.rubies)+state.upgrades[0]*upgrade_value+state.upgrades[3]*upgrade_value;
            }else{
                return -1;
            }
        case 16:
            if(state.money >= 24-board.avaliable_rubies_jeweler){
                return ruby_value*(6-state.rubies);
            }
            else{
                return -1;
            }
    }
    return -1;
}
void begin_strategy(plansza &board, player_state &state, uint pole = 7){
    tree_block[state.number].lock();
    tree_node wezel;
    wezel.i_pole = pole;
    wezel.level = 0;
    wezel.path = true;
    wezel.parent = 0;
    push_back(&tree, wezel);
    for(short i=0; i<4; i++){
        for(short j=0; j<4; j++){
            if(state.position!=board.pola[i][j].number && distance(board, state.position, board.pola[i][j].number)<3){
                float wartosc = heu_moja(board, state, board.pola[i][j].number);
                if(wartosc>0){
                    wezel.level = 1;
                    wezel.i_pole = board.pola[i][j].number;
                    wezel.parent = 0;
                    wezel.path = false;
                    push_back(&tree, wezel);
                    std::pair<float, uint> tmp;
                    tmp.first = wartosc;
                    tmp.second = tree.size-1;
                    push_back(&tree.start[0].childrens, tmp);
                    //printf("Ilosc dzieci: %d\n", tree.start[0].childrens.size);
                }
            }
        }
    }
    tree_block[state.number].unlock();
}
uint get_last_path(player_state &state, uint node=0){
    uint index = node;

    while (true) {
        tree_node &current = tree.start[index];

        bool found = false;

        for (int i = 0; i < current.childrens.size; i++) {
            uint child = current.childrens.start[i].second;

            if (tree.start[child].path) {
                index = child;
                found = true;
                break;
            }
        }

        if (!found) break;
    }
    return index;
}
float strategize_layer(plansza board, player_state state, uint node=0){
    //printf("Strategizing layer %d\n", tree.start[node].level);
    float avg = 0;
    short pola = 0; 
    bool all_bad = true;
    state.position = tree.start[node].i_pole;
    if(tree.start[node].childrens.size==0){
        for(short i=0; i<4; i++){
            for(short j=0; j<4; j++){
                if(board.pola[i][j].number!=state.position && distance(board, state.position, board.pola[i][j].number)<3){
                    float wartosc = heu_moja(board, state, board.pola[i][j].number);
                    avg+=wartosc;
                    pola++;
                    if(wartosc>=0){
                        all_bad = false;
                        tree_node wezel;
                        wezel.level = tree.start[node].level+1;
                        wezel.i_pole = board.pola[i][j].number;
                        wezel.parent = node;
                        push_back(&tree, wezel);
                        std::pair<float, uint> tmp;
                        tmp.first = wartosc;
                        tmp.second = tree.size-1;
                        push_back(&tree.start[node].childrens, tmp);
                    }
                }
            }
        }
    }else{
        bool done;
        for(short i=0; i<4; i++){
            for(short j=0; j<4; j++){
                done = false;
                if(board.pola[i][j].number!=state.position && distance(board, state.position, board.pola[i][j].number)<3){
                    float wartosc = heu_moja(board, state, board.pola[i][j].number);
                    avg+=wartosc;
                    pola++;
                    if(wartosc>0){
                        all_bad = false;
                        for(int k=0; k<tree.start[node].childrens.size; k++){
                            if(tree.start[tree.start[node].childrens.start[k].second].i_pole == board.pola[i][j].number){
                                tree.start[node].childrens.start[k].first = wartosc;
                                done = true;
                                break;
                            }
                        }
                        if(!done){
                            tree_node wezel;
                            wezel.level = tree.start[node].level+1;
                            wezel.i_pole = board.pola[i][j].number;
                            wezel.parent = node;
                            push_back(&tree, wezel);
                            std::pair<float, uint> tmp;
                            tmp.first = wartosc;
                            tmp.second = tree.size-1;
                            push_back(&tree.start[node].childrens, tmp);
                        }
                    }
                }
            }
        }
    }
    if(all_bad){
        avg = -pola;
    }
    return pola > 0 ? avg/pola : 0.0f;
}
void depth_strategize(plansza board, player_state state, short depth, uint index=0){
    float layer_avg;
    decision decyzja;
    //printf("Strategizing depth %d\n", depth);
    if(depth == MAX_DEPTH){
        return;
    }
    for(short i=0; i<tree.start[index].childrens.size; i++){
        uint child_node = tree.start[index].childrens.start[i].second;
        if(tree.start[child_node].i_pole == 14){            
            if(!state.upgrades[2] && state.things[2]>=board.upgrade_cost[2]){
                decyzja.produkt = 2;
            }else{
                decyzja.produkt = 1;
            }
        }else{
            if(!state.upgrades[3] && state.things[3]>=board.upgrade_cost[3]){
                decyzja.produkt = 3;
            }else{
                decyzja.produkt = 0;
            }
        }
        if(tree.start[child_node].i_pole == 8){
            decyzja.produkt = 0;
        }
        layer_avg = strategize_layer(board, state,  tree.start[index].childrens.start[i].second);
        //printf("Pole %d o heurystyce: %f\n", tree.start[index].childrens.start[i].second, tree.start[index].childrens.start[i].first);
        //print_player_state(state);
        //print_meczets(board);
        if(heu_moja(board, state, tree.start[child_node].i_pole) > 0){
            //printf("%f\n", heu_moja(board, state, tree.start[child_node].pole));
            //printf("%d \n", tree.start[child_node].pole.number);
            //printf("%d \n", decyzja.produkt);
            do_action(board, state, tree.start[child_node].i_pole, decyzja);
        }
        depth_strategize(board, state, depth+1, tree.start[index].childrens.start[i].second);
        state = state;
        board = board;
        if(layer_avg>0)
            tree.start[index].childrens.start[i].first += layer_avg/depth;
    }
    //printf("Rozmiar drzewa: %d\n", tree.size);
}
void strategize_tree(plansza board, player_state state){
    //tree_block[state.number].lock();
    uint index = get_last_path(state);
    if(tree.size>1000000){
        uint i_pole = tree.start[index].i_pole;
        tree = dynamic_array<tree_node>();
        begin_strategy(board, state, i_pole);
        index = 0;
    }
    printf("Ostatni wezel na sciezce ma index %u.\n", index);
    depth_strategize(board, state, 1, index);
    //tree_block[state.number].unlock();    
}
float bierzaca_heu(plansza &board, player_state &state, short i_pole){
    float heu_value = 0;
    float things_value[4] = {0};
    std::pair<short, short> index = find_pole(board, i_pole);
    for(short i=0; i<4; i++){
        things_value[i] = board.grand_targ[i]+board.small_targ[i]+(board.upgrade_cost[i]*state.upgrades[i])+board.sultan[i];
    }
    float money_value = state.money-(24-board.avaliable_rubies_jeweler+(3-state.cart_upgrades)*7);
    if(board.pola[index.first][index.second].player_marker[state.number]){
        heu_value += 2*((4+state.upgrades[3])-state.workers);
    } //Chałwa wielkiemu liczydłu!! Chałwa królowi Aloizemu Algebrowi
    if(board.pola[index.first][index.second].sprzedawca){
        for(short i=0; i<4; i++){
            if(things_value[i]>2*money_value){
                heu_value += things_value[i]-2*money_value;
            }
        }
    }
    for(short i=0; i<5; i++){
        if(board.pola[index.first][index.second].player_family[i]){
            heu_value+=money_value*3;
        }
        if(board.pola[index.first][index.second].player[i]){
            heu_value-=money_value*2;
        }
    }
    return heu_value;
}
void agent(plansza &board, player_state &state){
    tree_block[state.number].lock();
    printf("Player %d gra\n", state.number);
    int last_move = get_last_path(state);
    std::pair<short, short> index = find_pole(board, state.position);
    if(board.pola[index.first][index.second].monety[state.number]!=0){
        state.money += board.pola[index.first][index.second].monety[state.number];
        board.pola[index.first][index.second].monety[state.number] = 0;
    }
    uint max_strategie = 0;
    float max_value = 0;
    float things_value[4] = {0};
    uint max_things = 0;
    for(short i=0; i<4; i++){
        things_value[i] = board.grand_targ[i]+board.small_targ[i]+(board.upgrade_cost[i]*state.upgrades[i])+board.sultan[i];
        if(things_value[i]>things_value[max_things]){
            max_things = i;
        }
    }
    float money_value = state.money-(24-board.avaliable_rubies_jeweler+(3-state.cart_upgrades)*7);
    for(short i=0; i<tree.start[last_move].childrens.size; i++){
        index = find_pole(board, tree.start[tree.start[last_move].childrens.start[i].second].i_pole);
        pole tmp_pole = board.pola[index.first][index.second];
        short koszt = 0;
        for(short j=0; j<5; j++){
            koszt += 2*tmp_pole.player[j];
        }
        if(koszt>state.money){
            tree.start[last_move].childrens.start[i].first = -1;
        }else{
            float heu_v = bierzaca_heu(board, state, tree.start[tree.start[0].childrens.start[i].second].i_pole);
            if(heu_v>=0){
                tree.start[last_move].childrens.start[i].first += heu_v;
            }else{
                tree.start[last_move].childrens.start[i].first = -1;
            }
        }
    }
    for(short i=0; i<tree.start[last_move].childrens.size; i++){
        if(tree.start[last_move].childrens.start[i].first>max_value && heu_moja(board, state, tree.start[tree.start[last_move].childrens.start[i].second].i_pole)>0){
            max_strategie = tree.start[last_move].childrens.start[i].second;
            max_value = tree.start[last_move].childrens.start[i].first;
        }
    }
    decision decyzja;
    short max_pole=1;
    switch(tree.start[max_strategie].i_pole){
        case 6:
            if(2*things_value[max_things]>money_value){
                decyzja.logiczna = 0; 
                decyzja.produkt = max_things;
            }else{
                decyzja.logiczna = 1; 
            }        break;
        case 8:
            if(max_things < 3){
                decyzja.produkt = max_things;
            }else{
                uint tmp_max = 0;
                for(int i=0; i<3; i++){
                    if(things_value[i]>things_value[tmp_max]){
                        tmp_max=i;
                    }
                }
                decyzja.produkt = tmp_max;
            }
        break;
        case 9:
            decyzja.liczbowa = 7+state.upgrades[2];
        break;
        case 12:
            max_pole = 1;
            max_value = -1;
            for(short i=1; i<=16; i++){
                if(max_value<heu_moja(board, state, i)){
                    max_value = heu_moja(board, state, i);
                    max_pole = i;
                }
            }
            decyzja.i_pole = max_pole;
        break; //sprawdzenie ile jabłek ma Janek
        case 13:
            if(state.things[0]>(int)board.sultan[0]+1){
                decyzja.produkt = 0;
            }else if(state.things[1]>(int)board.sultan[1]+1){
                decyzja.produkt = 1;
            }else if(state.things[2]>(int)board.sultan[2]+1){
                decyzja.produkt = 2;
            }else{
                decyzja.produkt = 3;
            }
        break;
        case 14:
            if(!state.upgrades[2] && state.things[2]>=board.upgrade_cost[2]){
                decyzja.produkt = 2;
            }else{
                decyzja.produkt = 1;
            }
        break;
        case 15:
            if(!state.upgrades[3] && state.things[3]>=board.upgrade_cost[3]){
                decyzja.produkt = 3;
            }else{
                decyzja.produkt = 0;
            }
        break;
    }
    printf("Poruszam się z %d do %d \n", tree.start[last_move].i_pole, tree.start[max_strategie].i_pole);
    //printf("Ostatni ruch: %d ma dzieci:\n", tree.start[last_move].i_pole);
    tree.start[max_strategie].path = true;
    //for(int i=0; i<tree.start[last_move].childrens.size; i++){
    //    printf("Wezel %d to pole %d\n", tree.start[last_move].childrens.start[i].second, tree.start[tree.start[last_move].childrens.start[i].second].i_pole);
    //    if(tree.start[tree.start[last_move].childrens.start[i].second].i_pole == tree.start[max_strategie].i_pole){
    //        tree.start[tree.start[last_move].childrens.start[i].second].path = true;
    //    }
    //}
    printf("index ostateniego elementu na drodze: %d", get_last_path(state));
    printf("Wartosc pola: %f\n", max_value);
    move_player(board, state, tree.start[max_strategie].i_pole, decyzja);
    tree_block[state.number].unlock();
}
void player(plansza &board, player_state &player, player_state *inni){
    decision decyzja;
    print_board(board);
    print_player_state(player);
    std::pair<short, short> index = find_pole(board, player.position);
    if(board.pola[index.first][index.second].monety[player.number]!=0){
        player.money += board.pola[index.first][index.second].monety[player.number];
        board.pola[index.first][index.second].monety[player.number] = 0;
    }
    bool ruch_done = false;
    int tmp_input;
    int choice;
    char akcja = 'p';
    int gn;
    while(!ruch_done){
    std::cout<<"Co chcesz zrobić?\n Aby zobaczyc plansze wpisz v. \n Aby zobaczyc stan innego gracza wpisz g.\n Aby zobaczyc stan targow wpisz b.\n Aby zobaczyc stan poczty wpisz p.\n Aby zobaczyc stan meczetow wpisz m.\n  Aby wykonać ruch wpisz r.\n Twoj wybor: ";
    std::cin>>akcja;
    switch(akcja){
        case 'g':
            gn=0;
            printf("Podaj numer gracza: ");
            scanf("%d", &gn);
            if(gn>=0 && gn<gracze){
                print_player_state(inni[gn]);
            }
        break;
        case 'b':
            print_targ(board);
            print_targ(board, true);
        break;
        case 'p':
            printf("Produkt 1: %s\n Produkt 2: %s\n Pieniadze: %d\n", towary[post_states[board.post_state][0]].c_str(), towary[post_states[board.post_state][1]].c_str(), post_states[board.post_state][2]);
        break;
        case 'm':
            print_meczets(board);
        break;
        case 'v':
            print_board(board);
        break;
        case 'r':
            ruch_done = true;
        break;
        default:
        break;
    }
    }
    ruch_done = false;
    while(!ruch_done){
    printf("Wybierz pole do ktorego chcesz sie ruszyc: ");
    scanf("%d", &choice);
    if(distance(board, player.position, choice)<3 && player.position != choice){
        ruch_done = true;
        switch(choice){
            case 6:
                printf("Czy chcesz pobrać towar czy monety? (0-towar, 1-monety): ");
                scanf("%d", &tmp_input);
                decyzja.logiczna = (bool)tmp_input;
                if(decyzja.logiczna){
                    printf("Ktory towar chcesz pozyskac? (0-owoce, 1-przyprawy, 2-tkaniny, 3-pierscienie): ");
                    scanf("%d", &tmp_input);
                    if(tmp_input < 0 || tmp_input >3){
                        ruch_done = false;
                        printf("Wybrano niepoprawny towar! \n");
                    }
                    decyzja.produkt = (short)tmp_input;
                }
            break;
            case 8:
                printf("Ktory towar chcesz kupic? (0-owoce, 1-przyprawy, 2-tkaniny): ");
                scanf("%d", &tmp_input);
                if(tmp_input < 0 || tmp_input >2){
                    ruch_done = false;
                    printf("Wybrano niepoprawny towar! \n");
                }
                decyzja.produkt = (short)tmp_input;
            break;
            case 9:
                printf("Na jaka kwote chcesz postawic? (3-12): ");
                scanf("%d", &tmp_input);
                if(tmp_input < 3 || tmp_input >12){
                    ruch_done = false;
                    printf("Wybrano niepoprawna wartosc! \n");
                }
                decyzja.produkt = (short)tmp_input;
            break;
            case 12:
                printf("Na jakie pole chcesz sie przeniesc? (1-16): ");
                scanf("%d", &tmp_input);
                if(tmp_input < 1 || tmp_input >16 || tmp_input == 12){
                    ruch_done = false;
                    printf("Wybrano niepoprawne pole! \n");
                }
                decyzja.i_pole = (short)tmp_input;
            break;
            case 13:
                printf("Ktory towar chcesz oddac? (0-owoce, 1-przyprawy, 2-tkaniny, 3-pierscienie): ");
                scanf("%d", &tmp_input);
                if(tmp_input < 0 || tmp_input >3){
                        ruch_done = false;
                        printf("Wybrano niepoprawny towar! \n");
                }
                decyzja.produkt = (short)tmp_input;
            break;
            case 14:
                printf("Ktory upgrade chcesz kupic? (1-przyprawy, 2-tkaniny): ");
                scanf("%d", &tmp_input);
                if(tmp_input != 1 && tmp_input != 2){
                        ruch_done = false;
                        printf("Wybrano niepoprawny towar! \n");
                }
                if(player.things[tmp_input]<board.upgrade_cost[tmp_input]){
                    ruch_done = false;
                    printf("Nie posiadasz wystarczająco towarow na zakup tego ulepszenia.\n");
                }
                if(player.upgrades[tmp_input]){
                    ruch_done = false;
                    printf("Juz posiadasz to ulepszenie.\n");
                }
                decyzja.produkt = (short)tmp_input;
            break;
            case 15:
                printf("Ktory upgrade chcesz kupic? (0-owoce, 3-pierscienie): ");
                scanf("%d", &tmp_input);
                if(tmp_input != 0 && tmp_input != 3){
                        ruch_done = false;
                        printf("Wybrano niepoprawny towar! \n");
                }
                if(player.things[tmp_input]<board.upgrade_cost[tmp_input]){
                    ruch_done = false;
                    printf("Nie posiadasz wystarczająco towarow na zakup tego ulepszenia.\n");
                }
                if(player.upgrades[tmp_input]){
                    ruch_done = false;
                    printf("Juz posiadasz to ulepszenie.\n");
                }
                decyzja.produkt = (short)tmp_input;
            break;
        }
    }
    else{
        printf("Nie mozesz sie tam ruszyc\n");
    }
    }
    move_player(board, player, choice, decyzja);
}