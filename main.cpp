# if 0 // tehtävä 1

# include <iostream>
# include <vector>
# include <thread>

// palauta vektorin osasumma tietyltä väliltä
long long partialSum(const std::vector<int>& vec, size_t start, size_t end) {
    long long sum = 0;
    for (size_t i = start; i < end; ++i) {
        sum += vec[i];
    }
    return sum;
}

int main (void) {

    // säikeiden ja elementtien lukumäärät
    const int N_threads = 4;
    std::thread threads[N_threads];
    long long partialSums[N_threads] = {0};

    const int N_elements = 1000000; // miljoona

    // esimerkkivektori jossa miljoona lukua
    std::vector<int> numbers(N_elements);

    // täytä seuraavasti; koko summa on näin ollen (1000001/2)*1000000 = 5 * 10^11
    for (int i = 0; i < N_elements; ++i) {
        numbers[i] = i + 1;
    }

    // jaa osiin määrittelemällä range
    size_t chunkSize = N_elements / N_threads;
    for (int i = 0; i < N_threads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == N_threads - 1) ? N_elements : start + chunkSize;
        threads[i] = std::thread([&numbers, start, end, &partialSums, i]() {
            partialSums[i] = partialSum(numbers, start, end);
        });
    }

    // odotetaan että kaikki suoriutuu
    for (int i = 0; i < N_threads; ++i) {
        threads[i].join();
    }

    // Kokonaissumma
    long long totalSum = 0;
    for (int i = 0; i < N_threads; ++i) {
        totalSum += partialSums[i];
    }

    // tulosta tarkistaaksesi
    std::cout << "Total sum: " << totalSum << std::endl;

    return 0;
}

# endif // tehtävä 1

# if 0 // tehtävä 2

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

class BankAccount {
    mutable std::mutex mtx;
    double balance;

// mutexia käytetään std::lock_guard:n avulla, eksplisiittisten lock()- ja unlock()-kutsujen sijasta.
public:
    BankAccount() : balance(0) {}

    // talletus
    void deposit(double amount) {
        std::lock_guard<std::mutex> lock(mtx);
        balance += amount;
    }

    // nosto
    bool withdraw(double amount) {
        std::lock_guard<std::mutex> lock(mtx);
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }

    // saldo
    double getBalance() const {
        std::lock_guard<std::mutex> lock(mtx);
        return balance;
    }
};

// tapahtumia tuhansittain
void depositMoney(BankAccount& account) {
    for(int i = 0; i < 10000; ++i) {
        account.deposit(1);
    }
}

// tapahtumia tuhansittain
void withdrawMoney(BankAccount& account) {
    for(int i = 0; i < 10000; ++i) {
        account.withdraw(1);
    }
}

int main() {
    BankAccount account;
    std::thread t1(depositMoney, std::ref(account));
    std::thread t2(withdrawMoney, std::ref(account));

    t1.join();
    t2.join();

    // tulosta lopullinen saldo, jonka pitäisi olla nolla
    std::cout << "Final balance: " << account.getBalance() << std::endl;

    return 0;
}

# endif // tehtävä 2

# if 0 // tehtävä 3

# include <iostream>
# include <vector>
# include <thread>
# include <chrono>

class Game_Task {
public:
    virtual void perform() = 0;
};

class Task_1 : public Game_Task {
public:
    void perform() override {
        // mallinnetaan ajan käyttämistä johonkin tehtävään
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Task 1 completed." << std::endl;
    }
};

class Task_2 : public Game_Task {
public:
    void perform() override {
        // mallinnetaan ajan käyttämistä johonkin tehtävään
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Task 2 completed." << std::endl;
    }
};

class Task_3 : public Game_Task {
public:
    void perform() override {
        // mallinnetaan ajan käyttämistä johonkin tehtävään
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Task 3 completed." << std::endl;
    }
};

