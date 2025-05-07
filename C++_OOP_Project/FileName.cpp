#include<iostream>
#include<cassert>
#include<string>
#include"List.h"
#include<sstream>
//#include"UserManager.h"
#include<fstream> 
using namespace std;

//file system food

static string trim2(const string& str) {
	size_t first = str.find_first_not_of(" \t");
	size_t last = str.find_last_not_of(" \t");
	return (first == string::npos) ? "" : str.substr(first, last - first + 1);
}
//file system ingredients
void writetofileIngredients(string name, double quantity, double price) {
	ofstream fs("ingredients.txt", ios::app);
	if (fs.is_open()) {
		fs << name << " | " << quantity << " | " << price << " | " << endl;
		fs.close();
	}
	else {
		throw runtime_error("file couldn't open!!!");
	}
}


//file system user card
void writetofileCard(string name, int count, double price) {
	ofstream fs("card.txt", ios::app);
	if (fs.is_open()) {
		fs << name << " | " << count << " | " << price << " | " << endl;
		fs.close();
	}
	else {
		throw runtime_error("file couldn't open!!!");
	}
}

void readfromfileCard() {
	ifstream fs("card.txt");
	if (fs.is_open()) {
		string line;
		while (getline(fs, line)) {
			cout << line << endl;
		}
		fs.close();
	}
	else {
		throw runtime_error("file couldn't open!!!");
	}
}

//restaurant manegment

class Budget { //restoranin budcesi
	double balance;

public:
	Budget(double initialAmount = 0) : balance(initialAmount) {}; //default constructoru

	double GetBalance() const {
		return balance;
	}


	//eger budce azalsa ve pul elave edilmeli olsa
	bool AddFunds(double amount) {
		if (amount > 0) {
			balance += amount;
			cout << amount << " AZN added to budget.\n";
			return true;
		}
		else {
			cout << "invalid input.\n";
			return false;
		}
	}

	//admin stocka ingredient elave etdikde stockdan pul cixmasi ucun
	bool SubtractFunds(double amount) {
		if (amount > 0 && balance >= amount) {
			balance -= amount;
			cout << amount << " AZN subtracted from budget.\n";
			return true;
		}
		else {
			cout << "Not enough budget!\n";
			return false;
		}
	}

	//budceye baxmaq
	void ShowBalance() const {
		cout << "Current budget: " << balance << " AZN\n";
	}

	
};

class Ingredient {
	string name;
	double quantity; //gramla olacaq ingredientlerimiz
	double priceGram; //her gram ucun qiymet
public:
	Ingredient() {};

	Ingredient(string name, double quantity, double priceGram) {
		this->name = name;
		this->quantity = quantity;
		this->priceGram = priceGram;
	}


	//getter
	string GetName() const{
		return name;
	}
	double GetQuantity() const{
		return quantity;
	}
	double getTotalPrice() const {
		return quantity * priceGram;
	}
	double GetPricePerGram() const {  
		return priceGram;
	}

	//methods
	void DecreaseQuantity(double amount) { //ingredientin daxil edilecek qederi
		if (quantity >= amount) {
			quantity -= amount;
		}
		else {
			cout << "There are not enough " << name << " in stock!\n";
		}
	}

	void IncreaseQuantity(double amount, double newPricePerGram) {
		quantity += amount;

		//ingredient elave etdikde yeni qiymet olmasi yeni artmasi
		priceGram = newPricePerGram;
	}

};


class Food {
private:
	string name;
	string descp;
	double price;
	int    qty;
	DoubleLinkedList<Ingredient> ingredients;

public:
	Food() {}
	Food(const string& name, const string& descp, double price, int qty)
		: name(name), descp(descp), price(price), qty(qty) {}

	string GetName() const { return name; }
	string GetDescp() const { return descp; }
	double GetPrice() const { return price; }
	int GetQty()const {
		return qty;
	}

	DoubleLinkedList<Ingredient>& GetIngredients() { return ingredients; }
	void AddIngredient(const Ingredient& ing) { ingredients.AddEnd(ing); }

	// now const-qualified
	void ShowInfo() const {
		cout << "Food name: " << name << endl;
		cout << "Description: " << descp << endl;
		cout << "Price: " << price << " AZN" << endl;
		cout << "Ingredients:" << endl;
		for (int i = 0; i < ingredients.Size(); ++i) {
			const Ingredient& ing = ingredients[i];
			cout << "- " << ing.GetName() << " (" << ing.GetQuantity() << "g)" << endl;
		}
		cout << "----------------------------" << endl;
	}

