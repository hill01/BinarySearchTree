#include <stdlib.h>
#include <stdio.h>

struct node{
	int data;
	int height;
	struct node *left;
	struct node *right;
};

struct node *root;

void insert(int key, struct node **currentNode);
int search(int key, struct node *currentNode);
void delete(int key, struct node **currentNode);
int getBalanceFactor(struct node *currentNode);
void rebalance(struct node **currentNode);
void updateHeight(struct node *currentNode);
void printTreePreOrder(struct node *currentNode);
void printTreeInOrder(struct node *currentNode);
void printTreePostOrder(struct node *currentNode);

/*inserts key and maintains balance avl style*/
void insert(int key, struct node **currentNode){
	if(*currentNode == 0){
		*currentNode = (struct node*)malloc(sizeof(struct node));
		(*currentNode)->data = key;
		(*currentNode)->left = 0;
		(*currentNode)->right = 0;
		(*currentNode)->height = 1;

	}else if(key < (*currentNode)->data){
		insert(key, &(*currentNode)->left);
		(*currentNode)->height = (*currentNode)->left->height + 1;
		rebalance(currentNode);

	}else if(key > (*currentNode)->data){
		insert(key, &(*currentNode)->right);
		(*currentNode)->height = (*currentNode)->right->height + 1;
		rebalance(currentNode);
	}
}

/*returns 1 if found, else returns 0*/
int search(int key, struct node *currentNode){
	if(currentNode == 0){
		return 0;
	}else if(key < currentNode->data){
		return search(key, currentNode->left);
	}else if(key > currentNode->data){
		return search(key, currentNode->right);
	}else{
		return 1;
	}
}

/*deletes key if found and maintains balance*/
void delete(int key, struct node **currentNode){
	if(*currentNode == 0){
		return;
	}else if(key < (*currentNode)->data){
		delete(key, &(*currentNode)->left);
		updateHeight(*currentNode);
		rebalance(currentNode);

	}else if(key > (*currentNode)->data){
		delete(key, &(*currentNode)->right);
		updateHeight(*currentNode);
		rebalance(currentNode);
	}else{
		if((*currentNode)->left == 0 && (*currentNode)->right == 0){
			/*node has no children, delete it*/
			free(*currentNode);
			*currentNode = 0;
		}else if((*currentNode)->left == 0){
			/*node has one child, delete and replace with right child*/
			struct node *temp = (*currentNode)->right;
			free(*currentNode);
			*currentNode = temp;
		}else if((*currentNode)->right == 0){
			/*node has one child, delete and replace with left child*/
			struct node *temp = (*currentNode)->left;
			free(*currentNode);
			*currentNode = temp;
		}else{
			/*node has 2 children, replace w/ rightmost node of left subtree*/
			struct node *predecessor = (*currentNode)->left;
			int x;
			while(predecessor->right != 0){
				predecessor = predecessor->right;
			}
			x = predecessor->data;
			delete(x, &(*currentNode)->left);
			(*currentNode)->data = x;
			updateHeight(*currentNode);
			rebalance(currentNode);
		}
	}
}

/*returns height of left subtree minus height of right subtree*/
int getBalanceFactor(struct node *currentNode){
	int leftHeight, rightHeight;
	if(currentNode->left == 0){
		leftHeight = 0;
	}else{
		leftHeight = currentNode->left->height;
	}
	if(currentNode->right == 0){
		rightHeight = 0;
	}else{
		rightHeight = currentNode->right->height;
	}
	return leftHeight - rightHeight;
}

/*ensures the tree rooted at currentNode is balanced*/
void rebalance(struct node **currentNode){
	/*left subtree heavier than right*/
	if(getBalanceFactor(*currentNode) == 2){
		if(getBalanceFactor((*currentNode)->left) == -1){
			/*rotate left subtree to the left*/
			struct node *temp = (*currentNode)->left;
			(*currentNode)->left = (*currentNode)->left->right;
			temp->right = (*currentNode)->left->left;
			(*currentNode)->left->left = temp;
			updateHeight((*currentNode)->left->left);
			updateHeight((*currentNode)->left);
		}
		/*rotate to the right*/
		struct node *temp = (*currentNode);
		*currentNode = (*currentNode)->left;
		temp->left = (*currentNode)->right;
		(*currentNode)->right = temp;
		updateHeight((*currentNode)->right);
		updateHeight(*currentNode);

	/*right subtree heavier than left*/
	}else if(getBalanceFactor(*currentNode) == -2){
		if(getBalanceFactor((*currentNode)->right) == 1){
			/*rotate right subtree to the right*/
			struct node *temp = (*currentNode)->right;
			(*currentNode)->right = (*currentNode)->right->left;
			temp->left = (*currentNode)->right->right;
			(*currentNode)->right->right = temp;
			updateHeight((*currentNode)->right->right);
			updateHeight((*currentNode)->right);
		}
		/*rotate to the left*/
		struct node *temp = (*currentNode);
		*currentNode = (*currentNode)->right;
		temp->right = (*currentNode)->left;
		(*currentNode)->left = temp;
		updateHeight((*currentNode)->left);
		updateHeight(*currentNode);
	}
}

/*updates the height of currentNode to 1 + the height of its largest subtree*/
/*a leaf node has a height of 1, an empty node has a height of 0*/
/*child nodes must already have correct height for function to work*/
void updateHeight(struct node *currentNode){
	int leftHeight, rightHeight;
	if(currentNode->left == 0){
		leftHeight = 0;
	}else{
		leftHeight = currentNode->left->height;
	}
	if(currentNode->right == 0){
		rightHeight = 0;
	}else{
		rightHeight = currentNode->right->height;
	}

	if(leftHeight > rightHeight){
		currentNode->height = leftHeight + 1;
	}else{
		currentNode->height = rightHeight + 1;
	}
}

void printTreePreOrder(struct node *currentNode){
	if(currentNode != 0){
		printf("%d, height = %d\n", currentNode->data, currentNode->height);
		printTreePreOrder(currentNode->left);
		printTreePreOrder(currentNode->right);
	}
}

void printTreeInOrder(struct node *currentNode){
	if(currentNode != 0){
		printTreeInOrder(currentNode->left);
		printf("%d, height = %d\n", currentNode->data, currentNode->height);
		printTreeInOrder(currentNode->right);
	}
}

void printTreePostOrder(struct node *currentNode){
	if(currentNode != 0){
		printTreePostOrder(currentNode->left);
		printTreePostOrder(currentNode->right);
		printf("%d, height = %d\n", currentNode->data, currentNode->height);
	}
}

int main(){
	insert(3, &root);
	insert(1, &root);
	insert(5, &root);
	insert(0, &root);
	insert(2, &root);
	printTreePreOrder(root);
	printf("\n");
	delete(1, &root);
	printTreePreOrder(root);
	return 0;
}