int main() {
    std::vector<Game_Task*> tasks;
    tasks.push_back(new Task_1());
    tasks.push_back(new Task_2());
    tasks.push_back(new Task_3());

    // ohjelmallinen tapa selvittää, kuinka montaa samanaikaista säiettä laitteistosi pystyy
    // suorittamaan fyysisesti; käytä rinnakkaistamiseen yksi säie vähemmän.
    unsigned int hardware_threads = std::thread::hardware_concurrency();
    unsigned int num_threads = (hardware_threads > 1) ? hardware_threads - 1 : 1;

    // Mitataan aika peräkkäiselle suoritukselle
    auto start_serial = std::chrono::high_resolution_clock::now();
    for (auto task : tasks) {
        task->perform();
    }
    auto end_serial = std::chrono::high_resolution_clock::now();
    auto duration_serial = std::chrono::duration_cast<std::chrono::milliseconds>(end_serial - start_serial);
    std::cout << "Peräkkäinen suoritusaika: " << duration_serial.count() << " ms" << std::endl;

    // Mitataan aika rinnakkaiselle suoritukselle
    auto start_parallel = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;

    for (int i = 0; i < std::min(num_threads, static_cast<unsigned int>(tasks.size())); ++i) {
        threads.push_back(std::thread(&Game_Task::perform, tasks[i]));
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
    auto end_parallel = std::chrono::high_resolution_clock::now();
    auto duration_parallel = std::chrono::duration_cast<std::chrono::milliseconds>(end_parallel - start_parallel);
    std::cout << "Suoritusaika rinnakkain: " << duration_parallel.count() << " ms" << std::endl;

    // siivotaan
    for (auto task : tasks) {
        delete task;
    }

    return 0;
}

# endif // tehtävä 3

# if 0 // tehtävä 4

#include <iostream>
#include <vector>
#include <numeric>
#include <future>

// Palauta vektorin osasumma tietyltä väliltä
long long partialSum(const std::vector<int>& vec, size_t start, size_t end) {
    return std::accumulate(vec.begin() + start, vec.begin() + end, 0LL);
}

int main() {
    const int N_threads = 4;
    std::vector<std::future<long long>> futures;

    const int N_elements = 1000000; // miljoona

    // Esimerkkivektori jossa miljoona lukua
    std::vector<int> numbers(N_elements);
    for (int i = 0; i < N_elements; ++i) {
        numbers[i] = i + 1;
    }

    // Jaa osiin määrittelemällä range
    size_t chunkSize = N_elements / N_threads;
    for (int i = 0; i < N_threads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == N_threads - 1) ? N_elements : start + chunkSize;
        futures.emplace_back(std::async(std::launch::async, partialSum, std::ref(numbers), start, end));
    }

    // Kokonaissumma
    long long totalSum = 0;
    for (auto& future : futures) {
        totalSum += future.get();
    }

    std::cout << "Total sum: " << totalSum << std::endl;

    return 0;
}

# endif // tehtävä 4

# if 0 // tehtävä 5

#include <vector>
#include <iostream>
#include <numeric>
#include <execution>
#include <chrono>

int main() {
    const size_t N = 10000000; // 10 miljoonaa alkiota
    std::vector<int> vec(N);

    // Alustetaan vektori arvoilla 0...N-1
    std::iota(vec.begin(), vec.end(), 0);

    auto measureExecution = [&](auto&& policy, const std::string& policyName) {
        auto start = std::chrono::high_resolution_clock::now();

        std::for_each(policy, vec.begin(), vec.end(), [](int& n) { ++n; });

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        std::cout << "Execution time with " << policyName << ": " << elapsed.count() << " ms\n";
    };

    // Sekventiaalinen suoritus
    measureExecution(std::execution::seq, "seq");

    // Palauta
    std::iota(vec.begin(), vec.end(), 0);

    // Rinnakkainen suoritus
    measureExecution(std::execution::par, "par");

    // Palauta
    std::iota(vec.begin(), vec.end(), 0);

    // Rinnakkainen ja vektorisoitu suoritus
    measureExecution(std::execution::par_unseq, "par_unseq");

    return 0;
}

# endif // tehtävä 5

# if 0 // tehtävä 6

#include <iostream>
#include <vector>
#include <future>
#include <chrono>
#include <thread>

// samoin kuin kolmosessa
class Game_Task {
public:
    virtual void perform() = 0;
    virtual ~Game_Task() = default;
};

class Task_1 : public Game_Task {
public:
    void perform() override {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Task 1 completed." << std::endl;
    }
};

class Task_2 : public Game_Task {
public:
    void perform() override {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Task 2 completed." << std::endl;
    }
};

class Task_3 : public Game_Task {
public:
    void perform() override {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Task 3 completed." << std::endl;
    }
};

