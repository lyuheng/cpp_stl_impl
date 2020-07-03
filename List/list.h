#ifndef LIST_H
#define LIST_H

#include <iostream>

using namespace std;

// forward list
template<typename T>
class list_node
{
public:
    list_node(T data): data_(data), next_(nullptr){ }

    ~list_node()
    {
        delete next_;
    }

    T& data()
    {
        return data_;
    }

public:
    list_node*  next_;
    T           data_;
};


// forward_list
template<typename T, typename V>
class list
{
public:
    typedef T                   value_type;
    typedef const value_type&   const_ref_type;
    typedef list_node<T>        node_value_type;
    typedef list_node<T>*       node_pointer;
    typedef const node_pointer  const_node_pointer;

public:
    list(): start_(new node_value_type(-1)), len_(0)
    { }

    ~list()
    {
        delete start_;
    }

    int push_back(const_ref_type data)
    {
        return this-> insert(data, len_);
    }

    int insert(const_ref_type data, int pos)
    {
        node_pointer curr = start_;
        while(pos-- > 0)
        {
            curr = curr->next_;
        }
        auto next = curr->next_;
        auto new_node = new node_value_type(data);
        curr->next_ = new_node;
        new_node->next_ = next;
        len_ += 1;
        return 0;
    }

    int erase(int pos)
    {
        if(len_ == 0)
        {
            cout << "Cannot erase any node" << endl;
            return -1;
        }
        if(pos >= len_)
        {
            cout<<"position is not valid" << endl;
            return -1;
        }
        node_pointer curr = start_;
        while(pos-- > 0)
        {
            curr = curr->next_;
        }
        auto delete_node = curr->next_;
        curr->next_ = delete_node->next_;
        delete_node->next_ = nullptr; // delete next pointer
        delete delete_node;
        len_ -= 1;
        return 0;
    }

    void dump()
    {
        if(len_ == 0)
        {
            cout << "Nothing to dump in this list" << endl;
            return;
        }

        node_pointer curr = start_;
        curr = curr->next_;
        cout << "List is : ";
        for(; curr != nullptr; curr = curr->next_)
        {
            cout << curr->data_ << ' ';
        }
        cout << ' ' << endl;
    }

    int len()
    {
        return len_;
    }

private:
    node_pointer     start_;
    size_t           len_;
};


#endif // LIST_H