	void ClearIngredients() {
		while (ingredients.Size() > 0)
			ingredients.DeleteEnd();
	}

	void SetPrice(double newPrice) { price = newPrice; }
	// ---------- Food::RecalculatePrice (drop‑in) ----------
	void RecalculatePrice()
	{
		double total = price;                       // start with base menu price
		for (int i = 0; i < ingredients.Size(); ++i)
			total += ingredients[i].getTotalPrice();

		price = total;                              // update the selling price
	}
	void   IncreaseQty(int x) { qty += x; }
	void   DecreaseQty(int x) { qty -= x; }
};

class Stock {
private:
	int quantity;
	double total_value;
	DoubleLinkedList<Food> foods;
	DoubleLinkedList<Ingredient> ingredients;

	// helper – trim
	static std::string trim(const std::string& s) {
		size_t a = s.find_first_not_of(" \t");
		size_t b = s.find_last_not_of(" \t");
		return (a == std::string::npos) ? "" : s.substr(a, b - a + 1);
	}

public:
	Stock() : quantity(0), total_value(0.0) {
		LoadIngredientsFromFile("ingredients.txt");  // explicit, same result
	}

	int  Size() const { 
		return foods.Size();
	}
	Food& GetFood(int idx) { return foods.At(idx); }

	/* ------------------------------------------------------------
	   Stock::LoadIngredientsFromFile
	   ------------------------------------------------------------ */
	void LoadIngredientsFromFile(const std::string& fn /*= "ingredients.txt"*/)
	{
		/* 1) clear current list */
		while (ingredients.Size() > 0) ingredients.DeleteEnd();

		std::ifstream fs(fn);
		if (!fs.is_open()) return;

		std::string line;
		while (std::getline(fs, line))
		{
			/* skip blank lines */
			if (trim(line).empty()) continue;

			std::stringstream ss(line);
			std::string namePart, qtyStr, priceStr;

			/* line format:  name | qty | price |  */
			if (std::getline(ss, namePart, '|') &&
				std::getline(ss, qtyStr, '|') &&
				std::getline(ss, priceStr, '|'))
			{
				std::string name = trim(namePart);
				double qty = std::stod(trim(qtyStr));
				double price = std::stod(trim(priceStr));

				bool found = false;
				for (int i = 0; i < ingredients.Size(); ++i) {
					if (ingredients[i].GetName() == name) {
						ingredients[i].IncreaseQuantity(qty, price); // just top‑up
						found = true;
						break;
					}
				}
				if (!found)
					ingredients.AddEnd(Ingredient(name, qty, price));
			}
		}
		fs.close();
	}

	void ShowIngredients() const {
		if (ingredients.Size() == 0) {
			std::cout << "No ingredients in stock.\n";
			return;
		}
		std::cout << "Ingredients in stock:\n";
		for (int i = 0; i < ingredients.Size(); ++i) {
			const Ingredient& ing = ingredients[i];
			std::cout << i + 1 << ") " << ing.GetName()
				<< " - Qty: " << ing.GetQuantity() << "g"
				<< " - Unit Price: " << ing.GetPricePerGram() << " AZN/g"
				<< " - Total: " << ing.getTotalPrice() << " AZN\n";
		}
	}

	bool IsIngredientInStock(const std::string& n, double q) const {
		for (int i = 0; i < ingredients.Size(); ++i)
			if (ingredients[i].GetName() == n && ingredients[i].GetQuantity() >= q)
				return true;
		return false;
	}

	double GetIngredientPrice(const std::string& n) const {
		for (int i = 0; i < ingredients.Size(); ++i)
			if (ingredients[i].GetName() == n)
				return ingredients[i].GetPricePerGram();
		return 0.0;
	}

	void DecreaseIngredient(const std::string& n, double amt) {
		for (int i = 0; i < ingredients.Size(); ++i)
			if (ingredients[i].GetName() == n) {
				ingredients[i].DecreaseQuantity(amt);
				break;
			}
	}

