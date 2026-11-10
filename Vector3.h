#ifndef _VECTOR3
#define _VECTOR3

#include <iostream>

class Vector3{
 public:
  Vector3(double x=0, double y=0, double z=0):m_v{x,y,z}{};
  Vector3 operator+(Vector3);
  Vector3 operator-(Vector3);
  Vector3 operator-();
  Vector3 operator*(double);
  double  operator*(Vector3);
  Vector3 Cross(Vector3);
  Vector3 Unit();
  double  Mod();
  double X();
  double Y();
  double Z();
  void X(double);
  void Y(double);
  void Z(double);
 private:
  double m_v[3];
};

Vector3 operator*(double f, Vector3 a);
std::ostream& operator<<(std::ostream& output, Vector3 a);
std::istream& operator>>(std::istream& input, Vector3& a);

#endif
