//https://leetcode.com/problems/longest-palindromic-substring/
//Given a string S, find the longest palindromic substring in S.You may assume that the maximum length of S is 1000, and there exists one unique longest palindromic substring.

#include <vector>
#include <string>
#include <assert.h>
#include <map>
using namespace std;

class Solution {
 public:
  struct Range {
    int left, right;
    Range(int l, int r) :left(l), right(r) {}
    int length() {
      return right - left+1;
    }
    bool operator<(const Range rhs) const {
      return left < rhs.left || (left == rhs.left && right < rhs.right);
    }
    //bool operator==(const Range rhs) const {
    //  return left == rhs.left && right == rhs.right;
    //};
  };

  Range longestPalindrome(map<Range, int> &m,string &s, int left, int right) {
    auto it = m.find(Range(left, right));
    if (it!=m.end()) {
      return Range(left, right);
    }

    int l = left;
    int r = right;
    for (; l < r; l++, r--) {
      if (s[l] != s[r]) {
        break;
      }
    }
    //is true
    if (r <= l) {
      m[Range(left, right)] = 1;
      for (l = left + 1, r = right - 1; l <= r; l++, r--) {
        m[Range(l, r)] = 1;
      }
      printf("left=%d,right=%d\n", left, right);
      return Range(left, right);
    }
    m[Range(left, right)] = 0;

    Range r1= longestPalindrome(m,s, left + 1, right);
    Range r2 = longestPalindrome(m,s, left, right - 1);
    printf("r1=[%d,%d],r2=[%d,%d]\n", r1.left, r1.right, r2.left, r2.right);
    if (r1.length()>r2.length()) {
      return r1;
    }
    return r2;
  }

  string longestPalindrome(string s) {
    map<Range, int> m;
    Range r = longestPalindrome(m,s, 0, (int)s.length() - 1);
    string s1 = s.substr(r.left, r.right - r.left + 1);
    printf("s=%s\n", s1.c_str());
    return s1;
  }
};

int main() {
  Solution sln;
  //assert(sln.longestPalindrome("1") == "1");
  //assert(sln.longestPalindrome("11") == "11");
  //assert(sln.longestPalindrome("12") == "1");
  //assert(sln.longestPalindrome("111") == "111");
  //assert(sln.longestPalindrome("101") == "101");
  //assert(sln.longestPalindrome("1001") == "1001");
  //assert(sln.longestPalindrome("12321") == "12321");
  assert(sln.longestPalindrome("11112321") == "12321");
}