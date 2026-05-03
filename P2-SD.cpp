#include "P2-SD.hpp"
#define num_tests 10000
int main(){
    priority_queue_array<int, int> pqa[num_tests];
    priority_queue_heap<int, int> pqh[num_tests];
    priority_queue_linked_list<int, int> pqll[num_tests];
    for(int i = 0; i < num_tests; i++){
        pqa[i] = priority_queue_array<int, int>();
        pqh[i] = priority_queue_heap<int, int>();
        pqll[i] = priority_queue_linked_list<int, int>();
    }
    for(int i=0; i<num_tests; i++){
        for(int j=0; j<1000; j++){
            pqa[i].insert(j,j);
            pqh[i].insert(j,j);
            //pqll[i].insert(j,j);
        }
        pqll[i].insert(0,0);
    }
    
    {
    tic
    for(int i=0; i<num_tests; i++){
        pqa[i].insert(500, 500);
    }
    toc
    printf("Time of 1000 operation for insert in priority_queue_array: %d microseconds\n", time);
    }
    {
    tic
    for(int i=0; i<num_tests; i++){
        pqh[i].insert(500, 500);
    }
    toc
    printf("Time of 1000 operation for insert in priority_queue_heap: %d microseconds\n", time);
    }
    {
    tic
    for(int i=0; i<num_tests; i++){
        pqll[i].insert(1001, 1001);
    }
    toc
    printf("Time of 1000 operation for insert in priority_queue_linked_list: %d microseconds\n", time);
    }
    {
    tic
    for(int i=0; i<num_tests; i++){
        pqa[i].extract();
    }
    toc
    printf("Time of 1000 operation for extract in priority_queue_array: %d microseconds\n", time);
    }
    {
    tic
    for(int i=0; i<num_tests; i++){
        pqh[i].extract();
    }
    toc
    printf("Time of 1000 operation for extract in priority_queue_heap: %d microseconds\n", time);}
    {
    tic
    for(int i=0; i<num_tests; i++){
        pqll[i].extract();
    }
    toc
    printf("Time of 1000 operation for extract in priority_queue_linked_list: %d microseconds\n", time);
    }
    /*
   int size;
   int top;
    {
    tic
    for(int i=0; i<num_tests; i++){
        size = pqa[i].size();
    }
    toc
    printf("Time of 1000 operation for size in priority_queue_array: %d microseconds\n", time);
    }
    {
    tic
    for(int i=0; i<num_tests; i++){
        size = pqh[i].size();
    }
    toc
    printf("Time of 1000 operation for size in priority_queue_heap: %d microseconds\n", time);}
    {
    tic
    for(int i=0; i<num_tests; i++){
        size = pqll[i].size();
    }
    toc
    printf("Time of 1000 operation for size in priority_queue_linked_list: %d microseconds\n", time);
    }
    {
    tic
    for(int i=0; i<num_tests; i++){
        top = pqa[i].top().value;
    }
    toc
    printf("Time of 1000 operation for top in priority_queue_array: %d microseconds\n", time);
    }
    {
    tic
    for(int i=0; i<num_tests; i++){
        top = pqh[i].top().value;
    }
    toc
    printf("Time of 1000 operation for top in priority_queue_heap: %d microseconds\n", time);}
    {
    tic
    for(int i=0; i<num_tests; i++){
        top = pqll[i].top().value;
    }
    toc
    printf("Time of 1000 operation for top in priority_queue_linked_list: %d microseconds\n", time);
    }
    */
    printf("Sizes of priority queues:\n");
    printf("priority_queue_array: %d\n", pqa[0].size());
    printf("priority_queue_heap: %d\n", pqh[0].size());
    printf("priority_queue_linked_list: %d\n", pqll[0].size());
    {
    tic
    for(int i=0; i<num_tests; i++){
        pqa[i].increase_key(498+i%2);
    }
    toc
    printf("Time of 1000 operation for increase in priority_queue_array: %d microseconds\n", time);
    }
    {
    tic
    for(int i=0; i<num_tests; i++){
        pqh[i].increase_key(498+i%20);
    }
    toc
    printf("Time of 1000 operation for increase in priority_queue_heap: %d microseconds\n", time);}
    {
    tic
    for(int i=0; i<num_tests; i++){
        pqll[i].increase_key(498+i%2);
    }
    toc
    printf("Time of 1000 operation for increase in priority_queue_linked_list: %d microseconds\n", time);
    }
    return 0;
}