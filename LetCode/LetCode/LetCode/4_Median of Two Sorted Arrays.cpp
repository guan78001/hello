#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class Solution {
 public:
  double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    int len1 = nums1.size();
    int len2 = nums2.size();

    int t1 = (len1 + len2 - 1) / 2;
    int t2 = (len1 + len2) / 2;

    int p1 = 0, p2 = nums1.size();
    int q1 = 0, q2 = nums2.size();
    while (p1 < p2 && q1 < q2) {
      int mid1 = (p1 + p2 - 1) / 2;
      auto it2 = lower_bound(nums2.begin() + q1, nums2.begin() + q2, nums1[mid1]);
      int n = it2 - nums2.begin();
      if (mid1 + n < t1) {
        p1 = mid1 + 1;
        q1 = n;
      } else if (mid1 + n > t2) {
        p2 = mid1;
        q2 = q1 + n;
      } else if (mid1 + n == t1) {
        if (t1 == t2) return nums1[mid1];
        else {
          double other = numeric_limits<double>::max();
          if (mid1 + 1 < len1) if (other > nums1[mid1 + 1]) other = nums1[mid1 + 1];
          if (n<len2) if (other>nums2[n]) other = nums2[n];
          return (nums1[mid1] + other) / 2;
        }
      } else if (mid1 + n == t2) {
        if (t1 == t2) return nums1[mid1];
        else {
          double other = numeric_limits<double>::min();
          if (mid1 > 0) if (other < nums1[mid1 - 1]) other = nums1[mid1 - 1];
          if (n>0) if (other < nums2[n - 1]) other = nums2[n - 1];
          return (nums1[mid1] + other) / 2;
        }
      }
    }

    if (p1 < p2) {
      return (nums1[t1 - q2] + nums1[t2 - q2]) / 2.0;
    }
    if (q1 < q2) {
      return (nums2[t1 - p2] + nums2[t2 - p2]) / 2.0;
    }
    if (t1 < t2) {
      return(nums1[p1] + nums2[q1]) / 2.0;
    } else {
      return std::max(nums1[p1 - 1], nums2[q2 - 1]);
    }
    return 0;
  }
};

int main() {
  vector<int> v1{ 3, 4, 6, 7, 8, 9 };
  vector<int> v2{ 1, 2, 5 };
  Solution sln;
  cout<<sln.findMedianSortedArrays(v1, v2) << endl;
}