#ifndef _ODESOLVER
#define _ODESOLVER

#include <vector>
#include <functional>
#include "Vector3.h"
#include "MatPoint.h"

using namespace std;

class OdeSolver{
 public:
  OdeSolver(string method="Eulero", vector<MatPoint> p=vector<MatPoint>(0)):m_method(method),m_p(p),m_t(0),m_h(0.1){}
  void             AddMatPoint(MatPoint tmp);
  MatPoint         GetMatPoint(unsigned int i);
  vector<MatPoint> GetMatPoints();
  unsigned int     N();
  void             T(double t0);
  double           T();
  void             DeltaT(double h);
  double           DeltaT();
  void             Step();
  std::function < Vector3(unsigned int i,unsigned int j, double t, vector<MatPoint> p) > fInternal;
  std::function < Vector3(unsigned int i,double t, vector<MatPoint> p) >        fExternal;
 private:
  string   m_method;
  vector<MatPoint> m_p;
  double   m_t,m_h;
  Vector3  m_A(unsigned int i, double t, vector<MatPoint>);
};


#endif
