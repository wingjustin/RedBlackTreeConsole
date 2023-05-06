#include "BinarySearchTree.h"

using namespace BinarySearchTree;

RedBlackNode::RedBlackNode() : Node::Node(){ // NIL
	//initialize node status
	isRed = false;
	parent = NULL;
	left = NULL;
	right = NULL;
}

RedBlackNode::RedBlackNode(int key) : Node::Node(key){
	//initialize node status
	isRed = true;
	parent = NULL;
	left = new RedBlackNode();
	left->parent = this;
	right = new RedBlackNode();
	right->parent = this;
}

RedBlackNode::RedBlackNode(int key, string value) : Node::Node(key, value){
	//initialize node status
	isRed = true;
	parent = NULL;
	left = new RedBlackNode();
	left->parent = this;
	right = new RedBlackNode();
	right->parent = this;
}

RedBlackNode::~RedBlackNode(){
	delete left;
	delete right;

	parent = NULL;
	left = NULL;
	right = NULL;
}

void RedBlackNode::ChangeToRed(){
	isRed = !IsNIL();// NIL can't be black
}

void RedBlackNode::ChangeToBlack(){
	isRed = false;
}

bool RedBlackNode::IsRed() {
	return isRed;
}

bool RedBlackNode::IsBlack() {
	return !isRed;
}

bool RedBlackNode::IsNIL() {
	return !left; // left or right points to NULL
}