	void IncreaseIngredientQuantity(const std::string& n,
		double amt,
		double pricePerGram,
		Budget& budget)
	{
		double cost = amt * pricePerGram;
		if (!budget.SubtractFunds(cost)) {
			std::cout << "Not enough budget for " << n
				<< ". Need " << cost << " AZN\n";
			return;
		}

		bool found = false;
		for (int i = 0; i < ingredients.Size(); ++i) {
			if (ingredients[i].GetName() == n) {
				ingredients[i].IncreaseQuantity(amt, pricePerGram);   // just top‑up
				found = true;
				break;
			}
		}

		if (!found) {
			ingredients.AddEnd(Ingredient(n, amt, pricePerGram));   // brand‑new
			writetofileIngredients(n, amt, pricePerGram);             // write once
		}
		else {
			/* already existed – we UPDATED quantity in memory,
			   no new line is written to ingredients.txt */
		}

		std::cout << "Ingredient '" << n << "' successfully updated. Cost: "
			<< cost << " AZN\n";
	}


	void AddFood(const Food& f) {
		int idx = FindFood(f.GetName());
		if (idx != -1) foods[idx].IncreaseQty(f.GetQty()); // just increase count
		else           foods.AddEnd(f);                    // brand‑new dish
	}

	void ShowAllFoods() const {
		if (foods.Size() == 0) { std::cout << "No foods in stock.\n"; return; }
		std::cout << "--- Foods in stock ---\n";
		for (int i = 0; i < foods.Size(); ++i) foods[i].ShowInfo();
	}

	int FindFood(const string& n) const {
		for (int i = 0; i < foods.Size(); ++i)
			if (foods[i].GetName() == n) return i;
		return -1;
	}

	void DecreaseFood(const string& n) {            // called after a sale
		int idx = FindFood(n);
		if (idx == -1) return;
		foods[idx].DecreaseQty(1);
		if (foods[idx].GetQty() == 0) foods.DeleteIndex(idx);
	}

};

class Admin {
	string username;
	string password;

	DoubleLinkedList<Food>menu;
public:

	//methods

	void AddNewFood(DoubleLinkedList<Food>& menu,
		std::string   foodName,
		std::string   description,
		double        price,
		int           ingredientCount,
		Stock& stock,
		Budget& budget)
	{
		DoubleLinkedList<Ingredient> ingList;
		double totalCost = 0.0;

		for (int i = 0; i < ingredientCount; ++i) {
			std::string ingName;
			double qty, unitPrice;

			std::cout << i + 1 << "‑th ingredient name: ";
			std::getline(std::cin, ingName);

			std::cout << "Quantity (g) for " << ingName << ": ";
			std::cin >> qty;

			std::cout << "Price per gram (AZN) for " << ingName << ": ";
			std::cin >> unitPrice;
			std::cin.ignore();

			ingList.AddEnd(Ingredient(ingName, qty, unitPrice));
			totalCost += qty * unitPrice;
		}

		/* check the budget once */
		if (!budget.SubtractFunds(totalCost)) {
			std::cout << "Not enough balance. Need " << totalCost << " AZN\n";
			return;
		}

		/* build the food and update stock */
		Food newFood(foodName, description, price, 1);
		for (int i = 0; i < ingList.Size(); ++i) {
			const Ingredient& ing = ingList[i];
			newFood.AddIngredient(ing);
			stock.IncreaseIngredientQuantity(ing.GetName(),
				ing.GetQuantity(),
				ing.GetPricePerGram(),
				budget);          // handles file write ONLY if new
		}

		stock.AddFood(newFood);
		menu.AddEnd(newFood);

		std::cout << "Food added successfully!\n"
			<< totalCost << " AZN subtracted. Current budget: ";
		budget.ShowBalance();
	}

	//menuya baxmaq
	void ShowMenu(DoubleLinkedList<Food>& menu, Stock& stock) {
		cout << "All Foods in Menu: " << endl;
		for (size_t i = 0; i < menu.Size(); i++) {
			Food food = menu[i];
			bool canSeen = true;
			for (int j = 0; j < food.GetIngredients().Size(); j++) {
				Ingredient ingredient = food.GetIngredients()[j];
				if (!stock.IsIngredientInStock(ingredient.GetName(), ingredient.GetQuantity())) {
					canSeen = false;
					break;
				}
			}
			if (canSeen) {
				cout << i + 1 << ") " << food.GetName() << " - AZN" << food.GetPrice() << endl;
			}
		}
	}




};

//user teref
class Wallet { //userin bucesi
private:
	double balance;

public:
	Wallet() : balance(0) {}
	Wallet(double initial) : balance(initial) {};

