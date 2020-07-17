#ifndef DOUBLE_LIST_H
#define DOUBLE_LIST_H

#include <iostream>

using namespace std;

template<typename T>
class list_node
{
public:
    list_node(T data):
        data_(data),
        next_(nullptr),
        prev_(nullptr)
    { }

    ~list_node()
    {
        if(next_)
        {
            delete next_;
        }
        if(prev_)
        {
            delete prev_;
        }
    }

    T& data()
    {
        return data_;
    }

public:
    list_node*      next_;
    list_node*      prev_;
private:
    T               data_;
};

template<typename T>
class double_list
{
public:

    typedef T                   value_type;
    typedef const value_type&   const_ref_type;
    typedef list_node<T>        node_value_type;
    typedef list_node<T>*       node_pointer;
    typedef const node_pointer  const_node_pointer;

public:

    double_list():
        start_(new node_value_type(-1)),
        end_(new node_value_type(-1)),
        len_(0)
    {
        start_->next_ = end_;
        end_->prev_ = start_;
    }

    ~double_list()
    {
        start_->next_ = nullptr;
        end_->prev_ = nullptr;
        if(start_)
        {
            delete start_;
        }
        if(end_)
        {
            delete end_;
        }
    }

    int push_back(const_ref_type data)
    {
        return this->insert(data, len_);
    }

    int insert(const_ref_type data, int pos)
    {
        // corner case: if pos is larger than len_, return error
        if(pos > len_)
        {
            return -1;
        }
        node_pointer curr = start_;
        while(pos-- > 0)
        {
            curr = curr->next_;
        }
        auto next = curr->next_;
        auto new_node = new node_value_type(data);
        curr->next_ = new_node;
        new_node->next_ = next;
        new_node->prev_ = curr;
        next->prev_ = new_node;
        len_ += 1;
        return 0;
    }

    int erase(int pos)
    {
        if(len_ == 0)
        {
            cout << "Nothing to erase in this double list" << endl;
            return 0;
        }
        if(pos >= len_)
        {
            cout << "postion is not valid" << endl;
            return 0;
        }
        node_pointer curr = start_;
        while(pos -- > 0)
        {
            curr = curr->next_;
        }
        auto delete_node = curr->next_;
        curr->next_ = delete_node->next_;
        delete_node->next_->prev_ = curr;
        delete_node->next_ = nullptr;
        delete_node->prev_ = nullptr;
        delete delete_node;
        len_ -= 1;
        return 0;

    }

    int dump()
    {
        if(len_ == 0)
        {
            cout<< "Nothing to dump in this double_list!" << endl;
            return 0;
        }
        node_pointer curr = start_;
        curr = curr->next_;
        cout << "Double List is: ";
        for(; curr != end_; curr=curr->next_)
        {
            cout << curr->data() << ' ';
        }
        cout << ' ' << endl;

        return 0;
    }

    int reverse_dump()
    {
        if(len_ == 0)
        {
            cout << "Nothing to dump in this double_list!" << endl;
            return 0;
        }
        node_pointer curr = end_;
        curr = curr->prev_;
        cout << "reverse Double List is: ";
        for(; curr != start_; curr=curr->prev_)
        {
            cout << curr->data() << ' ';
        }
        cout << ' ' << endl;
        return 0;
    }

    int len()
    {
        return len_;
    }

private:

    node_pointer  start_;
    node_pointer  end_;
    size_t        len_;  // exclude start_ and end_




};

#endif // DOUBLE_LIST_H
