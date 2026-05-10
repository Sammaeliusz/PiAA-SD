#include "universal.hpp"
#include <vector>
#include "P2.hpp"
void test(){
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
            wyniki_lista[i/4][i%4] += time;
        }
        for(int i=0; i<20; i++){
            tic
            auto out = BellmanFord(g2[i],0);
            toc
            wyniki_macierz[i/4][i%4] += time;
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
}
void demo(){
    graph_list<int> g;
    graph_matrix<int> g2;
    g.insertVertex(0);
    g.insertVertex(1);
    g.insertVertex(2);
    g.insertVertex(3);
    g.insertVertex(4);
    g.insertEdge(0,1,6);
    g.insertEdge(0,2,7);
    g.insertEdge(1,2,8);
    g.insertEdge(1,3,5);
    g.insertEdge(1,4,-4);
    g.insertEdge(2,3,-3);
    g.insertEdge(2,4,9);
    g.insertEdge(3,1,-2);
    g.insertEdge(4,3,7);
    g.insertEdge(4,0,2);
    g2.insertVertex(0);
    g2.insertVertex(1);
    g2.insertVertex(2);
    g2.insertVertex(3);
    g2.insertVertex(4);
    g2.insertEdge(0,1,6);
    g2.insertEdge(0,2,7);
    g2.insertEdge(1,2,8);
    g2.insertEdge(1,3,5);
    g2.insertEdge(1,4,-4);
    g2.insertEdge(2,3,-3);
    g2.insertEdge(2,4,9);
    g2.insertEdge(3,1,-2);
    g2.insertEdge(4,3,7);
    g2.insertEdge(4,0,2);
    auto out = BellmanFord(g,0);
    std::cout<<"Lista:\n";
    for(int i=0; i<5; i++){
        std::cout<<"Odleglosc od wierzcholka 0 do wierzcholka "<<i<<": "<<out.first[i]<<" Poprzednik: "<<out.second[i]<<"\n";
    }
    std::cout<<"\n";
    auto out2 = BellmanFord(g2,0);
    std::cout<<"Macierz:\n";
    for(int i=0; i<5; i++){
        std::cout<<"Odleglosc od wierzcholka 0 do wierzcholka "<<i<<": "<<out2.first[i]<<" Poprzednik: "<<out2.second[i]<<"\n";
    }
    std::cout<<"\n";
}
int main(){
    srand(time(NULL));
    //test();
    demo();

    return 0;
}