#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED 0
#define BLACK 1

typedef struct tree_node_s {
  int key;
  void *value;
  struct tree_node_s *left;
  struct tree_node_s *right;
  struct tree_node_s *parent;
  char color;
} tree_node_t;

typedef struct red_black_tree_s {
  tree_node_t *root;  // 根节点
  tree_node_t *nil;   // 通用的叶子节点
} rbtree_t;

tree_node_t *search(rbtree_t *tree, int key);
void insert(rbtree_t *tree, tree_node_t *node);
void left_rotate(rbtree_t *tree, tree_node_t *node);
void right_rotate(rbtree_t *tree, tree_node_t *node);
tree_node_t *delete(rbtree_t *tree, tree_node_t *node);
void insert_balance(rbtree_t *tree, tree_node_t *node);
void inorder(rbtree_t *tree, tree_node_t *node);
tree_node_t *mini(rbtree_t *tree, tree_node_t *node);
tree_node_t *maxi(rbtree_t *tree, tree_node_t *node);
tree_node_t *successor(rbtree_t *tree, tree_node_t *node);
tree_node_t * delete_balance(rbtree_t *tree, tree_node_t *node);

int main() {
  int arr[20] = {24, 25, 13, 35, 23, 26, 67, 47, 38, 98, 20, 19, 17, 49, 12, 21, 9, 18, 14, 15};

  rbtree_t *tree = malloc(sizeof(rbtree_t));
  if (NULL == tree) {
    printf("malloc failed\n");
    return -1;
  }

  tree->nil = malloc(sizeof(tree_node_t));
  tree->nil->color = BLACK;
  tree->root = tree->nil;

  tree_node_t *node = tree->nil;
  int i = 0;
  for (i = 0; i < 20; i++) {
    node = malloc(sizeof(tree_node_t));
    node->key = arr[i];
    node->value = NULL;
    insert(tree, node);
  }

  // inorder(tree, tree->root);
  // printf("----------------------------------------\n");

  for (i = 0; i < 20; i++) {
    tree_node_t *node = search(tree, arr[i]);
    if (node != tree->nil) {
      printf("node.key = %d\n", node->key);
      tree_node_t *cur = delete(tree, node);
      free(cur);
      cur = NULL;
      inorder(tree, tree->root);
      printf("----------------------------------------\n");
    }
  }

  return 0;
}

tree_node_t *search(rbtree_t *tree, int key) {
  tree_node_t *node = tree->root;
  while (node != tree->nil) {
    if (key < node->key) {
      node = node->left;
    } else if (key > node->key) {
      node = node->right;
    } else {
      return node;
    }
  }
  return tree->nil;
}

void insert(rbtree_t *tree, tree_node_t *node) {
  // 找到插入的位置
  tree_node_t *cur = tree->root;
  tree_node_t *prev = tree->nil;
  while (tree->nil != cur) {
    prev = cur;
    if (node->key > cur->key)
      cur = cur->right;
    else if (node->key < cur->key)
      cur = cur->left;
    else
      return;
  }

  node->parent = prev;
  if (prev == tree->nil)  // 是根节点
    tree->root = node;
  else if (node->key < prev->key)
    prev->left = node;
  else
    prev->right = node;

  node->left = tree->nil;
  node->right = tree->nil;
  node->color = RED;
  insert_balance(tree, node);
}

void right_rotate(rbtree_t *tree, tree_node_t *node) {
  if (NULL == node)
    return;

  tree_node_t *lchild = node->left;  // 旋转支点
  node->left = lchild->right;
  if (lchild->right != tree->nil)
    lchild->right->parent = node;

  lchild->parent = node->parent;
  if (lchild->parent == tree->nil)  // 是根节点
    tree->root = lchild;
  else if (node == node->parent->left)
    node->parent->left = lchild;
  else
    node->parent->right = lchild;

  lchild->right = node;
  node->parent = lchild;
}

void left_rotate(rbtree_t *tree, tree_node_t *node) {
  if (NULL == node)
    return;

  tree_node_t *rchild = node->right;  // 旋转支点
  node->right = rchild->left;
  if (rchild->left != tree->nil)
    rchild->left->parent = node;

  rchild->parent = node->parent;
  if (rchild->parent == tree->nil)
    tree->root = rchild;
  else if (node == node->parent->left)
    node->parent->left = rchild;
  else
    node->parent->right = rchild;

  rchild->left = node;
  node->parent = rchild;
}

tree_node_t *mini(rbtree_t *tree, tree_node_t *node) {
  while (node->left != tree->nil)
    node = node->left;

  return node;
}

