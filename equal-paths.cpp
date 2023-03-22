#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>
#endif

#include "equal-paths.h"

//using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    // Add your code below

    // if (root == NULL){
    //     return true;
    // }

  // if ( (root->left == NULL) && ((root->right) == NULL)){
  //     return true;
  // }

    bool flag = true;

    getHeight(root,flag);
 
    return flag;
 
}

int getHeight(Node* root, bool& valid){

    if (root == NULL){
        return 0;
    }


    int leftHight = getHeight(root->left, valid);
    int rightHight = getHeight(root->right, valid);

    if ( (!root->left) && (root->right)){
        return (1 + rightHight);
    }

   else if ( (root->left) && (!root->right)){
        return (1 + leftHight);
    }
    
    else if ((root->left) && (root->right)){
        if (leftHight != rightHight){
            valid = false;
        }
    }
    return ( 1+ (std::max(leftHight,rightHight)));



}


