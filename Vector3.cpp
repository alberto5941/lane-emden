#include "Vector3.h"
#include <cmath>

Vector3 Vector3::operator+(Vector3 b){
  Vector3 sum;
  for (int i=0;i<3;i++)
    sum.m_v[i] = m_v[i]+b.m_v[i];
  return sum;
}

Vector3 Vector3::operator-(Vector3 b){
  Vector3 sum;
  for (int i=0;i<3;i++)
    sum.m_v[i] = m_v[i]-b.m_v[i];
  return sum;
}

Vector3 Vector3::operator*(double f){
  Vector3 v;
  for (int i=0;i<3;i++)
    v.m_v[i] = f*m_v[i];
  return v;
}

Vector3 Vector3::operator-(){
  Vector3 v;
  for (int i=0;i<3;i++)
    v.m_v[i] = -m_v[i];
  return v;
}

double Vector3::X(){
  return m_v[0];
}

double Vector3::Y(){
  return m_v[1];
}

double Vector3::Z(){
  return m_v[2];
}

void Vector3::X(double x){
  m_v[0] = x;
}

void Vector3::Y(double y){
  m_v[1] = y;
}

void Vector3::Z(double z){
  m_v[2] = z;
}


double Vector3::operator*(Vector3 a){
  double prod=0;
  for (int i=0;i<3;i++)
    prod += a.m_v[i]*m_v[i];
  return prod;
}

Vector3 Vector3::Cross(Vector3 a){
  Vector3 prod;
  prod.m_v[0] = m_v[1]*a.m_v[2]-m_v[2]*a.m_v[1];
  prod.m_v[1] = m_v[2]*a.m_v[0]-m_v[0]*a.m_v[2];
  prod.m_v[2] = m_v[0]*a.m_v[1]-m_v[1]*a.m_v[0];
  return prod;
}

double Vector3::Mod(){
  return sqrt((*this)*(*this));
}

Vector3 Vector3::Unit(){
  return (*this)*(1/this->Mod());
}

Vector3 operator*(double f, Vector3 a){
  return a*f;
}

std::ostream& operator<<(std::ostream& output, Vector3 a){
  output << "(" << a.X() << "," << a.Y() << "," << a.Z() << ")";
  return output;
}

std::istream& operator>>(std::istream& input, Vector3& a){
  double x,y,z;
  input >> x >> y >> z;
  a.X(x); a.Y(y); a.Z(z);
  return input;
}