	bool Subtract(double amount) {
		if (balance >= amount) {
			balance -= amount;
			return true;
		}
		return false;
	}

	void AddFunds(double amount) {
		if (amount > 0)
			balance += amount;
	}


	double GetBalance() const {
		return balance;
	}

	void ShowBalance() const {
		cout << "Your current balance: " << balance << " AZN\n";
	}
};

class Cart {
private:
	DoubleLinkedList<Food> items;         // foods currently in the cart

public:
	/* ---------- BASICS ---------- */
	void AddToCart(Food& food) {
		items.AddEnd(food);
		std::cout << food.GetName() << " added to cart.\n";
	}

	int Size() const { return items.Size(); }

	void ShowCart() const {
		if (items.Size() == 0) {
			std::cout << "Your cart is empty.\n";
			return;
		}
		std::cout << "--- Foods on cart ---\n";
		for (int i = 0; i < items.Size(); ++i) {
			std::cout << i + 1 << ". ";
			items[i].ShowInfo();           // ShowInfo is const in Food
		}
	}

	void RemoveFromCart(int index) {
		if (index < 1 || index > items.Size()) {
			std::cout << "Invalid input!\n";
			return;
		}
		std::cout << items.At(index - 1).GetName() << " removed from cart.\n";
		items.DeleteIndex(index - 1);
	}

	/* ---------- MODIFY INGREDIENTS ---------- */
	void ModifyFoodIngredients(Stock& stock) {
		if (items.Size() == 0) {
			std::cout << "Your cart is empty!\n";
			return;
		}

		int foodIdx;
		std::cout << "Enter the number of the food to modify ingredients: ";
		std::cin >> foodIdx;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (foodIdx < 1 || foodIdx > items.Size()) {
			std::cout << "Invalid input!\n";
			return;
		}

		Food& food = items.At(foodIdx - 1);

		int addCnt;
		std::cout << "How many ingredients do you want to add?: ";
		std::cin >> addCnt;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		for (int i = 0; i < addCnt; ++i) {
			stock.ShowIngredients();

			std::string ingName;
			std::cout << i + 1 << ". Enter ingredient name: ";
			std::getline(std::cin, ingName);

			std::cout << "Enter quantity in grams: ";
			double qty;
			std::cin >> qty;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			if (!stock.IsIngredientInStock(ingName, qty)) {
				std::cout << "Error: not enough " << ingName << " in stock!\n";
				--i;                               // retry this slot
				continue;
			}

			double unitPrice = stock.GetIngredientPrice(ingName);
			stock.DecreaseIngredient(ingName, qty);

			food.AddIngredient(Ingredient(ingName, qty, unitPrice));
			writetofileCard(ingName, static_cast<int>(qty), unitPrice * qty);

			std::cout << "Added " << qty << " g of '" << ingName
				<< "' for AZN " << unitPrice * qty << '\n';
		}

		food.RecalculatePrice();
		std::cout << "Item '" << food.GetName() << "' now costs: AZN "
			<< food.GetPrice()
			<< "\nUpdated cart total: AZN " << GetTotalPrice() << '\n';
	}

	/* ---------- PAYMENT ---------- */
	double GetTotalPrice() {
		double total = 0.0;
		for (int i = 0; i < items.Size(); ++i)
			total += items.At(i).GetPrice();      // At() is non‑const
		return total;
	}

	void ConfirmOrder(Wallet& wallet, Budget& budget, Stock& stock) {
		if (items.Size() == 0) { cout << "Your cart is empty!\n"; return; }

		double total = GetTotalPrice();
		if (!wallet.Subtract(total)) { cout << "Not enough balance\n"; return; }

		/* update budget & stock */
		budget.AddFunds(total);
		for (int i = 0; i < items.Size(); ++i)
			stock.DecreaseFood(items[i].GetName());

		cout << "Order confirmed: " << total << " AZN\n"
			<< "Wallet left: " << wallet.GetBalance() << " AZN\n";
		items.Clear();
	}

};

class User {
	string username;
	string password;
	//DoubleLinkedList<Food>basket; //user'in sebeti
	Wallet wallet;
	Cart cart;
public:
	User() {};
	User(string username, string password) {
		this->username = username;
		this->password = password;
	}

	//getter
	string GetUsername() {
		return username;
	}

	string GetPassword() {
		return password;
	}
	Wallet& GetWallet() {
		return wallet;
	}
	Cart& GetCart() {
		return cart;
	}
	//setter
	void SetUsername(string username) {
		if (username.length() > 10) {
			this->username = username;
		}
		else {
			throw invalid_argument("Username must be longer than 10 characters.");
		}
	}

