#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

using namespace std;

struct Group;

struct User {
    int id;
    string name;
    int age;
    weak_ptr<Group> group; 

    User(int id, const string& name, int age) : id(id), name(name), age(age) {}
};

struct Group {
    int id;
    vector<weak_ptr<User>> users;

    Group(int id) : id(id) {}
};

unordered_map<int, shared_ptr<User>> usersDB;
unordered_map<int, shared_ptr<Group>> groupsDB;

void printUser(const shared_ptr<User>& u) {
    cout << "  [User ID: " << u->id << "] Name: " << u->name << ", Age: " << u->age;
    
    if (shared_ptr<Group> g = u->group.lock()) {
        cout << " | In Group: " << g->id << "\n";
    } else {
        cout << " | No group\n";
    }
}

void printGroup(const shared_ptr<Group>& g) {
    cout << "  [Group ID: " << g->id << "] Users: ";
    bool hasUsers = false;
    
    for (const auto& weak_u : g->users) {
        if (shared_ptr<User> u = weak_u.lock()) {
            cout << u->name << " (ID:" << u->id << ") ";
            hasUsers = true;
        }
    }
    
    if (!hasUsers) cout << "empty";
    cout << "\n";
}

int main() {
    string cmd;
    while (cin >> cmd) {
        if (cmd == "exit") {
            break;
        }
        else if (cmd == "createUser") {
            int id, age;
            string name;
            cin >> id >> name >> age;
            if (usersDB.count(id)) {
                cout << "Error\n";
            } else {
                usersDB[id] = make_shared<User>(id, name, age);
            }
        }
        else if (cmd == "deleteUser") {
            int id;
            cin >> id;
            if (!usersDB.erase(id)) {
                cout << "Error\n";
            }
        }
        else if (cmd == "allUsers") {
            for (const auto& pair : usersDB) {
                printUser(pair.second);
            }
        }
        else if (cmd == "getUser") {
            int id;
            cin >> id;
            if (usersDB.count(id)) {
                printUser(usersDB[id]);
            } else {
                cout << "Error\n";
            }
        }
        else if (cmd == "createGroup") {
            int id;
            cin >> id;
            if (groupsDB.count(id)) {
                cout << "Error\n";
            } else {
                groupsDB[id] = make_shared<Group>(id);
            }
        }
        else if (cmd == "deleteGroup") {
            int id;
            cin >> id;
            if (!groupsDB.erase(id)) {
                cout << "Error\n";
            }
        }
        else if (cmd == "allGroups") {
            for (const auto& pair : groupsDB) {
                printGroup(pair.second);
            }
        }
        else if (cmd == "getGroup") {
            int id;
            cin >> id;
            if (groupsDB.count(id)) {
                printGroup(groupsDB[id]);
            } else {
                cout << "Error\n";
            }
        }
        else if (cmd == "addToGroup") {
            int uId, gId;
            cin >> uId >> gId;
            if (!usersDB.count(uId) || !groupsDB.count(gId)) {
                cout << "Error\n";
            } else {
                auto u = usersDB[uId];
                auto g = groupsDB[gId];
                u->group = g;
                g->users.push_back(u);
            }
        }
        else {
            string discard;
            getline(cin, discard);
        }
    }

    return 0;
}