#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError
{
};

/**
 * A special kind of node for an AVL tree, which adds the balance as a data member, plus
 * other additional helper functions. You do NOT need to implement any functionality or
 * add additional data members or helper functions.
 */
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key &key, const Value &value, AVLNode<Key, Value> *parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance() const;
    void setBalance(int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value> *getParent() const override;
    virtual AVLNode<Key, Value> *getLeft() const override;
    virtual AVLNode<Key, Value> *getRight() const override;

protected:
    int8_t balance_; // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
 * An explicit constructor to initialize the elements by calling the base class constructor
 */
template <class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key &key, const Value &value, AVLNode<Key, Value> *parent) : Node<Key, Value>(key, value, parent), balance_(0)
{
}

/**
 * A destructor which does nothing.
 */
template <class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{
}

/**
 * A getter for the balance of a AVLNode.
 */
template <class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
 * A setter for the balance of a AVLNode.
 */
template <class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
 * Adds diff to the balance of a AVLNode.
 */
template <class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
 * An overridden function for getting the parent since a static_cast is necessary to make sure
 * that our node is a AVLNode.
 */
template <class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value> *>(this->parent_);
}

/**
 * Overridden for the same reasons as above.
 */
template <class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value> *>(this->left_);
}

/**
 * Overridden for the same reasons as above.
 */
template <class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value> *>(this->right_);
}

/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/

template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert(const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key &key);                              // TODO
protected:
    virtual void nodeSwap(AVLNode<Key, Value> *n1, AVLNode<Key, Value> *n2);

    // Add helper functions here
    void insertFix(AVLNode<Key, Value> *parent, AVLNode<Key, Value> *node); // todo
    void removeFix(AVLNode<Key, Value> *parent, int diff); // todo
    void rotateRight(AVLNode<Key, Value> *root);                            // DONE
    void rotateLeft(AVLNode<Key, Value> *root);                             // DONE
};

/*
 * Recall: If key is already in the tree, you should
 * overwrite the current value with the updated value.
 */
