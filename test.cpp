#include <bits/stdc++.h>
#include "integrators.hpp"
using namespace std;

static double g_w = 1e-3;     

static double boxfunc(double x){
  return (fabs(x - 0.5) < 0.5 * g_w) ? (1.0 / g_w) : 0.0;
}

static constexpr double exact_box = 1.0;

int main(){
  const double a = 0.0, b = 1.0;

  vector<unsigned> Nlist = { 9, 17, 33, 65, 129, 257, 513, 1025, 2049, 4097, 8193, 16385 };

  vector<unsigned> Klist;
  for (unsigned k=2; k<=25; ++k) Klist.push_back(k);

  ofstream fts("errors_box_TS.dat");   // columns: N  eT  eS
  fts.setf(std::ios::scientific); fts.precision(12);

  for (unsigned npts : Nlist){
    unsigned nS = (npts % 2 ? npts : npts + 1); // Simpson requires odd npts
    double T = trapez(boxfunc, npts, a, b);
    double S = simpson(boxfunc, nS, a, b);
    double eT = fabs((T - exact_box)/exact_box);
    double eS = fabs((S - exact_box)/exact_box);
    fts << npts << "  " << eT << "  " << eS << "\n";
  }
  fts.close();

  ofstream fg("errors_box_G.dat");     // columns: k  eG
  fg.setf(std::ios::scientific); fg.precision(12);
  for (unsigned k : Klist){
    GaussInt G(k);
    double Gv = G.Integ(boxfunc, a, b);
    double eG = fabs((Gv - exact_box)/exact_box);
    fg << k << "  " << eG << "\n";
  }
  fg.close();

  
  ofstream gp("baderrors_box.gnu");
  gp <<
R"(set term pngcairo size 900,640
set output 'BadErrors.png'
set logscale xy
set xlabel 'points'
set ylabel '|relative error|'
set grid lw 1 dt 3
set title 'Rectangular pulse: width w=1e-3 at x=0.5 on [0,1] (area=1)'
set key top right

# TS/Simpson curves vs N (number of grid points)
# Gaussian curve vs k (number of Gauss points)
plot \
  'errors_box_TS.dat' using 1:2 with linespoints lw 1 pt 7  title 'Trapezoid (vs N)', \
  ''                   using 1:3 with linespoints lw 1 pt 5  title 'Simpson (vs N)',  \
  'errors_box_G.dat'   using 1:2 with lines dt 2 lw 2        title 'Gaussian (vs k ≤ 25)'
)";
  gp.close();