#pragma once
#include "universal.hpp"
#include "P1-SD.hpp"
#define uint unsigned int
template <typename K, typename V>
struct obj_with_key{
    V value;
    K key;
    bool operator==(const obj_with_key& other){
        return value == other.value && key == other.key;
    }};
template<typename K, typename V>
class hash_table{
    public:
    uint elements = 0;
    protected:
    dynamic_array<obj_with_key<K, V>> table;
    uint size = 10;
    double frac;
    int hash_function(K key, short type=1){
        switch (type)
        {
        case 0:
            return 0;
        break;
        case 1:
            return key % this->size;
        break;
        case 2:
            frac= std::fmod(key * 1.6180339887, 1.0);
            return static_cast<int>(this->size * frac);
        break;
        default:
            throw std::invalid_argument("Wybrano zla funkcje haszujaca");
        break;
        }
        
    }
    //virtual void insert(K key, V value);
    //virtual V get(K key);
    //virtual void remove(K key);
};
template<typename K, typename V>
class hash_table_lancuch : public hash_table<K, V>{
    private:
    dynamic_array<obj_with_key<K, V>>* table = new dynamic_array<obj_with_key<K, V>>[this->size];
    public:
    ~hash_table_lancuch(){
        delete[] table;
    }
    void insert(K key, V value, short function = 1){
        if(this->elements/this->size>2){
            this->size*=2;
            dynamic_array<obj_with_key<K, V>>* new_table = new dynamic_array<obj_with_key<K, V>>[this->size];
            for(uint i=0; i<this->size/2; i++){
                for(uint j=0; j<table[i].size; j++){
                    uint index1 = this->hash_function(table[i].start[j].key, function);
                    push_back(&new_table[index1], table[i].start[j]);
                }
            }
            delete[] table;
            table = new_table;
        }
        int index1 = this->hash_function(key, function);
        obj_with_key<K, V> obj;
        obj.value = value;
        obj.key = key;
        push_back(&table[index1], obj);
        this->elements++;
    }
    V get(K key, short function = 1){
        int index = this->hash_function(key, function);
        for(int i=0; i<table[index].size; i++){
            if(table[index].start[i].key == key){
                return table[index].start[i].value;
            }
        }
        throw std::out_of_range("Key not found");
    }
    void remove(K key, short function = 1){
        int index = this->hash_function(key, function);
        for(int i=0; i<table[index].size; i++){
            if(table[index].start[i].key == key){
                pop_at(&table[index], i);
                this->elements--;
                break;
            }
        }
    }
};
template<typename K, typename V>
class hash_table_lancuch_two : public hash_table<K, V>{
    private:
    dynamic_array<obj_with_key<K, V>>* table = new dynamic_array<obj_with_key<K, V>>[this->size];
    public:
    ~hash_table_lancuch_two(){
        delete[] table;
    }
    void insert(K key, V value, short fun1 = 1, short fun2 = 2){
        if(this->elements/this->size>2){
            this->size*=2;
            dynamic_array<obj_with_key<K, V>>* new_table = new dynamic_array<obj_with_key<K, V>>[this->size];
            for(uint i=0; i<this->size/2; i++){
                for(uint j=0; j<table[i].size; j++){
                    uint index1 = this->hash_function(table[i].start[j].key, fun1);
                    uint index2 = this->hash_function(table[i].start[j].key, fun2);
                    if(new_table[index2].size < table[index1].size){
                        push_back(&new_table[index2], table[i].start[j]);
                    }else{
                        push_back(&new_table[index1], table[i].start[j]);
                    }
                }
            }
            delete[] table;
            table = new_table;
        }
        int index1 = this->hash_function(key, fun1);
        int index2 = this->hash_function(key, fun2);
        obj_with_key<K, V> obj;
        obj.value = value;
        obj.key = key;
        if(table[index2].size < table[index1].size){
            push_back(&table[index2], obj);
        }else{
            push_back(&table[index1], obj);
        }
        this->elements++;
    }
    V get(K key, short fun1 = 1, short fun2 = 2){
        int index = this->hash_function(key, fun1);
        for(int i=0; i<table[index].size; i++){
            if(table[index].start[i].key == key){
                return table[index].start[i].value;
            }
        }
        index = this->hash_function(key, fun2);
        for(int i=0; i<table[index].size; i++){
            if(table[index].start[i].key == key){
                return table[index].start[i].value;
            }
        }
        throw std::out_of_range("Key not found");
    }
    void remove(K key, short fun1 = 1, short fun2 = 2){
        int index = this->hash_function(key, fun1);
        for(int i=0; i<table[index].size; i++){
            if(table[index].start[i].key == key){
                pop_at(&table[index], i);
                this->elements--;
                return;
            }
        }
        index = this->hash_function(key, fun2);
        for(int i=0; i<table[index].size; i++){
            if(table[index].start[i].key == key){
                pop_at(&table[index], i);
                this->elements--;
                return;
            }
        }
    }
};
template<typename K, typename V>
class hash_table_otwarte : public hash_table<K, V>{
    private:
    obj_with_key<K, V> *table = new obj_with_key<K, V>[this->size]{obj_with_key<K, V>{0, 0}};
    public:
    void insert(K key, V value, short function = 2){
        if(static_cast<double>(this->elements)/this->size>0.7){
            this->size*=2;
            obj_with_key<K, V> *new_table = new obj_with_key<K, V>[this->size]{obj_with_key<K, V>{0, 0}};
            for(uint i=0; i<this->size/2; i++){
                if(!(table[i].key == 0 && table[i].value == 0)){
                    uint index = this->hash_function(table[i].key,function);
                    while(new_table[index].key != 0 && new_table[index].value != 0){
                        index = (index+1)%this->size;
                    }
                    new_table[index] = table[i];
                }
            }
            delete[] table;
            table = new_table;
        }
        int index = this->hash_function(key, function);
        obj_with_key<K, V> obj;
        obj.key = key;
        obj.value = value;
        while(!(table[index].key == 0 && table[index].value == 0)){
            index = (index+1)%this->size;
        }
        table[index] = obj;
        this->elements++;
    }
    V get(K key, short function = 2){
        int index = this->hash_function(key, function);
        while(table[index].key != 0 && table[index].value != 0){
            if(table[index].key == key){
                return table[index].value;
            }
            index = (index+1)%this->size;
        }
        throw std::out_of_range("Key not found");
    }
    void remove(K key, short function = 2){
        int index = this->hash_function(key, function);
        while(table[index].key != 0 && table[index].value != 0){
            if(table[index].key == key){
                table[index].key = 0;
                table[index].value = 0;
                this->elements--;
                return;
            }
            index = (index+1)%this->size;
        }
    }
};