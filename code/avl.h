#ifndef AVL_H
#define AVL_H
#include <bits/stdc++.h>
using std::max;

class AVL {
 public:
    AVL(int elem) : height_(1), elem_(elem), left(nullptr), right(nullptr) {}
    ~AVL() {}
    AVL(const AVL &other) : height_(other.height()), elem_(other.elem_), left(other.left ? new AVL(*other.left) : nullptr), right(other.right ? new AVL(*other.right) : nullptr) {}
    int height() const { return  this ? this->height_: 0; }
    int elem() const { return this->elem_; }
    int width() { return this ? max(left->width() + right->width(), 1) : 0; }
    void fix_height() { if (this) this->height_ = max(this->left->height(), this->right->height()) + 1; }
    AVL* find(int elem) {
        if (this == nullptr) return nullptr;
        if (this->elem_ == elem) return this;
        if (this->elem_ < elem) return this->right->find(elem);
        if (this->elem_ > elem) return this->left->find(elem);
    }
    friend void SRR(AVL* &p) {
        AVL* q = p->left;
        p->left = q->right;
        q->right = p;
        p = q;
    }
    friend void SLR(AVL* &p) {
        AVL* q = p->right;
        p->right = q->left;
        q->left = p;
        p = q;
    }
    friend void BRR(AVL* &p) {
        AVL* q = p->left, *r = p->left->right;
        q->right = r->left;
        p->left = r->right;
        r->left = q;
        r->right = p;
        p = r;
    }
    friend void BLR(AVL* &p) {
        AVL* q = p->right, *r = p->right->left;
        q->left = r->right;
        p->right = r->left;
        r->right = q;
        r->left = p;
        p = r;
    }
    friend void balance(AVL* &p) {
        p->fix_height();
        if (p->left->height() - p->right->height() > 1) {
            if (p->left->right->height() > p->left->left->height()) {
                BRR(p);
            } else {
                SRR(p);
            }
        } else if (p->right->height() - p->left->height() > 1) {
            if (p->right->left->height() > p->right->right->height()) {
                BLR(p);
            } else {
                SLR(p);
            }
        }
        p->right->fix_height();
        p->left->fix_height();
        p->fix_height();
    }
    friend void insert(AVL* &p, int elem) {
        if (p == nullptr) {
            p = new AVL(elem);
            return;
        }
        if (p->elem_ == elem) return;
        if (elem > p->elem_) {
            insert(p->right, elem);
        } else {
            insert(p->left, elem);
        }
        balance(p);
    }
    AVL* find_min() { return this->left ? this->left->find_min() : this; }
    friend void remove_min(AVL* &p) {
        if (p->left == nullptr) {
            p = p->right;
            return;
        } else {
            remove_min(p->left);
        }
        balance(p);
    }
    friend void erase(AVL* &p, int elem) {
        if (p == nullptr) return;
        if (p->elem_ < elem) {
            erase(p->right, elem);
        } else if (p->elem_ > elem) {
            erase(p->left, elem);
        } else {
            AVL* left = p->left, *right = p->right;
            delete p;
            if (!right) {
                p = left;
                return;
            }
            AVL* min = right->find_min();
            remove_min(right);
            min->right = right;
            min->left = left;
            p = min;
        }
        balance(p);
    }

    int height_, elem_;
    AVL *left, *right;
};

#endif // AVL_H
