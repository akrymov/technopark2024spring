#include <iostream>
#include <assert.h>
#include <sstream>

class Queue {
    struct Node {
        Node* next;
        int value;

        Node(int value) : next(nullptr), value(value) {
        }
    };
public:
    Queue() : tail(nullptr), head(nullptr) {
    }

    ~Queue() {
        //std::cout << "~Queue" << std::endl;
        while (!is_empty()) {
            pop();
        }
    }

    Queue(const Queue &r) : head(nullptr), tail(nullptr) {
        //std::cout << "Queue(const Queue &r)" << std::endl;
        const Node *cur = r.head;
        while (cur)
        {
            const Node *node = cur;
            push(node->value);
            cur = cur->next;
        }
    }

    Queue& operator=(const Queue &r) {
        //std::cout << "Queue& operator=(const Queue &r)" << std::endl;
        while (!is_empty())
        {
            pop();
        }
        const Node *cur = r.head;
        while (cur)
        {
            const Node *node = cur;
            push(node->value);
            cur = cur->next;
        }
        return *this;
    }

    Queue(Queue &&)=delete;
    Queue &operator=(Queue&&)=delete;

    bool is_empty() const {
        return head == tail && head == nullptr;
    }

    void push(int value) {
        Node* node = new Node(value);
        if (is_empty()) {
            head = node;
            tail = node;
            return;
        }

        tail->next = node;
        tail = node;
    }

    int pop() {
        Node* node = head;
        head = head->next;

        if (head == nullptr) {
            tail = nullptr;
        }

        int value = node->value;
        delete node;

        return value;
    }

private:
    Node* head;
    Node* tail;
};


void testQueue() {
    {
        Queue q;
        assert(q.is_empty());
    }
    {
        Queue* q = new Queue();
        assert(q->is_empty());
        delete q;
    }
    //std::cout << "----" << std::endl;
    {
        Queue q;
        q.push(66);
        assert(!q.is_empty());
        assert(q.pop() == 66);
        assert(q.is_empty());
    }

    {
        Queue q;
        for (int i = 0; i < 10; ++i) {
            q.push(i);
        }
        for (int i = 0; i < 10; ++i) {
            assert(q.pop() == i);
        }

        assert(q.is_empty());
    }
    {
        Queue q;
        for (int i = 0; i < 10; ++i)
        {
            q.push(i);
        }
        Queue q2 = q;
        for (int i = 0; i < 10; ++i)
        {
            assert(q2.pop() == i);
        }
        for (int i = 0; i < 10; ++i)
        {
            assert(q.pop() == i);
        }

        assert(q.is_empty());

        //Queue q3 = std::move(q);
    }
    {
        Queue q;
        for (int i = 0; i < 10; ++i)
        {
            q.push(i);
        }
        Queue q2;
        q2 = q;
        for (int i = 0; i < 10; ++i)
        {
            assert(q2.pop() == i);
        }
        for (int i = 0; i < 10; ++i)
        {
            assert(q.pop() == i);
        }

        assert(q.is_empty());
    }
    std::cout << "testLogic OK" << std::endl;
}

void run(std::istream& in, std::ostream& out) {
    Queue q;
    int command = -1;
    int input_value = -1;
    int n = 0;
    in >> n;
    for (int i = 0; i < n; ++i)
    {
        in >> command >> input_value;
        int operation_value = -1;
        switch (command)
        {
        case 2: // pop
            if (q.is_empty())
            {
                operation_value = -1;
            }
            else
            {
                operation_value = q.pop();
            }
            if (operation_value != input_value)
            {
                out << "NO" << std::endl;
                return;
            }
            break;
        case 3: // push
            q.push(input_value);
            break;
        }
    }
    out << "YES" << std::endl;
}

void testLogic() {
    {
        std::stringstream in;
        std::stringstream out;

        in << "3\n";
        in << "3 44 \n";
        in << "3 50 \n";
        in << "2 44\n";
        run(in, out);
        assert(out.str() == "YES\n");
    }
    std::cout << "testLogic OK" << std::endl;
}


int main() {

    //testQueue();
    //testLogic();
    run(std::cin, std::cout);
}
