#include <iostream>
using namespace std;

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x = 0) : val(x), next(nullptr) {}
};

class Solution {
 public:
  ListNode *removeNthFromEnd(ListNode *head, int n) {
    ListNode *p = head;
    ListNode *q = p;
    for (int i = 0; i < n ; i++) {
      q = q->next;
    }
    if (!q) {
      return nullptr;
    }
    while (q->next) {
      q = q->next;
      p = p->next;
    }
    p->next = p->next->next;

    return head;
  }
};
void travel(ListNode *p) {
  while (p) {
    cout << p->val << "->";
    p = p->next;
  }
  cout << endl;
}
int main() {
  Solution sln;
  ListNode arr[6];

  for (int i = 0; i < 6; i++) {
    arr[i].val = i + 1;
    arr[i].next = &arr[i + 1];
  }
  arr[4].next = nullptr;
  travel(&arr[0]);
  sln.removeNthFromEnd(&arr[0], 2);
  travel(&arr[0]);

  return 0;
}