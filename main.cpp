//Author: Shuhao Duan a1807323
//thanks to geeksforgeeks.org for helping me out A LOT
//https://www.geeksforgeeks.org/avl-tree-set-2-deletion/?ref=lbp
//https://www.geeksforgeeks.org/avl-tree-set-1-insertion/?ref=lbp
//I basically followed these two design patterns to construct my work
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//define the node class which will be used to construct our tree
class treeNode {
public:
  int value;
  treeNode *leftchild;
  treeNode *rightchild;
  int height;

  treeNode(){
    leftchild = NULL;
    rightchild = NULL;
    value = 0;
    height = 1;
  }

};

int getheight(treeNode* subject){
  if(subject==NULL) return 0;
  return subject->height;
}

treeNode* rightRotation(treeNode* oldmain){
  //cout<<"rotating right"<<endl;
  treeNode* lc = oldmain->leftchild;
  treeNode* lcrc = oldmain->leftchild->rightchild;

  lc->rightchild = oldmain;
  oldmain->leftchild = lcrc;

  //cout<<"debug mark01"<<endl;
  oldmain->height = max(getheight(oldmain->leftchild),getheight(oldmain->rightchild))+1;
  lc->height = max(getheight(lc->leftchild),getheight(lc->rightchild))+1;

  return lc;
}

treeNode* leftRotation(treeNode* oldmain){
  //cout<<"rotating left on node "<<oldmain->value<<endl;
  treeNode* rc = oldmain->rightchild;
  treeNode* rclc = oldmain->rightchild->leftchild;

  rc->leftchild = oldmain;
  oldmain->rightchild = rclc;

  oldmain->height = max(getheight(oldmain->leftchild),getheight(oldmain->rightchild))+1;
  rc->height = max(getheight(rc->leftchild),getheight(rc->rightchild))+1;

  return rc;
}

int balancefactor(treeNode* subject){
  if (subject==NULL) return 0;
  return getheight(subject->leftchild) - getheight(subject->rightchild);
}

//the three tree-traversal functions:
void preOrderT(treeNode* root){
  if (root!=NULL){
    cout<<root->value<<" ";
    preOrderT(root->leftchild);
    preOrderT(root->rightchild);
  }
}

void postOrderT(treeNode* root){
  if(root!=NULL){
    postOrderT(root->leftchild);
    postOrderT(root->rightchild);
    cout<<root->value<<" ";
  }
}

void inOrderT(treeNode* root){
  if(root!=NULL){
    inOrderT(root->leftchild);
    cout<<root->value<<" ";
    inOrderT(root->rightchild);
  }
}


//and finally, i can begin to write insert and delete
treeNode* insertNode(treeNode* root, int num){
  //cout<<"inserted "<<num<<endl;
  if (root == NULL) {
    treeNode* newnode = new treeNode();
    newnode->value = num;
    newnode->height = 1;
    return newnode;
  }
  //find the sopt to put the new node in in a BST manner
  if (num < root->value)
  root->leftchild = insertNode(root->leftchild, num);

  else if (num > root->value)
  root->rightchild = insertNode(root->rightchild, num);

  else
  return root;

  //by now we should have inserted the new node in a BST manner.
  //next up we should seek to rebalance the tree;
  root->height = 1 + max(getheight(root->leftchild),getheight(root->rightchild));
  //cout<<"updated root "<<root->value<<"'s height to "<<root->height<<endl;

  int factor = balancefactor(root);
  //cout<<factor<<endl;

  if (factor > 1 && num < root->leftchild->value){
    return rightRotation(root);
  }

  if (factor < -1 && num > root->rightchild->value){
    return leftRotation(root);
  }

  if (factor > 1 && num > root->leftchild->value){
    root->leftchild = leftRotation(root->leftchild);
    return rightRotation(root);
  }

  if (factor < -1 && num < root->rightchild->value){
    root->rightchild = rightRotation(root->rightchild);
    return leftRotation(root);
  }
  //now we should be done

  return root;
}

treeNode* findheir(treeNode* subject){
  treeNode* iterator = subject;

  while(true){
    if(iterator->rightchild == NULL) break;
    iterator = iterator->rightchild;
  }

  return iterator;
}

treeNode* deleteNode(treeNode* root, int num){
  if (root==NULL) return root;
  if (root->value == 0) return root;

  if (num < root->value){
    root->leftchild = deleteNode(root->leftchild, num);

  } else if (num > root->value){
    root->rightchild = deleteNode(root->rightchild, num);

  } else {
    if ((root->leftchild == NULL) || (root->rightchild == NULL)){
      treeNode* iterator = root->leftchild ? root->leftchild : root->rightchild;

      if (iterator==NULL){
        iterator=root;
        root=NULL;
      } else {
        *root = *iterator;
        delete(iterator);
      }
    } else {
      treeNode* iterator = findheir(root->leftchild);

      root->value = iterator->value;

      root->leftchild = deleteNode(root->leftchild, iterator->value);
    }
  }

  if (root==NULL) return root;

  root->height = max(getheight(root->leftchild),getheight(root->rightchild))+1;
  int factor = balancefactor(root);
  //cout<<factor<<endl;

  if (factor > 1 && balancefactor(root->leftchild) >= 0){
    return rightRotation(root);
  }

  if (factor > 1 && balancefactor(root->leftchild) < 0){
    root->leftchild = leftRotation(root->leftchild);
    return rightRotation(root);
  }

  if (factor < -1 && balancefactor(root->rightchild) <= 0){
    return leftRotation(root);
  }

  if (factor < -1 && balancefactor(root->rightchild) > 0){
    root->rightchild = rightRotation(root->rightchild);
    return leftRotation(root);
  }
  //now we should be done

  return root;
}

//here begins the show
int main (){
  //firstly the parsing of input
  vector<string> commands;
  string prtcmd = "";
  int startingpos = 0;
  string input = "";
  getline(cin, input);
  for (unsigned int i = 0; i < input.length();i++){
    if (input[i]==' '){
      commands.push_back(input.substr(startingpos,i-startingpos));
      startingpos = i+1;
    }
  }
  prtcmd = input.substr(startingpos,input.length()-startingpos);
  //now i have a vector commands storing all number addition commands
  //and ptrcmd storing the specific priting command.

  treeNode* root = NULL;//this is my tree right here.

  int thenum = 0;
  for(unsigned int i = 0; i < commands.size();i++){
    if (commands[i][0]=='A'){
      thenum = stoi(commands[i].substr(1,commands[i].length()-1));
      root = insertNode(root,thenum);
    } else if (commands[i][0]=='D'){
      thenum = stoi(commands[i].substr(1,commands[i].length()-1));
      root = deleteNode(root,thenum);
    }
  }

  if (root == NULL || root->value == 0){
    cout<<"EMPTY"<<endl;
  } else if (prtcmd=="IN"){
    inOrderT(root);
  } else if (prtcmd=="POST"){
    postOrderT(root);
  } else if (prtcmd=="PRE"){
    //cout<<"root is: "<<root->value<<endl;
    //cout<<"root has height: "<<root->height<<endl;
    preOrderT(root);
  }

  cout<<endl;

  return 0;
}
