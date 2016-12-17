
//Given a linked list, remove the nth node from the end of list and return its head.
//
//For example,
//
//Given linked list : 1->2->3->4->5, and n = 2.
//
//After removing the second node from the end, the linked list becomes 1->2->3->5.
//Note :
//	 Given n will always be valid.
//	 Try to do this in one pass.

#include <iostream>
#include <assert.h>
using namespace std;

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x=0) : val(x), next(nullptr) {}
};

void travel(ListNode * p) {
  while (p) {
    cout << p->val << "->";
    p = p->next;
  }
  if (!p) {
    cout << "NULL" << endl;
  }
}
//class Solution {
// public:
//  ListNode* removeNthFromEnd(ListNode* head, int n) {
//    ListNode *p1 = head;
//    ListNode *p2 = head;
//    for (int i = 0; i < n; i++) {
//      p2 = p2->next;
//    }
//    if (!p2) {
//      head = head->next;
//      return head;
//    }
//    p2 = p2->next;
//    while (p2) {
//      p1 = p1->next;
//      p2 = p2->next;
//    }
//    p1->next = p1->next->next;
//    return head;
//  }
//};
class Solution {
 public:
  ListNode* removeNthFromEnd(ListNode* head, int n) {
    if (n < 1) return head;

    ListNode node;
    ListNode *dummy = &node;
    dummy->next = head;

    ListNode *p1 = dummy;
    ListNode *p2 = dummy;
    for (int i = 0; i <= n; i++) {
      p2 = p2->next;
    }

    while (p2) {
      p1 = p1->next;
      p2 = p2->next;
    }
    if (p1->next) {
      p1->next = p1->next->next;
    }

    return dummy->next;
  }
};
int main() {
  Solution sln;
  {
    ListNode arr[5];
    arr[0].next = NULL;
    assert(sln.removeNthFromEnd(&arr[0], 1) == nullptr);
  }
  {
    ListNode arr[5];
    arr[0].next = &arr[1];
    arr[1].next = 0;
    assert(sln.removeNthFromEnd(&arr[0], 2) == &arr[1]);
  }
  {
    ListNode arr[5];
    arr[0].next = &arr[1];
    arr[1].next = 0;
    assert(sln.removeNthFromEnd(&arr[0], 1) == &arr[0]);
  }
  {
    ListNode arr[5];
    arr[0].next = &arr[1];
    arr[1].next = &arr[2];
    arr[2].next = 0;
    assert(sln.removeNthFromEnd(&arr[0], 3) == &arr[1]);
  }
  return 0;
}