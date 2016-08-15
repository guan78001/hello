////https://sourcemaking.com/design_patterns/private_class_data/c-sharp-dot-net
#include <iostream>
using namespace std;
struct Point {
  double x, y;
  Point(double a = 0, double b = 0) {
    x = a;
    y = b;
  }
};
#if 1
class Circle {
  double m_radius;
  Point m_origin;

 public:
  Circle(double radius, Point origin) {
    m_radius = radius;
    m_origin = origin;
  }
  double &Circumference() {
    return m_radius;
  }
  void draw() {
    cout << "radius=" << m_radius << ", Origin=(" <<
         m_origin.x << "," << m_origin.y << ")" << endl;
  }
};
#else
class CircleData {
  double m_radius;
  Point m_origin;

 public:
  CircleData(double radius, Point origin) {
    m_radius = radius;
    m_origin = origin;
  }
  double &Circumference() {
    return m_radius;
  }
  void draw() {
    cout << "radius=" << m_radius << ", Origin=(" <<
         m_origin.x << "," << m_origin.y << ")" << endl;
  }
};

class  Circle {
  CircleData m_data;
 public:
  Circle(double radius, Point origin): m_data(radius, origin) {
  }
  double &Circumference() {
    return m_data.Circumference();
  }
  void draw() {
    m_data.draw();
  }

};
#endif

int main() {
  Circle c(4, Point(0, 0));
  c.draw();
}
