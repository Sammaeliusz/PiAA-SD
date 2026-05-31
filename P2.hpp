
template <typename T>
class graph_node{
    protected:
    T value;
    int index=-1;
    public:
    graph_node(){}
    graph_node(T c_value){
        value=c_value;
    }
    T getValue(){return value;}
    void setValue(T val){value=val;}
    int getIndex(){return index;}
    void setIndex(int i){index = i;}
    bool operator==(const graph_node<T> other) const {
        return value == other.value;
    }
};
template <typename T>
class graph_edge :public graph_node<T>
{
    private:
        int end1;
        int end2;
    public:
        graph_edge(){}
        graph_edge(int c_end1, int c_end2, T c_value){
            end1 = c_end1;
            end2 = c_end2;
            this->value = c_value;
        }
        std::pair<int, int> endVertices() {
            return {end1, end2};
        }
        graph_node<T> opposite(graph_node<T> v){
            if(v==end1)
                return end2;
            else
                return end1;
        }
        void replace(T r_value){
            this->value = r_value;
        }
};

template <typename T>
class graph_vertice_list:public graph_node<T>
{
    private:
        std::vector<int> incidence_list;
    public:
        graph_vertice_list(T c_value):graph_node<T>(c_value){
            incidence_list.clear();
        }
        std::vector<int>* getListPointer(){return &incidence_list;}
        void addEdge(int e){
            incidence_list.push_back(e);
        };

};
template <typename T>
class graph_edge_list :public graph_edge<T>
{
    private:
        std::vector<int>* incidence_list1;
        std::vector<int>* incidence_list2;
    public:
        graph_edge_list(graph_vertice_list<T>& c_end1, graph_vertice_list<T>& c_end2, T c_value):graph_edge<T>(c_end1.getIndex(), c_end2.getIndex(), c_value){
            incidence_list1 = c_end1.getListPointer();
            incidence_list2 = c_end2.getListPointer();
        }
};

template <typename T, typename V, typename E>
class graph {
    protected:
    std::vector<V> vertices;
    std::vector<E> edges;
    public:
    graph(){};
    V getVertice(int i){
        for(int j =0; j<vertices.size(); j++){
            if(i == vertices[j].getValue()){
                return vertices[j];
            }
        }
    }
    int getVerticeIndex(V v){
        for(int j =0; j<vertices.size(); j++){
            if(v == vertices[j]){
                return j;
            }
        }
    }
    int getVerticeIndex(int v){
        for(int j =0; j<vertices.size(); j++){
            if(v == vertices[j].getValue()){
                return j;
            }
        }
    }
    E getEdge(int i){
        return edges[i];
    }
    E getEdge(int v, int w){
        for(int i =0; i<edges.size(); i++){
            if((edges[i].endVertices().first == getVertice(v) && edges[i].endVertices().second == getVertice(w)) || (edges[i].endVertices().first == getVertice(w) && edges[i].endVertices().second == getVertice(v)    )){
                return edges[i];
            }
        }
    }
    int verticeNumber(){
        return vertices.size();
    }
    V& getVerticeFromIndex(int i){
        return this->vertices[i];
    }
    int edgeNumber(){
        return edges.size();
    }
    V endVertices(E e){
        for(int i =0; i<edges.size(); i++){
            if(e == edges[i]){
                return edges[i].endVertices();
            }
        }
    }
    graph_node<T> opposite(V &v, E e){
        for(int i =0; i<edges.size(); i++){
            if(e == edges[i]){
                return edges[i].opposite(v);
            }
        }
    }
    bool areAdjecent(V v, V w);
    std::vector<E>* incidentEdges(V v)
    {
        std::vector<E>* inc_edges = new std::vector<E>();
        return inc_edges;
    }
    void replace(V v, T x){
        for(int i =0; i<vertices.size(); i++){
            if(v == vertices[i]){
                vertices[i].setValue(x);
            }
        }
    }
    void replace(E e, T x){
        for(int i =0; i<edges.size(); i++){
            if(e == edges[i]){
                edges[i].setValue(x);
            }
        }
    }
    void insertVertex(T o){
        vertices.push_back(V(o));
        vertices[vertices.size()-1].setIndex(vertices.size()-1);
    }
    void removeEdge(E e){
        for(int i =0; i<edges.size(); i++){
            if(e == edges[i]){
                edges.erase(edges.begin() + i);
            }
        }
    }
    void removeVertex(V v){
        for(int i =0; i<vertices.size(); i++){
            if(v == vertices[i]){
                std::vector<E> inc_edg = incidentEdges(v);
                for(int j=0; j<inc_edg.size(); j++){
                    removeEdge(inc_edg[j]);
                }
                vertices.erase(i);
            }
        }
    }
    
};
template <typename T>
class graph_matrix:public graph<T, graph_node<T>, graph_edge<T>>{
    private:
    std::vector<std::vector<int>> matrix;
    public:
    graph_matrix():graph<T, graph_node<T>, graph_edge<T>>(){
        matrix.clear();
    };
    void insertVertex(T o){
        graph<T, graph_node<T>, graph_edge<T>>::insertVertex(o);
        for(int i =0; i<matrix.size(); i++){
            matrix[i].push_back(0);
        }
        std::vector<int> new_row;
        for(int i =0; i<this->vertices.size(); i++){
            new_row.push_back(0);
        }
        matrix.push_back(new_row);
    }
    void insertEdge(int v, int w, T o){
        this->edges.push_back(graph_edge<T>(this->getVerticeIndex(v), this->getVerticeIndex(w), o));
        matrix[v][w] = this->edges.size() - 1;
        //matrix[w][v] = this->edges.size() - 1;
    }
    bool areAdjecent(int v, int w){
        if(matrix[v][w] != 0)
            return true;
        else
            return false;
    }
    std::vector<int>* incidentEdges(int v){
        std::vector<int>* inc_edges = new std::vector<int>();
        for(int i =0; i<this->vertices.size(); i++){
            if(matrix[v][i] != 0){
                inc_edges->push_back(matrix[v][i]);
            }
        }
        return inc_edges;
    }
};
template <typename T>
class graph_list:public graph<T, graph_vertice_list<T>, graph_edge_list<T>>{
    public:
    graph_list(){};
    graph_vertice_list<T>& getVertice(int i){
        for(int j =0; j<this->vertices.size(); j++){
            if(i == this->vertices[j].getValue()){
                return this->vertices[j];
            }
        }
    }

