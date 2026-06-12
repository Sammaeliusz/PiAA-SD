#include "universal.hpp"
#include "P2.hpp"
#include "P3.hpp"

int main(){
    srand(time(NULL));
    plansza board;
    printf("Wprowadz ilosc graczy ludzkich i ogólną liczbę graczy w formacie: gracze_ludzcy/gracze\n");
    scanf("%d/%d", &ludzie, &gracze);
    boty = gracze-ludzie;
    player_state *states = new player_state[gracze];
    std::thread *stratedzy = new std::thread[boty];
    make_game(board, states, gracze);
    for(short i=0; i<boty; i++){
        begin_strategy(board, states[ludzie+i]);
        //stratedzy[i] = std::thread(strategize_tree, board, states[ludzie+i]);
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
    }
}