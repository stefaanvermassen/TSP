# Gnuplot script file for plotting data in files
# The output can be found in plot1a.eps
set term postscript eps enhanced
set term epslatex
set output "splitsdiepte.eps"
set datafile separator ';'
# Plaats van de legende
set key right top
set grid
#set xrange [3:23]
#set yrange [0:30000]
set xlabel "Splitsdiepte"
set ylabel "Uitvoeringstijd (ms)"
plot "splitdata_2.dat" using 1:2 with linespoints pt 6 ps 0.5 lt -1 linecolor rgb "red" title "2 processen", \
     "splitdata_3.dat" using 1:2 with linespoints pt 6 ps 0.5 lt -1 linecolor rgb "blue" title "3 processen", \
     "splitdata_4.dat" using 1:2 with linespoints pt 6 ps 0.5 lt -1 linecolor rgb "green" title "4 processen"
set output

