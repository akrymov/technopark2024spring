#include <iostream>



template <typename T>
struct DefaultComparator {
    int operator()(const T& l, const T& r) const {
        if (l < r) return -1;
        if (l > r) return 1;
        return 0;
    }
};

template <typename Key, typename Value, typename Comparator=DefaultComparator<Key> >
class AVLTree {
    struct Node {
        Node* left;
        Node* right;

        Key key;
        Value value;

        uint8_t height;

        Node(const Key& key, const Value& value):
            left(nullptr), right(nullptr),
            key(key), value(value), height(1) {
        }
        ~Node() {
            //delete left;
            //delete right;
        }
    };

public:
    AVLTree(Comparator comp = Comparator()) :
        root(nullptr),
        items_count(0),
        comp(comp) {

    }

    ~AVLTree() {
        //delete root;
    }

    size_t size() const { return items_count; }

    Value* find(const Key& key) {
        return find_aux(key, root);
    }

    void insert(const Key& key, const Value& Value) {
        root = insert_aux(key, value, root);
    }

    void erase(const Key& key) {
        root = erase_aux(key, root);
    }

private:
    Value* find_aux(const Key& key, Node* node) {
        if (!node) {
            return nullptr;
        }
        int cmp_res = comp(key, node->key);
        if (cmp_res == -1) { //key < node->key
            return find_aux(key, node->left);
        } else if (cmp_res == 1) { //key > node->key
            return find_aux(key, node->right);
        }
        //key == node->key
        return &node->value;
    }

    Node* insert_aux(const Key& key, const Value& Value, Node* node) {
        if (!node) {
            items_count++;
            return new Node(key, value);
        }
        int cmp_res = comp(key, node->key);
        if (cmp_res == -1) { //key < node->key
            node->left = insert_aux(key, value, node->left);
        } else if (cmp_res == 1) { //key > node->key
            node->right = insert_aux(key, value, node->right);
        }
        //key == node->key
        return balance(node);
    }

    Node* erase_aux(const Key& key, Node* node) {
        if (!node) {
            return nullptr;
        }

        int cmp_res = comp(key, node->key);
        if (cmp_res == -1) { //key < node->key
            node->left = erase_aux(key, node->left);
        } else if (cmp_res == 1) { //key > node->key
            node->right = erase_aux(key, node->right);
        } else { //key == node->key
            Node* left = node->left;
            Node* right = node->right;

            delete node;
            items_count--;

            if (!right) {
                return left;
            }

            // В ДЗ ДЕЛАТЬ ОДНОЙ ФУНКЦИЕЙ find_and_remove_min_node
            Node* min_node = find_min(right);
            right = remove_min_node(right);

            min_node->left = left;
            min_node->right = right;

            return balance(min_node);
        }
        return balance(node);
    }

    Node* remove_min_node(Node* node) {
        if (!node->left) {
            return node->right;
        }

        node->left = remove_min_node(node->left);
        return balance(node);
    }

    Node* find_min(Node* node) {
        if (!node->left) {
            return node;
        }
        return find_min(node->left);
    }

    uint8_t height(Node* node) {
        if (!node) {
            return 0;
        }
        return node->height;
    }

    void fix_height(Node* node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
        //
    }

    int bfactor(Node* node) {
        return height(node->right) - height(node->left);
    }

    Node* balance(Node* node) {
        fix_height(node);

        int bf = bfactor(node);
        if (bf == 2) {
            if (bfactor(node->right) < 0) {
                node->right = rotate_right(node->right);
            }
            return rotate_left(node);
        }

        if (bf == -2) {
            if (bfactor(node->left) > 0) {
                node->left = rotate_left(node->left);
            }
            return rotate_right(node);
        }

        return node;
    }

    Node* rotate_left(Node* node);
    Node* rotate_right(Node* node);


    Node* root;
    size_t items_count;
    Comparator comp;
};

template <typename Key>
class BTree
{
public:
    struct Node
    {
        Node(bool leaf)
        : leaf(leaf)
        {
        }

        ~Node()
        {
            for (Node* child: children)
            {
                delete child;
            }
        }

        bool leaf;
        std::vector<Key> keys; //t-1, 2t-1, в корне от 1 до 2t-1
        std::vector<Node*> children; // 2t
    };

    BTree(size_t min_degree)
    : root(nullptr), t(min_degree)
    {
        assert(min_degree >= 2);
    }

    ~BTree()
    {
        if (root)
            delete root;
    }

    void insert(const Key &key)
    {
        if (!root)
            root = new Node(true);

        if (is_node_full(root))
        {
            Node *newRoot = new Node(false);
            newRoot->children.push_back(root);
            root = newRoot;
            split(root, 0);
        }

        insert_non_full(root, key);
    }

    bool find(const Key &key)
    {
        return find_aux(root, key);
    }

private:
    bool is_node_full(Node *node)
    {
        return node->keys.size() == 2*t - 1;
    }

    bool find_aux(Node* node, const Key &key)
    {
        int i = 0;
        while (i < node->keys.size() && key > node->keys[i])
            i++;

        if (i < node->keys.size() && key == node->keys[i])
            return true;
        else if (node->leaf)
            return false;
        else
            return find_aux(node->children[i], key);
    }

    void split(Node *node, size_t index);


    void insert_non_full(Node *node, const Key &key)
    {
        int pos = node->keys.size() - 1;
        if (node->leaf)
        {
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && key < node->keys[pos])
            {
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            node->keys[pos + 1] = key;
        }
        else
        {
            while (pos >= 0 && key < node->keys[pos])
            {
                pos--;
            }

            if (is_node_full(node->children[pos + 1]))
            {
                split(node, pos + 1);
                if (key > node->keys[pos + 1])
                {
                    pos++;
                }
            }
            insert_non_full(node->children[pos + 1], key);
        }
    }




    Node *root;
    size_t t; // minimum degree

};

int main() {

}
