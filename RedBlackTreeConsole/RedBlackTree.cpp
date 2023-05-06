#include "BinarySearchTree.h"

using namespace BinarySearchTree;

RedBlackTree::RedBlackTree(){
	root = new RedBlackNode(); // NIL
}

RedBlackTree::~RedBlackTree() {
	delete root;
	root = NULL;
}

void RedBlackTree::Clear() {
	delete root;
	root = NULL;
}

void RedBlackTree::clockwiseRotation(RedBlackNode* const target) {
	if (!target || !target->left)
		return;
	//move left child to parent
	RedBlackNode* temp = target->parent;
	if (temp) {
		if (temp->left == target)
			temp->left = target->left;
		else // if (target->parent->right == target)
			temp->right = target->left;
		target->left->parent = temp;
	}
	else {
		target->left->parent = NULL;
		if (target == root)
			root = target->left;
	}
	//move old parent to right child of new parent
	temp = target->left;
	target->parent = temp;
	if (temp->right) {
		//move new parent's right child to old parent's left
		temp->right->parent = target;
		target->left = temp->right;
		temp->right = target;
	}
	else {
		target->left = NULL;
		temp->right = target;
	}
	temp = NULL;
}

void RedBlackTree::counter_clockwiseRotation(RedBlackNode* const  target) {
	if (!target || !target->right)
		return;
	//move right child to parent
	RedBlackNode* temp = target->parent;
	if (temp) {
		if (temp->left == target)
			temp->left = target->right;
		else // if (temp->right == target)
			temp->right = target->right;
		target->right->parent = temp;
	}
	else {
		target->right->parent = NULL;
		if (target == root)
			root = target->right;
	}
	//move old parent to left child of new parent
	temp = target->right;
	target->parent = temp;
	if (temp->left) {
		//move new parent's left child to old parent's right
		temp->left->parent = target;
		target->right = temp->left;
		temp->left = target;
	}
	else {
		target->right = NULL;
		temp->left = target;
	}
	temp = NULL;
}

void RedBlackTree::InsertRebalance(RedBlackNode* target) { // rebalance red tree from current -> parent -> grandparent(subtree's root) -> uncle(use color to check balance), difference of left and right is less than 2
	while (target->IsRed() && target->parent && target->parent->IsRed()) {
		if (target->parent->left == target) { 
			// =?L
			if (target->parent->parent->left == target->parent) {
				// =LL
				if (target->parent->parent->right->IsRed()) {  //Uncle is red = red tree's diff < 2 = no need to balance
					// change color
					target->parent->ChangeToBlack();
					target->parent->parent->ChangeToRed();
					target->parent->parent->right->ChangeToBlack();
					target = target->parent->parent; // go to the top of subtree
				}
				else { //if (target->parent->parent->right->IsBlack()) //Uncle is black = red tree's diff > 1 = need to balance
					clockwiseRotation(target->parent->parent);
					// change color
					target->parent->ChangeToBlack();
					target->parent->right->ChangeToRed(); // parent's old grandparent (=now uncle) change to red
					target = target->parent; // go to the top of subtree
				}
			}
			else { //if (target->parent->parent->right == target->parent)
				// =RL
				if (target->parent->parent->left->IsRed()) {  //Uncle is red = red tree's diff < 2 = no need to balance
					// change color
					target->parent->ChangeToBlack();
					target->parent->parent->ChangeToRed();
					target->parent->parent->left->ChangeToBlack();
					target = target->parent->parent; // go to the top of subtree
				}
				else { //if (target->parent->parent->left->IsBlack()) //Uncle is black = red tree's diff > 1 = need to balance
					clockwiseRotation(target->parent);
					counter_clockwiseRotation(target->parent);
					// change color
					target->ChangeToBlack();
					target->left->ChangeToRed(); // parent's old grandparent (=now uncle) change to red
				}
			}
		}
		else { //if (target->parent->right == target)
			// =?R
			if (target->parent->parent->left == target->parent) {
				// =LR
				if (target->parent->parent->right->IsRed()) {  //Uncle is red = diff < 2 = no need to balance
					// change color
					target->parent->ChangeToBlack();
					target->parent->parent->ChangeToRed();
					target->parent->parent->right->ChangeToBlack();
					target = target->parent->parent; // go to the top of subtree
				}
				else { //if (target->parent->parent->right->IsBlack()) //Uncle is black = diff > 1 = need to balance
					counter_clockwiseRotation(target->parent);
					clockwiseRotation(target->parent);
					// change color
					target->ChangeToBlack();
					target->right->ChangeToRed(); // parent's old grandparent (=now uncle) change to red
				}
			}
			else { //if (target->parent->parent->right == target->parent)
				// =RR
				if (target->parent->parent->left->IsRed()) {  //Uncle is red = diff < 2 = no need to balance
					// change color
					target->parent->ChangeToBlack();
					target->parent->parent->ChangeToRed();
					target->parent->parent->left->ChangeToBlack();
					target = target->parent->parent; // go to the top of subtree
				}
				else { //Uncle is black = diff > 1 = need to balance
					counter_clockwiseRotation(target->parent->parent);
					// change color
					target->parent->ChangeToBlack();
					target->parent->left->ChangeToRed(); // parent's old grandparent change to red
					target = target->parent; // go to the top of subtree
				}
			}
		}
	}
	if (!target->parent) {
		root = target;
		root->ChangeToBlack();
	}
}

