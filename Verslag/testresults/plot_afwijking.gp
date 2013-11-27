# Gnuplot script file for plotting data in files
# The output can be found in plot1a.eps
set term postscript eps enhanced
set term epslatex
set output "afwijking.eps"
set datafile separator ';'
# Plaats van de legende
set key left top
set grid
set xrange [3:23]
set yrange [0.8:2]
set xlabel "Aantal steden"
set ylabel "Kost"
plot "afwijking_opt.dat" using 1:2 with linespoints pt 6 ps 0.5 lt -1 linecolor rgb "red" title "optimale kost", \
     "afwijking_sa.dat" using 1:2 with linespoints pt 6 ps 0.5 lt -1 linecolor rgb "blue" title "simulated annealing", \
     "afwijking_tb.dat" using 1:2 with linespoints pt 6 ps 0.5 lt -1 linecolor rgb "green" title "simulated annealing+tabu search"
set output

