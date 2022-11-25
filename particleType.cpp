#include "particleType.hpp"

#include <iostream>
#include <string>
const std::string ParticleType::GetName() const { return name_; }
const double ParticleType::GetMass() const { return mass_; }
const int ParticleType::GetCharge() const { return charge_; }
const double ParticleType::GetWidth() const { return 0; }
void ParticleType::Print() const {
  std::cout << "NAME: " << name_ << '\n';
  std::cout << "MASS: " << mass_ << '\n';
  std::cout << "CHARGE: " << charge_ << '\n';
}