template <class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{

    AVLNode<Key, Value> *temp = static_cast<AVLNode<Key, Value> *>(this->root_);
    AVLNode<Key, Value> *parent = NULL;
    AVLNode<Key, Value> *child = NULL;

    if (this->root_ == NULL)
    {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
        this->root_->setLeft(NULL);
        this->root_->setRight(NULL);
        return;
    }

    else
    {
        while (true)
        {
            if (temp->getKey() > new_item.first)
            {
                if (temp->getLeft() == NULL)
                {
                    parent = temp;
                    child = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
                    temp->setLeft(child);
                    //-1 parent balance
                    parent->updateBalance(-1);

                    if (parent->getBalance() != 0)
                    {
                        insertFix(parent, child);
                    }

                    break;
                }

                temp = temp->getLeft();
            }

            else if (temp->getKey() < new_item.first)
            {
                if (temp->getRight() == NULL)
                {
                    parent = temp;
                    child = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
                    temp->setRight(child);
                    //+ 1 parent balance
                    parent->updateBalance(1);

                    if (parent->getBalance() != 0)
                    {
                        insertFix(parent, child);
                    }

                    break;
                }

                temp = temp->getRight();
            }
            else if (temp->getKey() == new_item.first)
            {
                temp->setValue(new_item.second);
                break;
            }
        }
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value> *parent, AVLNode<Key, Value> *node)
{

    if ((parent == NULL) || (parent->getParent() == NULL))
    {
        return;
    }

    AVLNode<Key, Value> *Gparent = parent->getParent();

    if ((Gparent->getLeft() != NULL) && (Gparent->getLeft() == parent))
    { // if parent is left child of gparent
        Gparent->updateBalance(-1);
        if (Gparent->getBalance() == -1)
        {
            insertFix(Gparent, parent); // recurse
        }
        else if (Gparent->getBalance() == -2)
        {

            if (Gparent->getLeft()->getLeft() == node)
            { // check if zig zig
                rotateRight(Gparent);
                parent->setBalance(0);
                Gparent->setBalance(0);
            }
            else
            {
                rotateLeft(parent);
                rotateRight(Gparent);

                if (node->getBalance() == -1)
                {
                    parent->setBalance(0);
                    Gparent->setBalance(1);
                }
                else if (node->getBalance() == 0)
                {
                    parent->setBalance(0);
                    Gparent->setBalance(0);
                }
                else if (node->getBalance() == 1)
                {
                    parent->setBalance(-1);
                    Gparent->setBalance(0);
                }
                node->setBalance(0);
            }
        }
    }
    else if ((Gparent->getRight() != NULL) && (Gparent->getRight() == parent))
    { // if parent is right child of gparent
        Gparent->updateBalance(1);
        if (Gparent->getBalance() == 1)
        {
            insertFix(Gparent, parent); // recurse
        }
        else if (Gparent->getBalance() == 2)
        {

            if (Gparent->getRight()->getRight() == node)
            { // check if zig zig
                rotateLeft(Gparent);
                parent->setBalance(0);
                Gparent->setBalance(0);
            }
            else
            {
                rotateRight(parent);
                rotateLeft(Gparent);

                if (node->getBalance() == 1)
                {
                    parent->setBalance(0);
                    Gparent->setBalance(-1);
                }
                else if (node->getBalance() == 0)
                {
                    parent->setBalance(0);
                    Gparent->setBalance(0);
                }
                else if (node->getBalance() == -1)
                {
                    parent->setBalance(1);
                    Gparent->setBalance(0);
                }
                node->setBalance(0);
            }
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template <class Key, class Value>
void AVLTree<Key, Value>::remove(const Key &key)
{

    // TODO
    if (this->root_ == NULL)
    { // empty tree
        return;
    }
    AVLNode<Key, Value> *temp = static_cast<AVLNode<Key, Value> *>(this->internalFind(key)) ;

    if (temp == NULL)
    { // if key dne
        return;
    }
    AVLNode<Key, Value> *child = NULL;

     if ((temp->getLeft() != NULL) && (temp->getRight() != NULL))
    { // if both children
        AVLNode<Key, Value> *pred = static_cast<AVLNode<Key, Value> *>(this->predecessor(temp));
        nodeSwap(temp, pred);
    }

    //for AVL implementation 
    int diff;
    AVLNode<Key, Value>* AVLparent = temp->getParent();
    
    if (AVLparent != NULL){
        if ((AVLparent->getLeft() != NULL) && (AVLparent->getLeft() == temp))
        {
            diff = 1;
        }
     else if ((AVLparent->getRight() != NULL) && (AVLparent->getRight() == temp))
        {
            diff = -1;
        }
  
    }


    


    if ((temp->getLeft() == NULL) && (temp->getRight() == NULL))
    { // if leaf node just remove

        if (temp->getParent() != NULL)
        {
            AVLNode<Key, Value> *parent = temp->getParent();

            if ((parent->getLeft() != NULL) && (parent->getLeft() == temp ))
            {
                parent->setLeft(NULL);  
                                  
            }
            else if ((parent->getRight() != NULL) && (parent->getRight() == temp))
            {
                parent->setRight(NULL);
            }
        }


       
    }

    if ((temp->getLeft() != NULL) && (temp->getRight() == NULL))
    { // only left node
        child = temp->getLeft();

        if (temp->getParent() != NULL)
        {
            AVLNode<Key, Value> *parent = temp->getParent();

            child->setParent(parent);

            if ((parent->getLeft() != NULL) && (parent->getLeft()->getKey() == temp->getKey()))
            {
                parent->setLeft(child);
            }
            else if ((parent->getRight() != NULL) && (parent->getRight()->getKey() == temp->getKey()))
            {
                parent->setRight(child);
            }
            
        }
        else
        {
            child->setParent(NULL);
            this->root_ = child;
        }
    }

    if ((temp->getLeft() == NULL) && (temp->getRight() != NULL))
    { // only right child
        child = temp->getRight();

        if (temp->getParent() != NULL)
        {
            AVLNode<Key, Value> *parent = temp->getParent();
            child->setParent(temp->getParent());

            if ((parent->getLeft() != NULL) && (parent->getLeft()->getKey() == temp->getKey()))
            {
                parent->setLeft(child);
            }
            else if ((parent->getRight() != NULL) && (parent->getRight()->getKey() == temp->getKey()))
            {
                parent->setRight(child);
            }
        }
        else
        {
            child->setParent(NULL);
            this->root_ = child;
        }
    }

    

        
    



    if (temp->getKey() == this->root_->getKey())
    {
        delete this->root_;
        this->root_ = NULL;
        temp = NULL;
    }
    else
    {
        delete temp;
        temp = NULL;
    }

     
   removeFix(AVLparent,diff);
   
}

template <class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value> *node, int diff)
{

    if (node == NULL){
        return;
    }

    
            

   AVLNode<Key,Value>* p = node->getParent();
   
    
   int ndiff;
   if (p != NULL){

    if ( (p->getLeft() != NULL) &&  p->getLeft() == node){      //if left subtree
        ndiff = 1;
    }
    else if ( (p->getRight() != NULL) && (p->getRight() == node) ){
        ndiff = -1;
    }


   }



    if (diff == -1) {     
          
     if ( (node->getBalance() + diff) == -2){       //case alt1
            AVLNode<Key,Value>* child = node->getLeft();
            if (child->getBalance() == -1){  //case 1a, zig zig
                rotateRight(node);
                node->setBalance(0);
                child->setBalance(0);
                removeFix(p,ndiff);
           
            } 
            
            else if (child->getBalance() == 0){  //case 1b, zig zig
                rotateRight(node);
                node->setBalance(-1);
                child->setBalance(1);
            } 
        else if (child->getBalance() == 1){  //case 1c, zig zag
                AVLNode<Key,Value>* g = child->getRight();
                rotateLeft(child); //
                rotateRight(node);

                if (g->getBalance() == 1){
                    node->setBalance(0);
                    child->setBalance(-1);
                }
                else if (g->getBalance() == 0){
                    node->setBalance(0);
                    child->setBalance(0);
                }
                else if (g->getBalance() == -1){
                    node->setBalance(1);
                    child->setBalance(0);
                }
                g->setBalance(0);
                removeFix(p,ndiff);

            } 
            

        } 
    
            else if (node->getBalance() + diff == -1){
                node->setBalance(-1);
            }
           
            else if (node->getBalance() + diff == 0){
                node->setBalance(0);
                removeFix(p,ndiff);
                
            }
            
        }
    else if (diff == 1){

        if (node->getBalance() + diff == 2){       //case 1
            AVLNode<Key,Value>* child = node->getRight();
            if (child->getBalance() == 1){  //case 1a, zig zig
                rotateLeft(node);
                node->setBalance(0);
                child->setBalance(0);
                removeFix(p,ndiff);

            } 
            else if (child->getBalance() == 0){  //case 1b, zig zag
                rotateLeft(node);
                node->setBalance(1);
                child->setBalance(-1);
            } 
        
        else if (child->getBalance() == -1){  //case 1a, zig zag
                AVLNode<Key,Value>* g = child->getLeft();
                rotateRight(child);
                rotateLeft(node);

                if (g->getBalance() == -1){
                    node->setBalance(0);
                    child->setBalance(1);
                }
                else if (g->getBalance() == 0){
                    node->setBalance(0);
                    child->setBalance(0);
                }
                else if (g->getBalance() == 1){
                    node->setBalance(-1);
                    child->setBalance(0);
                }
                g->setBalance(0);
                removeFix(p,ndiff);

            } 
    }

           
            else if (node->getBalance() + diff == 1){
                node->setBalance(1);
            }
            else if (node->getBalance() + diff == 0){
                node->setBalance(0);
                removeFix(p,ndiff);
            }
            

    }
   
        
}





template <class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key, Value> *n1, AVLNode<Key, Value> *n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template <class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value> *node)
{

    AVLNode<Key, Value> *parent = node->getLeft();
    //AVLNode<Key, Value> *child = parent->getLeft();
    AVLNode<Key, Value> *parentChild = parent->getRight();

    if (node->getParent() != NULL)
    {
        parent->setParent(node->getParent()); //
        if ((node->getParent()->getLeft() != NULL) && (node->getParent()->getLeft() == node))
        {
            node->getParent()->setLeft(parent); //
        }
        else if ((node->getParent()->getRight() != NULL) && (node->getParent()->getRight() == node))
        {
            node->getParent()->setRight(parent);
        }
    }
    else
    {

        parent->setParent(NULL); //
        this->root_ = parent;
    }

    if (parentChild != NULL)
    {
        node->setLeft(parentChild);   //
        parentChild->setParent(node); //
    }
    else
    {

        node->setLeft(NULL);
    }

    parent->setRight(node);
    node->setParent(parent);
}
template <class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value> *node)
{

    AVLNode<Key, Value> *parent = node->getRight();
  //  AVLNode<Key, Value> *child = parent->getRight();
    AVLNode<Key, Value> *parentChild = parent->getLeft();

    if (node->getParent() != NULL)
    {
        parent->setParent(node->getParent()); //
        if ((node->getParent()->getLeft() != NULL) && (node->getParent()->getLeft() == node))
        {
            node->getParent()->setLeft(parent); //
        }
        else if ((node->getParent()->getRight() != NULL) && (node->getParent()->getRight() == node))
        {
            node->getParent()->setRight(parent);
        }
    }
    else
    {

        parent->setParent(NULL); //
        this->root_ = parent;
    }

    if (parentChild != NULL)
    {
        node->setRight(parentChild);  //
        parentChild->setParent(node); //
    }
    else
    {

        node->setRight(NULL);
    }

    parent->setLeft(node);
    node->setParent(parent);
}

#endif
