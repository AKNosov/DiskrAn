#include <iostream>
#include <fstream>
#include <algorithm> 


using namespace std;

struct KV {
    string key;         
    unsigned long long int val;  

};

struct TNode {
    KV data;   
    int height;    
    TNode* left;
    TNode* right;
    TNode(KV d)
        : data(d), height(1), left(nullptr), right(nullptr) {}
};

class TAVLTree {

public:

    bool IsLoading = false;

    TNode* root;

    TAVLTree() : root(nullptr) {}

    ~TAVLTree() {
        Clear(root);
    }

    void Save(ofstream& file, TNode* node) {
        if (node) {
            file << node->data.key << " " << node->data.val << endl;
            Save(file, node->left);
            Save(file, node->right);
        }
    }

    TNode* Load(ifstream& file, TNode* node) {
        Clear(node);
        node = nullptr;
        KV kv;
        IsLoading = true;
        while (file >> kv.key >> kv.val) {
            node = Insert(node, kv);
        }
        IsLoading = false;
        return node;
    }


    TNode* Insert(TNode* node, KV d) {
        if (!node) {
            if (!IsLoading){
                cout << "OK" << endl;
            }
            return new TNode(d);
        }
        if (d.key == node->data.key) {
            cout << "Exist" << endl;
            return node;
        }
        if (d.key < node->data.key){
            node->left = Insert(node->left, d);
        } else {
            node->right = Insert(node->right, d);
        }
        return Balance(node);
    }

    TNode* Remove(TNode* node, const string& key) {
        if (node == nullptr){
            cout << "NoSuchWord" << endl;
            return nullptr;
        }
        if (key == node->data.key) {
            TNode* l = node->left;
            TNode* r = node->right;
            delete node;
            cout << "OK" << endl;
            if (r == nullptr){
                return l;
            }
            TNode* min = FindMin(r);
            min->right = RemoveMin(r);
            min->left = l;
            return Balance(min);
        }
        if (key < node->data.key){
            node->left = Remove(node->left, key);
        } else { 
            node->right = Remove(node->right, key);
        } 
        return Balance(node);
    }

    TNode* Find(TNode* node, const string& key) {
        if (!node) {
            return nullptr;
        }
        if (node->data.key > key) {
            return Find(node->left, key);
        }
        else if (node->data.key < key) {
            return Find(node->right, key);
        }
        else {
            return node;
        }
        return nullptr;
    }


private:

    void Clear(TNode* node) {
        if (node) {
            Clear(node->left);
            Clear(node->right);
            delete node;
        }
    }

    int Height(TNode* node) {
        return node ? node->height : 0;
    }

    int BalanceF(TNode* node) {
        return Height(node->right) - Height(node->left);
    }

    void FixHeight(TNode* node) {
        int hl = Height(node->left);
        int hr = Height(node->right);
        node->height = max(hl, hr) + 1;
    }

    TNode* RotateLeft(TNode* x) {
        TNode* y = x->right;
        x->right = y->left;
        y->left = x;
        FixHeight(x);
        FixHeight(y);
        return y;
    }

    TNode* RotateRight(TNode* y) {
        TNode* x = y->left;
        y->left = x->right;
        x->right = y;
        FixHeight(y);
        FixHeight(x);
        return x;
    }

    TNode* Balance(TNode* node) {
        FixHeight(node);
        if (BalanceF(node) == 2) {
            if (BalanceF(node->right) < 0)
                node->right = RotateRight(node->right);
            return RotateLeft(node);
        }
        if (BalanceF(node) == -2) {
            if (BalanceF(node->left) > 0)
                node->left = RotateLeft(node->left);
            return RotateRight(node);
        }
        return node;
    }

    TNode* FindMin(TNode* node) {
        return node->left ? FindMin(node->left) : node;
    }

    TNode* RemoveMin(TNode* node) {
        if (node->left == nullptr)
            return node->right;
        node->left = RemoveMin(node->left);
        return Balance(node);
    }

    string DoLower(string str){
        for (int i = 0; i < str.size(); ++i){
            str[i] = tolower(str[i]);
        }
        return str;
    }
};


int main()
{
    TAVLTree tree;
    string req;
    while (cin >> req) {
        if (req == "+") {
            KV el;
            cin >> el.key >> el.val;
            transform(el.key.begin(), el.key.end(), el.key.begin(), ::tolower);
            tree.root = tree.Insert(tree.root, el);
        } else if (req[0] == '-') {
            string key;
            cin >> key;
            transform(key.begin(), key.end(), key.begin(), ::tolower);
            tree.root = tree.Remove(tree.root, key);

        } else if (req == "!") {
            string comand;
            string path;
            cin >> comand >> path;
            if (comand == "Save"){
                ofstream file;
                file.open(path);
                tree.Save(file, tree.root);
                cout << "OK" << endl;
                file.close();
            } else if (comand == "Load"){
                ifstream file;
                file.open(path);
                tree.root = tree.Load(file, tree.root);
                cout << "OK" << endl;
                file.close();
            }
        } else {
            transform(req.begin(), req.end(), req.begin(), ::tolower);
            TNode* fn = tree.Find(tree.root, req);    
            if (fn == nullptr){
                std::cout << "NoSuchWord\n";
            } else {
                std::cout << "OK: " << fn->data.val << endl;
            }   
        }
    }
    return 0;
}
