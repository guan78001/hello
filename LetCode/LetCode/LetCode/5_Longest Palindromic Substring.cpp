//https://leetcode.com/problems/longest-palindromic-substring/
//Given a string S, find the longest palindromic substring in S.You may assume that the maximum length of S is 1000, and there exists one unique longest palindromic substring.

#include <vector>
#include <string>
#include <assert.h>
#include <map>
#include <unordered_map>
#include <omp.h>
#include <algorithm>
using namespace std;

//class Solution {
// public:
//  struct Range {
//    int left, right;
//    Range(int l, int r) : left(l), right(r) {}
//    int length() {
//      return right - left + 1;
//    }
//    bool operator<(const Range rhs) const {
//      return left < rhs.left || (left == rhs.left && right < rhs.right);
//    }
//  };
//
//  typedef map<Range, int> MyMap;
//  Range longestPalindrome(MyMap &m, string &s, int left, int right, Range &best) {
//    if (right + 1 - left < best.length()) {
//      return Range(left, left);
//    }
//    auto it = m.find(Range(left, right));
//    if (it != m.end()) {
//      if (it->second == 0) {
//        return Range(left, left);
//      }
//      return Range(left, right);
//    }
//
//    int l = left;
//    int r = right;
//    for (; l < r; l++, r--) {
//      if (s[l] != s[r]) {
//        break;
//      }
//    }
//    //is true
//    if (r <= l) {
//      m[Range(left, right)] = 1;
//      for (l = left + 1, r = right - 1; l <= r; l++, r--) {
//        m[Range(l, r)] = 1;
//      }
//      return Range(left, right);
//    }
//    m[Range(left, right)] = 0;
//
//    Range r1 = longestPalindrome(m, s, left + 1, right, best);
//    Range r2 = longestPalindrome(m, s, left, right - 1, best);
//    if (r1.length() > r2.length()) {
//      return r1;
//    }
//    return r2;
//  }
//
//  string longestPalindrome(string s) {
//    MyMap m;
//    Range best(0, 0);
//    Range r = longestPalindrome(m, s, 0, (int)s.length() - 1, best);
//    string s1 = s.substr(r.left, r.right - r.left + 1);
//    //printf("s=%s\n", s1.c_str());
//    return s1;
//  }
//};

//class Solution {
// public:
//  string longestPalindrome(string s) {
//    int len = s.length();
//    vector<char> p1(len, 1);
//    int bestStart = 0;
//    int bestLen = 1;
//    for (int n = 1; n < (len + 1) / 2; n++) {
//      bool isOver = true;
//      for (int i = n; i < len - n; i++) {
//        if (p1[i] && s[i - n] == s[i + n]) {
//          p1[i] = 1;
//          bestStart = i - n;
//          bestLen = 2 * n + 1;
//          isOver = false;
//        } else {
//          p1[i] = 0;
//        }
//      }
//      if (isOver) {
//        break;
//      }
//    }
//
//    vector<char> p2(len, 1);
//    int bestStart2 = 0;
//    int bestLen2 = 1;
//    for (int i = 0; i < len - 1; i++) {
//      p2[i] = s[i] == s[i + 1];
//      if (p2[i] && bestLen2 < 2) {
//        bestStart2 = i;
//        bestLen2 = 2;
//      }
//    }
//    for (int n = 0; n < len / 2; n++) {
//      bool isOver = true;
//      for (int i = n; i < len - n; i++) {
//        if (p2[i] && s[i - n] == s[i + 1 + n]) {
//          p2[i] = 1;
//          bestStart2 = i - n;
//          bestLen2 = 2 * n + 2;
//          isOver = false;
//        } else {
//          p2[i] = 0;
//        }
//      }
//      if (isOver) {
//        break;
//      }
//    }
//    if (bestLen < bestLen2) {
//      bestLen = bestLen2;
//      bestStart = bestStart2;
//    }
//    return s.substr(bestStart, bestLen);
//  }
//};


//class Solution {
// public:
//  string longestPalindrome(string s) {
//    int start = 0, best_len = 0;
//    for (int i = 0; i < s.length(); i++) {
//      int len1 = expandAroundCenter(s, i, i);
//      int len2 = expandAroundCenter(s, i, i + 1);
//      int len = std::max(len1, len2);
//      if (len > best_len) {
//        start = i - (len - 1) / 2;
//        best_len = len;
//      }
//    }
//    return s.substr(start, best_len);
//  }
//
//  int expandAroundCenter(string s, int left, int right) {
//    int L = left, R = right;
//    while (L >= 0 && R < s.length() && s[L] == s[R]) {
//      L--;
//      R++;
//    }
//    return R - L - 1;
//  }
//};

