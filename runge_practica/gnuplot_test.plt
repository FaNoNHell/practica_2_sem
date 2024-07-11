set output "test.png"

set grid xtics ytics
set xdata time

set xlabel 'time'
set title 'test'

plot 'data.txt' with l