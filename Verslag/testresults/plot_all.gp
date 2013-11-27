# Gnuplot script file for plotting data in files
# The output can be found in plot1a.eps
set term postscript eps enhanced
set term epslatex
set output "all.eps"
set datafile separator ';'
# Plaats van de legende
set key left top
set grid
#set xrange [3:23]
#set yrange [0.8:2]
set xlabel "Aantal steden"
set ylabel "Uitvoeringstijd (ms)"
plot "heuristics.dat" using 1:2 with linespoints pt 6 ps 0.5 lt -1 linecolor rgb "red" title "Heuristieken", \
     "all_2.dat" using 1:2 with linespoints pt 6 ps 0.5 lt -1 linecolor rgb "blue" title "branch and bound, 1 proces", \
     "all_3.dat" using 1:2 with linespoints pt 6 ps 0.5 lt -1 linecolor rgb "green" title "branch and bound, 2 processen"
set output