	void ShowBalance() const {
		wallet.ShowBalance();
	}

	void SetPassword(string password) {
		if (password.length() > 8) {
			this->password = password;
		}
		else {
			throw invalid_argument("Password must be longer than 8 characters.");
		}
	}
	// User class içində
	void SetWallet(const Wallet& wallet) {
		this->wallet = wallet;
	}

	//methods

	//sebete yemek elave etmek
	void AddToBasket(Food& food) {
		cart.AddToCart(food); 
		//cout << food.GetName() << " added to your basket!" << endl;
	}

	//sebeti gostermek
	void ShowBasket() {
		cart.ShowCart(); 
	}


	//menuya baxmaq
	void ShowMenu(DoubleLinkedList<Food>& menu, Stock& stock) {
		cout << "All Foods in Menu: " << endl;
		for (size_t i = 0; i < menu.Size(); i++) {
			Food food = menu.At(i);
			bool canSeen = true;
			for (int j = 0; j < food.GetIngredients().Size(); j++) {
				Ingredient ingredient = food.GetIngredients().At(j);
				if (!stock.IsIngredientInStock(ingredient.GetName(), ingredient.GetQuantity())) {
					canSeen = false;
					break;
				}
			}
			if (canSeen) {
				cout << i + 1 << ") " << food.GetName() << " - AZN" << food.GetPrice() << endl;
			}
		}
	}

	void BuyFood(Budget& budget, Stock& stock, const string& foodName) {
		for (int i = 0; i < stock.Size(); ++i) {
			Food& food = stock.GetFood(i);
			if (food.GetName() == foodName) {
				double price = food.GetPrice();
				budget.AddFunds(price);
				cout << foodName << " bought" << price << " AZN added.\n";
				return;
			}
		}
		cout << "food not found!\n";
	}


};

//acount manager
class UserManager {
private:
	DoubleLinkedList<User> users;

public:
	UserManager() {};
	void SignUp(const string& username, const string& password) {
		if (SearchUser(username) != -1) {
			throw runtime_error("This user already exists");
		}
		if (username.length() <= 10) {
			throw invalid_argument("username should be longer than 10");
		}
		if (password.length() <= 8) {
			throw invalid_argument("password should be more than 8 characters");
		}

		User newUser(username, password);
		users.AddEnd(newUser);
		SaveUsersToFile(); // Hər yeni istifadəçidən sonra fayla yaz
	}


	bool SignIn(const string& username, const string& password) {
		if (username == "admin") {
			if (password == "admin1234") {
				return true;
			}
			else {
				throw string("wrong password");
				//cout << "wrong password";
			}
		}

		int index = SearchUser(username);
		if (index == -1) {
			throw runtime_error("User not found!");
		}

		if (users.At(index).GetPassword() == password) {
			return true;
		}
		else {
			throw runtime_error("Wrong password!");
		}
	}



	void ChangePassword(const string& username, const string& oldPassword, const string& newPassword) {
		int index = SearchUser(username);
		if (index == -1) {
			throw runtime_error("User not found!");
		}

		if (users.At(index).GetPassword() != oldPassword) {
			throw invalid_argument("Your old password is not correct!");
		}

		if (newPassword.length() <= 8) {
			throw invalid_argument("New password should be longer than 8 character!");
		}

		users.At(index).SetPassword(newPassword);
	}

	int SearchUser(const string& username) {
		for (int i = 0; i < users.Size(); ++i) {
			if (users.At(i).GetUsername() == username) {
				return i;
			}
		}
		return -1;
	}
	void SaveUsersToFile(const string& filename = "users.txt") {
		std::ofstream file(filename);
		if (!file.is_open()) {
			throw runtime_error("Failed to open user file for writing.");
		}
		for (int i = 0; i < users.Size(); ++i) {
			file << users.At(i).GetUsername() << "," << users.At(i).GetPassword() << "\n";
		}
		file.close();
	}

	// Fayldan oxu
	void LoadUsersFromFile(const string& filename = "users.txt") {
		std::ifstream file(filename);
		if (!file.is_open()) {
			return;
		}
		string line;
		while (getline(file, line)) {
			size_t commaPos = line.find(',');
			if (commaPos != string::npos) {
				string username = line.substr(0, commaPos);
				string password = line.substr(commaPos + 1);
				User user(username, password);
				users.AddEnd(user);
			}
		}
		file.close();
	}
};


