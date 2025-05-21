#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;


class User {
public:
    string userId;
    string username;
    string email;
    string phone;
    string groupId;

    User() {}

    User(string id, string name, string email = "", string phone = "")
        : userId(id), username(name), email(email), phone(phone), groupId("") {}

    void print() const {
        cout << "User ID: " << userId
             << ", Username: " << username
             << ", Email: " << email
             << ", Phone: " << phone
             << ", Group ID: " << (groupId.empty() ? "None" : groupId)
             << endl;
    }
};


class Group {
public:
    string groupId;
    vector<string> userIds;

    Group() {}

    Group(string id) : groupId(id) {}

    void print() const {
        cout << "Group ID: " << groupId << ", Users: [";
        for (size_t i = 0; i < userIds.size(); ++i) {
            cout << userIds[i];
            if (i != userIds.size() - 1)
                cout << ", ";
        }
        cout << "]" << endl;
    }
};


class Manager {
private:
    unordered_map<string, User> users;
    unordered_map<string, Group> groups;

public:
    void createUser(const string& userId, const string& username,
                    const string& email = "", const string& phone = "") {
        if (users.count(userId)) {
            cout << "User already exists." << endl;
            return;
        }
        users[userId] = User(userId, username, email, phone);
        cout << "User created." << endl;
    }

    void deleteUser(const string& userId) {
        auto it = users.find(userId);
        if (it == users.end()) {
            cout << "User not found." << endl;
            return;
        }
        string groupId = it->second.groupId;
        if (!groupId.empty() && groups.count(groupId)) {
            auto& userList = groups[groupId].userIds;
            userList.erase(remove(userList.begin(), userList.end(), userId), userList.end());
        }
        users.erase(it);
        cout << "User deleted." << endl;
    }

    void allUsers() const {
        if (users.empty()) {
            cout << "No users found." << endl;
            return;
        }
        for (const auto& [id, user] : users) {
            user.print();
        }
    }

    void getUser(const string& userId) const {
        auto it = users.find(userId);
        if (it != users.end()) {
            it->second.print();
        } else {
            cout << "User not found." << endl;
        }
    }

    void createGroup(const string& groupId) {
        if (groups.count(groupId)) {
            cout << "Group already exists." << endl;
            return;
        }
        groups[groupId] = Group(groupId);
        cout << "Group created." << endl;
    }

    void deleteGroup(const string& groupId) {
        auto it = groups.find(groupId);
        if (it == groups.end()) {
            cout << "Group not found." << endl;
            return;
        }
        for (const string& uid : it->second.userIds) {
            users[uid].groupId = "";
        }
        groups.erase(it);
        cout << "Group deleted." << endl;
    }

    void allGroups() const {
        if (groups.empty()) {
            cout << "No groups found." << endl;
            return;
        }
        for (const auto& [id, group] : groups) {
            group.print();
        }
    }

    void getGroup(const string& groupId) const {
        auto it = groups.find(groupId);
        if (it != groups.end()) {
            it->second.print();
        } else {
            cout << "Group not found." << endl;
        }
    }

    void addUserToGroup(const string& userId, const string& groupId) {
        if (!users.count(userId)) {
            cout << "User not found." << endl;
            return;
        }
        if (!groups.count(groupId)) {
            cout << "Group not found." << endl;
            return;
        }
    
        // Удаляем из старой группы, если была
        string& currentGroupId = users[userId].groupId;
        if (!currentGroupId.empty() && groups.count(currentGroupId)) {
            auto& oldUserList = groups[currentGroupId].userIds;
            oldUserList.erase(remove(oldUserList.begin(), oldUserList.end(), userId), oldUserList.end());
        }
    
        // Добавляем в новую группу
        users[userId].groupId = groupId;
        groups[groupId].userIds.push_back(userId);
    
        cout << "User " << userId << " added to group " << groupId << "." << endl;
    }
};

int main() {
    Manager manager;
    string command;

    cout << "User-Group Manager (type 'exit' to quit)" << endl;

    while (true) {
        cout << "> ";
        getline(cin, command);
        if (command == "exit") break;

        istringstream iss(command);
        string cmd;
        iss >> cmd;

        if (cmd == "createUser") {
            string id, name, email, phone;
            iss >> id >> name >> email >> phone;
            manager.createUser(id, name, email, phone);
        } else if (cmd == "deleteUser") {
            string id;
            iss >> id;
            manager.deleteUser(id);
        } else if (cmd == "allUsers") {
            manager.allUsers();
        } else if (cmd == "getUser") {
            string id;
            iss >> id;
            manager.getUser(id);
        } else if (cmd == "createGroup") {
            string gid;
            iss >> gid;
            manager.createGroup(gid);
        } else if (cmd == "deleteGroup") {
            string gid;
            iss >> gid;
            manager.deleteGroup(gid);
        } else if (cmd == "allGroups") {
            manager.allGroups();
        } else if (cmd == "getGroup") {
            string gid;
            iss >> gid;
            manager.getGroup(gid);
        } else if (cmd == "addUserToGroup") {
            string uid, gid;
            iss >> uid >> gid;
            manager.addUserToGroup(uid, gid);
        }
        else {
            cout << "Unknown command." << endl;
        }
    }

    return 0;
}