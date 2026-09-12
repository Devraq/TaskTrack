#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <ctime>
#include "nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

struct Task {
    int id;
    string name;
    string status;
    string created_at;
    string updated_at;
};

string getProgramPath() {
    char buffer[MAX_PATH];

    GetModuleFileNameA(
        NULL,
        buffer,
        MAX_PATH
    );

    string path(buffer);

    size_t pos = path.find_last_of("\\/");

    if (pos != string::npos) {
        return path.substr(0, pos);
    }

    return ".";
}

vector<Task> loadTasks(const string& FILE_PATH) {

    vector<Task> tasks;

    ifstream file(FILE_PATH);

    if (!file.is_open()) {
        return tasks;
    }

    json data;

    file >> data;

    for (auto& item : data) {

        Task task;

        task.id = item["id"];
        task.name = item["name"];
        task.status = item["status"];
        task.created_at = item["created_at"];
        task.updated_at = item["updated_at"];

        tasks.push_back(task);
    }

    file.close();

    return tasks;
}

void saveTasks(
    const string& FILE_PATH,
    const vector<Task>& tasks
) {

    json data = json::array();

    for (const Task& task : tasks) {

        data.push_back({
            {"id", task.id},
            {"name", task.name},
            {"status", task.status},
            {"created_at", task.created_at},
            {"updated_at", task.updated_at}
        });
    }

    ofstream file(FILE_PATH);

    file << data.dump(4);

    file.close();
}

string getCurrentDateTime() {

    time_t now = time(nullptr);

    tm localTime;

    localtime_s(&localTime, &now);

    char buffer[20];

    strftime(
        buffer,
        sizeof(buffer),
        "%Y-%m-%d %H:%M:%S",
        &localTime
    );

    return string(buffer);
}

void createBackup(const string& FILE_PATH, const string& BACKUP_PATH) {
    
	ifstream source(FILE_PATH, ios::binary);
    ofstream backup(BACKUP_PATH, ios::binary);

    backup << source.rdbuf();

    source.close();
    backup.close();
}