tree_node_t *maxi(rbtree_t *tree, tree_node_t *node) {
  while (node->right != tree->nil)
    node = node->right;

  return node;
}

tree_node_t *successor(rbtree_t *tree, tree_node_t *node) {
  tree_node_t *cur = node->parent;
  if (node->right != tree->nil)
    return mini(tree, node->right);

  while (cur != tree->nil && node == cur->right) {
    node = cur;
    cur = cur->parent;
  }
  return cur;
}

tree_node_t *delete_balance(rbtree_t *tree, tree_node_t *node) {
  while (node != tree->nil && node->parent != tree->nil && node->color == BLACK) {
    if (node == node->parent->left) { // 当前节点位于左子树
      tree_node_t *brother = node->parent->right; // 兄弟节点
      if (brother != tree->nil && brother->color == BLACK) {  // 情况 1：兄弟节点是黑色
        if (brother->left->color == BLACK && brother->right->color == BLACK) { // 兄弟孩子都是黑色
          // 将 bro 变为红色，
          brother->color = RED;
          if (node->parent == tree->root) { // 如果父节点是根节点，直接退出
            tree->root->left = tree->nil;
            return node;
          }

          if (node->parent->color == RED) {// 如果父节点是红色的，只需要将父节点改为黑色并退出
            node->parent->color = BLACK;
            node->parent->left = tree->nil;
            return node;  
          }
          // 更改循环条件 node = node->parent
          node = node->parent;
        } else {  // 兄弟至少有一个红孩子
          if (brother->right != tree->nil && brother->right->color == RED) {  // 右子孩子是红色，RR
            // 兄弟孩子的颜色变成兄弟的颜色，兄弟的颜色变成父的颜色，父的颜色变为黑，对父节点进行左旋
            brother->right->color = brother->color;
            brother->color = brother->parent->color;
            brother->parent->color = BLACK;
            left_rotate(tree, brother->parent);
          } else if (brother->left != tree->nil && brother->left->color == RED) { // 右子孩子不存在或为黑色，左子孩子是红色，RL
            // 兄弟孩子的颜色变为父节点的颜色，父节点颜色变为黑色，兄弟节点先右旋父节点再左旋
            brother->left->color = brother->parent->color;
            brother->parent->color = BLACK;
            tree_node_t *temp = brother->parent;
            right_rotate(tree, brother);
            left_rotate(tree, temp);
          }
          // 旋转完成后，将需要删除的节点脱离红黑树
          node->parent->left = tree->nil;
          break;
        }
      } else if (brother != tree->nil) {  // 情况 2：兄弟节点是红色的
        // 兄弟节点和父节点交换颜色，然后向删除节点的位置旋转（在左子树就左旋，在右子树就右旋）
        int brother_color = brother->color;
        brother->color = brother->parent->color;
        brother->parent->color = brother_color;
        tree_node_t *temp = brother->right;
        left_rotate(tree, brother->parent);
        // 此时的兄弟会发生改变，则继续调整，待删除的节点不变
        continue;
      }
    } else {  // 当前节点位于右子树
      tree_node_t *brother = node->parent->left;
      if (brother != tree->nil && brother->color == BLACK) {  // 兄弟节点是黑色
        if (brother->left->color == BLACK && brother->right->color == BLACK) { // 兄弟孩子都是黑色
          // 将 bro 变为红色
          brother->color = RED;
          if (node->parent == tree->root) { // 如果父节点是根节点，直接退出
            tree->root->right = tree->nil;
            return node;
          }

          if (node->parent->color == RED) { // 如果父节点是红色的，只需要将父节点改为黑色并退出
            node->parent->color = BLACK;
            node->parent->right = tree->nil;
            return node;  
          }
          // 更改循环条件 node = node->parent
          node = node->parent;
        } else {  // 兄弟至少有一个红孩子
          if (brother->left != tree->nil && brother->left->color == RED) {  // 左子孩子是红色，LL
            // 兄弟孩子的颜色编程兄弟的颜色，兄弟的颜色编程父的颜色，父的颜色变为黑，对父节点进行右旋
            brother->left->color = brother->color;
            brother->color = brother->parent->color;
            brother->parent->color = BLACK;
            right_rotate(tree, brother->parent);
          } else if (brother->right != tree->nil && brother->right->color == RED) { // 左子孩子不存在或为黑色，右子孩子是红色，LR
            // 兄弟孩子的颜色变为父节点的颜色，父节点颜色变为黑色，兄弟节点先左旋，在将父节点右旋
            brother->right->color = brother->parent->color;
            brother->parent->color = BLACK;
            tree_node_t *temp = brother->parent;
            left_rotate(tree, brother);
            right_rotate(tree, temp);
          }
          // 旋转完成后，将需要删除的节点脱离红黑树
          node->parent->right = tree->nil;
          break;
        }
      } else if (brother != tree->nil) {  // 情况 2：兄弟节点是红色的
        // 兄弟节点和父节点交换颜色，然后向删除节点的位置旋转（在左子树就左旋，在右子树就右旋）
        int brother_color = brother->color;
        brother->color = brother->parent->color;
        brother->parent->color = brother_color;
        right_rotate(tree, brother->parent);
        // 此时的兄弟会发生改变，则继续调整，待删除的节点不变
        continue;
      }
    }
  }

  node->color = BLACK;
  return node;
}

