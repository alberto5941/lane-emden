#ifndef _MATPOINT
#define _MATPOINT
#include "Particle.h"
#include "Vector3.h"

namespace consts{
  const double c = 299792458;
  const double G = 1;
};

class MatPoint: public Particle{
 public:
  MatPoint(Particle p=Particle(), Vector3 r=Vector3(), Vector3 v=Vector3()):Particle(p),m_r(r),m_v(v){}
  MatPoint(double massa, double carica, Vector3 r, Vector3 v):MatPoint(Particle(massa,carica),r,v){}

  Vector3 R();    
  Vector3 V();
  void   R(Vector3);
  void   V(Vector3);

  Vector3 GravField(Vector3);
  
 private:
  Vector3 m_r;
  Vector3 m_v;
};

#endif
