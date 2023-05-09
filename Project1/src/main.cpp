#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>
using namespace std;

struct Student {
    Student() : id(""), name(""), left(nullptr), right(nullptr) {}
    Student(string _id, string _name, Student* _left, Student* _right)
        : id(_id), name(_name), left(_left), right(_right) {}
    string id;
    string name;
    Student* left;
    Student* right;
	int height = 1; 
	int balance_factor = 0;
};

struct MyAVLTree {
    MyAVLTree() {
        root = nullptr;
		// parse input whenever a tree is created
		parseInput();
    }

    void parseInput() {
        int num_lines;
        string num, line;

        getline(cin, num); 
        num_lines = stoi(num);
        
        while (num_lines > 0) {
			// cout << "on iteration: " << num_lines << endl;
            getline(cin, line);
            istringstream in(line);

            string command;
            in >> command;
			// now you can call the corresponding command in your tree
			if (command == "insert"){
				string next;
				getline(in, next, '"');

				string name;
				getline(in, name, '"');

				string id;
				getline(in, id);
				id = id.substr(1);
				//cout << "command: " << command << " name: " << name << " id: " << id << endl;
				insert(name, id);

					
			}
			else if (command == "search"){
				// check if im searching for a name or an id
				string id_or_name;
            	in >> id_or_name;

				if (isdigit(id_or_name[0])) { // check if the id is a number
					//cout << "command: " << command << " id: " << id_or_name << endl;
                	searchID(id_or_name);
            	}
				else {
					id_or_name = id_or_name.substr(1, id_or_name.size() - 2);
					//cout << "command: " << command << " id: " << id_or_name << endl;
					searchName(id_or_name);
				}
			}
			else if(command == "printInorder"){
				printInorder();
			}
			else if (command == "printPreorder"){
				printPreorder();
			}
			else if(command == "printPostorder"){
				printPostorder();
			}
			else if(command == "printLevelCount"){
				printLevelCount();
			}
			else if(command == "remove"){
				
				string id;
            	in >> id;
				remove(id);

			}
			else if(command == "removeInorder"){
				string n;
				in >> n;
				int N = stoi(n);
				removeInorder(N); 
			}
			num_lines--;

            
        }
    }
	// get levels by getting height
	int getLvlCount(Student* root) {
		if (root == NULL) 
			return 0;
		else {
			int left_height = getLvlCount(root->left);
			int right_height = getLvlCount(root->right);
			
			return max(left_height, right_height) + 1;	
		}
	}
	int getHeight(Student* root) {
		// change to 0 since its initalized to 1 in struct
        if (root == NULL) 
            return 0;
        else {
            return root->height;
        }
    }
	// need to update after rotating
	int updateHeight(Student* root) {
		// new height will be +1 from the left and rights
        int left_height = getHeight(root->left);
        int right_height = getHeight(root->right);
        root->height = max(left_height, right_height) + 1;
        return root->height;
    }
	// i copied my rotations from Stepik as Aman suggested
	Student* left_rotate(Student* node){
		Student* new_root = node->right;
    	Student* storage = new_root-> left;
		new_root->left = node;
		node-> right = storage;
		// updating height as sugggested by lecture notes pseudocode
		updateHeight(node);
        updateHeight(new_root);
		return new_root;

	}
	Student* right_rotate(Student* node){
		Student* new_root = node->left;
		Student* strg = new_root -> right;
		new_root-> right = node;
		node-> left = strg;
		updateHeight(node);
        updateHeight(new_root);
		return new_root; 

	}
	Student* left_right(Student* node){
		node-> left = left_rotate(node->left);
    	return right_rotate(node);

	}
	Student* right_left(Student* node){
		node -> right = right_rotate(node->right);
		return left_rotate(node);
	}
	bool validateName(string name){
		//cout << "name trying to insert:" << name << endl;
		// should only be letter or space
		for(int i = 0; i < name.size(); i++) {
			if(!isalpha(name[i]) && !isspace(name[i])){
				return false;
			}
		}
		return true;

	}
	// id oughta be 8 digits lonng
	bool validateID(string id){
		//cout << "id trying to insert:" << id << endl;
		if(id.length() != 8){

			return false;
		}
		// oughta only be digits
		for(int i=0; i< id.length()-1;i++){
			if(!isdigit(id[i])){
				return false;
			}
		}
		return true;
	}
	// traverse to find duplicate
	bool findDuplucates(Student* root, string target_id){
		if (!root){
			return false;
		}
		if (root->id == target_id){
			return true;
		}
		if (stoi(target_id) > stoi(root->id)){
			return findDuplucates(root->right, target_id);
		}
		else {
			return findDuplucates(root->left, target_id);
		}
	}
	void insert(string name, string id) {
		// make sure name and id are valid
		if(!validateName(name) || !validateID(id)) {
			cout << "unsuccessful" << endl;
			return; 
		}
		// make sure there are no duplicate ids 
		if(findDuplucates(root, id)) {
			cout << "unsuccessful" << endl;
			return; 
		}
		
		root = insertHelper(root, id, name);
		cout << "successful" << endl;
	}

