#include <iostream>
#include <vector>
#include <cmath>
#include "integrators.hpp"

static double f(double x){ return std::exp(-x); }

int main(){
  const double a = 0.0, b = 1.0;
  const double exact = 1.0 - std::exp(-1.0);
  std::vector<unsigned> N = {2,10,20,40,80,160,320,640,1000};

  std::cout.setf(std::ios::scientific);
  std::cout.precision(12);
  std::cout << "N\teps_T\teps_S\teps_G\n";

  for (unsigned npts : N){
    double T = trapez(f, npts, a, b);
    double eT = std::fabs((T - exact)/exact);

    unsigned nS = (npts % 2 ? npts : npts + 1);   // Simpson needs odd npts
    double S = simpson(f, nS, a, b);
    double eS = std::fabs((S - exact)/exact);

    unsigned k = std::min(25u, std::max(2u, npts/2)); // Gauss rule size
    GaussInt G(k);
    double Gv = G.Integ(f, a, b);
    double eG = std::fabs((Gv - exact)/exact);

    std::cout << npts << '\t' << eT << '\t' << eS << '\t' << eG << '\n';
  }
}
