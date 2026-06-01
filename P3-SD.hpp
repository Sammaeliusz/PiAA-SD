#pragma once
#include "P1-SD.hpp"
#include "P2-SD.hpp"
template<typename K, typename V>
class hash_table{
    protected:
    dynamic_array table;
    int hash_function(K key){
        return key % table.size;
    }
    int hash_function2(K key){
        return floor(table.size*((key*1.618)%1));
    }
    public:
    virtual void insert(K key, V value);
    virtual V get(K key);
    virtual void remove(K key);
};
template<typename K, typename V>
class hash_table_lancuch_two : public hash_table{
    private:
    dynamic_array<start <OW_Node<obj_with_priority<K, V>>>> table;
    public:
    void insert(K key, V value){
        int index1 = hash_function(key);
        int index2 = hash_function2(key);
        obj_with_priority<V, K> obj;
        obj.value = value;
        obj.priority = key;
        if(table.start[index2].size < table.start[index1].size){
            push_back(&table.start[index2], obj);
        }else{
            push_back(&table.start[index], obj);
        }
    }
    V get(K key){
        int index = hash_function(key);
        for(int i=0; i<table.start[index].size; i++){
            if(element_at(&table.start[index], i)->data.priority == key){
                return element_at(&table.start[index], i)->data.value;
            }
        }
    }
    void remove(K key){
        int index = hash_function(key);
        for(int i=0; i<table.start[index].size; i++){
            if(element_at(&table.start[index], i)->data.priority == key){
                pop_at(&table.start[index], i);
            }
        }
    }
};
template<typename K, typename V>
class hash_table_otwarte : public hash_table{
    private:
    dynamic_array<obj_with_priority<K, V>> table;
    int hash_function(K key){

    }
    public:
    
};