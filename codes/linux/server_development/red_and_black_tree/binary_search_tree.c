#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct tree_node_s {
  int val;
  struct tree_node_s *left;
  struct tree_node_s *right;
} tree_node_t;

tree_node_t* search(tree_node_t *root, int target);
void insert(tree_node_t **root, int num);
void delete(tree_node_t **root, int target);
void inorder(const tree_node_t *root);

int main() {
  tree_node_t *root = NULL;
  for (int i = 0; i < 10; i++)
    insert(&root, i+1);

  inorder(root);
  putchar('\n');

  for (int i = 0; i < 10; i++) {
    tree_node_t *cur = search(root, i+1);
    if (cur)
      printf("cur.val = %d\n", cur->val);
  }
  
  for (int i = 0; i < 10; i++) {
    delete(&root, i+1);
    inorder(root);
    putchar('\n');
  }

  return 0;
}

void inorder(const tree_node_t *root) {
  if (NULL == root)
    return;
  
  inorder(root->left);
  printf("%d ", root->val);
  inorder(root->right);
}

void delete(tree_node_t **root, int target) {
  if (NULL == *root || NULL == root)
    return;

  tree_node_t *cur = *root;
  tree_node_t *prev = NULL;
  while (cur != NULL && cur->val != target) {
    prev = cur;
    if (cur->val > target) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }

  if (NULL == cur) // 不存在待删除的节点
    return;

  if (NULL == cur->left && NULL == cur->right) {  // 待删除节点的度为 0
    if (NULL == prev)  // 如果是根节点
      *root = NULL;
    else if (prev->left == cur)
      prev->left = NULL;
    else
      prev->right = NULL;

    free(cur);
    cur = NULL;
  } else if (!cur->left || !cur->right) { // 待删除节点的度为 1
    // 保存叶子节点，判断在哪一棵子树上
    tree_node_t *tmp = (cur->left != NULL) ? cur->left : cur->right;
    if (prev == NULL) // 如果是根节点
      *root = tmp;
    // 判断要删除的节点是在左子树还是右子树
    else if (prev->left == cur)
      prev->left = tmp;
    else
      prev->right = tmp;
    free(cur);
    cur = NULL;
  } else { // 待删除节点的度为 2
    // 找到右子树最小节点，中序遍历
    tree_node_t *tmp = cur->right;
    tree_node_t *tmp_prev = cur;
    while (tmp->left != NULL) {
      tmp_prev = tmp;
      tmp = tmp->left;
    }

    // 保存右子树最小节点的值，然后将该节点删除
    cur->val = tmp->val;
    // 由于此节点肯定是最小节点，没有左子树，但是可能会有右子树
    if (tmp_prev->left == tmp)
      tmp_prev->left = tmp->right;
    else
      tmp_prev->right = tmp->right;
    free(tmp);
  }
}

void insert(tree_node_t **root, int num) {
  if (NULL == root)
    return;

  // 如果根节点为空，则初始化根节点
  if (NULL == *root) {
    *root = malloc(sizeof(tree_node_t));
    memset(*root, 0, sizeof(tree_node_t));
    (*root)->val = num;
    return;
  }

  // 根节点不为空，则查找直到子树节点为空
  tree_node_t *cur = *root;
  tree_node_t *prev = NULL;
  while (cur != NULL) {
    if (cur->val == num)// 如果树中已经存在则直接退出
      return;
    
    prev = cur;
    if (cur->val > num) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }

  // 插入节点
  tree_node_t *new_node = malloc(sizeof(tree_node_t));
  memset(new_node, 0, sizeof(tree_node_t));
  new_node->val = num;
  if (num < prev->val)
    prev->left = new_node;
  else
    prev->right = new_node;
}

tree_node_t* search(tree_node_t *root, int target) {
  if (NULL == root)
    return NULL;

  tree_node_t *cur = root;
  // 循环查找，越过叶节点后跳出
  while (cur != NULL) {
    if (cur->val == target) { // 找到目标节点，跳出循环
      break;
    } else if (cur->val > target) { // 目标节点在 cur 的右子树中
      cur = cur->left;
    } else {  // 目标节点在 cur 的左子树中
      cur = cur->right;
    }
  }

  return cur;
}