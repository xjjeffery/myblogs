#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree_node_s {
  int val;
  int height;
  struct tree_node_s *left;
  struct tree_node_s *right;
} tree_node_t;

int get_height(tree_node_t *node);
void update_height(tree_node_t *node);
tree_node_t* search(tree_node_t *node, int target);
tree_node_t *insert(tree_node_t *node, int num);
tree_node_t *delete(tree_node_t *node, int target);
int get_balance_factor(tree_node_t *node);
tree_node_t *balance(tree_node_t *node);
tree_node_t *left_rotate(tree_node_t *node);
tree_node_t *right_rotate(tree_node_t *node);
void inorder(const tree_node_t *root);


int main() {
  tree_node_t *node = NULL;
  for (int i = 0; i < 10; i++)
    node = insert(node, i+1);

  inorder(node);
  putchar('\n');

  for (int i = 0; i < 10; i++) {
    tree_node_t *cur = search(node, i+1);
    if (cur)
      printf("cur.val = %d\n", cur->val);
  }
  
  for (int i = 0; i < 10; i++) {
    node = delete(node, i+1);
    inorder(node);
    putchar('\n');
  }

  return 0;
}

int get_height(tree_node_t *node) {
  if (NULL != node)
    return node->height;
  
  return -1;
}

void update_height(tree_node_t *node) {
  if (NULL != node) {
    int rh = get_height(node->right); // 左子树的高度
    int lh = get_height(node->left);  // 右子树的高度
    node->height = (rh > lh ? rh : lh) + 1; // 更新高度
  }
}

tree_node_t* search(tree_node_t *root, int target) {
  if (NULL == root)
    return NULL;
  
  tree_node_t *cur = root;
  while (NULL != cur) {
    if (target == cur->val)
      return cur;
    else if (target > cur->val)
      cur = cur->right;
    else
      cur = cur->left;
  }

  return NULL;
}

tree_node_t *insert(tree_node_t *node, int num) {
  if (NULL == node) { // 平衡树为空树，插入根节点
    node = malloc(sizeof(tree_node_t));
    memset(node, 0, sizeof(tree_node_t));
    node->height = 0;
    node->val = num;
    return node;
  }

  // 插入的节点为其他节点
  if (num == node->val) // 节点已经存在，则直接退出
    return node;
  else if (num > node->val) // 插入的值大于此值，则在节点右子树上插入
    node->right = insert(node->right, num);
  else
    node->left = insert(node->left, num);  // 插入的值小于此值，则在节点左子树上插入

  update_height(node);  // 插入节点后，更新节点的高度
  return balance(node); // 平衡树
}

tree_node_t *delete(tree_node_t *node, int target) {
  if (NULL == node)
    return NULL;

  if (target > node->val) {
    node->right = delete(node->right, target);
  } else if (target < node->val) {
    node->left = delete(node->left, target);
  } else {
    if (NULL == node->left && NULL == node->right) {  // 度为 0，直接删除节点
      free(node);
      node = NULL;
      return NULL;
    } else if (node->left == NULL || node->right == NULL) { // 度为 1
      tree_node_t *child = node->left != NULL ? node->left : node->right ;
      free(node);
      node = child;
    } else {  // 度为 2
      // 找到右子树最小节点，中序遍历
      tree_node_t *tmp = node->right;
      tree_node_t *tmp_prev = node;
      while (tmp->left != NULL) {
        tmp_prev = tmp;
        tmp = tmp->left;
      }

      // 保存右子树最小节点的值，然后将该节点删除
      node->val = tmp->val;
      // 由于此节点肯定是最小节点，没有左子树，但是可能会有右子树
      if (tmp_prev->left == tmp)
        tmp_prev->left = tmp->right;
      else
        tmp_prev->right = tmp->right;

      free(tmp);
      tmp = NULL;
    }
  }

  // 节点删除以后，更新节点高度，平衡树
  update_height(node);
  return balance(node);
}

int get_balance_factor(tree_node_t *node) {
  if (NULL == node)
    return 0;
  
  return get_balance_factor(node->left) - get_balance_factor(node->right);
}

tree_node_t *balance(tree_node_t *node) {
  if (NULL == node)
    return NULL;

  int bf = get_balance_factor(node);  // 获取平衡因子
  if (bf > 1) { // 左偏树
    if (get_height(node->left->left) >= get_height(node->left->right)) { // 右旋
      return right_rotate(node);
    } else {  // 先左旋再右旋
      node->left = left_rotate(node->left);
      return right_rotate(node);
    }
  }

  if (bf < -1) {  // 右偏树
    if (get_height(node->right->right) >= get_height(node->right->left)) { // 左旋
      return left_rotate(node);
    } else {  // 先右旋再左旋
      node->right = right_rotate(node->right);
      return left_rotate(node);
    }
  }

  return node;  // 已经是平衡状态
}

tree_node_t *right_rotate(tree_node_t *node) {
  if (NULL == node)
    return NULL;

  tree_node_t *lchild = node->left;
  tree_node_t *grand_rchild = lchild->right;
  node->left = grand_rchild;
  lchild->right = node;
  // 改变的节点有两个，因此更新这两个节点的高度
  update_height(node);
  update_height(lchild);
  return lchild;
}

tree_node_t *left_rotate(tree_node_t *node) {
  if (NULL == node)
    return NULL;

  tree_node_t *rchild = node->right;
  tree_node_t *grand_lchild = rchild->left;
  node->right = grand_lchild;
  rchild->left = node;
  // 改变的节点有两个，因此更新这两个节点的高度
  update_height(node);
  update_height(rchild);
  return rchild;
}

void inorder(const tree_node_t *root) {
  if (NULL == root)
    return;
  
  inorder(root->left);
  printf("%d ", root->val);
  printf(" height = %d\n", root->height);
  inorder(root->right);
}