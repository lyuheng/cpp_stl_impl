#ifndef RBTREE_H
#define RBTREE_H

#include <iostream>
#include <list>

using namespace std;

const auto RED = 0;
const auto BLACK = 1;
const auto NULL_NODE = -100;
const auto LEFT_CHILD = 0;
const auto RIGHT_CHILD = 1;

template<typename T>
class rb_tree_node
{
public:
    rb_tree_node(const T& data, int color)
        : data_(data)
        , color_(color)
        , parent_(nullptr)
        , l_child_(nullptr)
        , r_child_(nullptr)
    {

    }

public:
    T		        data_;
    int		        color_;
    rb_tree_node*	parent_;
    rb_tree_node*	l_child_;
    rb_tree_node*	r_child_;
};

template<typename T>
class rb_tree
{
public:
    typedef T			        value_type;
    typedef const value_type&	const_ref_type;
    typedef rb_tree_node<T>		node_value_type;
    typedef rb_tree_node<T>*	node_pointer;
    typedef const node_pointer	const_node_pointer; // 指向const的指针

public:
    rb_tree()
    : root_(nullptr)
    {

    }

    int insert(const_ref_type data)
    {
        if (nullptr == this->root_)
        {
            this->root_ = new node_value_type(data, BLACK);
            return 0;
        }

        return insert_node(nullptr, LEFT_CHILD, this->root_, data);
    }

    int erase(const_ref_type data)
    {
        if (nullptr == this->root_)
            return -1;
        return erase_node(nullptr, LEFT_CHILD, this->root_, data);
    }

    void dump()
    {
        mid_visit_node(this->root_);
    }

    int height()
    {
        return this->height_i(this->root_);
    }

    void check_rb_feature_5()
    {
        list<int> list;
        char trace[100] = { 0 };
        check_rb_feature_5_impl(nullptr, this->root_, trace, 0, list);
        int len = -1;
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            if (-1 == len)
                len = *it;
            else if (len != *it)
            {
                cout << "check_rb_feature_5 failed: " << endl;
                return;
            }
        }

        cout << "check_rb_feature_5 success, black node count:  " << len << endl;
    }

private:
    int insert_node(node_pointer pp, int pp_child_tag, node_pointer p, const_ref_type data)
    {
        if (nullptr == p)
        {
            p = new node_value_type(data, RED);
            LEFT_CHILD == pp_child_tag ? pp->l_child_ = p : pp->r_child_ = p;
            p->parent_ = pp;
            if (RED == pp->color_)
                this->adjust_balance(p);
            return 0;
        }
        else if (p->data_ == data)
            return -1;
        else if (data < p->data_)
            return this->insert_node(p, LEFT_CHILD, p->l_child_, data);
        else
            return this->insert_node(p, RIGHT_CHILD, p->r_child_, data);
    }

    int erase_node(node_pointer pp, int pp_child_tag, node_pointer p, const_ref_type data)
    {
        if (nullptr == p)
            return -1;
        else if (p->data_ == data)
        {
            node_pointer fake_null = nullptr;
            int delete_node_color = p->color_;
            node_pointer & ref_p = (pp ? (LEFT_CHILD == pp_child_tag ? pp->l_child_ : pp->r_child_) : this->root_);

            if (p->l_child_ && p->r_child_)
            {
                // 如果需要删除的节点有两个儿子，那么问题可以被转化成删除另一个只有一个儿子的节点的问题
                auto prev = p->l_child_;
                auto curr = prev;
                for (; curr && curr->r_child_; prev = curr, curr = curr->r_child_);
                delete_node_color = curr->color_;
                if (curr == prev)
                {
                    p->data_ = curr->data_;
                    p->l_child_ = curr->l_child_;
                    if (curr->l_child_)
                        curr->l_child_->parent_ = p;
                    pp = p;
                    p = p->l_child_;
                    pp_child_tag = LEFT_CHILD;
                    delete curr;
                }
                else
                {
                    p->data_ = curr->data_;
                    pp = prev;
                    pp_child_tag = RIGHT_CHILD;
                    p = prev->r_child_ = curr->l_child_;
                    if (curr->l_child_)
                        curr->l_child_->parent_ = prev;
                    delete curr;
                }
            }
            else if (p->l_child_)
            {
                // 同时修改 pp->l_child_ or pp->r_child_ 的值
                ref_p = p->l_child_;
                p->l_child_->parent_ = pp;
                delete p;
                p = ref_p;
            }
            else if (p->r_child_)
            {
                // 同时修改 pp->l_child_ or pp->r_child_ 的值
                ref_p = p->r_child_;
                p->r_child_->parent_ = pp;
                delete p;
                p = ref_p;
            }
            else
            {
                // 同时修改 pp->l_child_ or pp->r_child_ 的值
                p = ref_p = nullptr;
            }


            // 如果删除的node是BLACK，则feature5可能会出现问题
            if (BLACK == delete_node_color && pp)
            {
                if ( p == nullptr)
                {
                    fake_null = p = new node_value_type(NULL_NODE, BLACK); // add null to leaf node
                    p->parent_ = pp;
                    LEFT_CHILD == pp_child_tag ? pp->l_child_ = p : pp->r_child_ = p;
                }

                this->adjust_delete_balance(pp, p);
                if (fake_null)
                {
                    if (fake_null->parent_->l_child_ == fake_null)
                        fake_null->parent_->l_child_ = nullptr;
                    else
                        fake_null->parent_->r_child_ = nullptr;
                    delete fake_null;
                }
            }
            return 0;
        }
        else if (data < p->data_)
            return this->erase_node(p, LEFT_CHILD, p->l_child_, data);
        else
            return this->erase_node(p, RIGHT_CHILD, p->r_child_, data);
    }

