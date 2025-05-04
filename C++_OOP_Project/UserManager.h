//#pragma once
//
//class User {
//	string username;
//	string password;
//public:
//	User() {
//		this->username = "";
//		this->password = "";
//	}
//	User(string username, string password) {
//		SetUsername(username);
//		SetPassword(password);
//
//	}
//
//	string GetUsername() const {
//		return username;
//	}
//	string GetPassword() const {
//		return password;
//	}
//
//	void SetUsername(string username) {
//		if (username.length() >= 8) {
//			this->username = username;
//		}
//		else {
//			assert(!"Username is short...");
//		}
//	}
//
//	void SetPassword(string password) {
//		if (password.length() >= 8) {
//			this->password = password;
//		}
//		else {
//			assert(!"Password is short...");
//		}
//	}
//};
//
//class Admin {
//	string username;
//	string fullname;
//	string password;
//	string id;
//public:
//	Admin() {
//		this->username = "";
//		this->password = "";
//		this->fullname = "";
//		this->id = id;
//	}
//	Admin(string username, string password) {
//		SetUsername(username);
//		SetPassword(password);
//
//	}
//
//	string GetUsername() const {
//		return username;
//	}
//	string GetPassword() const {
//		return password;
//	}
//
//	void SetUsername(string username) {
//		if (username == "admin") {
//			this->username = username;
//		}
//		else {
//			assert(!"Username is wrong...");
//		}
//	}
//
//	void SetPassword(string password) {
//		if (password == "admin") {
//			this->password = password;
//		}
//		else {
//			assert(!"Password is wrong...");
//		}
//	}
//};
//
//class UserManager {
//	DoubleLinkedList<User> users;
//	static int count;
//public:
//	UserManager() {
//		count++;
//	}
//
//	void SignUp(string username, string password) {
//		int index = SearchUser(username);
//		if (index != -1) {
//			assert(!"This username or mail already exists....");
//		}
//		else {
//			User* user = new User(username, password);
//			users.AddEnd(*user);
//		}
//	}
//	void SignIn(string username, string password) {
//		int index = SearchUser(username);
//		if (index == -1) {
//			cout << "This user not found.." << endl;
//		}
//		else {
//			cout << "Welcome" << endl;
//		}
//	}
//	void ChangePassword(string username, string password) {
//		int index = SearchUser(username);
//		if (index == -1) {
//			cout << "This user not found.." << endl;
//		}
//		else {
//			users[index].SetPassword(password);
//		}
//	}
//
//	int SearchUser(string username) {
//		for (size_t i = 0; i < users.Size(); i++)
//		{
//			if (users[i].GetUsername() == username) {
//				return i;
//			}
//		}
//		return -1;
//	}
//
//	~UserManager() {
//		count--;
//	}
//};
//
//int UserManager::count = 0;
//
//
//class AdminManager {
//	DoubleLinkedList<User> users;
//	static int count;
//public:
//	AdminManager() {
//		count++;
//	}
//
//	void SignUp(string username, string password) {
//		int index = SearchUser(username);
//		if (index != -1) {
//			assert(!"This username or mail already exists....");
//		}
//		else {
//			User* user = new User(username, password);
//			users.AddEnd(*user);
//		}
//	}
//	void SignIn(string username, string password, string fullname, string id) {
//		int index = SearchUser(username);
//		if (index == -1) {
//			cout << "This user not found.." << endl;
//		}
//		else {
//			cout << "Welcome" << endl;
//		}
//	}
//	void ChangePassword(string username, string password) {
//		int index = SearchUser(username);
//		if (index == -1) {
//			cout << "This user not found.." << endl;
//		}
//		else {
//			users[index].SetPassword(password);
//		}
//	}
//
//	int SearchUser(string username) {
//		for (size_t i = 0; i < users.Size(); i++)
//		{
//			if (users[i].GetUsername() == username) {
//				return i;
//			}
//		}
//		return -1;
//	}
//
//	~AdminManager() {
//		count--;
//	}
//
//};
//
