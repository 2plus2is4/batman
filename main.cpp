#include <iostream>
#include <pthread.h>
#include <bits/stdc++.h>

pthread_mutex_t *lock;

int bats = 0;

struct bat {
    int num;
    int dir;
    char des;
};

struct batman {
private:
    pthread_cond_t *Queue[4], *First[4];
    pthread_mutex_t batman_lock;
    int road[4];
    int margin[4];

    bool arewedead() {
        return road[0] > 0 && road[1] > 0 && road[2] > 0 && road[3] > 0;
    }


public:
    void init() {

        pthread_mutex_init(&batman_lock, nullptr);

        for (int i = 0; i < 4; ++i) {
            pthread_cond_init(Queue[i], nullptr);
            pthread_cond_init(First[i], nullptr);
            road[i] = 0;
            margin[i] = 5;
        }
    }

    void arrive(struct bat b) {

        // code to check traffic in line, use counters, condition variables etc.

        //if someone is at crossing, wait in line
        pthread_mutex_lock(&batman_lock);
        if (road[b.dir]++ > 0) {
            pthread_cond_wait(Queue[b.dir], &batman_lock);
            printf("road at %c got %d bats", b.des, road[b.dir]);
        }

        pthread_mutex_unlock(&batman_lock);

        printf("BAT %d from %c arrives at crossing\n", b.num, b.des);
    }

    void cross(struct bat b) {

        // code to check traffic from the right, use counters, condition variables etc


        pthread_mutex_lock(&batman_lock);

        //Wait for right first
        if (road[(b.dir - 1) % 4] > 0)
            pthread_cond_wait(First[(b.dir - 1) % 4], &batman_lock);

//        sleep(1); // it takes one second for a BAT to cross

        printf("BAT %d from %c crossing is crossing now\n", b.num, b.des);
        leave(b);
        pthread_mutex_unlock(&batman_lock);
    }

    void leave(struct bat b) {

        // code to check traffic, use counters, condition variables etc.
        pthread_mutex_lock(&batman_lock);
        if (margin[b.dir] > 0) {
            margin[b.dir]--;
            pthread_cond_signal(Queue[b.dir]);
        } else {
            margin[b.dir] = 5;
        }
        pthread_mutex_unlock(&batman_lock);

        printf("BAT %d from %c leaving crossing\n", b.num, b.des);
    }

    void check() {
        // the manager checks for deadlock and resolves it
        do {
            for (int i = 0; i < 1000000; ++i);
            printf("checking\n");
            if (road[0] > 0 && road[1] > 0 && road[2] > 0 && road[3] > 0) {
                printf("DEADLOCK: BAT jam detected, signalling North to go\n");
                pthread_cond_signal(First[3]);
            }
        } while (road[0] > 0 || road[1] > 0 || road[2] > 0 || road[3] > 0);
        pthread_exit(NULL);
    }

};

using namespace std;

struct bat *splitInput(struct bat *batslist, const string &str);

void *start(void *car);

void *check(void *);

void init(bat *bats, int size);


struct batman batman;

int main() {

    string input;
    cin >> input;

    int num = input.length();
    struct bat batslist[num];
    splitInput(batslist, input);

    batman.init();
    init(batslist, num);


    return 0;
}

void init(bat *bats, int size) {

    pthread_t threads[size];
    pthread_t checker_thread;
    pthread_attr_t attr;
    void *status;
    int rc;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    rc = pthread_create(&checker_thread, &attr, check,
                        nullptr);
    if (rc) {
        cout << "Error:unable to create thread, " << rc << endl;
        exit(-1);
    }

    for (int i = 0; i < size; ++i) {
        rc = pthread_create(&threads[i], &attr, start,
                            (void *) &bats[i]);
        if (rc) {
            cout << "Error:unable to create thread, " << rc << endl;
            exit(-1);
        }
    }
    for (int j = 0; j < size; ++j) {
        rc = pthread_join(threads[j], &status);
        if (rc) {
            cout << "Error:unable to join," << rc << endl;
            exit(-1);
        }
    }

    rc = pthread_join(checker_thread, &status);
    if (rc) {
        cout << "Error:unable to join," << rc << endl;
        exit(-1);
    }


}

void *start(void *car) {

    struct bat *bat;
    bat = (struct bat *) (car);
    batman.arrive(*bat);
    batman.cross(*bat);
//    batman.leave(*bat);
    pthread_exit(NULL);
}

void *check(void *) {
    batman.check();
}


struct bat *splitInput(struct bat *batslist, const string &str) {

    for (int i = 0; i < str.length(); i++) {
        struct bat temp{};
        temp.num = i;
        temp.des = str[i];
        if (str[i] == 'n')
            temp.dir = 0;

        if (str[i] == 'e')
            temp.dir = 1;

        if (str[i] == 's')
            temp.dir = 2;

        if (str[i] == 'w')
            temp.dir = 3;
        batslist[i] = temp;
    }

    return batslist;
}