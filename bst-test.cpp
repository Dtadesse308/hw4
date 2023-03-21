#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    // BinarySearchTree<char,int> bt;
    // bt.insert(std::make_pair('a',1));
    // bt.insert(std::make_pair('b',2));
    
    // cout << "Binary Search Tree contents:" << endl;
    // for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
    //     cout << it->first << " " << it->second << endl;
    // }
    // if(bt.find('b') != bt.end()) {
    //     cout << "Found b" << endl;
    // }
    // else {
    //     cout << "Did not find b" << endl;
    // }
    // cout << "Erasing b" << endl;
    // bt.remove('b');

    // AVL Tree Tests
    AVLTree<char,int> at;
   
    at.insert(std::make_pair(5,5));
    at.insert(std::make_pair(3,3));

    at.insert(std::make_pair(7,7));
	
    at.insert(std::make_pair(4,4));

    at.insert(std::make_pair(6,6));
	
    at.insert(std::make_pair(9,9));

     at.print();
	//ASSERT_TRUE(verifyAVL(b, {item5.first, item3.first, item7.first, item4.first, item6.first, item9.first}));

    cout<<"removing 5"<<endl;
	at.remove(5);
	//ASSERT_TRUE(verifyAVL(b, {item3.first, item7.first, item4.first, item6.first, item9.first}));
     at.print();
     cout<<"removing 7"<<endl;
	at.remove(7);
    at.print();
	//ASSERT_TRUE(verifyAVL(b, {item3.first, item4.first, item6.first, item9.first}));
    cout<<"removing 6"<<endl;
	at.remove(6);
    at.print();
	//ASSERT_TRUE(verifyAVL(b, {item3.first, item4.first, item9.first}));


     
    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(at.find('b') != at.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    // cout << "Erasing b" << endl;
    // at.remove('b');

    return 0;
}