class Solution {
 public:
  string longestPalindrome(string s) {
    int len = s.length();
    if (len < 2) {
      return s;
    }
    vector<char> P(len * len);
#define  P(i,j) P[((i)*len+(j))]

    int start = 0;
    int best_len = 1;

    //init
    for (int i = 0; i < len - 1; i++) {
      P(i, i) = true;
      P(i, i + 1) = (s[i] == s[i + 1]);
      if (P(i, i + 1)) {
        best_len = 2;
        start = i;
      }
    }
    P(len - 1, len - 1) = true;

    //Dynamic Plan
    for (int k = 2; k < len; k++) {
      for (int i = 0; i < len - k; i++) {
        int j = i + k;
        if (i + 1 >= len || j - 1 < 0) {
          continue;
        }
        P(i, j) = P(i + 1, j - 1) && s[i] == s[j];
        if (P(i, j) && best_len < j + 1 - i) {
          best_len = j + 1 - i;
          start = i;
        }
      }
    }
    return s.substr(start, best_len);
  }
};
int main() {
  Solution sln;
  //assert(sln.longestPalindrome("1") == "1");
  //assert(sln.longestPalindrome("11") == "11");
  //assert(sln.longestPalindrome("12") == "1");
  //assert(sln.longestPalindrome("111") == "111");
  //assert(sln.longestPalindrome("101") == "101");
  assert(sln.longestPalindrome("1001") == "1001");
  assert(sln.longestPalindrome("12321") == "12321");
  assert(sln.longestPalindrome("1112321") == "12321");
  assert(sln.longestPalindrome("1112321abc1cba") == "abc1cba");
  double t0 = omp_get_wtime();
  string s = sln.longestPalindrome("civilwartestingwhetherthatnaptionoranynartionsoconceivedandsodedicatedcanlongendureWeareqmetonagreatbattlefiemldoftzhatwarWehavecometodedicpateaportionofthatfieldasafinalrestingplaceforthosewhoheregavetheirlivesthatthatnationmightliveItisaltogetherfangandproperthatweshoulddothisButinalargersensewecannotdedicatewecannotconsecratewecannothallowthisgroundThebravelmenlivinganddeadwhostruggledherehaveconsecrateditfaraboveourpoorponwertoaddordetractTgheworldadswfilllittlenotlenorlongrememberwhatwesayherebutitcanneverforgetwhattheydidhereItisforusthelivingrathertobededicatedheretotheulnfinishedworkwhichtheywhofoughtherehavethusfarsonoblyadvancedItisratherforustobeherededicatedtothegreattdafskremainingbeforeusthatfromthesehonoreddeadwetakeincreaseddevotiontothatcauseforwhichtheygavethelastpfullmeasureofdevotionthatweherehighlyresolvethatthesedeadshallnothavediedinvainthatthisnationunsderGodshallhaveanewbirthoffreedomandthatgovernmentofthepeoplebythepeopleforthepeopleshallnotperishfromtheearth");
  double t1 = omp_get_wtime();
  printf("used time %f ms\n", (t1 - t0) * 1000);
  printf("s=%s\n", s.c_str());
  //assert(sln.longestPalindrome("civilwartestingwhetherthatnaptionoranynartionsoconceivedandsodedicatedcanlongendureWeareqmetonagreatbattlefiemldoftzhatwarWehavecometodedicpateaportionofthatfieldasafinalrestingplaceforthosewhoheregavetheirlivesthatthatnationmightliveItisaltogetherfangandproperthatweshoulddothisButinalargersensewecannotdedicatewecannotconsecratewecannothallowthisgroundThebravelmenlivinganddeadwhostruggledherehaveconsecrateditfaraboveourpoorponwertoaddordetractTgheworldadswfilllittlenotlenorlongrememberwhatwesayherebutitcanneverforgetwhattheydidhereItisforusthelivingrathertobededicatedheretotheulnfinishedworkwhichtheywhofoughtherehavethusfarsonoblyadvancedItisratherforustobeherededicatedtothegreattdafskremainingbeforeusthatfromthesehonoreddeadwetakeincreaseddevotiontothatcauseforwhichtheygavethelastpfullmeasureofdevotionthatweherehighlyresolvethatthesedeadshallnothavediedinvainthatthisnationunsderGodshallhaveanewbirthoffreedomandthatgovernmentofthepeoplebythepeopleforthepeopleshallnotperishfromtheearth") == "aaa");
}