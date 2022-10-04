#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>

using namespace std;

chrono::milliseconds wait_interval(3);


void wait_for_fork(mutex &first_fork) {
    while (true) {
        bool took_first_fork = first_fork.try_lock();
        if (!took_first_fork) {
            this_thread::sleep_for(wait_interval);
            return;
        }
    }
}

[[noreturn]] void philosopher_thread(
        int philosopher_index,
        int first_fork_index, mutex &first_fork,
        int second_fork_index, mutex &second_fork) {
    while (true) {
        printf("Philosopher %d thinks\n", philosopher_index + 1);

        wait_for_fork(first_fork);

        printf("Philosopher %d took first fork %d\n", philosopher_index + 1, first_fork_index);

        wait_for_fork(second_fork);

        printf("Philosopher %d took second fork %d\n", philosopher_index + 1, second_fork_index);

        printf("Philosopher %d eats\n", philosopher_index + 1);
        this_thread::sleep_for(wait_interval);

        second_fork.unlock();
        printf("Philosopher %d put down second fork %d\n", philosopher_index + 1, second_fork_index);

        first_fork.unlock();
        printf("Philosopher %d put down first fork %d\n", philosopher_index + 1, first_fork_index);
    }
}

int main() {
    cout << "Enter the number of philosophers (>= 2):" << endl;
    int philosophers_count;
    cin >> philosophers_count;

    vector<mutex> forks(philosophers_count);
    vector<thread> philosophers;

    for (int philosopher_index = 0; philosopher_index < philosophers_count; philosopher_index++) {
        int left_fork_index = philosopher_index;
        int right_fork_index = left_fork_index + 1;

        if (philosopher_index == philosophers_count - 1) {
            left_fork_index = 0;
            right_fork_index = philosophers_count - 1;
        }
        philosophers.emplace_back(philosopher_thread,
                                  philosopher_index,
                                  left_fork_index, ref(forks.at(left_fork_index)),
                                  right_fork_index, ref(forks.at(right_fork_index))

        );
    }

    for (thread &philosopher: philosophers) {
        philosopher.join();
    }
}

// из https://ru.wikipedia.org/wiki/%D0%97%D0%B0%D0%B4%D0%B0%D1%87%D0%B0_%D0%BE%D0%B1_%D0%BE%D0%B1%D0%B5%D0%B4%D0%B0%D1%8E%D1%89%D0%B8%D1%85_%D1%84%D0%B8%D0%BB%D0%BE%D1%81%D0%BE%D1%84%D0%B0%D1%85
//
// Решение достигается путём присвоения частичного порядка ресурсам (в данном случае вилкам) и установления соглашения,
// что ресурсы запрашиваются в указанном порядке, а возвращаются в обратном порядке.
// Кроме того, не должно быть двух ресурсов, не связанных порядком, используемых одной рабочей единицей.
//
// Пусть ресурсы (вилки) будут пронумерованы от 1 до 5, и каждая рабочая единица (философ)
// всегда берёт сначала вилку с наименьшим номером, а потом вилку с наибольшим номером из двух доступных.
// Далее, философ кладёт сначала вилку с бо́льшим номером, потом — с меньшим. В этом случае,
// если четыре из пяти философов одновременно возьмут вилку с наименьшим номером, на столе останется вилка
// с наибольшим возможным номером. Таким образом, пятый философ не сможет взять ни одной вилки.
// Более того, только один философ будет иметь доступ к вилке с наибольшим номером,
// так что он сможет есть двумя вилками. Когда он закончит использовать вилки,
// он в первую очередь положит на стол вилку с бо́льшим номером, потом — с меньшим,
// тем самым позволив другому философу взять недостающую вилку и приступить к еде.