private:

    // see https://zh.wikipedia.org/wiki/%E7%BA%A2%E9%BB%91%E6%A0%91
    void adjust_balance(node_pointer p)
    {
        // 增加的是RED，feature4可能会出现问题
        while (p->parent_ && p->parent_->parent_ && RED == p->parent_->color_)
        {
            auto pp = p->parent_;
            auto gp = pp->parent_;
            auto up = (gp->l_child_ == pp ? gp->r_child_ : gp->l_child_);

            if (gp->l_child_ == pp)
            {
                // scenario 3: 如果父节点P(大循环)和叔父节点U二者都是红色
                if (up && RED == up->color_)
                {
                    pp->color_ = up->color_ = BLACK;
                    gp->color_ = RED;
                    p = gp;  // gp 成为新增加的RED
                }
                else
                {
                    // scenario 4: 父节点P是红色(大循环)而叔父节点U是黑色或缺少，并且新节点N是其父节点P的右子节点而父节点P又是其父节点的左子节点
                    if (pp->r_child_ == p)
                    {
                        this->l_rotate(pp);
                        swap(p, pp);
                    }
                    // scenario 5: 父节点P是红色而叔父节点U是黑色或缺少，新节点N是其父节点的左子节点，而父节点P又是其父节点G的左子节点
                    pp->color_ = BLACK;
                    gp->color_ = RED;
                    this->r_rotate(gp);
                }
            }
            else
            {
                if (up && RED == up->color_)
                {
                    pp->color_ = up->color_ = BLACK;
                    gp->color_ = RED;
                    p = gp;
                }
                else
                {
                    if (pp->l_child_ == p)
                    {
                        this->r_rotate(pp);
                        swap(p, pp);
                    }

                    pp->color_ = BLACK;
                    gp->color_ = RED;
                    this->l_rotate(gp);
                }
            }
        }

        this->root_->color_ = BLACK;
    }

    void adjust_delete_balance(node_pointer pp, node_pointer p)
    {
        while ( (nullptr == p || BLACK == p->color_) && p != this->root_)
        {
            if (pp->l_child_ == p)
            {
                auto s = pp->r_child_;  // p的兄弟
                // scenario 2: S是红色。在这种情形下我们在N的父亲上做左旋转，把红色兄弟转换成N的祖父，我们接着对调N的父亲和祖父的颜色
                if (s && RED == s->color_)
                {
                    s->color_ = BLACK;
                    pp->color_ = RED;
                    this->l_rotate(pp);
                    s = pp->r_child_; // 修改p的兄弟
                }


                // scenario 3: N的父亲、S和S的儿子都是黑色的, 我们要从情形1开始，在P上做重新平衡处理。
                //
                // s->RED
                if ((nullptr == s->l_child_ || BLACK == s->l_child_->color_)
                    && (nullptr == s->r_child_ || BLACK == s->r_child_->color_))
                {
                    s->color_ = RED;
                    p = pp;
                    pp = p->parent_;
                }
                else
                {
                    // scenario 5: S是黑色，S的左儿子是红色，S的右儿子是黑色
                    if (nullptr == s->r_child_ || BLACK == s->r_child_->color_)
                    {
                        s->l_child_ ? s->l_child_->color_ = BLACK : 0;
                        s->color_ = RED;
                        this->r_rotate(s);
                        s = pp->r_child_;  // 现在 N 和 s_l 是兄弟， 也就是pp的right
                    }

                    // scenario 6:  S是黑色，S的右儿子是红色，而N是它父亲的左儿子。
                    s->color_ = pp->color_;
                    pp->color_ = BLACK;
                    s->r_child_->color_ = BLACK;
                    this->l_rotate(pp);
                    p = this->root_;
                }
            }
            else
            {
                auto s = pp->l_child_;
                if (s && RED == s->color_)
                {
                    s->color_ = BLACK;
                    pp->color_ = RED;
                    this->r_rotate(pp);
                    s = pp->l_child_;
                }

                if ((nullptr == s->l_child_ || BLACK == s->l_child_->color_)
                    && (nullptr == s->r_child_ || BLACK == s->r_child_->color_))
                {
                    s->color_ = RED;
                    p = pp;
                    pp = p->parent_;
                }
                else
                {
                    if (nullptr == s->l_child_ || BLACK == s->l_child_->color_)
                    {
                        s->r_child_ ? s->r_child_->color_ = BLACK : 0;
                        s->color_ = RED;
                        this->l_rotate(s);
                        s = pp->l_child_;
                    }

                    s->color_ = pp->color_;
                    pp->color_ = BLACK;
                    s->l_child_->color_ = BLACK;
                    this->r_rotate(pp);
                    p = this->root_;
                }
            }
        }

        p->color_ = BLACK;
    }

