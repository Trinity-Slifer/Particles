#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP
#include <string>

class ParticleType {
private:
  const std::string name_;
  const double mass_;
  const int charge_;

public:
  ParticleType(std::string name, double mass, int charge)
      : name_{name}, mass_{mass}, charge_{charge} {}

  const std::string GetName() const;
  const double GetMass() const;
  const int GetCharge() const;
  const double GetWidth() const;
  void Print() const;
};

#endif