template<typename T>
struct dynamic_array
{
    T* start = (T*)malloc(sizeof(T)*10);
    int size=10;
    int capacity=0;
};
template<typename T>
void push_back(dynamic_array<T>* a, T data){
    if(a->capacity==a->size){
        a->start = (T*)realloc(a->start, 2*a->size*sizeof(T));
        a->size *=2;
    }
    *(a->start+a->capacity) = data;
    a->capacity++;
}
template<typename T>
void push_begin(dynamic_array<T>* a, T data){
    if(a->capacity==a->size){
        a->start = (T*)realloc(a->start, 2*a->size*sizeof(T));
        a->size *= 2;
    }
    memcpy(a->start+1, a->start, a->capacity*sizeof(T));
    *(a->start) = data;
    a->capacity++;
}
template<typename T>
void push_at(dynamic_array<T>* a, T data, int position){
    if(a->capacity==a->size){
        a->start = (T*)realloc(a->start, 2*a->size*sizeof(T));
        a->size *= 2;
    }
    for(int i = a->capacity; i>position; i--){
        *(a->start+i) =*(a->start+i-1);
    }
    *(a->start+position) = data;
    a->capacity++;
}
template<typename T>
T* pop_back(dynamic_array<T>* a){
    if(a->size==0){
        throw std::out_of_range("Array is empty");
    }
    T* data = a->start+a->capacity-1;
    if(a->capacity+1==a->size/2){
        a->start = (T*)reallock(a->start, sizeof(T)*a->size/2);
        a->size /= 2;
    }
    a->capacity--;
    return data;
}
template<typename T>
T* pop_begin(dynamic_array<T>* a){
    if(a->size==0){
        throw std::out_of_range("Array is empty");
    }
    T* data = a->start;
    a->start++;
    if(a->capacity+1==a->size/2){
        a->start = (T*)reallock(a->start, sizeof(T)*a->size/2);
        a->size /= 2;
    }
    a->capacity--;
    return data;
}
template <typename T>
T* pop_at(dynamic_array<T>* a, int position){
    if(a->size==0){
        throw std::out_of_range("Array is empty");
    }
    T* data = a->start+position;
    memcpy(a->start+position, a->start+position+1, (a->capacity-position)*sizeof(T));
    if(a->capacity+1==a->size/2){
        a->start = (T*)reallock(a->start, sizeof(T)*a->size/2);
        a->size /= 2;
    }
    a->capacity--;
    return data;
}
template<typename T>
void print_list(dynamic_array<T>* array){
    std::cout<<"size: "<<array->size<<" capacity: "<<array->capacity<<"\n";
    for(int i=0; i<array->capacity; i++){
        std::cout<<*(array->start+i)<<" ";
    }
}
template<typename T>
struct start{
    int size = 0;
    T* first = new T;
    T* last = first;
};
template<typename T>
struct OW_Node{
    T data;
    OW_Node* next;
    using value_type = T;
};
template<typename T>
struct TW_Node{
    T data;
    TW_Node* next;
    TW_Node* prev;
    using value_type = T;
};
template<typename U>
void* element_at(start<TW_Node<U>>* list, int index){
    if(index>=list->size){
        throw std::out_of_range("Index out of range");
    }
    TW_Node<U>* current = list->first;
    if(index<=list->size/2){
        for(int i=0;i<index;i++){
            current = current->next;
        }
        return &current->data;
    }
    else{
        current = list->last;
        for(int i=list->size-1;i>index;i--){
            current = current->prev;
        }
        return &current->data;
    }
}
template<typename U>
void* element_at(start<OW_Node<U>>* list, int index){
    if(index>=list->size){
        throw std::out_of_range("Index out of range");
    }
    OW_Node<U>* current = list->first;
    for(int i=0;i<index;i++){
        current = current->next;
    }
    return &current->data;
}
template<typename U>
void push_back(start<OW_Node<U>>* list, U data){
    if(list->size==0){
        list->first = new OW_Node<U>;
        list->first->data = data;
        list->last = list->first;
        list->size++;
    }
    else{
        list->last->next = new OW_Node<U>;
        list->last->next->data = data;
        list->last->next->next = nullptr;
        list->last = list->last->next;
        list->size++;
    }
    
}
template<typename U>
void push_begin(start<OW_Node<U>>* list, U data){
    OW_Node<U>* new_node = new OW_Node<U>;
    new_node->data = data;
    new_node->next = list->first;
    list->first = new_node;
    list->size++;
}
template<typename U>
void push_at(start<OW_Node<U>>* list, int index, U data){
    if(index>list->size){
        throw std::out_of_range("Index out of range");
    }
    OW_Node<U>* new_node = new OW_Node<U>;
    new_node->data = data;
    void* current = element_at(list, index);
    new_node->next = (OW_Node<U>*)current;
    current = element_at(list, index-1);
    ((OW_Node<U>*)current)->next = new_node;
}
template<typename U> 
U pop_back(start<OW_Node<U>>* list){
    OW_Node<U>* current = list->last;
    OW_Node<U>* new_last = (OW_Node<U>*)element_at(list, list->size-1);
    list->last = (OW_Node<U>*)new_last;
    new_last->next = nullptr;
    U data = current->data;
    delete current;
    list->size--;
    return data;
}
template<typename U>
U pop_begin(start<OW_Node<U>>* list){
    OW_Node<U>* current = list->first;
    list->first = list->first->next;
    U data = current->data;
    delete current;
    list->size--;
    return data;
}
template<typename U>
U pop_at(start<OW_Node<U>>* list, int index){
    if(index>=list->size){
        throw std::out_of_range("Index out of range");
    }
    void* current = element_at(list, index);
    OW_Node<U>* current_node = (OW_Node<U>*)current;
    void* previous = element_at(list, index-1);
    ((OW_Node<U>*)previous)->next = current_node->next;
    U data = current_node->data;
    delete current_node;
    list->size--;
    return data;
}
template<typename U>
void push_back(start<TW_Node<U>>* list, U data){
    if(list->size==0){
        list->first = new TW_Node<U>;
        list->first->data = data;
        list->first->prev = nullptr;
        list->first->next = nullptr;
        list->last = list->first;
        list->size++;
    }
    else{
        list->last->next = new TW_Node<U>;
        list->last->next->data = data;
        list->last->next->next = nullptr;
        list->last->next->prev = list->last;
        list->last = list->last->next;
        list->size++;
    }
    
}
template<typename U>
void push_begin(start<TW_Node<U>>* list, U data){
    TW_Node<U>* new_node = new TW_Node<U>;
    new_node->data = data;
    new_node->next = list->first;
    new_node->prev = nullptr;
    new_node->next->prev = new_node;
    list->first = new_node;
    list->size++;
}
template<typename U>
void push_at(start<TW_Node<U>>* list, int index, U data){
    if(index>list->size){
        throw std::out_of_range("Index out of range");
    }
    TW_Node<U>* new_node = new TW_Node<U>;
    new_node->data = data;
    TW_Node<U>* current = (TW_Node<U>*)element_at(list, index);
    new_node->next = current;
    current->prev->next = new_node;
    new_node->prev = current->prev;
    current->prev = new_node;
}
template<typename U> 
U pop_back(start<TW_Node<U>>* list){
    TW_Node<U>* current = list->last;
    TW_Node<U>* new_last = (TW_Node<U>*)element_at(list, list->size-2);
    list->last = (TW_Node<U>*)new_last;
    new_last->next = nullptr;
    U data = current->data;
    delete current;
    list->size--;
    return data;
}
template<typename U>
U pop_begin(start<TW_Node<U>>* list){
    TW_Node<U>* current = list->first;
    list->first = list->first->next;
    list->first->prev = nullptr;
    U data = current->data;
    delete current;
    list->size--;
    return data;
}
template<typename U>
U pop_at(start<TW_Node<U>>* list, int index){
    if(index>=list->size){
        throw std::out_of_range("Index out of range");
    }
    TW_Node<U>* current_node = (TW_Node<U>*)element_at(list, index);
    (current_node->next)->prev = current_node->prev;
    (current_node->prev)->next = current_node->next;
    U data = current_node->data;
    delete current_node;
    list->size--;
    return data;
}
template<typename T>
void print_list(start<T>* list){
    T* current = list->first;
    while(current->next!=nullptr){
        std::cout<<current->data<<" ";//<<current->prev<<"<-"<<current<<"-> "<<current->next<<"\n";
        current = current->next;
    }
    std::cout<<current->data<<"\n";
}