	Student* insertHelper(Student* root, string id, string name) {
		// if root is null create a new student node and return
		if(root == nullptr) {
			return new Student(id, name, nullptr, nullptr);
		}
		
		// check if the passed in id already in the tree
		if(stoi(id) == stoi(root->id)) {
			cout << "unsuccessful" << endl;
			return root;
		}
		
		// find place in left subtree
		if(stoi(id) < stoi(root->id)) {
			root->left = insertHelper(root->left, id, name);
		}
		// find place in right subtree
		else {
			root->right = insertHelper(root->right, id, name);
		}
		
		// update height find balance factor
		root->height = 1 + max(getHeight(root->left), getHeight(root->right));
		root->balance_factor = getHeight(root->left) - getHeight(root->right);

		// Check if the tree is balanced, and perform rotations if necessary

		// left-heavy
		if(root->balance_factor > 1) { 
			// left left align
			if(stoi(id) < stoi(root->left->id)) { 
				return right_rotate(root);
			}
			else { 
				root->left = left_rotate(root->left);
				return right_rotate(root);
			}
		}
		// check if right heavy
		else if(root->balance_factor < -1) { 
			// right right alignment
			if(stoi(id) > stoi(root->right->id)) { 
				return left_rotate(root);
			}
			else {
				root->right = right_rotate(root->right);
				return left_rotate(root);
			}
		}
		return root;
	}
	// preorder travsere to find id
	string searchIDHelp(Student* root, string id){
		if(!root){
			// give a string that tells if it doesnt exist
			return " ";
		}
		if(root-> id == id){
			return root->name; 
		}
		
		else if(stoi(root->id) > stoi(id)){
			return searchIDHelp(root->left, id);
		}
		else if(stoi(root->id) < stoi(id)){
			return searchIDHelp(root->right, id);
		}
		return " ";

	}
    void searchID(string id) {
		string result = searchIDHelp(root, id);
		if(result == " "){
			cout << "unsuccessful" << endl;
		}
		else{
			cout << result << endl;
		}
    }
	// preorder travserse to find name
	void searchNameHelp(Student* root, string target_name, vector<string> &names_found){
		if(!root){
			return;
		}
		// add to vector in preorder order
		if(root->name == target_name){
			names_found.push_back(root->id);
		}
		searchNameHelp(root->left, target_name, names_found);
		searchNameHelp(root->right, target_name, names_found);
		
	}

