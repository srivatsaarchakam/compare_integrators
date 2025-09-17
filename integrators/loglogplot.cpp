#include <bits/stdc++.h>
using namespace std;

static bool is_number(const string& s){
    char* end=nullptr;
    strtod(s.c_str(), &end);
    return end && *end=='\0';
}

int main(int argc, char** argv){
    string in  = (argc >= 2) ? argv[1] : "errors.tsv";
    string out = (argc >= 3) ? argv[2] : "Errors.png";

    ifstream fin(in);
    if (!fin){

        cerr << "ERROR: cannot open input file: " << in << "\n";
        return 1;
    }

    vector<double> N, eT, eS, eG;
    string line;
    bool first = true;

    while (getline(fin, line)){

        if (line.empty()) continue;
        istringstream iss(line);
        string a,b,c,d;

        if (!(iss >> a >> b >> c >> d)){
            continue;
        } 
        if (first){
            first = false;

            if (!is_number(a)) {
                continue;
            }
        }

        if (!is_number(a) || !is_number(b) || !is_number(c) || !is_number(d)){
            continue;
        }

        double n = stod(a);
        double t = stod(b);
        double s = stod(c);
        double g = stod(d);

        if (n>0 && t>0 && s>0 && g>0){
            N.push_back(n);
            eT.push_back(t); 
            eS.push_back(s); 
            eG.push_back(g);
        }
    }
    if (N.size() < 2){
        cerr << "ERROR: need at least 2 valid rows in " << in << "\n";
        return 1;
    }

    const string clean = "errors_clean.dat";
    ofstream fclean(clean);

    fclean.setf(std::ios::scientific);
    fclean.precision(12);

    for (size_t i=0;i<N.size();++i){
        fclean << N[i] << "  " << eT[i] << "  " << eS[i] << "  " << eG[i] << "\n";
    }
        
    fclean.close();

    size_t idx = (N.size()>=3) ? size_t(0.7*N.size()) : N.size()-1;
    if (idx >= N.size()) idx = N.size()-1;

    const double x1 = N[idx];
    const double y1m2 = eS[idx];   // use Simpson as reference for -2 guide
    const double y1m4 = eS[idx]/4; // slightly lower so labels don't overlap

    const double decades = 0.6; // guide length in decades
    const double x2 = x1 / pow(10.0, decades);
    const double y2m2 = y1m2 * pow(x2/x1, -2.0);
    const double y2m4 = y1m4 * pow(x2/x1, -4.0);

    ofstream f2("slope2.dat");
    f2.setf(std::ios::scientific); f2.precision(12);
    f2 << x1 << " " << y1m2 << "\n" << x2 << " " << y2m2 << "\n";
    f2.close();

    ofstream f4("slope4.dat");
    f4.setf(std::ios::scientific); f4.precision(12);
    f4 << x1 << " " << y1m4 << "\n" << x2 << " " << y2m4 << "\n";
    f4.close();

    auto mid_idx = [&](const vector<double>& v){ return max<size_t>(1, v.size()*6/10)-1; };
    size_t iT = mid_idx(N), iS = mid_idx(N), iG = mid_idx(N);
    double xT = N[iT], yT = eT[iT];
    double xS = N[iS], yS = eS[iS];
    double xG = N[iG], yG = eG[iG];

    const string gp = "plot_errors.gnu";
    ofstream g(gp);
    g << "set term pngcairo size 900,640\n";
    g << "set output '" << out << "'\n";
    g << "set logscale xy\n";
    g << "set xlabel 'N'\n";
    g << "set ylabel '|error|'\n";
    g << "set grid lw 1 dt 3\n";
    g << "unset key\n";

    g << "# set xrange [2:*]\n";
    g << "# set yrange [1e-14:1e-1]\n";
    // on-curve labels
    g << "set label 1 'trapezoid' at " << xT << "," << yT << " offset 1,1\n";
    g << "set label 2 'Simpson'   at " << xS << "," << yS << " offset 1,1\n";
    g << "set label 3 'Gaussian'  at " << xG << "," << yG << " offset 1,1\n";

    g << "plot \\\n";
    g << "  '" << clean << "' using 1:2 with linespoints lw 1 pt 7, \\\n";
    g << "  '' using 1:3 with linespoints lw 1 pt 5, \\\n";
    g << "  '' using 1:4 with lines dt 2 lw 1, \\\n";
    g << "  'slope2.dat' using 1:2 with lines dt 2 lw 1, \\\n";
    g << "  'slope4.dat' using 1:2 with lines dt 2 lw 1\n";
    g.close();

}
