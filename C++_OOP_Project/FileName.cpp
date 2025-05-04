#include<iostream>
#include<cassert>
#include<string>
#include"List.h"
//#include"UserManager.h"
#include<fstream> 
using namespace std;

void writetofile(string str,int i) {
	ofstream fs("text.txt");

	if (fs.is_open()) {
		fs << str << " " << i;
	}
	else {
		throw exception("file couldn't open!!!");
	}
	fs.close();
}

void readfromfile() {
	ifstream fs("text.txt", ios::in); 

	if (fs.is_open()) {
		string str;
		int i = 0;
		while (!fs.eof()) {
			fs >> str;
			fs >> i;			
		}
		cout << str << " " << i << endl;
	}
	else {
		throw exception("file couldn't open!!!");
	}
	fs.close();
}

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
	string GetName() {
		return name;
	}
	double GetQuantity() {
		return quantity;
	}
	double getTotalPrice() const {
		return quantity * priceGram;
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

		//ingredient artanda qiymeti de artacaq ona uygun
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

	string GetName() { return name; }
	string GetDescp() { return descp; }
	double GetPrice() { return price; }

	DoubleLinkedList<Ingredient>& GetIngredients() { return ingredients; }

	void AddIngredient(const Ingredient& ing) {
		ingredients.AddEnd(ing);
	}

	void ShowInfo() {
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
		ingredients.DeleteEnd();  //ingredientleri silmek ucun stockdan ya da sebetden
	}


};

class Stock {
	//has composition
	DoubleLinkedList<Food>foods;
	DoubleLinkedList<Ingredient> ingredients; //umumi ingredientler
	int quantity;
	double total_value;
public:
	Stock() : quantity(0), total_value(0.0) {};  //default constrcutoru

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
	int Size() const {
		return foods.Size();
	}

	Food& GetFood(int index) {
		return foods.At(index); 
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

	void ShowIngredients() const {
		cout << "Ingredients in stock:\n";
		for (int i = 0; i < ingredients.Size(); ++i) {
			Ingredient ing = ingredients[i];
			cout << "Name: " << ing.GetName()
				<< ", Quantity: " << ing.GetQuantity() << " grams"
				<< ", Total Price: " << ing.getTotalPrice() << " AZN\n";
		}
	}

	void IncreaseIngredientQuantity(const string& name, double amount, double pricePerGram) {
		bool found = false;
		for (int i = 0; i < ingredients.Size(); ++i) {
			if (ingredients.At(i).GetName() == name) {
				ingredients.At(i).IncreaseQuantity(amount, pricePerGram);
				found = true;
				break;
			}
		}

		if (!found) {
			// Ingredient tapılmadısa, yenisini əlavə et
			Ingredient newIng(name, amount, pricePerGram);
			ingredients.AddEnd(newIng);
		}
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
			foods[i].ShowInfo();  // yemeyi gostermeye
		}
	}

};

class Budget { //restoranin budcesi
	double balance; 

public:
	Budget(double initialAmount = 0) : balance(initialAmount) {}; //default constructoru

	//eger budceye pul elave edilmeli olsa
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
		if (amount > 0 && amount >= amount) {
			amount -= amount;
			cout << amount << " AZN subtracted from budget.\n";
			return true;
		}
		else {
			cout << "not enough budget!\n";
			return false;
		}
	}

	//budceye baxmaq
	void ShowBalance() const {
		cout << "Current budget: " << balance << " AZN\n";
	}

	double GetBalance() const {
		return balance;
	}
};

class Admin {
	string username;
	string password;
	string fullname;
	string id;
	DoubleLinkedList<Food>menu;
public:
	Admin() {

	}
	Admin(string username, string password, string fullname, string id) {
		this->username = username;
		this->password = password;
		this->fullname = fullname;
		/*SetUsername(username);
		SetPassword(password);*/
		//SetFullname(fullname);
		this->id = id;
	}

	//getter
	string GetUsername() const {
		return username;
	}