int main() {
    std::vector<Game_Task*> tasks = {new Task_1(), new Task_2(), new Task_3()};
    std::vector<std::future<void>> futures;

    auto start_parallel = std::chrono::high_resolution_clock::now();

    for (auto& task : tasks) {
        // Käynnistetään jokainen tehtävä asynkronisesti
        futures.emplace_back(std::async(std::launch::async, [&task](){
            task->perform();
        }));
    }

    // Odota
    for (auto& future : futures) {
        future.wait();
    }

    auto end_parallel = std::chrono::high_resolution_clock::now();
    auto duration_parallel = std::chrono::duration_cast<std::chrono::milliseconds>(end_parallel - start_parallel);
    std::cout << "Rinnakkainen suoritusaika: " << duration_parallel.count() << " ms" << std::endl;

    for (auto task : tasks) {
        delete task;
    }

    return 0;
}

# endif // tehtävä 6

# if 0 // tehtävä 7 - miniprojekti

#include <iostream>
#include <memory>
#include "Game_Task.h"
#include "TaskQueue.h"

class Task_1 : public Game_Task {
public:
    void perform() override {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Task 1 completed." << std::endl;
    }
};

class Task_2 : public Game_Task {
public:
    void perform() override {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Task 2 completed." << std::endl;
    }
};

class Task_3 : public Game_Task {
public:
    void perform() override {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Task 3 completed." << std::endl;
    }
};

int main() {
    // kolme työsuorittajasäiettä
    TaskQueue queue(3);

    // Lisätään tehtäviä jonoon
    queue.addJob(std::make_shared<Task_1>());
    queue.addJob(std::make_shared<Task_2>());
    queue.addJob(std::make_shared<Task_3>());

    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}

# endif // tehtävä 7 - miniprojekti

# if 0 // tehtävät 8-11 - miniprojekti 2

#include <iostream>
#include <memory>
#include "Simple.h"
#include "Log_Ptr.h"

// Välitä shared_ptr parametrina johonkin funktioon, palauta shared_ptr
// funktion paluuarvona ja tee shared_ptr:ien välisiä sijoituksia
std::shared_ptr<Simple> processSharedPtr(std::shared_ptr<Simple> ptr) {
    std::cout << "... processSharedPtr ..." << std::endl;

    return ptr;
}

int main() {
    // Käytetään Log_Ptr:ää int-tyyppiselle oliolle
    Log_Ptr<int> int_ptr(new int(42));

    // Käytetään Log_Ptr:ää double-tyyppiselle oliolle
    Log_Ptr<double> double_ptr(new double(3.14));

    // Käytetään Log_Ptr:ää string-tyyppiselle oliolle
    Log_Ptr<std::string> string_ptr(new std::string("Hello, World!"));

    // shared_ptr olio, viitelaskuri = 1
    std::shared_ptr<Simple> ptr1 = std::make_shared<Simple>();

    // Välitetään parametrina funktioon, viitelaskuri + 1 = 2
    std::shared_ptr<Simple> ptr2 = processSharedPtr(ptr1);

    // Tehdään sijoitus shared_ptr:ien välillä, viitelaskuri edelleen 2
    // molemmat osoittavat samaan olioon
    ptr2 = ptr1;

    // tuhotaan automaattisesti
    return 0;
}

# endif

# if 1 // tehtävä 12

#include <vector>
#include <algorithm>
#include <iostream>

void reduceHP(int& hp) {
    if (hp <= 100) {
        hp = 0;
    } else {
        hp -= 100;
    }
}

class ReduceHPFunctor {
public:
    void operator()(int& hp) const {
        if (hp <= 100) {
            hp = 0;
        } else {
            hp -= 100;
        }
    }
};

auto reduceHPLambda = [](int& hp) {
    if (hp <= 100) {
        hp = 0;
    } else {
        hp -= 100;
    }
};

int main() {
    std::vector<int> enemiesHP = {150, 250, 50, 120, 500};

    // a. Erillisenä funktiona
    std::for_each(enemiesHP.begin(), enemiesHP.end(), reduceHP);

    // b. Funktio-objektina
    // std::for_each(enemiesHP.begin(), enemiesHP.end(), ReduceHPFunctor());

    /* // c. Lambda-funktiona
    std::for_each(enemiesHP.begin(), enemiesHP.end(), [](int& hp) {
        if (hp <= 100) {
            hp = 0;
        } else {
            hp -= 100;
        }
    }); */

    // d. Nimettynä lambda-funktiona
    // std::for_each(enemiesHP.begin(), enemiesHP.end(), reduceHPLambda);

    // Järjestetään lopuksi
    std::sort(enemiesHP.begin(), enemiesHP.end(), [](int a, int b) { return a > b; });

    // Tulostetaan testausta varten
    for (int hp : enemiesHP) {
        std::cout << hp << " ";
    }
    std::cout << std::endl;

    return 0;
}

# endif // tehtävä 12