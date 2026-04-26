#include <memory>

template <class T, class Allocator = std::allocator<T>> class List {
    struct Node {
        Node* prev;
        Node* next;
        T value;

        Node(T val, Node* p = nullptr, Node* n = nullptr)
            : value(val), prev{p}, next{n} {}
    };

  public:
    using value_type = T;
    using allocator_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    class iterator;
    class const_iterator;
};

template <class T, class A> class List<T, A>::iterator {
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

template <class T, class A> class List<T, A>::const_iterator {
    const Node* ptr;

  public:
    using value_type = T;
    using reference = const T&;
    using pointer = const T*;

    const_iterator(const Node* p) : ptr{p} {}

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
