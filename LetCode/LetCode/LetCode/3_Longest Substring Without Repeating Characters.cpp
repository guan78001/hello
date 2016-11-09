//https://leetcode.com/problems/longest-substring-without-repeating-characters/
//3. Longest Substring Without Repeating Characters  QuestionEditorial Solution  My Submissions
//Total Accepted : 195130
//Total Submissions : 839210
//Difficulty : Medium
//			 Given a string, find the length of the longest substring without repeating characters.
//
//		 Examples :
//
//				  Given "abcabcbb", the answer is "abc", which the length is 3.
//
//				  Given "bbbbb", the answer is "b", with the length of 1.
//
//				  Given "pwwkew", the answer is "wke", with the length of 3. Note that the answer must be a substring, "pwke" is a subsequence and not a substring.
//
//				  Subscribe to see which companies asked this question

#include <string>
#include <assert.h>
#include <unordered_map>
#include <map>
#include <functional>
#include <algorithm>
using namespace std;

//class Solution {
// public:
//  int lengthOfLongestSubstring(string s) {
//    int best = 0;
//    unordered_map<char, int> m;
//    int start = 0;
//    for (int j = 0; j < s.length(); j++) {
//      if (m[s[j]] == 0) {
//        m[s[j]] = j + 1;
//        int len = j - start + 1;
//        if (len>best) {
//          best = len;
//        }
//      } else {
//        if (j>0 && s[j]==s[j-1]) {
//          m.clear();
//          start = j;
//          m[s[j]] = j+1;
//        } else {
//          int start2 = m[s[j]];
//          for (int i = start; i <start2; i++) {
//            m.erase(s[i]);
//          }
//          m[s[j]] = j + 1;
//          start = start2;
//        }
//      }
//    }
//
//    return best;
//  }
//};

//class Solution {
// public:
//  int lengthOfLongestSubstring(string s) {
//    int best = 0;
//    unordered_map<char, int> m;
//    int start = 0;
//    for (int j = 0; j < s.length(); j++) {
//      auto it = m.find(s[j]);
//      //auto it = m.lower_bound(s[j]);
//      if (it==m.end()) {
//        m.insert(it,std::make_pair(s[j], j + 1));
//        //m[s[j]] = j + 1;
//        int len = j - start + 1;
//        if (len > best) {
//          best = len;
//        }
//      } else {
//        int start2 = it->second;
//        it->second = j + 1;
//        for (int i = start; i < start2-1; i++) {
//          m.erase(s[i]);
//          //m[s[i]] = 0;
//        }
//        start = start2;
//      }
//    }
//
//    return best;
//  }
//};

//class Solution {
// public:
//  int lengthOfLongestSubstring(string s) {
//    int best = 0;
//    unordered_map<char, int> m;
//    int start = 0;
//    for (int j = 0; j < s.length(); j++) {
//      auto it = m.find(s[j]);
//      if (it == m.end()) {
//        m.insert(it, std::make_pair(s[j], j + 1));
//      } else {
//        start = std::max(start, it->second);
//        it->second = j + 1;
//      }
//      best = std::max(best, j - start + 1);
//    }
//
//    return best;
//  }
//};

class Solution {
 public:
  int lengthOfLongestSubstring(string s) {
    int best = 0;
    int m[256] = { 0 };
    int start = 0;
    for (int j = 0; j < s.length(); j++) {
      start = std::max(start, m[s[j]]);
      best = std::max(best, j - start + 1);
      m[s[j]] = j + 1;
    }

    return best;
  }
};
void test1() {
  Solution sln;
  assert(sln.lengthOfLongestSubstring("abcabcbb") == 3);
  assert(sln.lengthOfLongestSubstring("bbbbb") == 1);
  assert(sln.lengthOfLongestSubstring("pwwkew") == 3);
  assert(sln.lengthOfLongestSubstring("dvdf") == 3);
  assert(sln.lengthOfLongestSubstring("tmmzuxt") == 5);
}
int main() {
  test1();
  return 0;
}