#ifndef RBT_H
#define RBT_H
#include <bits/stdc++.h>
using std::swap;
using std::max;

class RBT {
 public:
    RBT(int elem) : elem_(elem), red_(0), left(nullptr), right(nullptr) {}
    ~RBT() {}
    RBT(const RBT &other) : elem_(other.elem_), red_(other.red()), left(other.left ? new RBT(*other.left) : nullptr), right(other.right ? new RBT(*other.right) : nullptr) {}
    int elem() const { return this->elem_; }
    bool red() const { return this ? this->red_ : 0; }
    int height() { return this ?  max(right->height(), left->height()) + 1 : 0; }
    int width() { return this ? max(left->width() + right->width(), 1) : 0; }
    friend void SLR(RBT* &p) {
        RBT* q = p->right;
        p->right = q->left;
        q->left = p;
        swap(p->red_, q->red_);
        p = q;
    }
    friend void SRR(RBT* &p) {
        RBT* q = p->left;
        p->left = q->right;
        q->right = p;
        swap(p->red_, q->red_);
        p = q;
    }
    friend void insert(RBT* &p, int elem, RBT** D = nullptr, RBT** G = nullptr, RBT** U = nullptr) {
        if (p == nullptr) {
            p = new RBT(elem);
            p->red_ = 1;
        } else if (p->elem_ == elem) {
            return;
        } else if (p->elem_ < elem) {
            insert(p->right, elem, &p, D, D ? (*D)->right == p ? &(*D)->left : &(*D)->right : nullptr);
        } else {
            insert(p->left, elem, &p, D, D ? (*D)->right == p ? &(*D)->left : &(*D)->right : nullptr);
        }
        if (D == nullptr) {
            p->red_ = 0;
        } else if (!(*D)->red_ || !p->red_) {
            return;
        } else if ((*U)->red()) {
            (*G)->red_ = 1;
            (*D)->red_ = (*U)->red_ = 0;
        } else if ((*G)->elem_ > (*D)->elem_ ^ (*D)->elem_ > p->elem_) {
            if (p->elem_ < (*D)->elem_) {
                SRR(*D);
                SLR(*G);
            } else {
                SLR(*D);
                SRR(*G);
            }
        } else {
            if (p->elem_ < (*D)->elem_) {
                SRR(*G);
            } else {
                SLR(*G);
            }
        }
    }
    RBT* find_min() {return this->left ? this->left->find_min() : this; }
    friend bool evil_flex_left(RBT* &p) {
        RBT* B = p->right;
        if (!p->red_ && !B->red_ && !B->left->red() && !B->right->red()) {
            B->red_ = 1;
            return 1;
        } else if (p->red_ && !B->left->red() && !B->right->red()) {
            p->red_ = 0;
            B->red_ = 1;
        } else if (B->red_) {
            p->right = B->left;
            swap(p->red_, B->red_);
            B->left = p;
            p = B;
            evil_flex_left(p->left);
        } else if (B->right->red()) {
            B->red_ = p->red_;
            p->red_ = 0;
            B->right->red_ = 0;
            p->right = B->left;
            B->left = p;
            p = B;
        } else {
            swap(B->red_, B->left->red_);
            p->right = B->left;
            B->left = B->left->right;
            p->right->right = B;
            evil_flex_left(p);
        }
        return 0;
    }
    friend bool evil_flex_right(RBT* &p) {
        RBT* B = p->left;
        if (!B) return 0;
        if (!p->red_ && !B->red_ && !B->right->red() && !B->left->red()) {
            B->red_ = 1;
            return 1;
        } else if (p->red_ && !B->right->red() && !B->left->red()) {
            p->red_ = 0;
            B->red_ = 1;
        } else if (B->red_) {
            p->left = B->right;
            swap(p->red_, B->red_);
            B->right = p;
            p = B;
            evil_flex_right(p->right);
        } else if (B->right->red() && B->left->red()) {
            B->red_ = p->red_;
            p->red_ = 0;
            B->left->red_ = 0;
            p->left = B->right;
            B->right = p;
            p = B;
        } else if (B->left->red()) {
            B->red_ = p->red_;
            p->red_ = 0;
            B->left->red_ = 0;
            p->left = B->right;
            B->right = p;
            p = B;
        } else {
            swap(B->red_, B->right->red_);
            p->left = B->right;
            B->right = B->right->left;
            p->left->left = B;
            evil_flex_right(p);
        }
        return 0;
    }
    friend bool remove_min(RBT* &p) {
        if (p->left != nullptr)
            return remove_min(p->left) ? evil_flex_left(p) : 0;
        if (p->red_) {
            RBT* right = p->right;
            p = right;
            return 0;
        }
        if (p->right->red()) {
            RBT* right = p->right;
            p = right;
            p->red_ = 0;
            return 0;
        }
        p = nullptr;
        return 1;
    }
    friend bool erase(RBT* &p, int elem) {
        if (p == nullptr) return 0;
        if (p->elem_ > elem) {
            if (erase(p->left, elem)) return evil_flex_left(p);
            else return 0;
        } else if (p->elem_ < elem) {
            if (erase(p->right, elem)) return evil_flex_right(p);
            else return 0;
        } else {
            bool our_red = p->red_;
            RBT* left = p->left, *right = p->right;
            delete p;
            if (right == nullptr) {
                if (left == nullptr) {
                    p = nullptr;
                    if (our_red) return 0;
                    else return 1;
                } else {
                    p = left;
                    if (our_red || p->red_) {
                        p->red_ = 0;
                        return 0;
                    } else {
                        return 1;
                    }
                }
            }
            if (right->left == nullptr) {
                bool have_red = right->red_;
                right->left = left;
                p = right;
                p->red_ = our_red;
                if (have_red) return 0;
                else return evil_flex_right(p);
            }
            p = right->find_min();
            bool need_flex = remove_min(right);
            p->right = right;
            p->left = left;
            p->red_ = our_red;
            return need_flex ? evil_flex_right(p) : 0;
        }
    }

    int elem_;
    bool red_;
    RBT* left, *right;
};

#endif // RBT_H
