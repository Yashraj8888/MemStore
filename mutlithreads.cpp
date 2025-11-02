#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex mtx;
condition_variable cv;
bool t1_turn = true; // Start with thread 1
bool t2_turn = false;

void target1() {
    for(int i = 0; i < 5; i++) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, []{ return t1_turn; }); // wait until it's t1 turn

        cout << "Target 1 : " << i << endl;

        t1_turn = false;
        t2_turn = true;
        cv.notify_all();
    }
}

void target2() {
    for(int i = 0; i < 5; i++) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, []{ return t2_turn; }); // wait until it's t2 turn

        cout << "Target 2 : " << i << endl;

        t1_turn = true;
        t2_turn = false;
        cv.notify_all();
    }
}

int main() {
    thread t1(target1);
    thread t2(target2);

    t1.join();
    t2.join();

    cout << "Done\n";
    return 0;
}
