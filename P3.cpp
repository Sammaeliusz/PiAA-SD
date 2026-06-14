#include "universal.hpp"
#include "P2.hpp"
#include "P3.hpp"

int main(){
    srand(time(NULL));
    plansza board;
    bool intro = true;
    while(intro){
    printf("Wprowadz ilosc graczy ludzkich i ogólną liczbę graczy w formacie: gracze_ludzcy/gracze\n");
    scanf("%d/%d", &ludzie, &gracze);
    if(gracze >= 2 && gracze <= 5 && ludzie <= gracze && ludzie >=0){
        intro = false;
    }
    else{
        printf("Wprowadzono niepoprawne dane. Sprobuj jeszcze raz.\n");
    }
    }
    boty = gracze-ludzie;
    player_state *states = new player_state[gracze];
    std::thread *stratedzy = new std::thread[boty];
    make_game(board, states, gracze);
    for(short i=0; i<boty; i++){
        begin_strategy(board, states[ludzie+i]);
        strategize_tree(board, states[ludzie+i]);
    }
    bool gra = true;
    while(gra){
        for(short i=0; i<ludzie; i++){
            player(board, states[i], states);
        }
        for(short i=0; i<boty; i++){
            if (stratedzy[i].joinable())
                stratedzy[i].join();
            printf("Numer gracza komputerowego: %d\n", states[ludzie+i].number);
            strategize_tree(board, states[ludzie+i]);
            print_board(board);
            print_player_state(states[ludzie+i]);
            //print_drzewo(states[ludzie+i].number, 100);
            agent(board, states[ludzie+i]);
            //print_drzewo(states[ludzie+i].number, 100);
            //printf("%d ",strategie_tree[ludzie+i].size);
            //printf("%d", get_last_path(states[ludzie+i]));
            //stratedzy[i] = std::thread(strategize_tree, board, states[ludzie+i]);
        }
        for(short i=0; i<gracze; i++){
            if(states[i].rubies == 5+(gracze==2?1:0)){
                gra = false;
                for(short j=0;j<boty; j++){
                    if (stratedzy[j].joinable())
                        stratedzy[j].detach();
                }
            }
        }
        gra = false;
    }
    for(short j=0;j<boty; j++){
        if (stratedzy[j].joinable()){
            stratedzy[j].join();
        }
    }
    for(uint i =0; i<gracze; i++){
        /*
        print_path(i);
        print_player_state(states[i]);
        uint index = get_last_path(states[i]);
        for(int j = 0; j<strategie_tree[i].start[index].childrens.size; j++){
            printf("pole %d z heurystyką %f \n", strategie_tree[i].start[strategie_tree[i].start[index].childrens.start[j].second].i_pole, strategie_tree[i].start[index].childrens.start[j].first);
        }
            */
        printf("Rozmiar drzewa %u\n" ,strategie_tree[i].size);
    }
}  