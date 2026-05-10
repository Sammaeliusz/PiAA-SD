#include "universal.hpp"
#include <vector>
#include "P2.hpp"
int main(){
    srand(time(NULL));
    int wyniki_lista[5][4] = {0};
    int wyniki_macierz[5][4] = {0};
    int ilosci[5] = {10,50,100,500,1000};
    float gestosci[4] = {0.25, 0.5, 0.75, 1.0};
    for(int j = 0; j<100; j++){
        printf("Test number %d\n", j);
        graph_list<int> g[20];
        graph_matrix<int> g2[20];
        for(int i=0; i<20; i++){
            constructGraph(g[i], ilosci[i/4], gestosci[i%4]);
            constructGraph(g2[i], ilosci[i/4], gestosci[i%4]);
        }
        printf("Graphs generated\n");
        for(int i=0; i<20; i++){
            tic
            auto out = BellmanFord(g[i],0);
            toc
            wyniki_lista[i/4][i%4] = time;
        }
        for(int i=0; i<20; i++){
            tic
            auto out = BellmanFord(g2[i],0);
            toc
            wyniki_macierz[i/4][i%4] = time;
        }
    }
    std::cout<<"Lista:\n";
    for(int i=0; i<5; i++){
        for(int j=0; j<4; j++){
            std::cout<<wyniki_lista[i][j]/100<<" ";
        }
        std::cout<<"\n";
    }
    std::cout<<"Macierz:\n";
    for(int i=0; i<5; i++){
        for(int j=0; j<4; j++){
            std::cout<<wyniki_macierz[i][j]/100<<" ";
        }
        std::cout<<"\n";
    }
    return 0;
}