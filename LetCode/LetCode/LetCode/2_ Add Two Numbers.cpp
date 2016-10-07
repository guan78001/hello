//https://leetcode.com/problems/add-two-numbers/
//You are given two linked lists representing two non - negative numbers.The digits are stored in reverse order and each of their nodes contain a single digit.Add the two numbers and return it as a linked list.
//
//Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
//   Output : 7 -> 0 -> 8
/**
* Definition for singly-linked list.
* struct ListNode {
*     int val;
*     ListNode *next;
*     ListNode(int x) : val(x), next(NULL) {}
* };
*/

#include <stdio.h>
#include <assert.h>
struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x=0) : val(x), next(NULL) {}
};



class Solution {
 public:
  ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode *head = l1!=NULL ? l1 : l2;
    ListNode *last = head;
    int sum = 0;
    while (l1 && l2) {
      sum += l1->val + l2->val;
      int d = sum / 10;
      l1->val = sum -d*10;
      sum = d;

      last = l1;
      l1 = l1->next;
      l2 = l2->next;
    }

    ListNode *cur = l1 != NULL ? l1 : l2;
    last->next = cur;

    while (cur) {
      sum += cur->val;
      int d = sum / 10;
      cur->val = sum - d * 10;
      sum = d;

      last = cur;
      cur = cur->next;
    }
    if (sum) {
      last->next = new ListNode(sum);
    }

    return head;
  }
};

void test1() {
  ListNode l1[3];
  ListNode l2[3];

  l1[0].val = 2;
  l1[0].next = &l1[1];

  l1[1].val = 4;
  l1[1].next = &l1[2];

  l1[2].val = 3;
  l1[2].next = nullptr;

  l2[0].val = 5;
  l2[0].next = &l2[1];

  l2[1].val = 6;
  l2[1].next = &l2[2];

  l2[2].val = 4;
  l2[2].next = nullptr;

  Solution sln;
  ListNode *node=sln.addTwoNumbers(l1, l2);
  assert(node[0].val == 7 && node[1].val == 0 && node[2].val == 8);
}
void test2() {
  ListNode l1[3];
  ListNode l2[3];

  l1[0].val = 5;
  l1[0].next = nullptr;

  l2[0].val = 5;
  l2[0].next = nullptr;


  Solution sln;
  ListNode *node = sln.addTwoNumbers(l1, l2);
  assert(node->val == 0 && node->next->val ==1);
}
void test3() {
  ListNode l1[3];
  ListNode l2[3];

  l1[0].val = 1;
  l1[0].next = nullptr;

  l2[0].val = 9;
  l2[0].next = &l2[1];

  l2[1].val = 9;
  l2[1].next = nullptr;

  Solution sln;
  ListNode *node = sln.addTwoNumbers(l1, l2);
  assert(node->val == 0 && node->next->val == 0 && node->next->next->val == 1);
}
int main() {
  //test1();
  //test2();
  test3();
  return 0;
}