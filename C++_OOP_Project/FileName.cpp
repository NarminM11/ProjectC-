#include<iostream>
#include<cassert>
#include<string>
#include"List.h"
//#include"UserManager.h"
#include<fstream> 
using namespace std;

//file system food
void writetofile(string name, string desc, double price, int ingredients) {
	ofstream fs("text.txt", ios::app); 
	if (fs.is_open()) {
		fs << name << " | " << desc << " | " << price << " | " << ingredients << endl;
		fs.close();
	}
	else {
		throw runtime_error("file couldn't open!!!");
	}
}

void readfromfile() {
	ifstream fs("text.txt");
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

void readfromfileIngredients() {
	ifstream fs("ingredients.txt");
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
	string name;
	string descp;
	double price;
	DoubleLinkedList<Ingredient> ingredients;

public:
	Food() {};
	Food(string name, string descp, double price) {
		this->name = name;
		this->descp = descp;
		this->price = price;
	}

	string GetName() {
		return name; 
	}
	string GetDescp() { 
		return descp;
	}
	double GetPrice() { 
		return price; 
	}

	DoubleLinkedList<Ingredient>& GetIngredients() {
		return ingredients; 
	}

	void AddIngredient(const Ingredient& ing) {
		ingredients.AddEnd(ing);
	}

	void ShowInfo() { //yemekler haqqinda umumi melumatlarin hamisi
		cout << "Food name: " << name << endl;
		cout << "Description of food: " << descp << endl;
		cout << "Price: " << price << " AZN" << endl;
		cout << "Ingredients:\n";
		for (int i = 0; i < ingredients.Size(); ++i) {
			cout << "- " << ingredients[i].GetName()
				<< " (" << ingredients[i].GetQuantity() << "g)"
				<< endl;
		}
		cout << "----------------------------\n";
	}

	void ClearIngredients() {
		ingredients.DeleteEnd();  //ingredientleri sebetden ya da stockdan silmek
	}


};

class Stock {
	int quantity;
	double total_value;
	//has composition
	DoubleLinkedList<Food>foods;
	DoubleLinkedList<Ingredient> ingredients; //umumi ingredientler
	
public:
	Stock() : quantity(0), total_value(0.0) {};  //default constrcutoru


	int Size() const {
		return foods.Size();
	}

	Food& GetFood(int index) {
		return foods.At(index);
	}

	//methods
	void AddIngredientToStock(const Ingredient& ing) {
		ingredients.AddEnd(ing);
	}

	void DecreaseIngredient(string name, double amount) {
		for (int i = 0; i < ingredients.Size(); ++i) {
			if (ingredients[i].GetName() == name) {
				ingredients[i].DecreaseQuantity(amount);
				break;
			}
		}
	}

	//hansisa ingredientin stockda olub olmamasina baxiriq
	bool IsIngredientInStock(const string& ingredient_name, double quantity) {
		for (size_t i = 0; i < ingredients.Size(); i++)
		{
			if (ingredients[i].GetName() == ingredient_name) {
				if (ingredients[i].GetQuantity() >= quantity) {
					return true;
				}
			}
		}
		return false;
	}

	void ShowIngredients() const { //umumi ingredientlere baxmaq
		cout << "Ingredients in stock:\n";
		for (int i = 0; i < ingredients.Size(); ++i) {
			Ingredient ing = ingredients[i];
			cout << "Name: " << ing.GetName()
				<< ", Quantity: " << ing.GetQuantity() << " grams"
				<< ", Total Price: " << ing.getTotalPrice() << " AZN\n";
		}
	}

	void IncreaseIngredientQuantity(const string& name, double amount, double pricePerGram, Budget& budget) {
		double totalCost = amount * pricePerGram;

		// eger budgetde kifayet qeder pul olmayanda mesaj verir ki balans azdi
		if (!budget.SubtractFunds(totalCost)) {
			cout << "Not enough budget to add ingredient: " << name << ", you need " << totalCost << " AZN in budget\n";
			return;
		}

		bool found = false;
		for (int i = 0; i < ingredients.Size(); ++i) {
			if (ingredients.At(i).GetName() == name) {
				ingredients.At(i).IncreaseQuantity(amount, pricePerGram);
				found = true;
				break;
			}
		}

		if (!found) {
			Ingredient newIng(name, amount, pricePerGram);
			ingredients.AddEnd(newIng);
		}

		writetofileIngredients(name, amount, pricePerGram);

		cout << "Ingredient '" << name << "' successfully updated. Total cost: " << totalCost << " AZN\n";
	}

	void AddFood(const Food& food) {
		foods.AddEnd(food);
	}

	// stockda olan yeməkləre baxmaq
	void ShowAllFoods() {
		if (foods.Size() == 0) {
			cout << "not food found in stock.\n";
			return;
		}

		cout << "---Foods on stock---\n";
		for (int i = 0; i < foods.Size(); ++i) {
			foods[i].ShowInfo();  
		}
	}

};

class Admin {
	string username;
	string password;

	DoubleLinkedList<Food>menu;
public:

	//methods

	void AddNewFood(DoubleLinkedList<Food>& menu, string foodName, string description, double price, int ingredientCount, Stock& stock, Budget& budget) {
		DoubleLinkedList<Ingredient> ingredients;
		double totalCost = 0.0;

		for (int i = 0; i < ingredientCount; ++i) {
			string ingName;
			double quantity, pricePerGram;

			cout << i + 1 << "th ingredient's name: ";
			getline(cin, ingName);

			cout << "Quantity for " << ingName << ": ";
			cin >> quantity;

			cout << "Price per gram for " << ingName << ": ";
			cin >> pricePerGram;
			cin.ignore();

			Ingredient ing(ingName, quantity, pricePerGram);
			ingredients.AddEnd(ing);
			writetofileIngredients(ingName, quantity, pricePerGram);

			totalCost += quantity * pricePerGram;
		}

		if (!budget.SubtractFunds(totalCost)) {
			cout << "While adding food error occured, not enough balance: " << totalCost << " AZN\n";
			return;
		}

		Food new_food(foodName, description, price);

		for (int i = 0; i < ingredients.Size(); ++i) {
			Ingredient ing = ingredients.At(i);
			new_food.AddIngredient(ing);
			stock.IncreaseIngredientQuantity(ing.GetName(), ing.GetQuantity(), ing.GetPricePerGram(), budget);
		}


		stock.AddFood(new_food);
		menu.AddEnd(new_food);

		cout << "Food added successfully!\n";
		cout << totalCost << " AZN subtracted. Your current budget: ";
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

class Cart { //userin sebeti
private:
	DoubleLinkedList<Food> items; //sebetde olanlar
	DoubleLinkedList<Ingredient> stock;
public:
	void AddToCart(Food& food) {
		items.AddEnd(food);
		cout << food.GetName() << " added to card.\n";
	}

	void ShowCart() {
		if (items.Size() == 0) {
			cout << "your card is empty.\n";
			return;
		}

		cout << "--- Foods on card ---\n";
		for (int i = 0; i < items.Size(); ++i) {
			cout << i + 1 << ". ";
			items[i].ShowInfo();
		}
	}

	void RemoveFromCart(int index) {
		if (index < 1 || index > items.Size()) {
			cout << "invalid input!\n";
			return;
		}
		cout << items.At(index - 1).GetName() << " removed from card.\n";
		items.DeleteIndex(index - 1);
	}

	void ModifyFoodIngredients() {
		if (items.Size() == 0) {
			cout << "Your card is empty!\n";
			return;
		}

		// Səbətdəki yeməkləri göstər
		cout << "--- Foods on stock ---\n";
		for (int i = 0; i < items.Size(); ++i) {
			cout << i + 1 << ". " << items[i].GetName() << '\n';
		}

		int foodIndex;
		cout << "Enter the number of the food you want to change the ingredients for.: ";
		cin >> foodIndex;
		cin.ignore();

		if (foodIndex < 1 || foodIndex > items.Size()) {
			cout << "Invalid input!\n";
			return;
		}

		// Seçilmiş yemək
		Food& food = items.At(foodIndex - 1);

		// Əvvəlki ingredientlər silinir
		food.ClearIngredients();

		// Stock-u göstər
		cout << "--- Ingredients in stock ---\n";
		for (int i = 0; i < stock.Size(); ++i) {
			const Ingredient& ingredient = stock[i];
			cout << i + 1 << ". " << ingredient.GetName()
				<< " - " << ingredient.GetQuantity() << " gram, "
				<< ingredient.GetPricePerGram() << " AZN/gram\n";
		}

		int newCount;
		cout << "How many ingredients do you want to add?: ";
		cin >> newCount;
		cin.ignore();

		for (int i = 0; i < newCount; ++i) {
			int ingredientIndex;
			cout << i + 1 << ". Enter the ingredient number: ";
			cin >> ingredientIndex;
			cin.ignore();

			if (ingredientIndex < 1 || ingredientIndex > stock.Size()) {
				cout << "Invalid inout. Please try again.\n";
				--i;
				continue;
			}

			Ingredient selected = stock[ingredientIndex - 1];
			food.AddIngredient(selected);
		}

		cout << "Ingredients updated successfully!\n";
	}

	double GetTotalPrice() {
		double total = 0;
		for (int i = 0; i < items.Size(); ++i) {
			total += items.At(i).GetPrice();
		}
		return total;
	}

	void ConfirmOrder(Wallet& wallet, Budget& budget) {
		if (items.Size() == 0) {
			cout << "Your card is empty!\n";
			return;
		}

		double total = GetTotalPrice();

		if (!wallet.Subtract(total)) {
			cout << "Not enough balance\n";
			return;
		}

		budget.AddFunds(total);

		cout << "Order confirmed: " << total << " AZN\n";

		items.Clear(); //order confirm olduqdan sonra sebet sifirlanmalidi
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

	//methods

	//sebete yemek elave etmek
	void AddToBasket(Food& food) {
		cart.AddToCart(food); 
		cout << food.GetName() << " added to your basket!" << endl;
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
			throw invalid_argument("password should be more than 8 character");
		}

		User newUser(username, password);
		users.AddEnd(newUser);
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
};


int main() {
	cout << "-------------------------------------------------" << endl;
	cout << "|                                               |" << endl;
	cout << "|             Welcome Our Restoran              |" << endl;
	cout << "|                                               |" << endl;
	cout << "-------------------------------------------------" << endl;

	DoubleLinkedList<Food> menu;
	UserManager userManager;
	Admin admin;
	User user;
	Stock stock;
	Budget budget(5000);
	//Cart card;
	Wallet user_wallet(50);
	int first_choice;
	Ingredient ingredient;

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
				cin >> admin_second_choice;
				cin.ignore();


				if (admin_second_choice == 1) {
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

					Food newFood(foodName, description, price);
					admin.AddNewFood(menu, foodName, description, price, ingredientCount, stock, budget);
					writetofile(foodName, description, price, ingredientCount);

				}

				else if (admin_second_choice == 2) {
					admin.ShowMenu(menu, stock);
					readfromfile();
				}
				else if (admin_second_choice == 3) {
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
					//stock.ShowAllFoods();
					readfromfile();
				}
				else if (admin_second_choice == 5) {
					budget.ShowBalance();
				}
				else if (admin_second_choice == 6) {
					stock.ShowIngredients();
					readfromfileIngredients();
				}
				else if (admin_second_choice == 7) {
					try {
						readfromfile();
					}
					catch (const exception& e) {
						cout << "Error: " << e.what() << endl;
					}
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
			if(userLogin){
			int user_second_choice;
			cout << "\n1. Show menu and add food to basket\n2. View basket\n";
			cout << "0. Exit\n";
			cout << "Enter your choice: ";
			cin >> user_second_choice;
			cin.ignore();

			if (user_second_choice == 1) {
				readfromfile();
				if (menu.Size() == 0) {
					cout << "No items in the menu" << endl;
				}

				cout << "Menu" << endl;
				for (int i = 0; i < menu.Size(); i++) {
					cout << i + 1 << ") " << menu[i].GetName() << " - AZN" << menu[i].GetPrice() << endl;
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
				cout << menu[food_choice - 1].GetName() << " added to your basket!" << endl;
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
					cout << "Foods on menu:\n";
					for (int i = 0; i < menu.Size(); ++i) {
						cout << i + 1 << ") " << menu[i].GetName() << endl;
					}
					int foodIndex;
					cout << "Enter number of foods you want to change ingredient: ";
					cin >> foodIndex;
					user.GetCart().ModifyFoodIngredients();
				}
				else if (user_card_choice == 3) {
					user.GetCart().ConfirmOrder(user.GetWallet(), budget);
				}
			}
			else if (user_second_choice == 2) {
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
					cout << "Foods on menu:\n";
					for (int i = 0; i < menu.Size(); ++i) {
						cout << i + 1 << ") " << menu[i].GetName() << endl;
					}
					int foodIndex;
					cout << "Enter the number of the food you want to change the ingredients for.: ";
					cin >> foodIndex;

					if (foodIndex < 1 || foodIndex > menu.Size()) {
						cout << "Invalid input! Enter the correct number within the menu..\n";
					}
					else {
						user.GetCart().ModifyFoodIngredients();
					}
				}

				else if (user_card_choice == 3) {
					user.GetCart().ConfirmOrder(user.GetWallet(), budget);
				}

			}
			}

		}
	}

	return 0;
}
