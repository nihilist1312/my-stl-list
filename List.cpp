#include <cstddef>
#include <initializer_list>

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

    List() : size_(0), sentinel_(new Node{T{}}) {}
    List(size_type size, const T& value = T{});
    List(const std::initializer_list<T>& ilist);
    List(const List& other);
    List(List&& other);
    ~List();

    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

    bool empty() const noexcept { return size_; }
    size_type size() const noexcept { return size_; }

    void clear();
    iterator insert(const_iterator pos, const T& value);
    iterator erase(const_iterator pos);
    void push_back(const T& value);
    void pop_back();
    void push_front(const T& value);
    void pop_front();
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
