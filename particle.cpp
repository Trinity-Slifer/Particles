#include "particle.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "particleType.hpp"
#include "resonanceType.hpp"

std::vector<ParticleType *> Particle::table_{};

Particle::Particle(std::string name, double px, double py, double pz)
    : px_{px}, py_{py}, pz_{pz} {
  index_ = Particle::FindParticle(name);
}

int Particle::FindParticle(std::string name) {
  if (table_.size() == 0) {
    return -1;
  }

  for (int i = 0; i != table_.size(); i++) {
    if (table_[i]->GetName() == name) {
      return i;
    }
  }

  std::cout << "\n ** PARTICLE NOT FOUND ** \n";
  return -1;
}

int Particle::FindParticleTest(std::string name) {
  return Particle::FindParticle(name);
}
void Particle::SetIndex(std::string name) { index_ = FindParticle(name); };
std::string Particle::GetName() const { return table_[index_]->GetName(); }
double Particle::GetCharge() const { return table_[index_]->GetCharge(); }
void Particle::AddParticleType(std::string name, double mass, int charge,
                               double width) {
  ParticleType *new_particle;
  if (width > 0)
    new_particle = new ResonanceType{name, mass, charge, width};
  else
    new_particle = new ParticleType{name, mass, charge};

  table_.push_back(new_particle);
}

void Particle::SetIndex(int index) { index_ = index; };

void Particle::PrintTable() {
  std::cout << "TABLE:\n";
  for (size_t i = 0; i < table_.size(); i++) {
    table_[i]->Print();
    std::cout << '\n';
  }
}

void Particle::Print() const {
  std::cout << "INDEX: " << index_ << '\n';
  std::cout << "Px: " << px_ << '\n';
  std::cout << "Py: " << py_ << '\n';
  std::cout << "Pz: " << pz_ << '\n';
}

double Particle::GetPx() const { return px_; }
double Particle::GetPy() const { return py_; }
double Particle::GetPz() const { return pz_; }

void Particle::SetP(double px, double py, double pz) {
  px_ = px;
  py_ = py;
  pz_ = pz;
}

double Particle::GetMass() const { return table_[index_]->GetMass(); }
double Particle::GetPNorm() const {
  return std::sqrt(GetPx() * GetPx() + GetPy() * GetPy() + GetPz() * GetPz());
}
double Particle::GetEnergy() const {
  return std::sqrt(pow(GetMass(), 2) + pow(GetPNorm(), 2));
}
double Particle::InvMass(Particle const &other) {
  double c = std::sqrt(pow((GetEnergy() + other.GetEnergy()), 2) -
                       (pow((GetPx() + other.GetPx()), 2) +
                        pow((GetPy() + other.GetPy()), 2) +
                        pow((GetPz() + other.GetPz()), 2)));
  std::cout << GetName() << other.GetName() << c << std::endl;
  return c;
}

int Particle::Decay2body(Particle &dau1, Particle &dau2) const {
  if (GetMass() == 0.0) {
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }

  double massMot = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();

  if (index_ > -1) { // add width effect

    // gaussian random numbers

    float x1, x2, w, y1, y2;

    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;
    y2 = x2 * w;

    massMot += table_[index_]->GetWidth() * y1;
  }

  if (massMot < massDau1 + massDau2) {
    printf("Decayment cannot be preformed because mass is too low in this "
           "channel\n");
    return 2;
  }

  double pout =
      sqrt(
          (massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2)) *
          (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2))) /
      massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.SetP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi),
            pout * cos(theta));
  dau2.SetP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi),
            -pout * cos(theta));

  double energy = sqrt(px_ * px_ + py_ * py_ + pz_ * pz_ + massMot * massMot);

  double bx = px_ / energy;
  double by = py_ / energy;
  double bz = pz_ / energy;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);
  // dau1.Print();
  // dau2.Print();
  return 0;
}
void Particle::Boost(double bx, double by, double bz) {

  double energy = GetEnergy();

  // Boost this Lorentz vector
  double b2 = bx * bx + by * by + bz * bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx * px_ + by * py_ + bz * pz_;
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  px_ += gamma2 * bp * bx + gamma * bx * energy;
  py_ += gamma2 * bp * by + gamma * by * energy;
  pz_ += gamma2 * bp * bz + gamma * bz * energy;
}