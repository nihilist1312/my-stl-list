#include <concepts>
#include <iostream>
#include <string>

using std::cout;

template <class T>
concept Eq = requires(T a, T b) {
    { a == b } -> std::convertible_to<bool>;
};

struct God {
    std::string name;
    std::string data;
    God(const std::string& n = "", const std::string& d = "")
        : name(n), data(d) {}
};

bool operator==(const God& a, const God& b) { return a.name == b.name; }

template <class T> struct List {
    struct Node {
        Node* prev;
        Node* next;

        T data;

        Node(const T& d, Node* p = nullptr, Node* s = nullptr)
            : data(d), prev(p), next(s) {}
    };

    void push_front(Node* l);
    void push_back(Node* l);

    static Node* find_from(const T& target, Node* from);
    Node* find(const T& target) { return find_from(target, head_); }
    Node* find_tail(const T& target) { return find_from(target, tail_); }
    const Node* find(const T& target) const;

    void remove(Node* target);
    void remove(const T& target) { remove(find(target)); }
    void insert(Node* to, Node* l);

    void push_back(const T& g) { push_back(new Node{g}); }
    void push_front(const T& g) { push_front(new Node{g}); }
    void insert(Node* to, const T& g) { insert(to, new Node{g}); }

    int size() const { return size_; }
    Node* begin() { return head_; }
    Node* end() { return tail_; }
    const Node* begin() const { return head_; }
    const Node* end() const { return tail_; }

    ~List<T>();

  private:
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    int size_ = 0;
};

template <class T> void List<T>::push_front(Node* l) {
    if (l == nullptr)
        return;

    if (head_ == nullptr) {
        head_ = l;
        tail_ = l;
        size_ = 1;
        return;
    }

    l->next = head_;
    l->prev = nullptr;
    head_->prev = l;
    head_ = l;
    ++size_;
}

template <class T> void List<T>::push_back(Node* l) {
    if (l == nullptr)
        return;

    if (tail_ == nullptr) {
        head_ = l;
        tail_ = l;
        size_ = 1;
        return;
    }

    l->next = nullptr;
    l->prev = tail_;
    tail_->next = l;
    tail_ = l;
    ++size_;
}

template <class T>
List<T>::Node* List<T>::find_from(const T& target, List<T>::Node* from) {
    if (from == nullptr)
        return nullptr;
    if (from->data == target)
        return from;

    List<T>::Node* forward = from->next;
    List<T>::Node* back = from->prev;
    while (true) {
        if (forward) {
            if (forward->data == target)
                return forward;
            forward = forward->next;
        }
        if (back) {
            if (back->data == target)
                return back;
            back = back->prev;
        }
        if (!forward && !back)
            return nullptr;
    }
}

template <class T> const List<T>::Node* List<T>::find(const T& target) const {
    if (head_ == nullptr)
        return nullptr;
    const List<T>::Node* curr = head_;
    while (curr) {
        if (curr->data == target)
            return curr;
        curr = curr->next;
    }
    return nullptr;
}

template <class T> void List<T>::insert(List<T>::Node* to, List<T>::Node* l) {
    if (to == nullptr || l == nullptr)
        return;
    l->next = to->next;
    l->prev = to;
    to->next = l;
    ++size_;
}

template <class T> void List<T>::remove(List<T>::Node* target) {
    target->prev->next = target->next;
    target->next->prev = target->prev;
    delete target;
}

template <class T> List<T>::~List<T>() {
    List<T>::Node* curr = head_;
    while (curr) {
        List<T>::Node* t = curr->next;
        delete curr;
        curr = t;
    }
}

template <class T> void print(const List<T>& l) {
    const typename List<T>::Node* curr = l.begin();
    while (curr) {
        cout << curr->data.name << ' ';
        curr = curr->next;
    }
    cout << '\n';
}

int main() {
    List<God> l;
    l.push_back({"thor"});
    l.push_back({"loki"});
    l.push_front({"odin"});

    print(l);

    return 0;
}
