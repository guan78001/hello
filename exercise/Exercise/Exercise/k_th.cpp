#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>

using namespace std;

int rotateArray(int *arr, int N) {
  /*
  1  2  3  4
  12 13 14  5
  11 16 15  6
  10 9  8  7
  */

  int val = 1;
  int loop = 0;
  int loops = (N + 1) / 2;
  int y = 0;
  int x = 0;
  while (loop <= loops) {
    y = loop;
    for (x = loop; x < N - loop; x++) arr[y * N + x] = val++;
    x = N - 1 - loop;
    for (y = loop; y < N - 1 - loop; y++) arr[y * N + x] = val++;
    for (x = N - 1 - loop; x > loop; x--) arr[y * N + x] = val++;
    for (y = N - 1 - loop; y > loop ; y--) arr[y * N + x] = val++;
    loop++;
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++)
      printf("%4d", arr[i * N + j]);
    printf("\n");
  }

  return 0;
}
int lower_pos(int *arr, int len, int val)
{
	int left = 0;
	int right = len;

	while (left<right)
	{
		int mid = (left + right) / 2;
		if (arr[mid]<val)
		{
			left = mid + 1;
		}
		else
		{
			right = mid;
		}
	}
	return left;
}
int lower_pos2(int *arr, int len, int val)
{
	int left = 0;
	int right = len;
	if (left >= right)
	{
		return 0;
	}

	int mid = (left + right) / 2;
	if (arr[mid] < val)
	{
		left = mid + 1;
	}
	else
	{
		right = mid;
	}
	return left + lower_pos2(arr + left, right - left, val);
}
void test_bin_search() {
  int arr[] = { 1, 2, 4, 3, 6, 5, 7, 8 };
  int n = sizeof(arr) / sizeof(arr[0]);

  sort(arr, arr + n);
  auto it = lower_bound(arr, arr + n, 3);
  auto it2 = upper_bound(arr, arr + n, 3);
  auto it3 = it;
  std::advance(it3, 1);
  printf("*it3=%d\n", *it3);
  printf("%d-%d,diff=%d\n", *it, *it2, std::distance(it, it2));
  std::binary_search(arr, arr + n, 8);
  for (int i : arr) {
    printf("%d ", i);
  }
  printf("\n");
}
void foo(int x)
{
	cout << "x=" << x << endl;
}
void test_lower_bound()
{
	vector<int> v{ 1, 2 };
	
	for (int val = 1; val < 4; val++)
	{
		auto it = lower_bound(v.begin(), v.end(), val);
		cout << lower_pos(&v[0], v.size(), val) << endl;
		cout << lower_pos2(&v[0], v.size(), val) << endl;
		if (it == v.end()) {
			cout << "end" << endl;
		}
		else {
			cout << "push at pos " << (it - v.begin()) << endl;
		}
	}

}
int main() {
  test_lower_bound();
  return 0;

  test_bin_search();
  return 0;

  const int N = 5;
  int arr[N * N] = { 0 };

  rotateArray(arr, N);
  return 0;
  std::vector<int>myvector;
  //set somevalues:
  for (int i = 1; i < 10; i++) myvector.push_back(i); //123456789

  std::random_shuffle(myvector.begin(), myvector.end());

  //usingdefaultcomparison(operator<):
  std::nth_element(myvector.begin(), myvector.begin() + 0, myvector.end(), greater<int>());
  for (int n : myvector) {
    printf("%d ", n);
  }

  {
    printf("\npriority_queue\n");
    priority_queue<float> q;
    q.push(2);
    q.push(1);
    q.push(4);
    q.push(3);
    while (!q.empty()) {
      printf("%f ", q.top());
      q.pop();
    }
    printf("\npriority_queue\n");
  }
  printf("\n");


  vector<int> vec{ 1, 2, 3, 4, 9, 8, 7, 6, 5, 4, 3 };
  std::nth_element(vec.begin(), vec.begin() + 4, vec.end());

  for (int n : vec) {
    printf("%d ", n);
  }

  return 0;
}