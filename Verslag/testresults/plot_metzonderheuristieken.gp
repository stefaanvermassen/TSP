# Gnuplot script file for plotting data in files
# The output can be found in plot1a.eps
set term postscript eps enhanced
set term epslatex
set output "metzonderheuristieken.eps"
set datafile separator ';'
# Plaats van de legende
set key left top
set grid
#set xrange [3:23]
set yrange [0.8:2]
set xlabel "Aantal steden"
set ylabel "Relatieve uitvoeringstijd"
plot "met_heuristieken.dat" using 1:2 with linespoints pt 6 ps 0.5 lt -1 linecolor rgb "red" title "Met heuristieken", \
     "zonder_heuristieken.dat" using 1:2 with linespoints pt 6 ps 0.5 lt -1 linecolor rgb "blue" title "Zonder heuristieken"
set output

