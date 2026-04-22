/*
* Josiah Roske
* CS-300 
* Prof. Jamalian
* 4/15/26
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <limits>
using namespace std;

// trim helper to remove leading/trailing whitespace and CR
static inline std::string trim(const std::string& s) {
	const char* ws = " \t\r\n";
	size_t start = s.find_first_not_of(ws);
	if (start == std::string::npos) return "";
	size_t end = s.find_last_not_of(ws);
	return s.substr(start, end - start + 1);
}

//Structure for Courses and their information
struct Course {
	string courseNumber;
	string courseTitle;
	list<string> prerequisites;
};

//Structure to define BST Nodes
struct Node {
	Course course;
	Node* left;
	Node* right;

	//Default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	//Constructor that initializes a node with course
	Node(Course course) :
		Node() {
			this->course = course;
		}
	

	
};

//Class for the BST and its related functions
class BinarySearchTree {

private:
	Node* root;
	void ValidatePrerequisites(Node* node);
	void PrintAll(Node* node);
public:
	BinarySearchTree();
	void InsertCourse(Course course);
	bool PrintCourse();
    bool SearchCourse(const string& courseNumber) const;
	void PrintAll();
	void ValidatePrerequisites();
};

//BST defualt constructor
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

//BST Insert function 
void BinarySearchTree::InsertCourse(Course course) {
	
	//checks if root is null and creates new node
	if (root == nullptr) {
		root = new Node(course);
		return;
	}
	//creates current and sets it to the root
	Node* current = root;

	while (true) {
		//path to go left
		if (course.courseNumber < current->course.courseNumber) {
			if (current->left == nullptr) {
				current->left = new Node(course);
				return;
			}
			//sewts current to left
			current = current->left;
		}
		//path to go right
		else if (course.courseNumber > current->course.courseNumber) {
			if (current->right == nullptr) {
				current->right = new Node(course);
				return;
			}
			//sets current to the right
			current = current->right;
		}
		else {
			return;
		}
	}

}

//public calling for the validation function
void BinarySearchTree::ValidatePrerequisites()
{
	ValidatePrerequisites(root);
}

void BinarySearchTree::ValidatePrerequisites(Node* node) {
	//checks if node is null and then returns if it is
	if (node == nullptr) {
		return;
	}


		//travels down the tree to the left
		ValidatePrerequisites(node->left);

		//calls search function for prereq
		for (const string& prereq : node->course.prerequisites) {
			if (!SearchCourse(prereq)) {
				cout << "Invalid prerequisite: " << prereq << endl;
			}
		}

		//travels down the right
		ValidatePrerequisites(node->right);
	
}

bool BinarySearchTree::SearchCourse(const string& courseNumber) const {

	//creates and sets current to be the root
	Node* current = root;

	while (current != nullptr) {
		if (courseNumber == current->course.courseNumber) {

			// iterate through list to find prerequiasites
			if (!current->course.prerequisites.empty()) {
				for (const auto& prereq : current->course.prerequisites) {
					
				}
			}
			//if no prereq
			else {
				cout << "(no prerequisites)" << endl;
			}

			return true;
		}
		//travels down left
		else if (courseNumber < current->course.courseNumber) {
			current = current->left;
		}
		//travels down right
		else {
			current = current->right;
		}
	}

	return false;
}

//BST print single course function 
bool BinarySearchTree::PrintCourse() {
	
	//sets current node to the root
    Node* current = root;
	
	//initializes string for courseNumber
	string courseNumber;

	//asks for user input 
	cout << "Enter the course number." << endl;
	cin >> courseNumber;

	//loop that goes through tree as long as current is not null
    while (current != nullptr) {

		//loop that checks if courseNumber is the same as current node
        if (courseNumber == current->course.courseNumber) {
			cout << "Course Number: " << current->course.courseNumber << endl;
            cout << "Course Title: " << current->course.courseTitle << endl;

            // Print prerequisites (iterate list<string>)
            if (!current->course.prerequisites.empty()) {
				cout << "Prerequisites: " << endl;
                for (const auto& prereq : current->course.prerequisites) {
					cout << prereq << endl;
                }
                cout << endl;
            } 
			//outputs if there are no prereqs
			else {
                cout << "(no prerequisites)" << endl;
            }

            return true;
        } 
		//travels down to the left
		else if (courseNumber < current->course.courseNumber) {
            current = current->left;
        } 
		//travels down to the right
		else {
            current = current->right;
        }
    }

    return false;
}

//BST print list of courses function

//public call to the printall function
void BinarySearchTree::PrintAll() {
	PrintAll(root);
}

//private member
void BinarySearchTree::PrintAll(Node* node) {

	//checks if node is not null
	//recursively travels the tree printing in order
	if (node != nullptr) {
		PrintAll(node->left);
		cout << node->course.courseNumber << " , " << node->course.courseTitle << endl;
		PrintAll(node->right);
	}
}



//Function to load and parse files with course info 
void LoadFile(const std::string& fileName, BinarySearchTree& bst) {

	//opens file
	ifstream file(fileName);

	//checks if file opened
	if (!file.is_open()) {
		cerr << "Error: " << fileName << " could not be opened. " << endl;
		return;

	}
	//creates a string for each line
	string line;

	//loops through file and gets lines
	while (getline(file, line)) {
		stringstream ss(line);
		vector<string> tokens;
		string token;

		

		//loop to parse lines by commas and adds them to token vector
		while (getline(ss, token, ',')) {
			tokens.push_back(token);
		}

		// trim tokens to remove extra whitespace and CR characters
		for (auto &t : tokens) t = trim(t);

		//checks if there are enough tokens 
		if (tokens.size() < 2) {
			cout << "Not enough parameters." << endl;
			continue;
		}

		//creates course object and assigns values to tokens
		Course course;
		course.courseNumber = tokens[0];
		course.courseTitle = tokens[1];

		//loop to assign prereqs to token value and add them to course object
		for (size_t i = 2; i < tokens.size(); ++i) {
			course.prerequisites.push_back(tokens[i]);
		}

		//adds course object into the BST
		bst.InsertCourse(course);

	}
	//closes file
	file.close();

}

//function to display main menu and call other functions
void MenuDisplay() {

	int choice = 0;
	BinarySearchTree bst;

	//loop to keep menu open until user enters 9
	while (choice != 9) {

		cout << "1. Load file. " << endl;
		cout << "2. Print Course List. " << endl;
		cout << "3. Print Course." << endl;
		cout << "9. Exit Program." << endl;
		cout << endl;
		cout << "Enter the number for the option you would like to do." << endl;
		cin >> choice;

		//switch case to handle the user input
		//and calls all required functions
		switch (choice) {

			//used to load and parse file, as well as check prereqs
		case 1: {
			LoadFile("CS 300 ABCU_Advising_Program_Input.csv", bst);
			bst.ValidatePrerequisites();
			break;
		}

			  //used to print a list of all courses 
		case 2:
			bst.PrintAll();
			break;

			//used to print a single course
		case 3:
			bst.PrintCourse();
			break;

			//handles invalid inputs
		default:
			cout << "Invalid input, please enter a valid option." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

	}
	cout << "GoodBye" << endl;
}




//Main function
int main() {
	MenuDisplay();
}