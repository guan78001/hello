//https://leetcode.com/problems/two-sum/
#include <vector>
#include <map>
#include <unordered_map>
#include <assert.h>
using namespace std;


//class Solution {
// public:
//  vector<int> twoSum(vector<int>& nums, int target) {
//    vector<int> ret;
//    ret.reserve(2);
//    for (int i = 0; i < nums.size(); i++) {
//      for (int j = i+1; j < nums.size(); j++) {
//        if (nums[i]+nums[j]==target) {
//          ret.push_back(i);
//          ret.push_back(j);
//          return ret;
//        }
//      }
//    }
//    return ret;
//  }
//};

class Solution {
 public:
  vector<int> twoSum(vector<int>& nums, int target) {
    vector<int> ret;
    unordered_map<int, int> m;
    for (int i = 0; i < nums.size(); i++) {
      auto it = m.find(target - nums[i]);
      if (it!=m.end()) {
        ret.reserve(2);
        ret.push_back(it->second);
        ret.push_back(i);
        return ret;
      }
      m[nums[i]] = i;
    }
    return ret;
  }
};

void test1() {
  Solution test;
  vector<int> nums{ 3, 2, 4 };
  int target = 6;
  vector<int> ret = test.twoSum(nums, target);
  assert(ret.size() == 2 && nums[ret[0]] + nums[ret[1]] == target);
}
void test2() {
  Solution test;
  vector<int> nums{ 0,4,3,0 };
  int target = 0;
  vector<int> ret = test.twoSum(nums, target);
  assert(ret.size() == 2 && nums[ret[0]] + nums[ret[1]] == target);
  if (ret.size()==2) {
    printf("%d %d\n", ret[0], ret[1]);
  }
}
int main() {
  test1();
  test2();
}