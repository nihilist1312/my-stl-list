#include <cstddef>
#include <initializer_list>
#include <list>

template <class T> struct List {
    using value_type = T;
    using allocator_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    class iterator;
    class const_iterator;

    List() : size_(0), sentinel_(new Node{}) {
        sentinel_->next = sentinel_;
        sentinel_->prev = sentinel_;
    }
    List(size_type size, const T& value = T{});
    List(const std::initializer_list<T>& ilist);
    List(const List& other);
    List(List&& other);
    ~List();

    reference front() { return sentinel_->next->value; }
    const_reference front() const { return sentinel_->next->value; }
    reference back() { return sentinel_->prev->value; }
    const_reference back() const { return sentinel_->prev->value; }

    iterator begin() { return sentinel_->next; }
    const_iterator begin() const { return sentinel_->next; }
    const_iterator cbegin() const { return sentinel_->next; }
    iterator end() { return sentinel_; }
    const_iterator end() const { return sentinel_; }
    const_iterator cend() const { return sentinel_; }

    bool empty() const noexcept { return size_; }
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
    struct Node;

    size_type size_;
    Node* sentinel_;
};

template <class T> class List<T>::iterator {
    Node* ptr;

  public:
    using value_type = T;
    using reference = T&;
    using pointer = T*;

    iterator(Node* p) : ptr{p} {}

    reference operator*() const { return ptr->value; }
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
    bool operator==(const const_iterator& other) const;

    friend class List;
};

template <class T> class List<T>::const_iterator {
    const Node* ptr;

  public:
    using value_type = T;
    using reference = const T&;
    using pointer = const T*;

    const_iterator(const Node* p) : ptr{p} {}
    const_iterator(const iterator& it) : ptr{it.ptr} {}

    reference operator*() const { return ptr->value; }
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

    friend class List;
};

template <class T> struct List<T>::Node {
    Node* prev;
    Node* next;
    T value;

    Node(T val, Node* p = nullptr, Node* n = nullptr)
        : value(val), prev{p}, next{n} {}
};

template <class T>
List<T>::iterator List<T>::erase(List<T>::const_iterator pos) {
    Node* ptr = pos.ptr;
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
    for (auto it = first; first != last; ++first) {
        erase(it);
        --size_;
    }
    return last;
}

template <class T>
List<T>::iterator List<T>::insert(List<T>::const_iterator pos, const T& value) {
    Node* ptr = const_cast<Node*>(pos.ptr);
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

int main() {
    std::list<int> l;
    l.resize(10);
    return 0;
}
