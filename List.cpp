#include <cstddef>
#include <initializer_list>
#include <iostream>

template <class T> struct List {
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    class iterator;
    class const_iterator;

    List() : size_(0), sentinel_(new NodeBase{}) {
        sentinel_->next = sentinel_;
        sentinel_->prev = sentinel_;
    }
    List(size_type size, const T& value = T{});
    List(const std::initializer_list<T>& ilist);
    List(const List& other);
    List(List&& other);
    ~List();

    reference front() { return static_cast<Node*>(sentinel_->next)->value; }
    const_reference front() const {
        return static_cast<Node*>(sentinel_->next)->value;
    }
    reference back() { return static_cast<Node*>(sentinel_->prev)->value; }
    const_reference back() const {
        return static_cast<Node*>(sentinel_->prev)->value;
    }

    iterator begin() { return sentinel_->next; }
    const_iterator begin() const { return sentinel_->next; }
    const_iterator cbegin() const { return sentinel_->next; }
    iterator end() { return sentinel_; }
    const_iterator end() const { return sentinel_; }
    const_iterator cend() const { return sentinel_; }

    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }

    void clear() { erase(begin(), end()); }
    iterator insert(const_iterator pos, const T& value);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void push_back(const T& value) { insert(end(), value); }
    void pop_back() { erase(sentinel_->prev); }
    void push_front(const T& value) { insert(begin(), value); }
    void pop_front() { erase(begin()); }
    void resize(size_type new_size, const T& value = T{});

    List& operator=(const List& other);
    List& operator=(List&& other);
    List& operator=(const std::initializer_list<T>& ilist);

  private:
    struct NodeBase;
    struct Node;

    size_type size_;
    NodeBase* sentinel_;
};

template <class T> class List<T>::iterator {
    NodeBase* ptr;

  public:
    using value_type = T;
    using reference = T&;
    using pointer = T*;

    iterator(NodeBase* p) : ptr{p} {}

    reference operator*() const { return static_cast<Node*>(ptr)->value; }
    pointer operator->() const { return &ptr->value; }

    iterator& operator++() {
        ptr = ptr->next;
        return *this;
    }
    iterator operator++(int) {
        iterator tmp = *this;
        ptr = ptr->next;
        return tmp;
    }
    iterator& operator--() {
        ptr = ptr->prev;
        return *this;
    }
    iterator operator--(int) {
        iterator tmp = *this;
        ptr = ptr->prev;
        return tmp;
    }

    bool operator==(const iterator& oth) const noexcept {
        return ptr == oth.ptr;
    }
    bool operator!=(const iterator& oth) const noexcept {
        return !(*this == oth);
    }
    bool operator==(const const_iterator& other) const noexcept {
        return ptr == other.ptr;
    }
    bool operator!=(const const_iterator& other) const noexcept {
        return !(*this == other);
    }

    friend class List;
    friend class const_iterator;
};

template <class T> class List<T>::const_iterator {
    const NodeBase* ptr;

  public:
    using value_type = T;
    using reference = const T&;
    using pointer = const T*;

    const_iterator(const NodeBase* p) : ptr{p} {}
    const_iterator(const iterator& it) : ptr{it.ptr} {}

    reference operator*() const { return static_cast<const Node*>(ptr)->value; }
    pointer operator->() const { return &ptr->value; }

    const_iterator& operator++() {
        ptr = ptr->next;
        return *this;
    }
    const_iterator operator++(int) {
        const_iterator tmp = *this;
        ptr = ptr->next;
        return tmp;
    }
    const_iterator& operator--() {
        ptr = ptr->prev;
        return *this;
    }
    const_iterator operator--(int) {
        const_iterator tmp = *this;
        ptr = ptr->prev;
        return tmp;
    }

    bool operator==(const const_iterator& oth) const noexcept {
        return ptr == oth.ptr;
    }
    bool operator!=(const const_iterator& oth) const noexcept {
        return !(*this == oth);
    }
    bool operator==(const iterator& oth) const noexcept {
        return ptr == oth.ptr;
    }
    bool operator!=(const iterator& oth) const noexcept {
        return !(*this == oth);
    }

    friend class List;
    friend class iterator;
};

template <class T> struct List<T>::NodeBase {
    NodeBase* prev;
    NodeBase* next;

    NodeBase(NodeBase* p = nullptr, NodeBase* n = nullptr) : prev{p}, next{n} {}
};

template <class T> struct List<T>::Node : NodeBase {
    T value;

    Node(const T& val, NodeBase* p, NodeBase* n) : value(val), NodeBase(p, n) {}
};

template <class T>
List<T>::iterator List<T>::erase(List<T>::const_iterator pos) {
    NodeBase* base = const_cast<NodeBase*>(pos.ptr);
    Node* ptr = static_cast<Node*>(base);
    ptr->prev->next = ptr->next;
    ptr->next->prev = ptr->prev;
    iterator tmp = ptr->next;
    delete ptr;
    --size_;
    return tmp;
}

template <class T>
List<T>::iterator List<T>::erase(List<T>::const_iterator first,
                                 const_iterator last) {
    iterator it = const_cast<NodeBase*>(first.ptr);
    iterator end_it = const_cast<NodeBase*>(last.ptr);
    while (it != end_it) {
        erase(it);
        it++;
    }
    return end_it;
}