tree_node_t *delete(rbtree_t *tree, tree_node_t *node) {
  if (node->left == tree->nil && node->right == tree->nil) { // 删除的节点度为 0
    if (node->color == RED) { // 此节点的颜色是红色的直接删除
      if (node == node->parent->left)
        node->parent->left = tree->nil;
      else
        node->parent->right = tree->nil;
      return node;
    } else { // 此节点的颜色是黑色的，需要根据兄弟的情况进行调整
      if (node == tree->root) { // 删除的是根节点
        tree->root = tree->nil;
        return node;
      }

      if (node->parent == tree->root) {  // 删除的是根节点，则会转换成为删除其子节点
        if (node->parent->left == node) {
          node->parent->left = tree->nil;
          node->parent->right->color = RED;
        } else {
          node->parent->right = tree->nil;
          node->parent->left->color = RED;
        }
        
        return node;
      }
      return delete_balance(tree, node);
    }
  } else if (node->left == tree->nil || node->right == tree->nil) { // 删除的节点度为 1
    // 被删除的节点一定是黑色节点并且子节点一定是红色节点，否则不满足红黑树性质
    tree_node_t *cur = (node->left != tree->nil ? node->left : node->right);
    cur->color = BLACK;
    cur->parent = node->parent;

    if (node == tree->root)
      tree->root = cur;
    else if (node->parent->left == node)
      node->parent->left = cur;
    else
      node->parent->right = cur;
    return node;
  } else { // 删除的节点度为 2
    // 找到该节点的后继，右子树中的最小节点
    tree_node_t *next_node = successor(tree, node);
    node->key = next_node->key;
    return delete(tree, next_node);  // 转换为删除 node 的后继节点
  }
}

void insert_balance(rbtree_t *tree, tree_node_t *node) {
  if (NULL == tree || NULL == node)
    return;

  while (node->parent != tree->nil && node->parent->color == RED) {  // 父节点是红色的
    if (node->parent == node->parent->parent->left) {                // 是左子树
      tree_node_t *cur = node->parent->parent->right;
      if (cur != tree->nil && cur->color == RED) {  // 叔父节点是红色的，逐级向上变色
        node->parent->color = BLACK;
        cur->color = BLACK;
        node->parent->parent->color = RED;
        node = node->parent->parent;
      } else {                              // 叔父节点是黑色的，需要根据插入节点所在位置来进行旋转
        if (node == node->parent->right) {  // 在父节点的右侧
          node = node->parent;
          left_rotate(tree, node);
        }

        node->parent->color = BLACK;
        node->parent->parent->color = RED;
        right_rotate(tree, node->parent->parent);
      }
    } else {  // 是右子树
      tree_node_t *cur = node->parent->parent->left;
      if (cur != tree->nil && cur->color == RED) {  // 叔父节点是红色的，逐级向上变色
        node->parent->color = BLACK;
        cur->color = BLACK;
        node->parent->parent->color = RED;
        node = node->parent->parent;
      } else {  // 叔父节点是黑色的，需要根据插入节点所在位置来进行旋转
        if (node == node->parent->left) {
          node = node->parent;
          right_rotate(tree, node);
        }

        node->parent->color = BLACK;
        node->parent->parent->color = RED;
        left_rotate(tree, node->parent->parent);
      }
    }
  }

  // 确保根节点为黑色
  tree->root->color = BLACK;
}

void inorder(rbtree_t *tree, tree_node_t *node) {
  if (tree->nil == node)
    return;

  inorder(tree, node->left);
  printf("%d ", node->key);
  printf(" color = %d\n", node->color);
  inorder(tree, node->right);
}