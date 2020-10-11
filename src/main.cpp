#include<iostream>
#include<functional>
#include<chrono>
#include "../include/threadpool.h"
#include "../include/threadpoolVT.h"
using namespace std;

void test1(){
    thread_pool tp(5);
    
    for(int i = 0; i < 8; i++){
        function<void()> task = [_i = i](){
        cout<<"hello world: "<<_i<<endl;
        };
        tp.execute(task);
    }
}
void test2(){
    thread_pool_VT tp(5);
    
    for(int i = 0; i < 2; i++){
        function<void(int, char)> task = [](int i, char c){
        cout<<"hello world: "<<i<<c<<endl;
        };
        tp.execute(task, i, 'a' + i);
    }
}
int main(){
    test2();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}