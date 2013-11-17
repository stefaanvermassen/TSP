# Gnuplot script file for plotting data in files
# The output can be found in plot1a.eps
set term postscript eps enhanced
set term epslatex
set output "aantal_processen2.eps"
# Plaats van de legende
set nokey
set grid
set xlabel "Aantal processen"
set ylabel "Uitvoeringstijd"
plot "aantal_processen2.dat" using 1:2 with linespoints pt 6 ps 0.5 lt -1 linecolor rgb "red"
set output

