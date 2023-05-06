#pragma once

#include <string>

using namespace std;

namespace BinarySearchTree{
	class RedBlackTree : public Tree{
	private:
		bool HealthCheck(RedBlackNode* current, int& currentBlackHeight); // for debug
	protected:
		RedBlackNode* root;

		void clockwiseRotation(RedBlackNode* target);
		void counter_clockwiseRotation(RedBlackNode* target);

		void InsertRebalance(RedBlackNode* target);
		void RemoveRebalance(RedBlackNode* target);
	public:
		RedBlackTree();
		~RedBlackTree();

		void Clear();

		RedBlackNode* SearchNode(int key);
		void Insert(int key, string value);
		bool Remove(int key); //return success

		bool HealthCheck(); // for debug
	};
}