    void insertVertex(T o){
        graph_vertice_list<T> new_vertice(o);
        new_vertice.setIndex(this->vertices.size());
        this->vertices.push_back(new_vertice);
    }
    void insertEdge(int v, int w, T o){
        graph_edge_list<T> new_edge(this->getVertice(v), this->getVertice(w), o);
        this->edges.push_back(new_edge);
        this->getVertice(v).addEdge(this->edges.size() - 1);
        //this->getVertice(w).addEdge(this->edges.size() - 1);
    }
    bool areAdjecent(int v, int w){
        std::vector<int>* inc_edg = incidentEdges(v);
        for(int i =0; i<inc_edg->size(); i++){
            if(this->opposite(this->getVertice(v), this->edges[inc_edg->at(i)]) == w)
                return true;
        }
        return false;
    }
    std::vector<int>* incidentEdges(int v){
        return this->vertices[v].getListPointer();
    }
};
template <typename G>
void constructGraph(G& g, int vertices, float density){
    for(int i =0; i<vertices; i++){
        g.insertVertex(i);
    }
    for(int i =0; i<vertices; i++){
        for(int j = 0; j<vertices*density; j++){
            if(i!=j)
                g.insertEdge(i, j, rand()%100+1);
        }
    }/*
        else{
            for(int j = 0; j<g.verticeNumber()-1*density; j++){
                int e_ver = rand()%g.verticeNumber();
                while(e_ver == i){
                    e_ver = rand()%g.verticeNumber();
                }
                while(g.areAdjecent(i, e_ver)){
                    e_ver = rand()%g.verticeNumber();
                }
                g.insertEdge(i, e_ver, rand()%100+1);
            }
        }
    }*/
}

template <typename G>
void printGraph(G& g){
        printf("Printing graph with %d vertices and %d edges:\n", g.verticeNumber(), g.edgeNumber());
        for(int i =0; i<g.verticeNumber(); i++){
            auto inc_edges = g.incidentEdges(i);
            std::cout<<"Vertex: "<<g.getVerticeFromIndex(i).getValue()<<" Edges: "<<inc_edges->size()<<" | ";
            for(int j =0; j<inc_edges->size(); j++){
                std::pair<graph_node<int>, graph_node<int>> endv = g.getEdge(inc_edges->at(j)).endVertices();
                std::cout<<endv.first.getValue()<<" - "<<g.getEdge(inc_edges->at(j)).getValue()<<" - "<<endv.second.getValue()<<", ";
            }
            std::cout<<std::endl;
        }
        
    }
template <typename G>
std::pair<int*, int*> BellmanFord(G& g, int startVertex){
    int* odleglosc = new int[g.verticeNumber()];
    int* poprzednik = new int[g.verticeNumber()];
    for(uint i = 0; i<g.verticeNumber(); i++){
        odleglosc[i] = INT_MAX;
        poprzednik[i] = -1;
    }
    odleglosc[g.getVerticeIndex(startVertex)] = 0;
    for(uint i=1; i<g.verticeNumber(); i++){
        for(uint j=0; j<g.edgeNumber(); j++){
            graph_edge<int> edge = g.getEdge(j);
            std::pair<int, int> endv = edge.endVertices();
            if(odleglosc[endv.first] != INT_MAX && odleglosc[endv.first]+edge.getValue() < odleglosc[endv.second]){
                odleglosc[endv.second] = odleglosc[endv.first]+edge.getValue();
                poprzednik[endv.second] = endv.first;
            }/*
            else if(odleglosc[endv.second] != INT_MAX && odleglosc[endv.second]+edge.getValue() < odleglosc[endv.first]){
                printf("Updating distance to vertex %d: from: %d to: %d\n", endv.first, odleglosc[endv.first], odleglosc[endv.second]+edge.getValue());
                odleglosc[endv.first] = odleglosc[endv.second]+edge.getValue();
                poprzednik[endv.first] = endv.second;
            }*/
        }
    }
    return std::pair<int*, int*>(odleglosc, poprzednik);
}