	string GetPassword() const {
		return password;
	}
	string GetFullname() {
		return fullname;
	}
	string GetId() {
		return id;
	}

	//setter
	void SetFullname(string fullname) {
		if (fullname.length() > 10) {
			this->fullname = fullname;
		}
		else {
			assert(!"invalid fullname input....");
		}
	}

	void SetUsername(string username) {
		if (username=="Admin") {
			this->username = username;
		}
		else {
			assert(!"invalid username input....");
		}
	}

	void SetPassword(string password) {
		if (password=="admin") {
			this->password = password;
		}
		else {
			assert(!"invalid password input....");
		}
	}
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

			// ingredientin qiymeti ucun
			totalCost += quantity * pricePerGram;
		}

		// Əgər büdcə kifayət etmirsə, prosesi dayandır
		if (!budget.SubtractFunds(totalCost)) {
			cout << "while adding food error happened, not enough balance: " << totalCost << " AZN\n";
			return;
		}

		Food new_food(foodName, description, price);
		for (int i = 0; i < ingredients.Size(); ++i) {
			new_food.AddIngredient(ingredients.At(i));
		}

		stock.AddFood(new_food);
		menu.AddEnd(new_food);

		cout << "Food added successfully!\n";
		cout << totalCost << " AZN subtracted. cuurent budge: ";
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
		cout << "Current balance: " << balance << " AZN\n";
	}
};

class Cart { //userin sebeti
private:
	DoubleLinkedList<Food> items; //sebetde olanlar

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

	void ModifyFoodIngredients(int index) {
		if (index < 1 || index > items.Size()) {
			cout << "invalid input!\n";
			return;
		}

		Food& food = items.At(index - 1);
		food.ClearIngredients(); 

		int newCount;
		cout << "new ingredient count: ";
		cin >> newCount;
		cin.ignore();

		for (int i = 0; i < newCount; ++i) {
			string name;
			double qty, pricePerGram;

			cout << i + 1 << ". ingredient name: ";
			getline(cin, name);

			cout << " (gram): ";
			cin >> qty;

			cout << "price for gram: ";
			cin >> pricePerGram;
			cin.ignore();

			Ingredient ing(name, qty, pricePerGram);
			food.AddIngredient(ing);
		}

		cout << "updated succesfully!\n";
	}

	double GetTotalPrice() {
		double total = 0;
		for (int i = 0; i < items.Size(); ++i) {
			total += items.At(i).GetPrice();
		}
		return total;
	}

	void ConfirmOrder(Wallet& wallet, Budget& budget) {
		double total = GetTotalPrice();

		if (!wallet.Subtract(total)) {
			cout << "not enough balance\n";
			return;
		}

		budget.AddFunds(total);

		cout << "order confirmed: " << total << " AZN\n";

		items.DeleteEnd(); //sebet sifirlanmasi ucun
	}



};

class User {
	string username;
	string password;
	DoubleLinkedList<Food>basket; //user'in sebeti
	Wallet wallet;
	Cart cart;
	//DoubleLinkedList<User>& users;
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

	//setter
	void SetUsername(string username) {
		if (username.length() > 10) {
			this->username = username;
		}
		else {
			assert(!"Invalid username input........");
		}
	}
	Wallet& GetWallet() {
		return wallet;
	}

	void ShowBalance() const {
		wallet.ShowBalance();
	}

	Cart& GetCart() {
		return cart;
	}
	void SetPassword(string password) {
		if (password.length() > 8) {
			this->password = password;
		}
		else {
			assert(!"Invalid password input........");
		}
	}

	//methods

	//sebete yemek elave etmek
	void AddToBasket( Food& food) {
		basket.AddEnd(food);
		cout << food.GetName() << " added to your basket!" << endl;
	}

