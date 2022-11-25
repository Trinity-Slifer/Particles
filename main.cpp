#include "TCanvas.h"
#include "TH1F.h"
#include "TRandom.h"
#include "particle.hpp"
#include "particleType.hpp"
#include "resonanceType.hpp"

#include <cmath>
#include <iostream>
#include <vector>
/*
R__LOAD_LIBRARY(particleType_cpp.so);
R__LOAD_LIBRARY(resonanceType_cpp.so);
R__LOAD_LIBRARY(particle_cpp.so);
*/
int Main() {
  gRandom->SetSeed();
  Particle::AddParticleType("pione+", 0.13957, 1);
  Particle::AddParticleType("pione-", 0.13957, -1);
  Particle::AddParticleType("Kaone+", 0.49367, 1);
  Particle::AddParticleType("Kaone-", 0.49367, -1);
  Particle::AddParticleType("protone+", 0.93827, 1);
  Particle::AddParticleType("protone-", 0.93827, -1);
  Particle::AddParticleType("K*", 0.89166, 0, 0.050);
  std::vector<Particle> new_set;

  TCanvas *c1 = new TCanvas("Data About Generated Particles", "c1", 1000, 1000);
  TCanvas *c2 = new TCanvas("Invariance mass", "c2", 1000, 1000);
  TH1F *h1 = new TH1F("hist 1", "Tipi generati", 7, 0, 7);
  TH1F *polar = new TH1F("hist 2", "Angoli polari", 1000, 0, 2 * M_PI);
  TH1F *azimuthal = new TH1F("hist 3", "Angoli azimutali", 1000, 0, M_PI);
  TH1F *impulse = new TH1F("hist 4", "Impulso", 1000, 0, 5);
  TH1F *transverseimpulse = new TH1F("hist 5", "Impulso trasverso", 1000, 0, 3);
  TH1F *energy = new TH1F("hist 6", "Energia particella", 1000, 0, 2.5);
  TH1F *imdau = new TH1F("hist 7", "Massa invariante figlie", 1000, 0, 1.5);
  TH1F *totalInvMass = new TH1F("totalInv", "Total Mass Invariant", 1000, 0, 4);
  TH1F *invMassOppositeCharge = new TH1F(
      "invmassoc", "Mass invariant of opposite charge particles", 1000, 0, 4);
  TH1F *invMassSameCharge = new TH1F(
      "invmasssc", "Mass invariant of same charge particles", 1000, 0, 4);
  TH1F *invMassKPOpposite =
      new TH1F("invmasskp", "Mass invariant of Kaon/Pion with opposite charge",
               1000, 0, 4);
  TH1F *invMassKPSame = new TH1F(
      "invmasskpsame", "Mass invariant Kaon/Pion with same charge", 1000, 0, 4);
  c1->Divide(2, 3);
  c2->Divide(3, 3);
  std::string name;
  double x;
  double phi;
  double theta;
  double p;
  double px;
  double py;
  double pz;
  double tp;
  double invmas;

  for (size_t i = 0; i < 100000; i++) {
    new_set.clear();
    new_set.reserve(105);
    for (size_t j = 0; j < 100; j++) {
      x = gRandom->Uniform(0., 1);
      phi = gRandom->Uniform(0., 2 * M_PI);
      theta = gRandom->Uniform(0., M_PI);
      p = gRandom->Exp(1);
      px = p * cos(theta) * sin(phi);
      py = p * sin(theta) * sin(phi);
      pz = p * cos(phi);
      tp = sqrt(px * px + py * py);
      impulse->Fill(p);
      polar->Fill(phi);
      azimuthal->Fill(theta);
      transverseimpulse->Fill(tp);
      if (x <= 0.4) {
        name = "pione+";
      } else if (x <= 0.8) {
        name = "pione-";
      } else if (x <= 0.85) {
        name = "Kaone+";
      } else if (x <= 0.9) {
        name = "Kaone-";
      } else if (x <= 0.945) {
        name = "protone+";
      } else if (x <= 0.99) {
        name = "protone-";
      } else {
        name = "K*";
      }
      if (name == "K*") {
        double probability = gRandom->Rndm();
        Particle resonance("K*", px, py, pz);
        Particle pi = Particle("pione+");
        Particle k = Particle("Kaone-");
        if (probability > 0.5) {
          pi.SetIndex("pione-");
          k.SetIndex("Kaone+");
        }
        resonance.Decay2body(pi, k);
        invmas = pi.InvMass(k);
        std::cout << invmas << std::endl;
        new_set.push_back(pi);
        new_set.push_back(k);
        h1->Fill(Particle::FindParticleTest(resonance.GetName()));
        energy->Fill(resonance.GetEnergy());
        imdau->Fill(invmas);
      } else {
        new_set.emplace_back(name, px, py, pz);
        // new_set.push_back(particle);
        h1->Fill(new_set[j].FindParticleTest(new_set[j].GetName()));
        energy->Fill(sqrt(new_set[j].GetMass() * new_set[j].GetMass() + p * p));
        // std::for_each(new_set.begin(), new_set.end(), ()[]{});
      }
    }
    for (size_t i = 0; i < new_set.size() - 1; i++) {
      for (size_t j = i + 1; j < new_set.size(); j++) {
        double invMass = new_set[j].InvMass(new_set[i]);

        const auto first = new_set[j];
        const std::string first_type =
            first.GetName().substr(0, first.GetName().size() - 1);
        const auto second = new_set[i];
        const std::string second_type =
            second.GetName().substr(0, second.GetName().size() - 1);

        totalInvMass->Fill(invMass);

        if (first.GetCharge() * second.GetCharge() > 0) {
          invMassSameCharge->Fill(invMass);

          if ((first_type == "pione+" && second_type == "Kaone+") ||
              (first_type == "Kaone-" && second_type == "pione-"))
            invMassKPSame->Fill(invMass);

        } else {
          invMassOppositeCharge->Fill(invMass);

          if ((first_type == "pione+" && second_type == "Kaone-") ||
              (first_type == "Kaone+" && second_type == "pione-"))
            invMassKPOpposite->Fill(invMass);
        }
      }
    }
  }
  c1->cd(1);
  h1->Draw();
  c1->cd(2);
  impulse->Draw();
  c1->cd(3);
  polar->Draw();
  c1->cd(4);
  azimuthal->Draw();
  c1->cd(5);
  transverseimpulse->Draw();
  c1->cd(6);
  energy->Draw();
  c2->cd(1);
  imdau->Draw();
  c2->cd(2);
  totalInvMass->Draw();
  c2->cd(3);
  invMassOppositeCharge->Draw();
  c2->cd(4);
  invMassSameCharge->Draw();
  c2->cd(5);
  invMassKPOpposite->Draw();
  c2->cd(6);
  invMassKPSame->Draw();
  return 0;
}