void listTasks(const vector<Task>& tasks) {
    if (tasks.empty()) {
        cout << "No tasks found." << endl;
        return;
    }

    for (const Task& task : tasks) {

        string displayStatus;

        if (task.status == "todo")
            displayStatus = "todo";
        else if (task.status == "in-progress")
            displayStatus = "in-progress";
        else if (task.status == "done")
            displayStatus = "done";

        cout << task.id << ". ["
             << displayStatus << "] "
             << task.name << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Please provide a command." << endl;
        return 1;
    }

    string folder = getProgramPath();

    const string FILE_PATH = folder + "\\tasks.json";
    const string BACKUP_PATH = folder + "\\tasks_backup.json";

    string command = argv[1];
      
	vector<Task> tasks = loadTasks(FILE_PATH);

	if (command == "add") {

    if (argc < 3) {
        cout << "Please provide a \"<task>\" ." << endl;
        return 1;
    }
    
    createBackup(FILE_PATH, BACKUP_PATH);
	
	Task newTask;
	
	newTask.id = tasks.size() + 1;	
	newTask.name = argv[2];
	newTask.status = "todo";
	newTask.created_at = getCurrentDateTime();
	newTask.updated_at = newTask.created_at;
	
	tasks.push_back(newTask);
	
	saveTasks(FILE_PATH, tasks);

    cout << "Task added!" << endl;
    listTasks(tasks);
	}	
	
	else if (command == "remove") {

    if (argc < 3) {
        cout << "Please provide a task number." << endl;
        return 1;
    }


    int removeNumber;    
    try {
    	removeNumber = stoi(argv[2]);
	}
	catch (...) {
    	cout << "Task number must be a number." << endl;
    	return 1;
	}

	if (removeNumber < 1 || removeNumber > tasks.size()) {
	    cout << "Task does not exist." << endl;
	    return 1;
	}
	
	createBackup(FILE_PATH, BACKUP_PATH);

	tasks.erase(tasks.begin() + (removeNumber - 1));
	
	for(int i = 0;i < tasks.size();i ++){
		tasks[i].id = i + 1;
	}
	
	saveTasks(FILE_PATH, tasks);

    cout << "Task removed!" << endl;
    listTasks(tasks);

}
	else if (command == "list") {
		listTasks(tasks);
}
	else if (command == "update") {

    if (argc < 4) {
        cout << "Usage: task-cli update <task number> \"<new task>\"" << endl;
        return 1;
    }
    
    int updateNumber;    
    try {
    	updateNumber = stoi(argv[2]);
	}
	catch (...) {
    	cout << "Task number must be a number." << endl;
    	return 1;
	}

    if (updateNumber < 1 || updateNumber > tasks.size()) {
        cout << "Task does not exist." << endl;
        return 1;
    }

   	createBackup(FILE_PATH, BACKUP_PATH);
    tasks[updateNumber - 1].name = argv[3];
    tasks[updateNumber - 1].updated_at = getCurrentDateTime();

    saveTasks(FILE_PATH, tasks);

    cout << "Task updated!" << endl;
    listTasks(tasks);

}
	else if (command == "mark") {

    if (argc < 4) {
        cout << "Usage: task-cli mark <task number> <status>" << endl;
        return 1;
    }
    int markNumber;
    try {
    	markNumber = stoi(argv[2]);
	}
	catch (...) {
    	cout << "Task number must be a number." << endl;
    	return 1;
	}

    string newStatus = argv[3];

    if (newStatus != "todo" &&
        newStatus != "in-progress" &&
        newStatus != "done") {

        cout << "Invalid status." << endl;
        return 1;
    }


    if (markNumber < 1 || markNumber > tasks.size()) {
        cout << "Task does not exist." << endl;
        return 1;
    }
    
    createBackup(FILE_PATH, BACKUP_PATH);

    tasks[markNumber - 1].status = newStatus;
    tasks[markNumber - 1].updated_at = getCurrentDateTime();

    saveTasks(FILE_PATH, tasks);

    cout << "Task marked as " << newStatus << "!" << endl;
    listTasks(tasks);

}
	else if (command == "undo") {

	    ifstream backup(BACKUP_PATH, ios::binary);
	
	    if (!backup.is_open()) {
	        cout << "Nothing to undo." << endl;
	        return 1;
	    }
	
	    ofstream file(FILE_PATH, ios::binary);
	
	    if (!file.is_open()) {
	        cout << "Failed to open Tasks.txt!" << endl;
	        return 1;
	    }
	
	    file << backup.rdbuf();
	
	    backup.close();
	    file.close();
		// undo
		tasks = loadTasks(FILE_PATH);
	    cout << "Last action undone!" << endl;
	    listTasks(tasks);

	}
	else if (command == "help") {
	
	    cout << "TaskTrack - Command Line Task Manager" << endl;
	    cout << endl;
	
	    cout << "Usage:" << endl;
	    cout << "  TaskTrack <command> [arguments]" << endl;
	    cout << endl;
	
	    cout << "Commands:" << endl;
	    cout << endl;
	
	    cout << "  add \"<task>\"" << endl;
		cout << "      Add a new task." << endl;
	    cout << endl;
	
	    cout << "  list" << endl;
	    cout << "      Display all tasks." << endl;
	    cout << endl;
	
	    cout << "  update <task number> \"<new task>\"" << endl;
	    cout << "      Update a task's name." << endl;
	    cout << endl;
	
	    cout << "  remove <task number>" << endl;
	    cout << "      Remove a task." << endl;
	    cout << endl;
	
	    cout << "  mark <task number> <status>" << endl;
	    cout << "      Change a task's status." << endl;
	    cout << "      Status: todo, in-progress, done" << endl;
	    cout << endl;
	
	    cout << "  undo" << endl;
	    cout << "      Undo the last change." << endl;
	    cout << endl;
	
	    cout << "  help" << endl;
	    cout << "      Display this help message." << endl;
	    cout << endl;
	    	    
		cout << "  notes : must have quotation mark for task for command add and update " << endl;
	    cout << endl;

	
	    cout << "Examples:" << endl;
	    cout << endl;
	
	    cout << "  TaskTrack add \"Study Go\"" << endl;
	    cout << "  TaskTrack list" << endl;
	    cout << "  TaskTrack update 1 \"Study Go pointers\"" << endl;
	    cout << "  TaskTrack remove 2" << endl;
	    cout << "  TaskTrack mark 1 done" << endl;
	    cout << "  TaskTrack undo" << endl;
	}
	
	else{
		cout << "Invalid command, please use TaskTrack help for syntax" << endl;
	}
    return 0;
}