private:
    node_pointer l_rotate(node_pointer p)
    {
        auto lr = p->r_child_;
        p->r_child_ = lr->l_child_;
        if (lr->l_child_)
            lr->l_child_->parent_ = p;

        lr->parent_ = p->parent_;
        if (nullptr == p->parent_)
            this->root_ = lr;
        else if (p->parent_->l_child_ == p)
            p->parent_->l_child_ = lr;
        else
            p->parent_->r_child_ = lr;

        lr->l_child_ = p;
        p->parent_ = lr;

        return lr;
    }

    node_pointer r_rotate(node_pointer p)
    {
        auto lc = p->l_child_;
        p->l_child_ = lc->r_child_;
        if (lc->r_child_)
            lc->r_child_->parent_ = p;

        lc->parent_ = p->parent_;
        if (nullptr == p->parent_)
            this->root_ = lc;
        else if (p->parent_->l_child_ == p)
            p->parent_->l_child_ = lc;
        else
            p->parent_->r_child_ = lc;

        lc->r_child_ = p;
        p->parent_ = lc;

        return lc;
    }

    int height_i(node_pointer p)
    {
        if (nullptr == p)
            return 0;

        int left_height = 0;
        if (p->l_child_)
            left_height = height_i(p->l_child_) + 1;

        int right_height = 0;
        if (p->r_child_)
            right_height = height_i(p->r_child_) + 1;

        return left_height >= right_height ? left_height : right_height; // get max
    }

    void mid_visit_node(const_node_pointer p)
    {
        if (nullptr != p)
        {
            mid_visit_node(p->l_child_);
            std::cout << p->data_ << std::endl;
            mid_visit_node(p->r_child_);
        }
    }

    void check_rb_feature_5_impl(node_pointer pp, node_pointer p, char trace[], int idx, std::list<int>& list)
    {
        if (nullptr == p)
        {
            trace[idx] = BLACK;
            int result = 0;
            for (auto k = 0; k <= idx; ++k)
            {
                if (BLACK == trace[k])
                    ++result;
            }

            list.push_back(result);
        }
        else
        {
            trace[idx] = p->color_;
            check_rb_feature_5_impl(p, p->l_child_, trace, idx + 1, list);
            check_rb_feature_5_impl(p, p->r_child_, trace, idx + 1, list);
        }
    }

private:
    node_pointer	root_;
};




#endif // RBTREE_H
