#pragma once

#include <string>

using namespace std;

namespace BinarySearchTree{
	class RedBlackNode : public Node{
	protected:
		// node status
		bool isRed;
		RedBlackNode* parent;
		RedBlackNode* left;
		RedBlackNode* right;

		void ChangeToRed();
		void ChangeToBlack();

		bool IsRed();
		bool IsBlack();
		bool IsNIL();

		friend class RedBlackTree;
	public:
		RedBlackNode(); // for NIL
		RedBlackNode(int key);
		RedBlackNode(int key, string value);
		~RedBlackNode();
	};
}
