#include "task_manager.h"

json Task::toJSON() const {
    return {{"id", id}, {"description", description}, {"completed", completed},
            {"created_at", created_at}, {"updated_at", updated_at}};
}

Task Task::fromJSON(const json& j) {
    return {j["id"], j["description"], j["completed"], j["created_at"], j["updated_at"]};
}

TaskManager::TaskManager() {
    loadTasks();
    nextTaskID = tasks.empty() ? 1 : tasks.back().id + 1;
}

void TaskManager::loadTasks() {
    ifstream file(taskFile);
    if (!file.is_open()) return;

    json data;
    file >> data;
    file.close();

    tasks.clear();
    for (const auto& j : data) {
        tasks.push_back(Task::fromJSON(j));
    }
}

void TaskManager::saveTasks() {
    ofstream file(taskFile);
    json data;
    for (const auto& task : tasks) {
        data.push_back(task.toJSON());
    }
    file << data.dump(4);
    file.close();
}

void TaskManager::logHistory(const string& action, const Task& task) {
    ofstream file(historyFile, ios::app);
    if (file.is_open()) {
        file << "[" << time(nullptr) << "] " << action << " Task ID: " << task.id
             << ", Description: " << task.description << "\n";
    }
    file.close();
}

void TaskManager::addTask(const string& description) {
    Task task{nextTaskID++, description, false, time(nullptr), time(nullptr)};
    tasks.push_back(task);
    saveTasks();
    logHistory("Added", task);
    cout << "Task added successfully!\n";
}

void TaskManager::editTask(int id, const string& newDescription) {
    for (auto& task : tasks) {
        if (task.id == id) {
            task.description = newDescription;
            task.updated_at = time(nullptr);
            saveTasks();
            logHistory("Edited", task);
            cout << "Task updated successfully!\n";
            return;
        }
    }
    cout << "Task not found!\n";
}

void TaskManager::completeTask(int id) {
    for (auto& task : tasks) {
        if (task.id == id) {
            task.completed = true;
            task.updated_at = time(nullptr);
            saveTasks();
            logHistory("Completed", task);
            cout << "Task marked as completed!\n";
            return;
        }
    }
    cout << "Task not found!\n";
}

void TaskManager::listTasks() const {
    if (tasks.empty()) {
        cout << "No tasks available!\n";
        return;
    }
    cout << "Task List:\n";
    for (const auto& task : tasks) {
        cout << "[ID: " << task.id << "] " << task.description
             << " [" << (task.completed ? "Completed" : "Pending") << "]\n";
    }
}

void TaskManager::viewHistory() const {
    ifstream file(historyFile);
    if (!file.is_open()) {
        cout << "No history available!\n";
        return;
    }
    cout << "Task History:\n";
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

int main() {
    TaskManager manager;
    int choice;
    string desc;
    int id;

    while (true) {
        cout << "\nTask Manager CLI\n";
        cout << "1. Add Task\n";
        cout << "2. Edit Task\n";
        cout << "3. Complete Task\n";
        cout << "4. List Tasks\n";
        cout << "5. View History\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter task description: ";
                getline(cin, desc);
                manager.addTask(desc);
                break;
            case 2:
                cout << "Enter task ID to edit: ";
                cin >> id;
                cin.ignore();
                cout << "Enter new description: ";
                getline(cin, desc);
                manager.editTask(id, desc);
                break;
            case 3:
                cout << "Enter task ID to mark as completed: ";
                cin >> id;
                manager.completeTask(id);
                break;
            case 4:
                manager.listTasks();
                break;
            case 5:
                manager.viewHistory();
                break;
            case 6:
                return 0;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }
}

