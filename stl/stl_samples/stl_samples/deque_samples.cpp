#include <deque>
#include <forward_list>
#include <iostream>
using namespace std;

enum  Color { R = 1, G = 2, B = 3 };
int print(Color c)
{
  cout << (int)c << endl;
  return 0;
}
int main()
{

  forward_list<int> sl;//only support pop_front,push_front.

//#define _DEQUEMAPSIZ	8	/* minimum map size, at least 1 */
//#define _DEQUESIZ	(sizeof (value_type) <= 1 ? 16 \
//	: sizeof (value_type) <= 2 ? 8 \
//	: sizeof (value_type) <= 4 ? 4 \
//	: sizeof (value_type) <= 8 ? 2 \
//	: 1)	/* elements per block (a power of 2) */

  deque<int> deq;
  deq.push_back(0);
  for (int i = 1; i < 16; i++)
  {
    deq.push_back(i);
    cout << deq[i] << " &=" << &deq[i] << endl;
    size_t diff = &deq[i] - &deq[i - 1];

    if (diff != 1)
    {
      cout << "new block " << i << endl;
    }
  }
  deq[9] = 1;
  deq[10] = 1;
  return 0;
}