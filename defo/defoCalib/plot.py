#!/usr/bin/python

# usage:
# plots all defoDump_beforeDefo_afterDefo.txt file in 2D Graph

# execute
# ./plot.py *.txt

# out put
# defoDump_beforeDefo_afterDefo.txt.eps

import sys
import string

from ROOT import TCanvas, TPaveText, TPad, TH1, TGraph2D
from ROOT import gROOT, gStyle

gROOT.SetBatch()
canvas1 = TCanvas( 'c1', 'Surfaces Drawing Options', 1024, 768 )

def remove_values_from_list(the_list, val):
	while val in the_list:
			the_list.remove(val)
			
for i in range(len(sys.argv)):
	if string.count(sys.argv[i],'defoDump') > 0:
		print 'Opening file number ' + str(i) + ' ' + sys.argv[i]
		file = open(sys.argv[i])
		
		canvas1.cd()
		canvas1.Clear()
		
		nPointsX = 0
		
		grX = TGraph2D()
		
		for line in file:
			
			values = []
			tokenizedLine = line.split(' ',99)
			
			remove_values_from_list(tokenizedLine, ' ')
			remove_values_from_list(tokenizedLine, '')
			
			for token in tokenizedLine:
				values.append(token)
			
			nPointsX += 1
			grX.SetPoint(nPointsX, float(values[2]), float(values[3]), float(values[6]) )
			
			print line
			
		grX.Draw('surf1')
		
		canvas1.Modified()
		canvas1.Update()
		
		outputFileName = sys.argv[i] + '.eps'
		canvas1.Print(outputFileName)
print "                                     "
print ".................................done"
print "                                     "

