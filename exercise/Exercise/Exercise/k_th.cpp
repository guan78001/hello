#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
using namespace std;

int rotateArray(int *arr,int N)
{
	/*
	1  2  3  4
	12 13 14  5
	11 16 15  6
	10 9  8  7
	*/

	int val = 1;
	int loop = 0;
	int loops = (N+1) / 2;
	int y = 0;
	int x = 0;
	while (loop<=loops)
	{
		y = loop;
		for (x = loop; x < N - 1-loop; x++) arr[y*N + x] = val++;
		for (y = loop; y < N - 1-loop; y++) arr[y*N + x] = val++;
		for (x = N - 1-loop; x >loop; x--) arr[y*N + x] = val++;
		for (y = N - 1 - loop; y >loop ; y--) arr[y*N + x] = val++;
		loop++;
	}
	if (N%2) arr[N/2*N+N/2] = val++;

	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++)
			printf("%4d", arr[i*N + j]);
		printf("\n");
	}
	
	return 0;
}
int main()
{
	const int N = 5;
	int arr[N*N] = { 0 };
	
	rotateArray(arr,N);
	return 0;
	std::vector<int>myvector;
	//set somevalues:
	for (int i = 1; i<10; i++) myvector.push_back(i);//123456789

	std::random_shuffle(myvector.begin(), myvector.end());

	//usingdefaultcomparison(operator<):
	std::nth_element(myvector.begin(), myvector.begin() + 0, myvector.end(),greater<int>());
	for (int n : myvector)
	{
		printf("%d ", n);
	}
	
	{
		printf("\npriority_queue\n");
		priority_queue<float> q;
		q.push(2);
		q.push(1);
		q.push(4);
		q.push(3);
		while (!q.empty())
		{
			printf("%f ", q.top());
			q.pop();
		}
		printf("\npriority_queue\n");
	}
	printf("\n");


	vector<int> vec{ 1, 2, 3, 4, 9, 8, 7, 6, 5, 4, 3 };
	std::nth_element(vec.begin(), vec.begin() + 4, vec.end());

	for (int n : vec)
	{
		printf("%d ", n);
	}

	return 0;
}