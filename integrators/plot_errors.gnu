set term pngcairo size 900,640
set output 'Errors.png'
set logscale xy
set xlabel 'N'
set ylabel '|error|'
set grid lw 1 dt 3
unset key
# set xrange [2:*]
# set yrange [1e-14:1e-1]
set label 1 'trapezoid' at 2,0.0819767 offset 1,1
set label 2 'Simpson'   at 2,0.000337153 offset 1,1
set label 3 'Gaussian'  at 2,0.000224323 offset 1,1
plot \
  'errors_clean.dat' using 1:2 with linespoints lw 1 pt 7, \
  '' using 1:3 with linespoints lw 1 pt 5, \
  '' using 1:4 with lines dt 2 lw 1, \
  'slope2.dat' using 1:2 with lines dt 2 lw 1, \
  'slope4.dat' using 1:2 with lines dt 2 lw 1