    void searchName(string name) {
		vector<string> names_found;	
		searchNameHelp(root, name, names_found);
		if(names_found.size() == 0){
			cout << "unsuccessful" << endl;
		}
		// get the associated ids frrom vector
		else{
			for(int i = 0; i < names_found.size(); i++){
				cout << names_found[i] << endl;
			}
		}
		
    }
	void inorder(Student* root, vector<string> &inorder_nodes){
		if(!root){
			return;
		}
		inorder(root->left,inorder_nodes);
		inorder_nodes.push_back(root->name);
		inorder(root->right,inorder_nodes);

	}
    void printInorder() {
        vector<string> inorder_nodes;
		inorder(root, inorder_nodes);
		for (auto it = inorder_nodes.begin(); it != inorder_nodes.end(); it++) {
        	cout << *it;
			if (it != inorder_nodes.end() - 1) {
				cout << ", ";
			}
    	}
		cout << "\n";

    }
	void preorder(Student* root, vector<string> &preorder_nodes){
		if(!root){
			return;
		}
		preorder_nodes.push_back(root->name);
		preorder(root->left, preorder_nodes);
		preorder(root->right,preorder_nodes);

	}
    void printPreorder() {
        // printPreorder implementation
		vector<string> preorder_nodes;
		preorder(root, preorder_nodes);
		for (auto it = preorder_nodes.begin(); it != preorder_nodes.end(); it++) {
        	cout << *it;
			if (it != preorder_nodes.end() - 1) {
				cout << ", ";
			}
    	}
		cout << "\n";

		
    }
	void postorder(Student* root, vector<string> &postorder_nodes){
		if(!root){
			return;
		}
		postorder(root->left, postorder_nodes);
		postorder(root->right,postorder_nodes);
		postorder_nodes.push_back(root->name);
	}

    void printPostorder() {
        // printPostorder implementation
		vector<string> postorder_nodes;
		postorder(root, postorder_nodes);
		for (auto it = postorder_nodes.begin(); it != postorder_nodes.end(); it++) {
        	cout << *it;
			if (it != postorder_nodes.end() - 1) {
				cout << ", ";
			}
    	}
		cout << "\n";
		
    }

    void printLevelCount() {
        // printLvlCount implementation
		int level_count = getLvlCount(root);
		cout << level_count << endl;
    }

	Student* removeHelper(Student* node, string id){
		if (!node) {
			return nullptr; 
		}
		// search for student node to remove
		if (stoi(id) < stoi(node->id)) {
			node->left = removeHelper(node->left, id);
		} 
		else if (stoi(id) > stoi(node->id)) {
			node->right = removeHelper(node->right, id);
		} 
		// if we get here it exists
		else {
			// node has no children
			if (!node->left && !node->right) {
				delete node;
				return nullptr;
			} 
			// only right child
			else if (!node->left) {
				
				// remove and make right new right
				Student* temp = node->right;
				delete node;
				return temp;
			} 
			// only has left child
			else if (!node->right) {
				Student* temp = node->left;
				delete node;
				return temp;
			} 
			// node has two children, replace with the inorder successor
			else {
				// get the inorder successor
				Student* successor = node->right;
				while (successor->left) {
					successor = successor->left;
				}
				// transfer info
				node->id = successor->id;
				node->name = successor->name; 
				node->right = removeHelper(node->right, successor->id);
			}
				
		}
		return node;
    }

	void remove(string id) {
		// make sure its there
		string result = searchIDHelp(root, id);
		if(result == " "){
			cout << "unsuccessful" << endl;
			return;
		}
		else{
			root = removeHelper(root, id);
			cout << "successful" << endl;
		}
	}

	void inorderRemover(Student* node, vector<Student*> &nodes, int &count, int N){
		if(!node){
			return;
		}
		// inorder traversal structure
		inorderRemover(node->left,nodes,count,N);
		nodes.push_back(node);
		count++;
		// go until we get to N, starts at 0
		if(count == N+1){
			return;
		}
		inorderRemover(node->right,nodes,count,N);
	}
	void removeInorder(int N){
		int count = 0;
		// get a vector to pass by reference
		vector<Student*> inorder_nodes;
		inorderRemover(root,inorder_nodes,count,N);
		// for(int i=0; i < inorder_nodes.size();i++){
		// 	cout << inorder_nodes[i]->name << " ";
		// }
		// if the count is less than the number passed in, we never got to N
		if(count < N){
			cout << "unsuccessful" << endl;
			return;
		}
		Student* nth_node = inorder_nodes[N];
		remove(nth_node->id);
		//cout << "successfull" << endl;
	}
	// root 
    Student* root;
};

int main() {
    MyAVLTree t;
    return 0;
}
