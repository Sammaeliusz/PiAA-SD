#pragma once
#include "P1-SD.hpp"
#include <stdexcept>
template <typename T, typename U>
struct obj_with_priority{
    T value;
    U priority;
    bool operator==(const obj_with_priority& other){
        return &value == &other.value && priority == other.priority;
    }
};
template <typename T, typename U>
class priority_queue{
    public:
    virtual void insert(T o, U p) = 0;
    virtual void insert(obj_with_priority<T, U> o) = 0;
    virtual obj_with_priority<T, U> extract() = 0;
    virtual obj_with_priority<T, U> top() = 0;
    virtual void increase_key(T o) = 0;
    virtual void decrease_key(T o) = 0;
    virtual int size() = 0;
};
template <typename T, typename U>
class priority_queue_array:public priority_queue<T, U>{
    private:
    dynamic_array<obj_with_priority<T, U>> array;
    public:
    void insert(T o, U p){
        obj_with_priority<T, U> new_obj;
        new_obj.value = o;
        new_obj.priority = p;
        for(int i = array.size; i>=0; i--){
            if(array.start[i].priority < p){
                push_at(&array, i, new_obj);
                return;
            }
        }
        push_back(&array, new_obj);
    }
    void insert(obj_with_priority<T, U> o){
        for(int i = array.size; i>=0; i--){
            if(array.start[i].priority < o.priority){
                push_at(&array, i, o);
                return;
            }
        }
        push_back(&array, o);
    }
    obj_with_priority<T, U> extract(){
        if(array.size==0){
            throw std::out_of_range("Priority queue is empty");
        }
        obj_with_priority<T, U> data = pop_begin(&array);
        return data;
    }
    obj_with_priority<T, U> top(){
        if(array.size==0){
            throw std::out_of_range("Priority queue is empty");
        }
        return array.start[0];
    }
    int size(){
        return array.size;
    }
    void increase_key(T o){
        for(int i = array.size; i>=0; i--){
            if(array.start[i].value == o){
                array.start[i].priority++;
                obj_with_priority<T, U> temp =pop_at(&array, i);
                insert(temp);
                return;
            }
        }
    } 
    void decrease_key(T o){
        for(int i = array.size; i>=0; i--){
            if(array.start[i].value == o){
                array.start[i].priority--;
                obj_with_priority<T, U> temp = pop_at(&array, i);
                insert(temp);
                return;
            }
        }
    }
};
template <typename T, typename U>
class priority_queue_heap:public priority_queue<T, U>{
    private:
    dynamic_array<obj_with_priority<T, U>> array;
    void heapify_up(int index){
        if(index == 0){
            return;
        }
        int parent = (index-1)/2;
        if(array.start[index].priority > array.start[parent].priority){
            obj_with_priority<T, U> temp = array.start[index];
            array.start[index] = array.start[parent];
            array.start[parent] = temp;
            heapify_up(parent);
        }
    }
    void heapify_down(int index){
        int left_child = 2*index + 1;
        int right_child = 2*index + 2;
        int largest = index;
        if(left_child < array.size && array.start[left_child].priority > array.start[largest].priority){
            largest = left_child;
        }
        if(right_child < array.size && array.start[right_child].priority > array.start[largest].priority){
            largest = right_child;
        }
        if(largest != index){
            obj_with_priority<T, U> temp = array.start[index];
            array.start[index] = array.start[largest];
            array.start[largest] = temp;
            heapify_down(largest);
        }
    }
    public:
    void insert(T o, U p){
        obj_with_priority<T, U> new_obj;
        new_obj.value = o;
        new_obj.priority = p;
        push_back(&array, new_obj);
        heapify_up(array.size-1);
    }
    void insert(obj_with_priority<T, U> o){
        push_back(&array, o);
        heapify_up(array.size-1);
    }
    obj_with_priority<T, U> extract(){
        if(array.size==0){
            throw std::out_of_range("Priority queue is empty");
        }
        obj_with_priority<T, U> data = array.start[0];
        array.start[0] = array.start[array.size-1];
        pop_back(&array);
        heapify_down(0);
        return data;
    }
    obj_with_priority<T, U> top(){
        if(array.size==0){
            throw std::out_of_range("Priority queue is empty");
        }
        return array.start[0];
    }
    int size(){
        return array.size;
    }
    void increase_key(T o){
        for(int i = 0; i<array.size; i++){
            if(array.start[i].value == o){
                array.start[i].priority++;
                heapify_up(i);
                return;
            }
        }
    }
    void decrease_key(T o){
        for(int i = 0; i<array.size; i++){
            if(array.start[i].value == o){
                array.start[i].priority--;
                heapify_down(i);
                return;
            }
        }
    }
};
template <typename T, typename U>
class priority_queue_linked_list:public priority_queue<T, U>{
    private:
    start <OW_Node<obj_with_priority<T, U>>> list;
    public:
    void insert(T o, U p){
        obj_with_priority<T, U> new_obj;
        new_obj.value = o;
        new_obj.priority = p;
        if(list.size==0){
            push_back(&list, new_obj);
            return;
        }
        for(int i = 0; i<list.size; i++){
            if(((OW_Node<obj_with_priority<T, U>>*)element_at(&list, i))->data.priority >= p){
                push_at(&list, i, new_obj);
                return;
            }
        }
        push_back(&list, new_obj);
    }
    void insert(obj_with_priority<T, U> o){
        if(list.size==0){
            push_back(&list, o);
            return;
        }
        for(int i = 0; i<list.size; i++){
            if(((OW_Node<obj_with_priority<T, U>>*)element_at(&list, i))->data.priority >= o.priority){
                push_at(&list, i, o);
                return;
            }
        }
        push_back(&list, o);
    }
    obj_with_priority<T, U> extract(){
        if(list.size==0){
            throw std::out_of_range("Priority queue is empty");
        }
        obj_with_priority<T, U> data = pop_begin(&list);
        return data;
    }
    obj_with_priority<T, U> top(){
        if(list.size==0){
            throw std::out_of_range("Priority queue is empty");
        }
        return list.first->data;
    }
    void increase_key(T o){
        for(int i = 0; i<list.size; i++){
            if((element_at(&list, i))->value == o){
                obj_with_priority<T, U> temp = pop_at(&list, i);
                temp.priority++;
                insert(temp);
                return;
            }
        }
    }
    void decrease_key(T o){
        for(int i = 0; i<list.size; i++){
            if((element_at(&list, i))->value == o){
                (element_at(&list, i))->priority--;
                obj_with_priority<T, U> temp = pop_at(&list, i);
                insert(temp);
                return;
            }
        }
    }
    int size(){
        return list.size;
    }
};