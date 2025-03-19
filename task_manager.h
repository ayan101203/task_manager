#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include "include/json.hpp"

using json = nlohmann::json;
using namespace std;

struct Task {
    int id;
    string description;
    bool completed;
    time_t created_at;
    time_t updated_at;

    json toJSON() const;
    static Task fromJSON(const json& j);
};

class TaskManager {
private:
    vector<Task> tasks;
    string taskFile = "tasks.json";
    string historyFile = "history.log";
    int nextTaskID;

    void loadTasks();
    void saveTasks();
    void logHistory(const string& action, const Task& task);

public:
    TaskManager();
    void addTask(const string& description);
    void editTask(int id, const string& newDescription);
    void completeTask(int id);
    void listTasks() const;
    void viewHistory() const;
};

#endif // TASK_MANAGER_H

