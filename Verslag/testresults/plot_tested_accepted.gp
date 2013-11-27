# Gnuplot script file for plotting data in files
# The output can be found in plot1a.eps
set term postscript eps enhanced
set term epslatex
set output "tested_accepted.eps"
set datafile separator ';'
# Plaats van de legende
set key left top
set grid
set xrange [0:336]
#set yrange [0.8:2]
set xlabel "Stap"
set ylabel "Afstand"
plot "tested.dat" using 1:2 with linespoints pt 6 ps 0.5 lt -1 linecolor rgb "blue" title "Geteste afstand", \
     "accepted.dat" using 1:2 with linespoints pt 6 ps 0.5 lt -1 linecolor rgb "red" title "Geaccepteerde afstand"
set output