void RedBlackTree::RemoveRebalance(RedBlackNode* target) {
	if (!target->parent) {
		root = target;
		root->ChangeToBlack();
		return;
	}
	if (target->IsRed()) {
		target->ChangeToBlack();
		return;
	}
	//target is black
	//please control and keep the black tree height of 2 left and right subtrees to equal
	//switch red to black = height + 1, black to red = height - 1
	bool isSbgLeft = 0;// sibling side
	RedBlackNode* sibling = (isSbgLeft = target->parent->left != target)
		? target->parent->left // sibling = L
		: target->parent->right; // sibling = R
	while (target && target->parent && !sibling->IsNIL()) {
		//check sibling's color
		if (sibling->IsBlack()) {
			if (isSbgLeft) {
				//check sibling's children color
				if (sibling->left->IsRed()) { // LL or 2 red nephews
					clockwiseRotation(target->parent);
					if (target->parent->IsRed())
						sibling->ChangeToRed();
					sibling->left->ChangeToBlack();
					sibling->right->ChangeToBlack();
					target = NULL; // end the loop
					if (!sibling->parent)
						root = sibling;
				}
				else if (sibling->right->IsRed()) { // LR
					counter_clockwiseRotation(sibling);
					clockwiseRotation(target->parent);
					target->parent->IsRed()
						? sibling->parent->ChangeToRed()
						: sibling->parent->ChangeToBlack();
					target->parent->ChangeToBlack();
					target = NULL; // end the loop
					if (!sibling->parent->parent)
						root = sibling->parent;
				}
				else { // 2 black nephews
					sibling->ChangeToRed();
					if (sibling->parent->IsRed()) {
						sibling->parent->ChangeToBlack();
						target = NULL; // end the loop
					}
					else {
						target = target->parent;
						if(target == root)
							target = NULL; // end the loop
						else {
							sibling = (isSbgLeft = target->parent->left != target)
								? target->parent->left // sibling = L
								: target->parent->right; // sibling = R
						}
						//the whole subtree's height has been dropped one.
					}
				}
			}
			else {
				//check sibling's children color
				if (sibling->right->IsRed()) { // RR or 2 red nephews
					counter_clockwiseRotation(target->parent);
					if (target->parent->IsRed())
						sibling->ChangeToRed();
					sibling->left->ChangeToBlack();
					sibling->right->ChangeToBlack();
					target = NULL; // end the loop
					if (!sibling->parent)
						root = sibling;
				}
				else if (sibling->left->IsRed()) { // RL
					clockwiseRotation(sibling);
					counter_clockwiseRotation(target->parent);
					target->parent->IsRed()
						? sibling->parent->ChangeToRed()
						: sibling->parent->ChangeToBlack();
					target->parent->ChangeToBlack();
					target = NULL; // end the loop
					if (!sibling->parent->parent)
						root = sibling->parent;
				}
				else { // 2 black nephews
					sibling->ChangeToRed();
					if (sibling->parent->IsRed()) {
						sibling->parent->ChangeToBlack();
						target = NULL; // end the loop
					}
					else {
						target = target->parent;
						if (target == root)
							target = NULL; // end the loop
						else {
							sibling = (isSbgLeft = target->parent->left != target)
								? target->parent->left // sibling = L
								: target->parent->right; // sibling = R
						}
						//the whole subtree's height has been dropped one.
					}
				}
			}
		}
		else { // sibling is red
			//Its left, right and parent are black.
			//Do rotation to downgrade and get new black sibling, black subtrees' height are still not equal.
			if (isSbgLeft) {
				clockwiseRotation(sibling->parent);
				sibling->ChangeToBlack();
				target->parent->ChangeToRed();
				if (!sibling->parent)
					root = sibling;
				//target = target;
				sibling = sibling->right->left;
			}
			else {
				counter_clockwiseRotation(sibling->parent);
				sibling->ChangeToBlack();
				target->parent->ChangeToRed();
				if (!sibling->parent)
					root = sibling;
				//target = target;
				sibling = sibling->left->right;
			}
		}
	}
}

RedBlackNode* RedBlackTree::SearchNode(const int key) {
	// return NULL if empty
	if (!root)
		return NULL;
	// search from root
	if (key == root->key)
		return root;
	RedBlackNode* cNode = root; // current node
	while (!cNode->IsNIL() && key != cNode->key) {
		cNode = key < cNode->key
			? cNode->left    // go left if less than current node
			: cNode->right;  // go right if greater than current node
	}
	return cNode->IsNIL() ? NULL : cNode;
}

