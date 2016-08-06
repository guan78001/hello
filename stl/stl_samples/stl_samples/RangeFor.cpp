#include <vector>
#include <iostream>
using namespace std;

std::vector<int> range(int start, int stop, int step)
{
	std::vector<int>v;
	for (int i = start; i != stop; i+=step)
	{
		v.push_back(i);
	}
	return v;
}
struct RangeIter;
struct Range
{
	Range(int start, int stop, int step = 1);
	RangeIter  begin();
	RangeIter  end();
	int m_start;
	int m_stop;
	int m_step;
};


struct RangeIter :public std::iterator < std::forward_iterator_tag, int >
{
	RangeIter(Range * r, int pos) :m_r(r), m_pos(pos){}
	bool operator==(RangeIter &rhs)
	{
		return m_r == rhs.m_r && m_pos == rhs.m_pos;
	}
	bool operator !=(RangeIter &rhs)
	{
		return !(*this == rhs);
	}
	RangeIter& operator++()
	{
		m_pos += m_r->m_step;
		return *this;
	}
	int operator *()
	{
		return m_pos;
	}
	Range *m_r;
	int m_pos;
};

Range::Range(int start, int stop, int step /*= 1*/)
{
	m_start = start;
	m_stop = stop;
	m_step = step;
}

RangeIter Range::begin()
{
	return RangeIter(this, m_start);
}

RangeIter Range::end()
{
	return RangeIter(this, m_stop);
}

int main()
{
	std::vector<int> v{ 1, 3, 5, 7, 9 };
	v.begin();
	for (auto n: v)
	{
		cout << n << " ";
	}
	cout << endl;
	for (auto n: range(1,11,2))
	{
		cout << n << " ";
	}
	cout << endl;
	for (auto n : Range(-1, -14, -1))
	{
		cout << n << " ";
	}
	cout << endl;
	return 0;
}