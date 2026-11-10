#include "MatPoint.h"
#include "Vector3.h"
#include <cmath>

Vector3 MatPoint::R(){
  return m_r;
}

Vector3 MatPoint::V(){
  return m_v;
}

void MatPoint::R(Vector3 r){
  m_r = r;
}

void MatPoint::V(Vector3 v){
  m_v = v;
}

Vector3 MatPoint::GravField(Vector3 r){

  Vector3 r0   = m_r;
  Vector3 dist = r-r0;

  Vector3 val  = -consts::G*Mass()/pow(dist.Mod(),2)*dist.Unit();

  return val;
  
  
}
