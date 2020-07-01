###############################
name1="envelope"
data1=sprintf("%s.dat",name1)
name2="power"
data2=sprintf("%s.dat",name2)


epsname = "plot.eps"

#epsファイルのサイズ変更
epswidth = 7
epsheight= 5

###############################
set autoscale
#unset mouse
set multiplot layout 2,1 

set style line 1 lt 1
set style line 2 lt 2
set style line 3 lt 3
set style line 4 lt 4
#set ticslevel 0
unset xlabel
set xlabel "Time [s]"
unset grid
unset key

###########################################
#			     応答値x	1			  #
###########################################
set autoscale
set ylabel "Output"
p data1 u 1:2 w l, data1 u 1:3 w l, data1 u 1:4 w l

###########################################
#			     応答値x	1			  #
###########################################
set autoscale
set ylabel "Power"
set logscale x
p data2 u 1:2 w l

		
unset multiplot
reset
