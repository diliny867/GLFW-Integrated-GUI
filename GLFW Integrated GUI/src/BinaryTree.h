#pragma once

#include <functional>
#include <stack>

template <typename T>
struct BinNode {
	T data;
	BinNode* left = nullptr;
	BinNode* right = nullptr;
	void Traverse(std::function<void(T)>& handle){
		if(left!=nullptr) {
			left->Traverse(handle);
		}
		handle(data);
		if(right!=nullptr) {
			right->Traverse(handle);
		}
	}
	void Traverse(std::function<void(T)>& handle) const{
		if(left!=nullptr) {
			left->Traverse(handle);
		}
		handle(data);
		if(right!=nullptr) {
			right->Traverse(handle);
		}
	}
	void Insert(T data_,std::function<bool(T,T)>& compare) {
		if(compare(data_,data)) {
			if(left==nullptr) {
				left = new BinNode(data_);
			}else {
				left->Insert(data_,compare);
			}
		}else {
			if(right==nullptr) {
				right = new BinNode(data_);
			}else {
				right->Insert(data_,compare);
			}
		}
	}
	bool Remove(T data_) {
		if(left!=nullptr) {
			if(left->data==data_) {
				if(left->left==nullptr) {
					if(left->right==nullptr) {
						left=nullptr;
					} else{
						left = left->right;
					}
				} else{
					if(left->right==nullptr) {
						left = left->left;
					} else{

						BinNode* deepest = left->right;

						while(deepest->left!=nullptr) {
							deepest = deepest->left;
						}
						deepest->left = left->left;

						left = left->right;
					}
				}
				return true;
			}else {
				return left->Remove(data_);
			}
		}
		if(right!=nullptr) {
			if(right->data==data_) {
				if(right->left==nullptr) {
					if(right->right==nullptr) {
						right=nullptr;
					} else{
						right = right->right;
					}
				} else{
					if(right->right==nullptr) {
						right = right->left;
					} else{
						BinNode* deepest = right->right;

						while(deepest->left!=nullptr) {
							deepest = deepest->left;
						}
						deepest->left = right->left;

						right = right->right;
					}
				}
				return true;
			} else {
				return right->Remove(data_);
			}
		}
		return false;
	}
	bool Contains(T data_) {
		if(data == data_) {
			return true;
		}
		if(left == nullptr) {
			if(right == nullptr) {
				return false;
			}
			return right->Contains(data_);
		}
		if(right == nullptr) {
			return left->Contains(data_);
		}
		return left->Contains(data_) || right->Contains(data_);
	}
	BinNode(){}
	BinNode(T data_):data(data_){}
	BinNode(BinNode* left_, BinNode* right_):left(left_),right(right_){}
	BinNode(T data_, BinNode* left_, BinNode* right_):left(left_),right(right_){}
};

template <typename T>
class BinaryTree {
public:
	BinNode<T>* root;
	BinaryTree(): root(nullptr) {}
	BinaryTree(T data): root(new BinNode(data)) {}
	void Traverse(std::function<void(T)>& handle) {
		root->Traverse(handle);
	}
	void Traverse(std::function<void(T)> handle) {
		root->Traverse(handle);
	}
	void Traverse(std::function<void(T)>& handle) const {
		root->Traverse(handle);
	}
	void Traverse(std::function<void(T)> handle) const {
		root->Traverse(handle);
	}
	void Insert(T data, std::function<bool(T,T)>& compare) {
		if(root == nullptr) {
			root = new BinNode<T>(data);
			return;
		}
		root->Insert(data,compare);
	}
	void Insert(T data, std::function<bool(T,T)> compare) {
		if(root == nullptr) {
			root = new BinNode<T>(data);
			return;
		}
		root->Insert(data,compare);
	}
	void Remove(T data) { //idk if this works
		if(root == nullptr){ return; }
		if(data == root->data) {
			if(root->left == nullptr) {
				if(root->right == nullptr) {
					root = nullptr;
				}else {
					root = root->right;
				}
			} else{
				if(root->right == nullptr) {
					root = root->left;
				}else {
					BinNode<T>* deepest = root->right;

					while(deepest->left!=nullptr) {
						deepest = deepest->left;
					}
					deepest->left = root->left;

					root = root->right;
				}
			}
			return;
		}
		root->Remove(data);
	}
	bool Contains(T data) {
		if(root == nullptr){ return false; }
		return root->Contains(data);
	}

	class InOrderIterator {
	private:
		std::stack<BinNode<T>*> traversal_stack;
	public:
		explicit InOrderIterator(BinNode<T>* current) {
			while(current != nullptr) {
				traversal_stack.push(current);
				current = current->left;
			}
		}
		T& operator*() const {
			return traversal_stack.top()->data;
		}
		InOrderIterator& operator++() {
			BinNode<T>* current = traversal_stack.top();
			traversal_stack.pop();

			if(current->right != nullptr) {
				current = current->right;
				while(current != nullptr) {
					traversal_stack.push(current);
					current = current->left;
				}
			}

			return *this;
		}
		bool operator!=(const InOrderIterator& other) const {
			return !traversal_stack.empty() || !other.traversal_stack.empty();
		}
	};
	InOrderIterator begin() {
		if(root == nullptr) {
			return end();
		}
		return InOrderIterator(root);
	}
	InOrderIterator end() {
		return InOrderIterator(nullptr);
	}
	InOrderIterator begin() const {
		if(root == nullptr) {
			return end();
		}
		return InOrderIterator(root);
	}
	InOrderIterator end() const {
		return InOrderIterator(nullptr);
	}

	class ReverseInOrderIterator {
	private:
		std::stack<BinNode<T>*> traversal_stack;
	public:
		explicit ReverseInOrderIterator(BinNode<T>* current) {
			while(current != nullptr) {
				traversal_stack.push(current);
				current = current->right;
			}
		}
		T& operator*() const {
			return traversal_stack.top()->data;
		}
		ReverseInOrderIterator& operator++() {
			BinNode<T>* current = traversal_stack.top();
			traversal_stack.pop();

			if(current->left != nullptr) {
				current = current->left;
				while(current != nullptr) {
					traversal_stack.push(current);
					current = current->right;
				}
			}

			return *this;
		}
		bool operator!=(const ReverseInOrderIterator& other) const {
			return !traversal_stack.empty() || !other.traversal_stack.empty();
		}
	};
	ReverseInOrderIterator rbegin() {
		if(root == nullptr) {
			return rend();
		}
		return ReverseInOrderIterator(root);
	}
	ReverseInOrderIterator rend() {
		return ReverseInOrderIterator(nullptr);
	}
	ReverseInOrderIterator rbegin() const {
		if(root == nullptr) {
			return rend();
		}
		return ReverseInOrderIterator(root);
	}
	ReverseInOrderIterator rend() const {
		return ReverseInOrderIterator(nullptr);
	}


};