template <class T>
List<T>::iterator List<T>::insert(List<T>::const_iterator pos, const T& value) {
    NodeBase* ptr = const_cast<NodeBase*>(pos.ptr);
    Node* new_node = new Node{value, ptr->prev, ptr};
    ptr->prev->next = new_node;
    ptr->prev = new_node;
    ++size_;
    return new_node;
}

template <class T>
void List<T>::resize(List<T>::size_type new_size, const T& value) {
    for (size_type i = size_; i > new_size; i--) {
        erase(sentinel_->prev);
    }
    for (size_type i = size_; i < new_size; i++) {
        push_back(value);
    }
    size_ = new_size;
}

template <class T>
List<T>::List(List<T>::size_type size, const T& value)
    : size_(0), sentinel_(new NodeBase{}) {
    sentinel_->next = sentinel_;
    sentinel_->prev = sentinel_;
    for (int i = 0; i < size; i++)
        push_back(value);
}

template <class T>
List<T>::List(const std::initializer_list<T>& ilist)
    : size_(0), sentinel_(new NodeBase{}) {
    sentinel_->prev = sentinel_;
    sentinel_->next = sentinel_;
    for (const T& elem : ilist)
        push_back(elem);
}

template <class T>
List<T>::List(const List& other) : size_(0), sentinel_(new NodeBase{}) {
    sentinel_->prev = sentinel_;
    sentinel_->next = sentinel_;
    for (const T& elem : other)
        push_back(elem);
}

template <class T>
List<T>::List(List&& other) : size_{other.size_}, sentinel_{other.sentinel_} {
    other.sentinel_ = new NodeBase{};
    other.size_ = 0;
}

template <class T> List<T>::~List() {
    // if (!sentinel_)
    //     return;
    NodeBase* curr = sentinel_->next;
    while (curr != sentinel_) {
        NodeBase* next = curr->next;
        delete static_cast<Node*>(curr);
        curr = next;
    }
    delete sentinel_;
}

template <class T> List<T>& List<T>::operator=(const List& other) {
    if (sentinel_ == other.sentinel_)
        return *this;
    clear();
    for (const T& elem : other)
        push_back(elem);

    return *this;
}

template <class T> List<T>& List<T>::operator=(List&& other) {
    clear();
    delete sentinel_;

    size_ = other.size_;
    sentinel_ = other.sentinel_;

    other.size_ = 0;
    other.sentinel_ = new NodeBase{};

    return *this;
}

template <class T>
List<T>& List<T>::operator=(const std::initializer_list<T>& ilist) {
    clear();
    for (const T& elem : ilist)
        push_back(elem);

    return *this;
}

int main() {
    List<int> lst;

    // 🔹 push_back / push_front
    lst.push_back(1);
    lst.push_back(2);
    lst.push_front(0);

    std::cout << "After push: ";
    for (auto x : lst)
        std::cout << x << " ";
    std::cout << "\n";

    // 🔹 insert
    auto it = lst.begin();
    ++it; // на элемент 1
    lst.insert(it, 42);

    std::cout << "After insert: ";
    for (auto x : lst)
        std::cout << x << " ";
    std::cout << "\n";

    // 🔹 erase (один элемент)
    it = lst.begin();
    ++it; // 42
    lst.erase(it);

    std::cout << "After erase one: ";
    for (auto x : lst)
        std::cout << x << " ";
    std::cout << "\n";

    // 🔹 erase диапазон
    lst.push_back(3);
    lst.push_back(4);
    lst.push_back(5);

    auto first = lst.begin();
    ++first; // 1
    auto last = first;
    ++last; // 2

    lst.erase(first, last);

    std::cout << "After erase range: ";
    for (auto x : lst)
        std::cout << x << " ";
    std::cout << "\n";

    // 🔹 front / back
    std::cout << "Front: " << lst.front() << "\n";
    std::cout << "Back: " << lst.back() << "\n";

    // 🔹 pop
    lst.pop_front();
    lst.pop_back();

    std::cout << "After pop: ";
    for (auto x : lst)
        std::cout << x << " ";
    std::cout << "\n";

    // 🔹 resize
    lst.resize(5, 9);

    std::cout << "After resize up: ";
    for (auto x : lst)
        std::cout << x << " ";
    std::cout << "\n";

    lst.resize(2);

    std::cout << "After resize down: ";
    for (auto x : lst)
        std::cout << x << " ";
    std::cout << "\n";

    // 🔹 copy constructor
    List<int> copy(lst);
    std::cout << "Copy: ";
    for (auto x : copy)
        std::cout << x << " ";
    std::cout << "\n";

    // 🔹 move constructor
    List<int> moved(std::move(lst));
    std::cout << "Moved: ";
    for (auto x : moved)
        std::cout << x << " ";
    std::cout << "\n";

    std::cout << "Original after move (should be empty or valid): ";
    for (auto x : lst)
        std::cout << x << " ";
    std::cout << "\n";

    // 🔹 assignment
    List<int> assign;
    assign = moved;

    std::cout << "Assigned: ";
    for (auto x : assign)
        std::cout << x << " ";
    std::cout << "\n";

    // 🔹 initializer_list
    List<std::string> str_list{"hello", "world", "!"};
    std::cout << "String list: ";
    for (const auto& s : str_list)
        std::cout << s << " ";
    std::cout << "\n";

    return 0;
}
