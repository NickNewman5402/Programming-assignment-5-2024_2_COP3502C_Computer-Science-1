/*
  Nick Newman
  7/22/24
  Computer Science 1
  Summer 2024
  Dr. Tanvir Ahmed

  I want to be considered for the bonus and I agree that any of my submissions
  after<July 23> (put July 22 or July 23 based on your wish which level of bonus
  do you want) will not be considered for grading regardless of my score on the
  most recent submission before <July 23> (put the same date here)


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct tree_node {
  char name[25];
  int points;
  struct tree_node *left;
  struct tree_node *right;
} tree_node;

void printInorder(tree_node* current_ptr);
void printNode(tree_node* current_ptr);
tree_node* insert(tree_node* root, tree_node* element);
tree_node* create_node(char name[25], int points);
tree_node* delete(tree_node* root, char* name);
tree_node* findNode(struct tree_node *current_ptr, char* value);
int numnodes(struct tree_node* root);
int numNodesLeft(tree_node* root);
int numNodesRight(tree_node* root);
tree_node* parent(struct tree_node *root, struct tree_node *node);
tree_node* minVal(struct tree_node *root);
tree_node* maxVal(struct tree_node *root);
int isLeaf(struct tree_node *node);
int hasOnlyLeftChild(struct tree_node *node);
int hasOnlyRightChild(struct tree_node *node);

int main(void) {

  int numInputs, points, height;
  char command[15], name[25];


  tree_node *my_root = NULL, *temp_node, *activeNode;



  scanf("%d", &numInputs);


  for (int i = 0; i < numInputs; i++) {

    scanf("%14s", command);


    //printf("\nCommand read: '%s'\n", command);


    if (strcmp(command, "add") == 0) {
      scanf("%s %d", name, &points);
      if (!findNode(my_root, name)) {
        temp_node = create_node(name, points); // Create the node.
        my_root = insert(my_root, temp_node); // Insert the value.
        activeNode = findNode(my_root, name);
        int numNodes = numnodes(my_root);
        height = log2(numNodes);
        printf("%s %d %d\n", activeNode->name, activeNode->points, height);
      }
      else {

        activeNode = findNode(my_root, name);
        activeNode->points += points;
        int numNodes = numnodes(my_root);
        height = log2(numNodes);
        printf("%s %d %d", activeNode->name, activeNode->points, height);

      }
    }

    if (strcmp(command, "sub") == 0) {
      scanf("%s %d", name, &points);
      if (!findNode(my_root, name)) {
        temp_node = create_node(name, points); // Create the node.
        my_root = insert(my_root, temp_node); // Insert the value.
        activeNode = findNode(my_root, name);
        printf("%s %d %d", activeNode->name, activeNode->points, height);
      }
      else {

        activeNode = findNode(my_root, name);
        activeNode->points -= points;
        if (activeNode->points < 0) {
          activeNode->points = 0;
        }
        printf("%s %d %d", activeNode->name, activeNode->points, height);

      }
    }

    if (strcmp(command, "del") == 0) {
      scanf("%s %d", name, &points);
      if (findNode(my_root, name)) {
        printf("%s not found", name);
      }

      activeNode = findNode(my_root, name);
      delete(activeNode, name);
    }

    if (strcmp(command, "height_balance") == 0) {

      int a, b;

      if (my_root->left == NULL) {
        a = -1;
      }
      else {
        a = log2(numNodesLeft(my_root));
      }

      if (my_root->right == NULL) {
        b = -1;
      }
      else {
        b = log2(numNodesRight(my_root));
      }

      printf("left height = %d right height = %d ", a, b);

      if (a == b)
        printf("balanced\n");

      printf("unbalanced");

    }

    if (strcmp(command, "search") == 0) {
      activeNode = findNode(my_root, name);
      printNode(activeNode);

    }

  }

  return 0;
}

void printInorder(tree_node *current_ptr) {

  // Only traverse the node if it's not null.
  if (current_ptr != NULL) {

    printInorder(current_ptr->left);                          // Go Left.
    printf("%s %d ", current_ptr->name, current_ptr->points); // Print the root.
    printf("\n");
    printInorder(current_ptr->right);                         // Go Right.
  }
}

void printNode(tree_node* ptr) {
  printf("%s %d", ptr->name, ptr->points);
}

tree_node* insert(tree_node* root, tree_node* element) {

  // Inserting into an empty tree.
  if (root == NULL) {
    //printf("Element %s points %d -> address: %p\n", element->name, element->points, &(element));
    return element;
  }
  else {

    // element should be inserted to the right.
    if (strcmp(root->name, element->name) < 0) {

      // There is a right subtree to insert the node.
      if (root->right != NULL)
        root->right = insert(root->right, element);

      // Place the node directly to the right of root.
      else
        root->right = element;
    }

    // element should be inserted to the left.
    else {

      // There is a left subtree to insert the node.
      if (root->left != NULL)
        root->left = insert(root->left, element);

      // Place the node directly to the left of root.
      else
        root->left = element;
    }

    // Return the root pointer of the updated tree.
    return root;
  }
}

tree_node* create_node(char name[25], int points) {

  tree_node *temp = malloc(sizeof(tree_node));
  if (temp == NULL) {
    // Handle memory allocation failure
    return NULL;
  }
  strcpy(temp->name, name);
  temp->points = points;
  temp->left = NULL;
  temp->right = NULL;
  return temp;
}

tree_node* delete(tree_node* root, char* name) {

  tree_node *delnode, *new_del_node, *save_node;
  tree_node *par;
  char* save_val;

  delnode = findNode(root, name); // Get a pointer to the node to delete.

  par = parent(root, delnode); // Get the parent of this node.

  // Take care of the case where the node to delete is a leaf node.
  if (isLeaf(delnode)) {// case 1

    // Deleting the only node in the tree.
    if (par == NULL) {
      free(root); // free the memory for the node.
      return NULL;
    }

    // Deletes the node if it's a left child.
    if (strcmp(par->name, name) < 0) {
      free(par->left); // Free the memory for the node.
      par->left = NULL;
    }

    // Deletes the node if it's a right child.
    else {
      free(par->right); // Free the memory for the node.
      par->right = NULL;
    }

    return root; // Return the root of the new tree.
  }

  // Take care of the case where the node to be deleted only has a left
  // child.
  if (hasOnlyLeftChild(delnode)) {

    // Deleting the root node of the tree.
    if (par == NULL) {
      save_node = delnode->left;
      free(delnode); // Free the node to delete.
      return save_node; // Return the new root node of the resulting tree.
    }

    // Deletes the node if it's a left child.
    if (strcmp(par->name, name) < 0) {
      save_node = par->left; // Save the node to delete.
      par->left = par->left->left; // Readjust the parent pointer.
      free(save_node); // Free the memory for the deleted node.
    }

    // Deletes the node if it's a right child.
    else {
      save_node = par->right; // Save the node to delete.
      par->right = par->right->left; // Readjust the parent pointer.
      free(save_node); // Free the memory for the deleted node.
    }

    return root; // Return the root of the tree after the deletion.
  }

  // Takes care of the case where the deleted node only has a right child.
  if (hasOnlyRightChild(delnode)) {

    // Node to delete is the root node.
    if (par == NULL) {
      save_node = delnode->right;
      free(delnode);
      return save_node;
    }

    // Delete's the node if it is a left child.
    if (strcmp(par->name, name) < 0) {
      save_node = par->left;
      par->left = par->left->right;
      free(save_node);
    }

    // Delete's the node if it is a right child.
    else {
      save_node = par->right;
      par->right = par->right->right;
      free(save_node);
    }
    return root;
  }
  //if your code reaches hear it means delnode has two children
    // Find the new physical node to delete.
  new_del_node = minVal(delnode->right);
  save_val = new_del_node->name;

  delete(root, save_val);  // Now, delete the proper value.

  // Restore the data to the original node to be deleted.
  strcpy(save_val, delnode->name);

  return root;
}

tree_node* findNode(struct tree_node *current_ptr, char* value) {

  // Check if there are nodes in the tree.
  if (current_ptr != NULL) {

    // Found the value at the root.
    if (strcmp(current_ptr->name, value) == 0)
      return current_ptr;

    // Search to the left.
    if (strcmp(current_ptr->name, value) > 0) // strcmp
      return findNode(current_ptr->left, value);

    // Or...search to the right.
    else
      return findNode(current_ptr->right, value);

  }
  else
    return NULL; // No node found.
}

int numnodes(struct tree_node* root) {

  if (root == NULL) return 0;
  return 1 + numnodes(root->left) + numnodes(root->right);
}

int numNodesLeft(struct tree_node* root) {
  if (root == NULL) return 0;
  return 1 + numnodes(root->left);
}

int numNodesRight(tree_node* root) {
  if (root == NULL) return 0;
  return 1 + numnodes(root->right);
}

tree_node* parent(struct tree_node *root, struct tree_node *node) {

  // Take care of NULL cases.
  if (root == NULL || root == node)
    return NULL;

  // The root is the direct parent of node.
  if (root->left == node || root->right == node)
    return root;

  // Look for node's parent in the left side of the tree.
  if (strcmp(root->name, node->name) < 0)
    return parent(root->left, node);

  // Look for node's parent in the right side of the tree.
  else if (strcmp(root->name, node->name) > 0)
    return parent(root->right, node);

  return NULL; // Catch any other extraneous cases.

}

tree_node* minVal(struct tree_node *root) {

  // Root stores the minimal value.
  if (root->left == NULL)
    return root;

  // The left subtree of the root stores the minimal value.
  else
    return minVal(root->left);
}

tree_node* maxVal(struct tree_node *root) {

  // Root stores the maximal value.
  if (root->right == NULL)
    return root;

  // The right subtree of the root stores the maximal value.
  else
    return maxVal(root->right);
}

int isLeaf(struct tree_node *node) {

  return (node->left == NULL && node->right == NULL);
}

int hasOnlyLeftChild(struct tree_node *node) {
  return (node->left != NULL && node->right == NULL);
}

int hasOnlyRightChild(struct tree_node *node) {
  return (node->left == NULL && node->right != NULL);
}
