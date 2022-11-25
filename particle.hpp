#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <string>
#include <vector>

#include "particleType.hpp"

class Particle {
  double px_;
  double py_;
  double pz_;
  int index_;
  static std::vector<ParticleType *> table_;
  static int FindParticle(std::string name);
  void Boost(double bx, double by, double bz);

public:
  Particle(std::string name, double px = 0, double py = 0, double pz = 0);
  static void AddParticleType(std::string name, double mass, int charge,
                              double width = 0);
  static int FindParticleTest(std::string name);
  static void PrintTable();
  void SetIndex(std::string name);
  void SetIndex(int index);
  void Print() const;
  std::string GetName() const;
  double GetCharge() const;
  double GetPx() const;
  double GetPy() const;
  double GetPz() const;
  void SetP(double px = 0, double py = 0, double pz = 0);
  double GetPNorm() const;
  double GetMass() const;
  double GetEnergy() const;
  double InvMass(Particle const &other);
  int Decay2body(Particle &dau1, Particle &dau2) const;
};

#endif