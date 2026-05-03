template <typename T>
class graph_node{
    protected:
    T value;
    public:
    graph_node(){}
    graph_node(T c_value){
        value=c_value;
    }
    T getValue(){return value;}
    void setValue(T val){value=val;}
    operator==(const graph_node<T>& other) const {
        return &value == &other.value;
    }
};
template <typename T>
class graph_edge :public graph_node<T>
{
    private:
        graph_node<T>* end1;
        graph_node<T>* end2;
    public:
        graph_edge(){}
        graph_edge(graph_node<T>* c_end1, graph_node<T>* c_end2, T c_value){
            end1 = c_end1;
            end2 = c_end2;
            this->value = c_value;
        }
        graph_node<T>* endVertices(){
            graph_node<T>* wyn = new graph_node<T>[2];
            wyn[0] = *end1;
            wyn[1] = *end2;
            return wyn;
        }
        graph_node<T> opposite(graph_node<T> v){
            if(&v==end1)
                return *end2;
            else
                return *end1;
        }
        void replace(T r_value){
            this->value = r_value;
        }
};

template <typename T>
class graph_vertice_list:public graph_node<T>
{
    private:
        std::vector<graph_edge<T>*> incidence_list;
    public:
        graph_vertice_list(T c_value):graph_node<T>(c_value){
            incidence_list.clear();
        }
        std::vector<graph_edge<T>*>* getListPointer(){return &incidence_list;}
        void addEdge(graph_edge<T>);

};
template <typename T>
class graph_edge_list :public graph_edge<T>
{
    private:
        std::vector<graph_edge<T>*>* incidence_list1;
        std::vector<graph_edge<T>*>* incidence_list2;
    public:
        graph_edge_list(graph_vertice_list<T>* c_end1, graph_vertice_list<T>* c_end2, T c_value){
            graph_edge<T>(c_end1, c_end2, c_value);
            incidence_list1 = c_end1->getListPointer();
            incidence_list2 = c_end2->getListPointer();
        }
};
template <typename T>
class graph_vertice_matrix:public graph_node<T>
{
    private:
    int id;
    public:
    int getId(){
        return id;
    }
    void setId(int i){
        id=i;
    }
};
template <typename T>
class graph{
    protected:
    std::vector<graph_node<T>> vertices;
    std::vector<graph_edge<T>> edges;
    public:
    graph(){};
    graph_node<T> getVertice(int i){
        return vertices[i];
    }
    graph_edge<T> getEdge(int i){
        return edges[i];
    }
    int verticeNumber(){
        return vertices.size();
    }
    int edgeNumber(){
        return edges.size();
    }
    graph_node<T> endVertices(graph_edge<T> e){
        for(int i =0; i<edges.size(); i++){
            if(e == edges[i]){
                return edges[i].endVertices();
            }
        }
    }
    graph_node<T> opposite(graph_node<T> v, graph_edge<T> e){
        for(int i =0; i<edges.size(); i++){
            if(e == edges[i]){
                return edges[i].opposite(v);
            }
        }
    }
    bool areAdjecent(graph_node<T> v, graph_node<T> w);
    void replace(graph_node<T> v, T x){
        for(int i =0; i<vertices.size(); i++){
            if(v == vertices[i]){
                vertices[i].setValue(x);
            }
        }
    }
    void replace(graph_edge<T> e, T x){
        for(int i =0; i<edges.size(); i++){
            if(e == edges[i]){
                edges[i].setValue(x);
            }
        }
    }
    void insertVertex(T o){
        vertices.push_back(graph_node<T>(o));
    }
    void removeEdge(graph_edge<T> e){
        for(int i =0; i<edges.size(); i++){
            if(e == edges[i]){
                edges.erase(i);
            }
        }
    }
    std::vector<graph_edge<T>> incidentEdges(graph_node<T> v){
        std::vector<graph_edge<T>> wyn;
        for(int i =0; i<edges.size(); i++){
            graph_node<T>* endv = edges[i].endVertices();
            if(endv[0] == v || endv[1] == v){
                wyn.push_back(edges[i]);
            }
        }
        return wyn;
    }
    void removeVertex(graph_node<T> v){
        for(int i =0; i<vertices.size(); i++){
            if(v == vertices[i]){
                std::vector<graph_edge<T>> inc_edg = incidentEdges();
                for(int j=0; j<inc_edg.size(); j++){
                    removeEdge(inc_edg[j]);
                }
                vertices.erase(i);
            }
        }
    }
    void printGraph(){
        for(int i =0; i<vertices.size(); i++){
            std::cout<<vertices[i].getValue()<<": ";
            std::vector<graph_edge<T>> inc_edg = incidentEdges(vertices[i]);
            for(int j=0; j<inc_edg.size(); j++){
                graph_node<T>* endv = inc_edg[j].endVertices();
                if(endv[0] == vertices[i])
                    std::cout<<endv[1].getValue()<<" ";
                else
                    std::cout<<endv[0].getValue()<<" ";
            }
            std::cout<<std::endl;
        }
    }
};
template <typename T>
class graph_matrix:public graph<T>{
    private:
    std::vector<graph_vertice_matrix<T>> vertices;
    std::vector<std::vector<graph_edge<T>*>> matrix;
    public:
    graph_matrix(){
    };
    graph_vertice_matrix<T> getVertice(int i){
        return vertices[i];
    }
    void insertVertex(T o){
        graph<T>::insertVertex(o);
        for(int i =0; i<matrix.size(); i++){
            matrix[i].push_back(0);
        }
        std::vector<graph_edge<T>*> new_row;
        for(int i =0; i<vertices.size(); i++){
            new_row.push_back(NULL);
        }
        matrix.push_back(new_row);
    }
    void insertEdge(graph_vertice_matrix<T> v, graph_vertice_matrix<T> w, T o){
        this->edges.push_back(graph_edge<T>(&v, &w, o));
        int id1 = v.getId();
        int id2 = w.getId();
        matrix[id1][id2] = &this->edges[this->edges.size()-1];
        matrix[id2][id1] = &this->edges[this->edges.size()-1];
    }
    bool areAdjecent(graph_node<T> v, graph_node<T> w){
        int id1 = v.getId();
        int id2 = w.getId();
        if(matrix[id1][id2] != NULL)
            return true;
        else
            return false;
    }

};
template <typename T>
class graph_list:public graph<T>{
    private:
    std::vector<graph_vertice_list<T>> vertices;
    std::vector<graph_edge_list<T>> edges;
    public:
    graph_list(){};
    graph_vertice_list<T> getVertice(int i){
        return vertices[i];
    }
    void insertVertex(T o){
         graph_vertice_list<T> new_vertice(o);
         new_vertice.getListPointer()->clear();
         vertices.push_back(new_vertice);
    }
    void insertEdge(graph_vertice_list<T> v, graph_vertice_list<T> w, T o){
        edges.push_back(graph_edge_list<T>(&v, &w, o));
        v.getListPointer()->push_back(&edges[edges.size()-1]);
        w.getListPointer()->push_back(&edges[edges.size()-1]);
    }
    bool areAdjecent(graph_node<T> v, graph_node<T> w){
        std::vector<graph_edge<T>> inc_edg = incidentEdges(v);
        for(int i =0; i<inc_edg.size(); i++){
            if(inc_edg[i].opposite(v) == w)
                return true;
        }
        return false;
    }
};