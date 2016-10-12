#include <amp.h>
#include <iostream>
#include <stdio.h>
using namespace concurrency;

void AddArrays() {
  int aCPP[] = { 1, 2, 3, 4, 5 };
  int bCPP[] = { 6, 7, 8, 9, 10 };
  int sumCPP[5] = { 0, 0, 0, 0, 0 };

  array_view<int, 1> a(5, aCPP);
  array_view<int, 1> b(5, bCPP);
  array_view<int, 1> sum(5, sumCPP);

  parallel_for_each(
    sum.extent,
  [ = ](index<1> idx) restrict(amp) {
    sum[idx] = a[idx] + b[idx];
  }
  );

  for (int i = 0; i < 5; i++) {
    std::cout << sum[i] << "\n";
  }
}
int main() {
  // Sample data:
  int sampledata[] = {
    2, 2, 9, 7, 1, 4,
    4, 4, 8, 8, 3, 4,
    1, 5, 1, 2, 5, 2,
    6, 8, 3, 2, 7, 2
  };

  // The tiles:
  // 2 2    9 7    1 4
  // 4 4    8 8    3 4
  //
  // 1 5    1 2    5 2
  // 6 8    3 2    7 2

  // Averages:
  int averagedata[] = {
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
  };

  array_view<int, 2> sample(4, 6, sampledata);
  array_view<int, 2> average(4, 6, averagedata);

  parallel_for_each(
    // Create threads for sample.extent and divide the extent into 2 x 2 tiles.
    sample.extent.tile<2, 2>(),
  [ = ](tiled_index<2, 2> idx) restrict(amp) {
    // Create a 2 x 2 array to hold the values in this tile.
    tile_static int nums[2][2];
    // Copy the values for the tile into the 2 x 2 array.
    //printf("%d,%d,%d\n", idx.local[1], idx.local[0], idx.global);
    nums[idx.local[0]][idx.local[1]] = sample[idx.global];
    // When all the threads have executed and the 2 x 2 array is complete, find the average.
    idx.barrier.wait();
    int sum = nums[0][0] + nums[0][1] + nums[1][0] + nums[1][1];
    // Copy the average into the array_view.
    average[idx.global] = sum / 4;
  }
  );

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 6; j++) {
      std::cout << average(i, j) << " ";
    }
    std::cout << "\n";
  }

  // Output:
  // 3 3 8 8 3 3
  // 3 3 8 8 3 3
  // 5 5 2 2 4 4
  // 5 5 2 2 4 4
  return 0;
}