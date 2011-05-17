#!/usr/bin/python -O

from scipy import *
import matplotlib.pyplot as plt
import numpy as np
import sys

    
class Event:
	hits = []
	total_hits = 0
	p = [0, 0, 0]
	classification = 0;
	def angle(self, a, b, c):
		arctan(sqrt(a**2 + b**2) / c)
	def classify(self):
		cut = 0.42 # arg of tan
		x, y, z = self.p[0], self.p[1], self.p[2]
		arg1 = sqrt(x**2 + y**2 / z**2)
		arg2 = sqrt(y**2 + z**2 / x**2)
		arg3 = sqrt(x**2 + z**2 / y**2)
		sgn3 = sign(y)
		if arg1 < cut or arg2 < cut or (arg3 < cut and sgn3 > 0):
			self.classification = 1
		
	def __init__(self, tokens):
		self.hits = [int(tokens[i]) for i in range(0,5)]
		self.total_hits = sum(self.hits)
		self.p = [float(tokens[i]) for i in range(5,8)]
		self.classify()
	
def main():
	events = []
	print 'reading file'
	counter = 0
	for line in file('100_5.out'):
		counter += 1
		if counter % 1000 == 0:
			sys.stdout.write('*')
			sys.stdout.flush()
		tokens = line.split()
		events.append(Event(tokens))
		
	print '\nnumber of events: %d ' % len(events)
	data0 = [event.total_hits for event in events if event.classification == 0]
	data1 = [event.total_hits for event in events if event.classification == 1]
	
	xlo, xhi = 0, max([max(data0), max(data1)])
	print 'xrange =  %d - %d' % (xlo, xhi)
	discrete_bins = [x - 0.5 for x in range(0, xhi + 2)]
	(n0, bins, patches) = plt.hist(data0, bins=discrete_bins,linewidth=0,alpha=0.5)
	(n1, bins, patches) = plt.hist(data1, bins=discrete_bins,linewidth=0,alpha=0.5)
	ylo, yhi = 0, max([max(n0), max(n1)]) * 1.05
	
	plt.axis([xlo, xhi, ylo, yhi])
	
	plt.xlabel('# of detector hits')
	plt.title('Simulated UCNb response function at 100 keV (5 PMTs)')
	 
	frame1 = plt.gca()
	frame1.axes.get_yaxis().set_ticks([])
	frame1.axes.get_xaxis().set_ticks([0,200,400,600,800])
	fontsize = 8
	for tick in frame1.xaxis.get_major_ticks():
		tick.label1.set_fontsize(fontsize)
	for tick in frame1.yaxis.get_major_ticks():
		tick.label1.set_fontsize(fontsize)
	
	plt.savefig('hist.pdf',format='pdf')
	

if __name__ == '__main__':
	main()


