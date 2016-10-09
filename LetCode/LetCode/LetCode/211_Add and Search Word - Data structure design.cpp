//https://leetcode.com/problems/add-and-search-word-data-structure-design/
//211. Add and Search Word - Data structure design
//Design a data structure that supports the following two operations :
//
//void addWord(word)
//bool search(word)
//search(word) can search a literal word or a regular expression string containing only letters a - z or ..A.means it can represent any one letter.
//
//For example :
//
//addWord("bad")
//addWord("dad")
//addWord("mad")
//search("pad") -> false
//search("bad") -> true
//search(".ad") -> true
//search("b..") -> true
//Note:
//You may assume that all words are consist of lowercase letters a - z.
//
//click to show hint.
//
//You should be familiar with how a Trie works.If not, please work on this problem : Implement Trie(Prefix Tree) first.

#include <string>
#include <algorithm>
#include <stdio.h>
#include <assert.h>
#include <queue>
#include <stack>
using namespace std;

class TrieNode {
  // R links to node children
  //enum {R = 26};
 public:
  static const int R = 26;
  TrieNode *links[R];

  bool m_isEnd;

 public:

  TrieNode() {
    std::fill(links, links + R, nullptr);
    m_isEnd = false;
  }

  bool containsKey(char ch) {
    return links[ch - 'a'] != NULL;
  }
  TrieNode *get(char ch) {
    return links[ch - 'a'];
  }
  void put(char ch, TrieNode *node) {
    links[ch - 'a'] = node;
  }
  void setEnd() {
    m_isEnd = true;
  }
  bool isEnd() {
    return m_isEnd;
  }
};

class Trie {
  TrieNode *root;

 public:
  Trie() {
    root = new TrieNode();
  }
  ~Trie() {
    queue<TrieNode *> q;
    q.push(root);
    while (!q.empty()) {
      TrieNode *p = q.front();
      q.pop();

      for (int i = 0; i < TrieNode::R; i++) {
        if (p->links[i]) {
          q.push(p->links[i]);
        }
      }
      delete p;
    }
  }

  // Inserts a word into the trie.
  void insert(string word) {
    TrieNode *node = root;
    for (int i = 0; i < word.length(); i++) {
      char currentChar = word[i];
      if (!node->containsKey(currentChar)) {
        node->put(currentChar, new TrieNode());
      }
      node = node->get(currentChar);
    }
    node->setEnd();
  }

  // search a prefix or whole key in trie and
  // returns the node where search ends
  TrieNode *searchPrefix(string word) {
    TrieNode *node = root;
    for (int i = 0; i < word.length(); i++) {
      char curLetter = word[i];
      if (node->containsKey(curLetter)) {
        node = node->get(curLetter);
      } else {
        return NULL;
      }
    }
    return node;
  }

  TrieNode *searchPattern(string word) {
    typedef std::pair<TrieNode *, int> Match;
    stack<Match>s;
    s.push(make_pair(root, 0));
    int len = word.length();
    while (!s.empty()) {
      Match m = s.top();
      TrieNode *node = m.first;
      s.pop();

      int id = m.second;
      if (id >= len) {
        if (node->isEnd() || s.empty()) {
          return node;
        }
        continue;
      }

      char ch = word[m.second];
      if (ch == '.') {
        for (int i = 0; i < TrieNode::R; i++) {
          if (node->links[i]) {
            s.push(make_pair(node->links[i], id + 1));
          }
        }
      } else if (node->containsKey(ch)) {
        s.push(make_pair(node->get(ch), id + 1));
      }
    }
    return NULL;
  }
  // Returns if the word is in the trie.
  bool search(string word) {
    //TrieNode *node = searchPrefix(word);
    TrieNode *node = searchPattern(word);
    return node != NULL && node->isEnd();
  }
};

class WordDictionary {
 public:

  // Adds a word into the data structure.
  void addWord(string word) {
    m_trie.insert(word);
  }

