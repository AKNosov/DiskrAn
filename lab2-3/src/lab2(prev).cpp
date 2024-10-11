#include <iostream>
#include <string>
#include <fstream>

using namespace std;

enum TColor { RED, BLACK };

struct KV
{
    string key;
    unsigned long long int val;
};

struct TNode 
{
    KV data;
    TColor color;
    TNode *left, *right, *parent;
    TNode(KV d)
    {
        data = d;
        parent = nullptr;
        left = nullptr;
        right = nullptr;
        color = RED;
    };
};

class TRBTree 
{
private:
    TNode* root;
    TNode* NIL;
    void LeftRotate(TNode* x);
    void RightRotate(TNode* y);
    void InsertFixup(TNode* z);
    void RemoveFixup(TNode* x);
    void Transplant(TNode* u, TNode* v);
    void RecSave(TNode* node, ofstream& file);
    TNode* Minimum(TNode* node);
    int Compare(const string& a, const string& b) const;

public:
    TRBTree();
    ~TRBTree();
    void Insert(KV);
    bool Remove(const string& key);
    void Find(const string& key) const;
    void Save(ofstream& file);
    void Load(ifstream& file);
    void Destroy(TNode* node);
};

TRBTree::TRBTree() {
    KV d;
    NIL = new TNode(d);
    NIL->color = BLACK;
    NIL->left = NIL->right = NIL;
    root = NIL;
}

TRBTree::~TRBTree() {
    if (root != NIL){
        Destroy(root);
        delete NIL;
    }
}

void TRBTree::Destroy(TNode* node) {
    if (node != NIL) {
        Destroy(node->left);
        Destroy(node->right);
        delete node;
    }
}

int TRBTree::Compare(const string& a, const string& b) const {
    int ca = a.length();
    int cb = b.length();
    if (ca < cb){
        return -1;
    }
    if (ca > cb){
        return 1;
    }
    for (int i = 0; i < ca; ++i) {
        if (a[i] < b[i]){
            return -1;
        }
        if (a[i] > b[i]){
            return 1;
        }
    }
    return 0;
}

void TRBTree::LeftRotate(TNode* x) {
    TNode* y = x->right;
    x->right = y->left;
    if (y->left != NIL)
    {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NIL){
        root = y;
    }
    else if (x == x->parent->left){
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void TRBTree::RightRotate(TNode* y) {
    TNode* x = y->left;
    y->left = x->right;
    if (x->right != NIL){
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NIL){
        root = x;
    }
    else if (y == y->parent->right){
        y->parent->right = x;
    }
    else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

void TRBTree::Insert(KV d) {
    TNode* z = new TNode(d);
    z->left = z->right = z->parent = nullptr;
    TNode* y = NIL;
    TNode* x = root;
    int f = 0;
    while (x != NIL) {
        y = x;
        int cmp = Compare(z->data.key, x->data.key);
        if (cmp == 0) {
            delete z;
            cout << "Exist" << endl;
            f = 1;
            break;
        } 
        else if (cmp < 0) {
            x = x->left;
        } 
        else {
            x = x->right;
        }
    }
    if (f == 0) {
        z->parent = y;
        if (y == NIL) {
            root = z;
        } 
        else if (Compare(z->data.key, y->data.key) < 0) {
            y->left = z;
        } 
        else {
            y->right = z;
        }
        z->left = z->right = NIL;
        z->color = RED;
        cout << "OK" << endl;
        InsertFixup(z);
    }   
}

void TRBTree::InsertFixup(TNode* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            TNode* y = z->parent->parent->right; 
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } 
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    LeftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RightRotate(z->parent->parent);
            }
        } 
        else {
            TNode* y = z->parent->parent->left; 
            if (y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } 
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    RightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                LeftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void TRBTree::Transplant(TNode* u, TNode* v) {
    if (u->parent == NIL) {
        root = v;
    } 
    else if (u == u->parent->left){
        u->parent->left = v;
    } 
    else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

TNode* TRBTree::Minimum(TNode* node) {
    while (node->left != NIL) {
        node = node->left;
    }
    return node;
}

bool TRBTree::Remove(const string& key) {
    TNode* z = root;
    while (z != NIL) {
        int cmp = Compare(key, z->data.key);
        if (cmp == 0){
            break;
        } 
        else if (cmp < 0){
            z = z->left;
        } 
        else{
            z = z->right;
        }
    }
    if (z == NIL) {
        return false; 
    }

    TNode* y = z;
    TNode* x;
    TColor yOriginalColor = y->color;
    if (z->left == NIL) {
        x = z->right;
        Transplant(z, z->right);
    } else if (z->right == nullptr) {
        x = z->left;
        Transplant(z, z->left);
    } else {
        y = Minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            Transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        Transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;
    if (yOriginalColor == BLACK) {
        RemoveFixup(x);
    }
    return true; 
}

void TRBTree::RemoveFixup(TNode* x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            TNode* w = x->parent->right; 
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                LeftRotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK){
                w->color = RED;
                x = x->parent;
            } 
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(x->parent);
                x = root;
            }
        } 
        else {
            TNode* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                RightRotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } 
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                RightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

void TRBTree::Find(const string& key) const {
    TNode* current = root;
    while (current != NIL) {
        int cmp = Compare(key, current->data.key);
        if (cmp == 0) {
            cout << "OK: " << current->data.val << endl;
            return;
        } 
        else if (cmp < 0) {
            current = current->left;
        } 
        else {
            current = current->right;
        }
    }
    cout << "NoSuchWord" << endl;

}

void TRBTree::RecSave(TNode* node, ofstream& file) {
    file << node->data.key << " " << node->data.val << endl;
    RecSave(node->left, file);
    RecSave(node->right, file);
}

void TRBTree::Save(ofstream& file) {
    if (root != NIL){
        RecSave(root, file);
    }
}

void TRBTree::Load(ifstream& file) {
    Destroy(root);
    root = NIL;
    KV d;
    while (file >> d.key >> d.val) {
        this->Insert(d);
    }
}


int main()
{
    TRBTree tree;
    string request;
    while (cin >> request) {
        int f = 0;
        if (request == "+") {
            KV el;
            f += 1;
            cin >> el.key >> el.val;
            for (int i = 0; i < el.key.length(); ++i) {
                el.key[i] = tolower(el.key[i]);
            }
            tree.Insert(el);
        }
        if (request == "-") {
            string key;
            cin >> key;
            for (int i = 0; i < key.length(); ++i) {
                key[i] = tolower(key[i]);
            }
            if (tree.Remove(key)) {
                    cout << "OK" << endl;
            } 
            else {
                    cout << "NoSuchWord" << endl;
            }

        }
        else if (request == "!") {
            string command;
            string path;
            cin >> command >> path;
            if (command == "Save")
            {
                ofstream file;
                try 
                {
                    file.open(path);
                }
                catch (exception& ex)
                {
                    cout << "ERROR: " << ex.what() << endl;
                }
                tree.Save(file);
            }
            else if (command == "Load")
            {
                ifstream file;
                try 
                {
                    file.open(path);
                }
                catch (exception& ex)
                {
                    cout << "ERROR: " << ex.what() << endl;
                }
                tree.Load(file);
            }
            std::cout << "OK\n";
        }
        else if (f == 0) {
            for (int i = 0; i < request.length(); ++i) {
                request[i] = tolower(request[i]);
            }
            tree.Find(request);        
        }
    }
    return 0;
}
