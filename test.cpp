#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

// 定义进程状态
enum ProcessState { READY, RUNNING, FINISHED };

// 进程控制块（PCB）结构
struct PCB {
    int id;                 // 进程标识数
    int priority;           // 优先数（优先数法）/轮转时间片数（轮转法）
    int cpuTime;            // 占用CPU时间片数
    int requiredTime;       // 进程所需时间片数
    ProcessState state;     // 进程状态
};

// 按优先数从高到低排序的比较函数
bool comparePriority(const PCB &a, const PCB &b) {
    return a.priority > b.priority;
}

// 打印进程的状态
void printProcesses(const vector<PCB> &processes, int timeSlice) {
    cout << "时间片：" << timeSlice << endl;
    cout << setw(10) << "ID" 
         << setw(15) << "Priority" 
         << setw(15) << "CPU Time" 
         << setw(15) << "Required Time" 
         << setw(10) << "State" << endl;

    for (const auto &p : processes) {
        cout << setw(10) << p.id
             << setw(15) << p.priority
             << setw(15) << p.cpuTime
             << setw(15) << p.requiredTime
             << setw(10) << (p.state == READY ? "READY" : 
                             (p.state == RUNNING ? "RUNNING" : "FINISHED")) 
             << endl;
    }
    cout << "----------------------------------------" << endl;
}

// 优先数法调度
void priorityScheduling(vector<PCB> &processes) {
    cout << "=== 优先数法调度 ===" << endl;
    int timeSlice = 0;

    // 初始化所有进程为READY状态
    for (auto &p : processes) {
        p.state = READY;
    }

    while (true) {
        // 过滤掉已完成的进程
        vector<PCB*> readyQueue;
        for (auto &p : processes) {
            if (p.state != FINISHED) readyQueue.push_back(&p);
        }

        if (readyQueue.empty()) break; // 所有进程已完成

        // 按优先数从高到低排序
        sort(readyQueue.begin(), readyQueue.end(), [](PCB* a, PCB* b) {
            return a->priority > b->priority;
        });

        // 选取优先级最高的进程运行
        PCB *current = readyQueue.front();
        current->state = RUNNING;

        // 打印当前进程状态
        printProcesses(processes, timeSlice);

        // 执行一个时间片
        current->requiredTime--;
        current->priority -= 3; // 降低优先级
        current->cpuTime++;
        if (current->requiredTime == 0) {
            current->state = FINISHED; // 完成任务
        } else {
            current->state = READY; // 回到就绪状态
        }

        timeSlice++;
    }
}

// 简单轮转法调度
void roundRobinScheduling(vector<PCB> &processes, int timeQuantum) {
    cout << "=== 简单轮转法调度 ===" << endl;
    int timeSlice = 0;

    // 初始化队列
    queue<PCB*> readyQueue;
    for (auto &p : processes) {
        p.state = READY;
        readyQueue.push(&p);
    }

    while (!readyQueue.empty()) {
        PCB *current = readyQueue.front();
        readyQueue.pop();

        // 运行当前进程
        current->state = RUNNING;
        printProcesses(processes, timeSlice);

        // 执行一个时间片
        current->requiredTime--;
        current->cpuTime++;

        if (current->requiredTime == 0) {
            current->state = FINISHED; // 完成任务
        } else {
            current->state = READY; // 回到就绪状态
            readyQueue.push(current); // 放到队列尾部
        }

        timeSlice++;
    }
}

// 主函数
int main() {
    srand(time(0));

    // 初始化进程
    vector<PCB> processes(5);
    for (int i = 0; i < 5; ++i) {
        processes[i] = {
            i + 1,                           // 进程标识数
            rand() % 50 + 1,                 // 随机优先数或轮转时间片数
            0,                               // 初始占用CPU时间片数为0
            rand() % 10 + 1,                 // 随机所需时间片数
            READY                            // 初始状态为READY
        };
    }

    // 打印初始进程状态
    cout << "=== 初始进程状态 ===" << endl;
    printProcesses(processes, 0);

    // 选择调度算法
    int choice;
    cout << "选择调度算法: 1. 优先数法  2. 简单轮转法" << endl;
    cin >> choice;

    if (choice == 1) {
        priorityScheduling(processes);
    } else if (choice == 2) {
        int timeQuantum;
        cout << "输入时间片大小: ";
        cin >> timeQuantum;
        roundRobinScheduling(processes, timeQuantum);
    } else {
        cout << "无效选择！" << endl;
    }

    return 0;
}

