#ifndef _PARTICLE
#define _PARTICLE
class Particle{
 public:
  Particle(double mass=0, double charge=0):m_mass(mass),m_charge(charge){}
  double Mass() {return  m_mass;}
  double Charge(){return m_charge;}
 private:
  double m_mass;
  double m_charge;
};
#endif