void RedBlackTree::Insert(const int key, const string value) {
	if (root->IsNIL()) {
		delete root;
		root = new RedBlackNode(key, value);
		root->ChangeToBlack();
		return;
	}
	//search insert entry
	RedBlackNode* entry = root;
	bool isLeft = 0; // 0 = right, 1 = left
	do {
		if (key == entry->key) { // if the key is equal, replace value and return it.
			entry->value = value;
			return;
		}
		entry = (isLeft = key < entry->key)
			? entry->left    // go left if less than current node
			: entry->right;  // go right if greater than current node
	} while (!entry->IsNIL());
	//start to insert new node and now entry is NIL
	RedBlackNode* nNode = new RedBlackNode(key, value);
	nNode->parent = entry->parent;
	if (isLeft)
		nNode->parent->left = nNode;
	else
		nNode->parent->right = nNode;
	delete entry;
	InsertRebalance(nNode);

	entry = NULL;
}

bool RedBlackTree::Remove(const int key) {//return success
	if (root->IsNIL())
		return false;
	// search from root
	RedBlackNode* cNode = root; // current node
	while (!cNode->IsNIL() && key != cNode->key) {
		cNode = key < cNode->key
			? cNode->left    // go left if less than current node
			: cNode->right;  // go right if greater than current node
	}
	if (!cNode->IsNIL()) {
		//find out the Node of near key
		RedBlackNode* temp = cNode;
		if (temp->left && !temp->left->IsNIL()) {
			temp = temp->left;
			while (temp->right && !temp->right->IsNIL())
				temp = temp->right;
		}
		else if (temp->right && !temp->right->IsNIL()) {
			temp = temp->right;
			while (temp->left && !temp->left->IsNIL())
				temp = temp->left;
		}
		//replace removing key(*cNode) with near key(*temp)
		if (temp != cNode) {
			cNode->key = temp->key;
			cNode->value = temp->value;
		}
		// be removed from parent
		if (temp->parent) {
			if (temp->parent->left == temp) {
				if (!temp->right->IsNIL()) {
					temp->right->parent = temp->parent;
					temp->parent->left = temp->right;
					temp->right = NULL;
				}
				else if (!temp->left->IsNIL()) {
					temp->left->parent = temp->parent;
					temp->parent->left = temp->left;
					temp->left = NULL;
				}
				else {
					temp->parent->left = new RedBlackNode(); // NIL
					temp->parent->left->parent = temp->parent;
				}
				//rebalance
				RedBlackNode* rebalanceTarget = temp->parent->left;
				RemoveRebalance(rebalanceTarget);
				rebalanceTarget = NULL;
			}
			else {
				if (!temp->left->IsNIL()) {
					temp->left->parent = temp->parent;
					temp->parent->right = temp->left;
					temp->left = NULL;
				}
				else if (!temp->right->IsNIL()) {
					temp->right->parent = temp->parent;
					temp->parent->right = temp->right;
					temp->right = NULL;
				}
				else {
					temp->parent->right = new RedBlackNode(); // NIL
					temp->parent->right->parent = temp->parent;
				}
				//rebalance
				RedBlackNode* rebalanceTarget = temp->parent->right;
				RemoveRebalance(rebalanceTarget);
				rebalanceTarget = NULL;
			}
			//delete Node
			temp->parent = NULL;
			delete temp;
			temp = NULL;
		}
		else {
			if (root == temp)
				root = new RedBlackNode(); // NIL
			//delete Node
			temp->parent = NULL;
			delete temp;
			temp = NULL;
		}
		cNode = NULL;
		return true;
	}
	cNode = NULL;
	return false;
}

 // for debug
bool RedBlackTree::HealthCheck(RedBlackNode* current, int& currentBlackHeight) {
	if (!current)
		return false;

	if (current->IsBlack())
		currentBlackHeight++;

	// check NIL
	if (current->IsNIL()) {
		return current->IsBlack()
			&& currentBlackHeight == 1
			&& !current->left && !current->right
			&& current->key == INT32_MIN && current->value == "";
	}

	// check Red Node
	if (current->IsRed()
		&& !(current->parent && current->parent->IsBlack()
			&& current->left && current->left->IsBlack()
			&& current->right && current->right->IsBlack()
			)
		)
		return false;

	// check if 2 side of Black Subtree Height is equal
	int leftBlackHeight = 0;
	int rightBlackHeight = 0;
	if (!current->left || !(current->left->IsNIL() || current->left->key <= current->key) || !HealthCheck(current->left, leftBlackHeight))
		return false;
	if (!current->right || !(current->right->IsNIL() || current->right->key >= current->key) || !HealthCheck(current->right, rightBlackHeight))
		return false;
	// check current height if it is correct
	currentBlackHeight += (leftBlackHeight > rightBlackHeight ? leftBlackHeight : rightBlackHeight);
	return leftBlackHeight == rightBlackHeight;
}

bool RedBlackTree::HealthCheck() {
	int blackHeight = 0;
	return root
		&& !root->parent
		&& root->IsBlack()
		&& HealthCheck(root, blackHeight);
}