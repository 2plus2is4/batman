#include <iostream>
#include <pthread.h>
#include <bits/stdc++.h>

pthread_mutex_t *lock;

int bats = 0;

struct bat {
    int num;
    char *dir;
};

struct batman {
private:
    pthread_cond_t *NorthQueue{}, *EastQueue{}, *SouthQueue{},
            *WestQueue{}, *NorthFirst{}, *EastFirst{}, *SouthFirst{}, *WestFirst{};
public:
    batman(){
        pthread_mutex_init(lock,nullptr);
        pthread_cond_init(NorthQueue,nullptr);
        pthread_cond_init(WestQueue,nullptr);
        pthread_cond_init(SouthQueue,nullptr);
        pthread_cond_init(EastQueue,nullptr);
        pthread_cond_init(NorthFirst,nullptr);
        pthread_cond_init(EastFirst,nullptr);
        pthread_cond_init(SouthFirst,nullptr);
        pthread_cond_init(WestFirst,nullptr);
    }

    static void arrive(struct bat b) {
        printf("BAT %d from %s arrives at crossing\n", b.num, b.dir);
        // code to check traffic in line, use counters, condition variables etc.
    }

    static void cross(struct bat b) {
        printf("BAT %d from %s crossing\n", b.num, b.dir);
//        sleep(1); // it takes one second for a BAT to cross
    }

    static void leave(struct bat b) {
        printf("BAT %d from %s leaving crossing\n", b.num, b.dir);
         // code to check traffic, use counters, condition variables etc.
    }

    static void check()
    {
        // the manager checks for deadlock and resolves it
    }

};

using namespace std;

vector<string> splitInput(string str);

int main() {
    string input;
    cin >> input;
    auto batslist = splitInput(input);

    struct batman robin;

    return 0;
}



vector<string> splitInput(string str) {

    vector<string> args;
    string word = "";
    for (auto x : str) {
        if (x == ' ') {
            args.push_back(word);
            word = "";
        } else {
            word = word + x;
        }
    }
    args.push_back(word);
    return args;
}