	//sebeti gostermek
	void ShowBasket() {
		cout << "Your basket" << endl;
		for (int i = 0; i < basket.Size(); i++)
		{
			cout << i + 1 << ") " << basket.At(i).GetName()
				<< " - $" << basket.At(i).GetPrice() << endl;
		}
		cout << "-------------------\n";
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
				cout << i + 1 << ") " << food.GetName() << " - $" << food.GetPrice() << endl;
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



//manage accounts
class AdminManager {
private:
	DoubleLinkedList<Admin> admins;

public:
	AdminManager() {
		//admins.AddEnd(Admin("Admin", "admin", " Admin", "1"));
	}

	int SearchAdmin(const string& username) {
		for (int i = 0; i < admins.Size(); ++i) {
			if (admins.At(i).GetUsername() == username) {
				return i;
			}
		}
		return -1;
	}

	bool SignIn(const string& username, const string& password) {
		int index = SearchAdmin(username);
		if (index == -1) {
			assert(! "Admin tapılmadı!");
			return false;
		}

		Admin admin = admins.At(index);
		if (admin.GetPassword() == password) {
			cout << "Giriş uğurludur! Xoş gəldiniz, " << admin.GetFullname() << "!\n";
			return true;
		}
		else {
			cout << "Şifrə yalnışdır!\n";
			return false;
		}
	}

};

class UserManager {
private:
	DoubleLinkedList<User> users;

public:
	UserManager() {};
	void SignUp(const string& username, const string& password) {
		if (SearchUser(username) != -1) {
			throw runtime_error("Bu istifadəçi adı artıq mövcuddur!");
		}

		if (username.length() <= 10) {
			throw invalid_argument("İstifadəçi adı 10 simvoldan uzun olmalıdır!");
		}

		if (password.length() <= 8) {
			throw invalid_argument("Şifrə 8 simvoldan uzun olmalıdır!");
		}

		User newUser(username, password);
		users.AddEnd(newUser);
	}

	bool SignIn(const string& username, const string& password) {
		int index = SearchUser(username);
		if (index == -1) {
			throw runtime_error("İstifadəçi tapılmadı!");
		}

		if (users.At(index).GetPassword() == password) {
			return true;
		}
		else {
			throw invalid_argument("Şifrə yanlışdır!");
		}
	}

	void ChangePassword(const string& username, const string& oldPassword, const string& newPassword) {
		int index = SearchUser(username);
		if (index == -1) {
			throw runtime_error("İstifadəçi tapılmadı!");
		}

		if (users.At(index).GetPassword() != oldPassword) {
			throw invalid_argument("Köhnə şifrə yanlışdır!");
		}

		if (newPassword.length() <= 8) {
			throw invalid_argument("Yeni şifrə 8 simvoldan uzun olmalıdır!");
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

/*Admin admin("Narmin", "1234567", "Narmin Murshudova", "4563479631");
	cout << admin.GetFullname() << endl;
	Food food1("pizza", "tasty new pizza", 10.89);
	DoubleLinkedList<Ingredient> ingredients;

	Ingredient ing1("Mozzarella", 100);
	Ingredient ing2("BBQ souce", 50);

	food1.AddIngredient(ing1);
	food1.AddIngredient(ing2);
	cout << "elave olundu" << endl;*/

void main() {
	cout << "-------------------------------------------------" << endl;
	cout << "|                                               |" << endl;
	cout << "|             Welcome Our Restoran              |" << endl;
	cout << "|                                               |" << endl;
	cout << "-------------------------------------------------" << endl;

	DoubleLinkedList<Food> menu; 
	UserManager userManager;
	AdminManager adminManager;
	Admin admin;
	User user;
	Stock stock;
	Budget budget(4.5);
	Cart card;
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

			cout << "Enter fullname: ";
			getline(cin, fullname);

			cout << "Enter id: ";
			getline(cin, id);

			//Admin admin(username, password, fullname, id);
			adminManager.SignIn(username, password);
			int admin_second_choice;
			cout << "1.Add new food to menu\n2.Show Menu\n3.Increase the number of ingredient\n4.Show all food in stock\n5.See budget\n6.Show ingredint in stock";
			cin >> admin_second_choice;
			cin.ignore();

			if (admin_second_choice == 1) { //add food
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
			}
			else if (admin_second_choice == 2) { //show menu
				admin.ShowMenu(menu, stock);
			}
			else if (admin_second_choice == 3) {
				string name;
				double amount;
				double pricePerGram;

				cout << "Ingredient adını daxil edin: ";
				getline(cin, name);

				cout << "Əlavə etmək istədiyiniz miqdarı (qramla) daxil edin: ";
				cin >> amount;

				cout << "1 qram üçün qiyməti daxil edin: ";
				cin >> pricePerGram;

				// Ingredienti artırırıq (əgər yoxdursa, əlavə edir)
				stock.IncreaseIngredientQuantity(name, amount, pricePerGram);

				cout << name << " ingredienti stokda yeniləndi.\n";			
			}
			else if (admin_second_choice == 4) {
				stock.ShowAllFoods();
			}
			else if (admin_second_choice == 5) {
				budget.ShowBalance();
			}
			else if (admin_second_choice == 6) {//in gredientlere baxmaq stockdaki
				stock.ShowIngredients();
			}
		}
		else if (first_choice == 2) {//user teref
			int user_account;
			cout << "1. Sign in sytsem\n2.Sign up\n3.Change Password: ";
			cin >> user_account;

			string user_username, user_password;
			cin.ignore();
			cout << "Enter your username: ";
			getline(cin, user_username);

			cout << "Enter your password: ";
			getline(cin, user_password);
			if(user_account==1){
				try{
					userManager.SignIn(user_username, user_password);
					cout << "Ugurla giris edildi!\n";
				}
				catch(const exception& e){
					cout << "Xəta baş verdi: " << e.what() << endl;
				}
			}
			else if (user_account == 2) {
				try {
					userManager.SignUp(user_username, user_password);
					cout << "Qeydiyyat uğurla tamamlandı!\n";
				}
				catch (const exception& e) {
					cout << "Xəta baş verdi: " << e.what() << endl;
				}
			}
			else if (user_account == 3) {
				string new_password;
				cout << "Enter new password: ";
				getline(cin, new_password);
				try {
					userManager.ChangePassword(user_username, user_password, new_password);
					cout << "sifre uğurla deyisdirildi!\n";
				}
				catch (const exception& e) {
					cout << "Xəta baş verdi: " << e.what() << endl;
				}

			}
			
			int user_second_choice;
			cout << "\n1. Show menu and add food to basket\n2. View Basket\n";
			cout << "2. View basket\n";
			cout << "0. Exit\n";
			cout << "Enter your choice: ";
			cin >> user_second_choice;
			cin.ignore();
			if (user_second_choice == 1) {
				for (int i = 0; i < menu.Size(); i++) {
					cout << i + 1 << ") " << menu[i].GetName()
						<< " - $" << menu[i].GetPrice() << endl;
				}
				user.ShowMenu(menu, stock);
				cout << "enter food number to add basket: ";
				int food_choice;
				cin >> food_choice;
				if (food_choice >= 1 && food_choice <= menu.Size()) {
					user.AddToBasket(menu[food_choice - 1]);
				}
				else {
					cout << "Invalid choice!\n";
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
					card.RemoveFromCart(index);
					user.ShowBasket();
				}
				else if (user_card_choice == 2) {
					cout << "Menudakı yeməklər:\n";
					for (int i = 0; i < menu.Size(); ++i) {
						cout << i + 1 << ") " << menu[i].GetName() << endl;
					}
					int foodIndex;
					cout << "İngrediyentlərini dəyişmək istədiyiniz yeməyin nömrəsini daxil edin: ";
					cin >> foodIndex;

					card.ModifyFoodIngredients(foodIndex - 1);
				}
				else if (user_card_choice == 3) {
					card.ConfirmOrder(user_wallet, budget);;
				}
			}
		}
	}
}

