#define czlonek_value 6
#define ruby_value 10
#define upgrade_value 5
#define tree strategie_tree[state.number]
const short small_bazar[5]={2,5,9,14,20};
const short grand_bazar[5]={3,7,12,18,25};
std::mutex player_turn_mutex[5];
struct tree_node{
    uint level;
    short pole;
    bool path=false;
    uint parent;
    dynamic_array<std::pair<float, uint>> childrens;
};
dynamic_array<tree_node> strategie_tree[5];
struct decision{
    short pole=7;
    bool logiczna=false;
    short produkt=0;
    short liczbowa=0;
};
decision decyzja;
enum color{
    red,
    white,
    green, 
    cyjan,
    yellow
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
    short things[4]={0};
    bool upgrades[4]={0};
    short rubies=0;
    short cart_upgrades=0; 
    short workers=4;
    short number=0;
    color player_color;
};
struct pole{
    short number;
    bool player_marker[5] = {0};
    bool player_family[5] = {0};
    bool sprzedawca = 0;
};
struct plansza{
    pole pola[4][4];
    short avaliable_rubies_sultan=7;
    short avaliable_rubies_jeweler=12;
    short post_office_state=0; //0-5
    short grand_bazar[4]={0}; 
    short small_bazar[4]={0};
    short upgrade_cost[4]={2};
    float sultan[4]={1};
    short post_state=0;
    uint move=0;
};
const short small_bazar_states[6][4]={
    {1,2,1,0},
    {2,2,0,1},
    {2,2,1,0},
    {2,1,1,1},
    {1,3,1,0},
    {3,1,1,0}
};
const short grand_bazar_states[6][4]={
    {0,1,2,2},
    {1,0,1,3},
    {1,0,2,2},
    {0,1,1,3},
    {1,1,1,2},
    {0,0,3,2}
};
const short post_states[5][3]={
    {0,1,2},
    {2,1,2},
    {2,1,3},
    {2,3,3},
    {2,3,4}
};
void print_board(plansza &board){
    for(short i=0; i<4; i++){
        for(short j=0; j<4; j++){
            std::cout<<nazwy_pol[board.pola[i][j].number]<<"("<<board.pola[i][j].number<<")"<<" | ";
        }
        std::cout<<std::endl;
    }
}
void print_bazar(plansza &board, bool grand=false){
    if(grand){
        printf("Towary na wielkim bazarze: \n Owoce: %d\n przyprawy: %d\n Tkaniny: %d\n Pierscienie: %d\n", board.grand_bazar[0], board.grand_bazar[1], board.grand_bazar[2], board.grand_bazar[3]);
    }else{
        printf("Towary na malym bazarze: \n Owoce: %d\n przyprawy: %d\n Tkaniny: %d\n Pierscienie: %d\n", board.small_bazar[0], board.small_bazar[1], board.small_bazar[2], board.small_bazar[3]);
    }
}
void print_player_state(player_state &state){
    printf("Stan gracza %d:\n Pieniadze: %d\n Towary: Owoce: %d, przyprawy: %d, Tkaniny: %d, Pierscienie: %d\n Ulepszenia: Owoce: %d, przyprawy: %d, Tkaniny: %d, Pierscienie: %d\n Rubiny: %d\n Ulepszenia wozka: %d\n Pracownicy w stosie: %d\n", state.number, state.money, state.things[0], state.things[1], state.things[2], state.things[3], state.upgrades[0], state.upgrades[1], state.upgrades[2], state.upgrades[3], state.rubies, state.cart_upgrades, state.workers);
}
std::pair<short, short> find_pole(plansza &board, short pole){
    for(short i=0; i<4; i++){
        for(short j=0; j<4; j++){
            if(board.pola[i][j].number == pole){
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
void make_game(plansza &board, player_state *states,short player_number){
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
    memcpy(board.grand_bazar,grand_bazar_states[rand()%6],sizeof(board.grand_bazar));
    memcpy(board.small_bazar,small_bazar_states[rand()%6],sizeof(board.small_bazar));
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
    dynamic_array<color>* colors = new dynamic_array<color>();
    for(short i=0; i<5; i++){
        push_back(colors, static_cast<color>(i));
    }
    for(short i=0; i<player_number; i++){
        states[i].number = i;
        states[i].money = 2+i;
        states[i].player_color = static_cast<color>(pop_at(colors, rand()%colors->size));
    }
}
void do_action(plansza &board, player_state &player, pole &pole){
    int rzut;
    std::pair<short, short> index;
    switch (pole.number){
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
                player.things[decyzja.produkt]= min(player.things[decyzja.produkt]+1, 2+player.cart_upgrades);;
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
                if(player.things[i]>board.small_bazar[i]){
                    player.money+=small_bazar[player.things[i]-board.small_bazar[i]];
                }
            }
        break;
        case 11:
            for(short i=0; i<4; i++){
                if(player.things[i]>board.grand_bazar[i]){
                    player.money+=grand_bazar[player.things[i]-board.grand_bazar[i]];
                }
            }
        break;
        case 12:
            index = find_pole(board, 12);
            if(board.pola[index.first][index.second].player_family[player.number]){
                std::pair<short, short> target_pos = find_pole(board, decyzja.pole);
                do_action(board, player, board.pola[target_pos.first][target_pos.second]);
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
                if(!player.upgrades[1]&&player.things[1]>board.upgrade_cost[1]){
                    player.things[1]--;
                    player.upgrades[1] = 1;
                    if(player.upgrades[2]){
                        player.rubies++;
                    }
                }else{
                    throw std::invalid_argument("Not enough products to buy upgrade or upgrade already bought");
                }
            }else if(decyzja.produkt==2){
                if(!player.upgrades[2]&&player.things[2]>board.upgrade_cost[2]){
                    player.things[2]--;
                    player.upgrades[2] = 1;
                    if(player.upgrades[1]){
                        player.rubies++;
                    }
                }else{
                    throw std::invalid_argument("Not enough products to buy upgrade or upgrade already bought");
                }
            }
            else{
                throw std::invalid_argument("Invalid product choice");
            }            
        break;
        case 15:
            if(decyzja.produkt==0){
                if(!player.upgrades[0]&&player.things[0]>board.upgrade_cost[0]){
                    player.things[0]--;
                    player.upgrades[0] = 1;
                    if(player.upgrades[3]){
                        player.rubies++;
                    }
                }else{
                    throw std::invalid_argument("Not enough products to buy upgrade or upgrade already bought");
                }
            }else if(decyzja.produkt==3){
                if(!player.upgrades[3]&&player.things[3]>board.upgrade_cost[3]){
                    player.things[3]--;
                    player.upgrades[3] = 1;
                    if(player.upgrades[0]){
                        player.rubies++;
                    }
                }else{
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
void move_player(plansza &board, player_state &player, pole &pole){
    if(distance(board, player.position, pole.number)<3 && player.position != pole.number){
        player.position = pole.number;
        if(pole.player_marker[player.number]){
            pole.player_marker[player.number] = 0;
            player.workers++;
            do_action(board, player, pole);
        }
        else{
            if(player.workers!=0){
                pole.player_marker[player.number] = 1;
                player.workers--;
                do_action(board, player, pole);
            }
        }

    }
}
float heu_moja(plansza &board, player_state &state, pole &pole){
    float things_value[4] = {0};
    float dice;
    std::pair<short, short> index;
    for(short i=0; i<4; i++){
        things_value[i] = board.grand_bazar[i]+board.small_bazar[i]+(board.upgrade_cost[i]*state.upgrades[i])+board.sultan[i];
    }
    float money_value = state.money-(24-board.avaliable_rubies_jeweler+(3-state.cart_upgrades)*7);
    switch(pole.number){
        case 1:
            if(state.money >= 7){
                return 7*(3-state.cart_upgrades);
            }else{
                return -1;
            }
        case 2:
            return (2+state.cart_upgrades-state.things[0]);
        case 3:
            return (2+state.cart_upgrades-state.things[1]);
        case 4:
            return (2+state.cart_upgrades-state.things[2]);    
        case 5:
            return (2+state.cart_upgrades-state.things[post_states[board.post_state][0]])*things_value[post_states[board.post_state][0]]+(2+state.cart_upgrades-state.things[post_states[board.post_state][1]])*things_value[post_states[board.post_state][1]]+post_states[board.post_state][2];
        case 6:
            return 6;
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
            if((state.things[0]-board.small_bazar[0]+state.things[1]-board.small_bazar[1]+state.things[2]-board.small_bazar[2]+state.things[3]-board.small_bazar[3])>0){
                return small_bazar[max((state.things[0]-board.small_bazar[0]+state.things[1]-board.small_bazar[1]+state.things[2]-board.small_bazar[2]+state.things[3]-board.small_bazar[3]),5)];
            }
            else{
                return -1;
            }
        case 11:
            if((state.things[0]-board.grand_bazar[0]+state.things[1]-board.grand_bazar[1]+state.things[2]-board.grand_bazar[2]+state.things[3]-board.grand_bazar[3])>0){
                return grand_bazar[max((state.things[0]-board.grand_bazar[0]+state.things[1]-board.grand_bazar[1]+state.things[2]-board.grand_bazar[2]+state.things[3]-board.grand_bazar[3]),5)];
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
            if(state.things[1]>=board.upgrade_cost[1]&&!state.upgrades[1] || state.things[2]>=board.upgrade_cost[2]&&!state.upgrades[2]){
                return 0.5*ruby_value*(6-state.rubies)+state.upgrades[1]*upgrade_value+state.upgrades[2]*upgrade_value;
            }else{
                return -1;
            }
        case 15:
            if(state.things[0]>=board.upgrade_cost[0]&&!state.upgrades[0] || state.things[3]>=board.upgrade_cost[3]&&!state.upgrades[3]){
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
}
void begin_strategy(plansza &board, player_state &state){
    tree_node wezel;
    wezel.pole = 7;
    wezel.level = 0;
    wezel.path = true;
    tree[0]=wezel;
    tree[0].parent = 0;
    for(short i=0; i<4; i++){
        for(short j=0; j<4; j++){
            if(board.pola[i][j].number!=state.position && distance(board, state.position, board.pola[i][j].number)<3){
                float wartosc = heu_moja(board, state, board.pola[i][j]);
                if(wartosc>0){
                    wezel.level = 1;
                    wezel.pole = board.pola[i][j].number;
                    wezel.parent = 0;
                    std::pair<float, int> tmp;
                    tmp.first = wartosc;
                    tmp.second = wezel.pole;
                    push_back(tree[0].childrens, tmp);
                }
            }
        }
    }
}
void strateg(plansza &board, player_state &state, short pole){
    bool path = true;
    tree_node current=tree[0];
    while(path){
        for(int i=0; i<current.childrens.size; i++){
            if(*current.childrens.(start+1).path){
                current = *current.childrens.(start+1);
                break;
            }
        }
        path=false;
    }
    
    
}
void agent(plansza &board, player_state &player){
    player_turn_mutex[player.number].lock();

}
void player(plansza &board, player_state &player){
    player_turn_mutex[player.number].lock();
    print_board(board);
    print_player_state(player);
    printf("Wybierz pole do ktorego chcesz sie ruszyc: ");
    int choice;
    scanf("%d", &choice);
    std::pair<short, short> index = find_pole(board, choice);
    if(index.first==-1){
        printf("Nie ma takiego pola\n");
    }
    else if(distance(board, player.position, choice)<3 && player.position != choice){
        switch(choice){
            case 6:
                printf("Czy chcesz pobrać towar czy monety? (0-towar, 1-monety): ");
                scanf("%d", &decyzja.logiczna);
                if(decyzja.logiczna){
                    printf("Ktory towar chcesz sprzedac? (0-owoce, 1-przyprawy, 2-tkaniny, 3-pierscienie): ");
                    scanf("%d", &decyzja.produkt);
                }
            break;
            case 8:
                printf("Ktory towar chcesz kupic? (0-owoce, 1-przyprawy, 2-tkaniny): ");
                scanf("%d", &decyzja.produkt);
            break;
            case 9:
                printf("Na jaka kwote chcesz postawic? (3-12): ");
                scanf("%d", &decyzja.liczbowa);
            break;
            case 12:
                printf("Na jakie pole chcesz sie przeniesc? (1-16): ");
                scanf("%d", &decyzja.pole);
            break;
            case 13:
                printf("Ktory towar chcesz oddac? (0-owoce, 1-przyprawy, 2-tkaniny, 3-pierscienie): ");
                scanf("%d", &decyzja.produkt);
            break;
            case 14:
                printf("Ktory upgrade chcesz kupic? (1-przyprawy, 2-tkaniny): ");
                scanf("%d", &decyzja.produkt);
            break;
            case 15:
                printf("Ktory upgrade chcesz kupic? (0-owoce, 3-pierscienie): ");
                scanf("%d", &decyzja.produkt);
            break;
        }
        move_player(board, player, board.pola[index.first][index.second]);
    }
    else{
        printf("Nie mozesz sie tam ruszyc\n");
    }
    player_turn_mutex[player.number].unlock();
}
void game_master(short player_number){
    for(short i=0; i<player_number; i++){
        player_turn_mutex[i].lock();
    }
    while(true){
        for(short i=0; i<player_number; i++){
            player_turn_mutex[i].unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            player_turn_mutex[i].lock();
        }
    }
}