  // Returns if the word is in the data structure. A word could
  // contain the dot character '.' to represent any one letter.
  bool search(string word) {
    bool res = m_trie.search(word);
    return res;
    //return m_trie.searchPattern(word) != NULL;
  }
 private:
  Trie m_trie;
};

// Your WordDictionary object will be instantiated and called as such:
// WordDictionary wordDictionary;
// wordDictionary.addWord("word");
// wordDictionary.search("pattern");

//addWord("bad")
//addWord("dad")
//addWord("mad")
//search("pad") -> false
//search("bad") -> true
//search(".ad") -> true
//search("b..") -> true
void test1() {
  WordDictionary wordDictionary;
  //wordDictionary.addWord("a");
  //wordDictionary.addWord("as");
  //wordDictionary.addWord("ass");
  //wordDictionary.addWord("bad");
  //wordDictionary.addWord("dad");
  //wordDictionary.addWord("mad");

  //assert(wordDictionary.search("a") == true);
  //assert(wordDictionary.search("as") == true);
  //assert(wordDictionary.search("ass") == true);
  //assert(wordDictionary.search("b") == true);

  //assert(wordDictionary.search("pad") == false);
  //assert(wordDictionary.search("bad") == true);
  //assert(wordDictionary.search(".ad") == true);
  //assert(wordDictionary.search("b..") == true);

  //addWord("at"), addWord("and"), addWord("an"), addWord("add"), search("a"), search(".at"), addWord("bat"),
  //search(".at"), search("an."), search("a.d."), search("b."), search("a.d"), search(".")
  //[false,false,true,true,false,false,true,false]

  wordDictionary.addWord("at");
  wordDictionary.addWord("and");
  wordDictionary.addWord("an");
  wordDictionary.addWord("add");

  assert(wordDictionary.search("a") == false);
  //printf("%d\n", wordDictionary.search(".at"));
  assert(wordDictionary.search(".at") == false);

  wordDictionary.addWord("bat");

  assert(wordDictionary.search(".at") == true);
  assert(wordDictionary.search("an.") == true);
  assert(wordDictionary.search("a.d.") == false);
  assert(wordDictionary.search("b.") == false);
  assert(wordDictionary.search("a.d") == true);

  assert(wordDictionary.search(".") == false);
}
void test2() {
//Input:
//	addWord("ran"), addWord("rune"), addWord("runner"), addWord("runs"), addWord("add"), addWord("adds"),
  //addWord("adder"), addWord("addee"), search("r.n"), search("ru.n.e"), search("add"), search("add."), search("adde."),
  //search(".an."), search("...s"), search("....e."), search("......."), search("..n.r")
//	Output :
//		   [false, false, true, true, true, false, true, true, false, false]
//Expected :
//	[true, false, true, true, true, false, true, true, false, false]
  WordDictionary wd;
  wd.addWord("ran");
  wd.addWord("rune");
  wd.addWord("runner");
  wd.addWord("runs");
  wd.addWord("add");
  wd.addWord("adds");
  wd.addWord("adder");
  wd.addWord("addee");

  //assert(wd.search("r.n") == true);
  //assert(wd.search("ru.n.e") == false);

  //assert(wd.search("add") == true);
  //assert(wd.search("add.") == true);
  //assert(wd.search(".an.") == true);
  assert(wd.search("adde.") == true);

  assert(wd.search("...s") == true);
  assert(wd.search("....e.") == true);
  assert(wd.search(".......") == false);
  assert(wd.search("..n.r") == false);
}
void test3() {
  WordDictionary wd;
  wd.addWord("at");
  wd.addWord("and");
  wd.addWord("an");
  wd.addWord("add");

  assert(wd.search("a") == false);
  assert(wd.search("at") == true);
  wd.addWord("bat");

  assert(wd.search("an.") == true);
  assert(wd.search("a.d.") == false);
  assert(wd.search("b.") == false);
  assert(wd.search("a.d") == true);
  //assert(wd.search(".") == false);
}
int main() {
  test1();
  test2();
  test3();
  return 0;
}