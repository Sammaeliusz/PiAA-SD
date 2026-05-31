#include "universal.hpp"
#include "P2.hpp"
#include "P1-SD.hpp"
#include "P3.hpp"


int main(){
    srand(time(NULL));
    plansza board;
    player_state states[5];
    make_game(board, states, 5);
    while(1){
        player(board, states[0]);
    }
    /*
    for(short i=0; i<5; i++){
        print_player_state(states[i]);
    }
        */
}