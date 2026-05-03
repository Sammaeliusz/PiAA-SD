#include "universal.hpp"
#include <vector>
#include "P2.hpp"
int main(){
    graph_list<int> g1;
    graph_matrix<int> g2;
    for(int i =0; i<10; i++){
        g1.insertVertex(i);
        g2.insertVertex(i);
    }
    g1.printGraph();
    g2.printGraph();
    std::cout<<"wprowadzono 10 wierzcholkow do obu grafow"<<std::endl;
    for(int i =0; i<9; i++){
        g1.insertEdge(g1.getVertice(i), g1.getVertice(i+1), i);
        g2.insertEdge(g2.getVertice(i), g2.getVertice(i+1), i);
    }
    std::cout<<"wprowadzono 9 krawedzi do obu grafow"<<std::endl;
    g1.printGraph();
    g2.printGraph();
    std::cout<<"wyswietlono oba grafy"<<std::endl;
    return 0;
}