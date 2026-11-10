#include "Vector3.h"
#include "OdeSolver.h"

void OdeSolver::AddMatPoint(MatPoint tmp){
  m_p.push_back(tmp);
}

vector<MatPoint> OdeSolver::GetMatPoints(){
  return m_p;
}

MatPoint OdeSolver::GetMatPoint(unsigned int i){
  return m_p[i];
}

unsigned int OdeSolver::N(){
  return m_p.size();
}

void OdeSolver::T(double t0){
  m_t=t0;
}

double OdeSolver::T(){
  return m_t;
}

void OdeSolver::DeltaT(double h){
  m_h = h;
}

double OdeSolver::DeltaT(){
  return   m_h;
}


Vector3 OdeSolver::m_A(unsigned int i, double t, vector<MatPoint> p){

  Vector3 FTot = Vector3();
  unsigned int n = p.size();
  MatPoint Pi = p.at(i); //corpo i
  for(unsigned int j=0; j<n; j++) {
    if(j!=i) {
      FTot = FTot + fInternal(i, j, t, p);
    }
  }
  FTot = FTot + fExternal(i, t, p);
  double mi = Pi.Mass(); //massa del corpo i
  Vector3 Acc = (1./mi)*FTot;
  return Acc;
}

void OdeSolver::Step(){

  if (m_method=="Eulero"){
    vector<Vector3>  k1(m_p.size());
    vector<Vector3>  w1(m_p.size());
    for (unsigned int i=0;i<m_p.size();i++){
      k1[i] = m_h*m_p[i].V();
      w1[i] = m_h*m_A(i,m_t,m_p);
    }

    for (unsigned int i=0;i<m_p.size();i++){
      m_p[i].R(m_p[i].R() + k1[i]);
      m_p[i].V(m_p[i].V() + w1[i]);
    }

  } else if (m_method=="Rk2"){

    vector<Vector3> k1(m_p.size());
    vector<Vector3> w1(m_p.size());
    vector<Vector3> k2(m_p.size());
    vector<Vector3> w2(m_p.size());
    vector<Vector3> x_int(m_p.size());
    vector<Vector3> v_int(m_p.size());

    for (unsigned int i=0;i<m_p.size();i++){
      k1[i] = m_h*m_p[i].V();
      w1[i] = m_h*m_A(i, m_t, m_p);
    }

    auto p_int(m_p);
    for(unsigned int i=0; i<m_p.size(); i++) {
      p_int[i].R(m_p[i].R() + k1[i]*(1./2));
      p_int[i].V(m_p[i].V() + w1[i]*(1./2));
    }

    for(unsigned int i=0; i<m_p.size(); i++){
      k2[i] = m_h*p_int[i].V();
      w2[i] = m_h*m_A(i, m_t+m_h/2, p_int);
  }
    for(unsigned int i=0; i<m_p.size(); i++) {
      m_p[i].R(m_p[i].R()+k2[i]);
      m_p[i].V(m_p[i].V()+w2[i]);
    }
  m_t += m_h;
  } else if (m_method == "Rk4"){

    vector<Vector3> k1(m_p.size());
    vector<Vector3> w1(m_p.size());
    vector<Vector3> k2(m_p.size());
    vector<Vector3> w2(m_p.size());
    vector<Vector3> k3(m_p.size());
    vector<Vector3> w3(m_p.size());
    vector<Vector3> k4(m_p.size());
    vector<Vector3> w4(m_p.size());
    vector<Vector3> x_int(m_p.size());
    vector<Vector3> v_int(m_p.size());

    for(unsigned int i=0;i<m_p.size(); i++) {
      k1[i] = m_h*m_p[i].V();
      w1[i] = m_h*m_A(i, m_t, m_p);
    }

    auto p_int1(m_p);
    for(unsigned int i=0; i<m_p.size(); i++) {
      p_int1[i].R(m_p[i].R() + k1[i]*(1./2));
      p_int1[i].V(m_p[i].V() + w1[i]*(1./2));
    }

    for(unsigned int i=0; i<m_p.size(); i++){
      k2[i] = m_h*p_int1[i].V();
      w2[i] = m_h*m_A(i, m_t+m_h/2, p_int1);
  }

    auto p_int2(m_p);
    for(unsigned int i=0; i<m_p.size(); i++) {
      p_int2[i].R(m_p[i].R() + k2[i]*(1./2));
      p_int2[i].V(m_p[i].V() + w2[i]*(1./2));
    }

    for(unsigned int i=0; i<m_p.size(); i++){
      k3[i] = m_h*p_int2[i].V();
      w3[i] = m_h*m_A(i, m_t+m_h/2, p_int2);
  }

    auto p_int3(m_p);
    for(unsigned int i=0; i<m_p.size(); i++) {
      p_int3[i].R(m_p[i].R() + k3[i]);
      p_int3[i].V(m_p[i].V() + w3[i]);
    }

    for(unsigned int i=0; i<m_p.size(); i++){
      k4[i] = m_h*p_int3[i].V();
      w4[i] = m_h*m_A(i, m_t+m_h/2, p_int3);
  }

    for(unsigned int i=0; i<m_p.size(); i++){
      m_p[i].R(m_p[i].R()+k1[i]*(1./6) + k2[i]*(1./3) + k3[i]*(1./3) + k4[i]*(1./6));
      m_p[i].V(m_p[i].V()+w1[i]*(1./6) + w2[i]*(1./3) + w3[i]*(1./3) + w4[i]*(1./6));
    }
  m_t += m_h;
  }
}