//file food
void writetofile(string name, string desc, double price, int ingredients) {
	ofstream fs("menu.txt", ios::app);
	if (fs.is_open()) {
		fs << name << " | " << desc << " | " << price << " | " << ingredients << endl;
		fs.close();
	}
	else {
		throw runtime_error("file couldn't open!!!");
	}
}

string trim(const string& str) {
	size_t first = str.find_first_not_of(" \t");
	size_t last = str.find_last_not_of(" \t");
	return str.substr(first, (last - first + 1));
}

void readfromfile(DoubleLinkedList<Food>& menu) {
	ifstream file("menu.txt");
	string line;

	while (getline(file, line)) {
		stringstream ss(line);
		string name, description, priceStr, ingredientsStr;

		if (getline(ss, name, '|') &&
			getline(ss, description, '|') &&
			getline(ss, priceStr, '|') &&
			getline(ss, ingredientsStr)) {

			name = trim(name);
			description = trim(description);
			double price = stod(trim(priceStr));
			int ingredients = stoi(trim(ingredientsStr));

			Food food(name, description, price, 1);
			menu.AddEnd(food);
		}
	}
	file.close();
}

int main() {
	cout << "-------------------------------------------------" << endl;
	cout << "|                                               |" << endl;
	cout << "|             Welcome Our Restoran              |" << endl;
	cout << "|                                               |" << endl;
	cout << "-------------------------------------------------" << endl;



	DoubleLinkedList<Food> menu;
	UserManager userManager;
	userManager.LoadUsersFromFile();
	Admin admin;
	User user;
	Stock stock;
	Budget budget(5000);
	Cart card;
	Wallet user_wallet(50);
	int first_choice;
	Ingredient ingredient;
	user.SetWallet(user_wallet); 
	readfromfile(menu);
	stock.LoadIngredientsFromFile("ingredients.txt");  // explicit, same result

	while (true) {
		cout << "1.Admin\n2.User: ";
		cin >> first_choice;

		if (first_choice == 1) {
			string username, password, id, fullname;
			cin.ignore();
			cout << "Enter username: ";
			getline(cin, username);

			cout << "Enter password: ";
			getline(cin, password);
			bool loginSuccess = false;

			try {
				if (userManager.SignIn(username, password)) {
					cout << "Login succesfully!" << endl;
					loginSuccess = true;
				}
			}
			catch (string) {
				cout << "Couldnt sign in" << endl;
			}

			if (loginSuccess) {
				int admin_second_choice;
				cout << "1.Add new food to menu\n2.Show Menu\n3.Increase the number of ingredient\n";
				cout << "4.Show all food in stock\n5.See budget\n6.Show ingredint in stock\n";
				cout << "7.Read from file\n";
				cout << "8.Show balance\n";

				cin >> admin_second_choice;
				cin.ignore();


				if (admin_second_choice == 1) { //add new food
					string foodName, description;
					double price;
					int ingredientCount;

					cout << "Enter new food's name: ";
					getline(cin, foodName);

					cout << "Enter food's description: ";
					getline(cin, description);

					cout << "Enter food's price: ";
					cin >> price;

					cout << "Enter ingredient's count: ";
					cin >> ingredientCount;
					cin.ignore();

					Food newFood(foodName, description, price, 1);
					admin.AddNewFood(menu, foodName, description, price, ingredientCount, stock, budget);
					writetofile(foodName, description, price, ingredientCount);

				}

				else if (admin_second_choice == 2) { //show menu
					admin.ShowMenu(menu, stock);
				}
				else if (admin_second_choice == 3) { //increase 
					string name;
					double amount, pricePerGram;

					cout << "Enter name of ingredient: ";
					getline(cin, name);
					cout << "Enter amount of ingredient(gram): ";
					cin >> amount;
					cout << "Enter price of ingredient for gram: ";
					cin >> pricePerGram;

					stock.IncreaseIngredientQuantity(name, amount, pricePerGram, budget);
					cout << name << " succesfully changed.\n";
					writetofileIngredients(name, amount, pricePerGram);
				}
				else if (admin_second_choice == 4) {
					stock.ShowAllFoods();
				}
				else if (admin_second_choice == 5) {
					budget.ShowBalance();
				}
				else if (admin_second_choice == 6) {
					stock.ShowIngredients();
				}
				else if (admin_second_choice == 7) {
					try {
						readfromfile(menu);
					}
					catch (const exception& e) {
						cout << "Error: " << e.what() << endl;
					}
				}
				else if (admin_second_choice == 8) {
					budget.ShowBalance();
				}
			}
		}
		else if (first_choice == 2) {
			int user_account;
			cout << "1. Sign in system\n2.Sign up\n3.Change Password: ";
			cin >> user_account;

			string user_username, user_password;
			cin.ignore();
			cout << "Enter your username: ";
			getline(cin, user_username);
			cout << "Enter your password: ";
			getline(cin, user_password);

			bool userLogin = false;
			if (user_account == 1) {
				try {
					userManager.SignIn(user_username, user_password);
					cout << "Successfully signed in!\n";
					userLogin = true;
				}
				catch (const exception& e) {
					cout << "Error: " << e.what() << endl;
				}
			}
			else if (user_account == 2) {
				try {
					userManager.SignUp(user_username, user_password);
					cout << "Singed up succesfully!\n";
				}
				catch (const exception& e) {
					cout << "Error: " << e.what() << endl;
					break;
				}
			}
			else if (user_account == 3) {
				string new_password;
				cout << "Enter new password: ";
				getline(cin, new_password);
				try {
					userManager.ChangePassword(user_username, user_password, new_password);
					cout << "Password changed succesfully\n";
				}
				catch (const exception& e) {
					cout << "Error: " << e.what() << endl;
				}
			}
			if (userLogin) {
				int user_second_choice;
				cout << "\n1. Show menu and add food to basket\n2. View basket\n";
				cout << "3. Show balance\n";
				cout << "0. Exit\n";
				cout << "Enter your choice: ";
				cin >> user_second_choice;
				cin.ignore();

				if (user_second_choice == 1) {
					if (menu.Size() == 0) {
						cout << "No items in the menu" << endl;
					}

					user.ShowMenu(menu, stock);
					cout << "Enter food number to add basket: ";
					int food_choice;
					while (true) {
						cin >> food_choice;
						if (food_choice >= 1 && food_choice <= menu.Size()) {
							break;
						}
						else {
							cout << "Invalid choice! Please enter a number between 1 and " << menu.Size() << ": ";
						}
					}
					user.AddToBasket(menu[food_choice - 1]);
					user.ShowBasket();
					int user_card_choice;
					cout << "choose:\n1.Remove food from card\n2.Change ingredients\n3.Confirm order: ";
					cin >> user_card_choice;
					if (user_card_choice == 1) {
						int index;
						cout << "enter number of food you want to remove from cart: ";
						cin >> index;
						user.GetCart().RemoveFromCart(index);
						user.ShowBasket();
					}
					else if (user_card_choice == 2) {
						user.ShowBasket();
						stock.LoadIngredientsFromFile("ingredients.txt");  // explicit, same result
						user.GetCart().ModifyFoodIngredients(stock);


					}
					else if (user_card_choice == 3) {
						user.GetCart().ConfirmOrder(user.GetWallet(), budget, stock);
					}
				}
				else if (user_second_choice == 2) {
					user.ShowBasket();

					if (user.GetCart().Size() == 0) {
						cout << "Your cart is empty. Please add food to your cart first.\n";
					}
					else {
						int user_card_choice;
						cout << "Choose:\n1. Remove food from cart\n2. Change ingredients\n3. Confirm order: ";
						cin >> user_card_choice;

						if (user_card_choice == 1) {
							int index;
							cout << "Enter number of food you want to remove from cart: ";
							cin >> index;
							user.GetCart().RemoveFromCart(index);
							user.ShowBasket();
						}
						else if (user_card_choice == 2) {
							int foodIndex;
							cout << "Enter the number of the food you want to change the ingredients for: ";
							cin >> foodIndex;

							if (foodIndex < 1 || foodIndex > menu.Size()) {
								cout << "Invalid input! Enter the correct number within the menu.\n";
							}
							else {
								//user.GetCart().ModifyFoodIngredients();
							}
						}
						else if (user_card_choice == 3) {
							user.GetCart().ConfirmOrder(user.GetWallet(), budget, stock);
						}
					}
				}

				else if (user_second_choice == 3) {
					user.GetWallet().ShowBalance();   // shows 38 AZN (or whatever is left)
				}

			}
		}
	}
	return 0;
}
