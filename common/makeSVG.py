#!/usr/bin/python

ofile = open('MartaSVG.h', 'w')

ofile.write("""/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef MARTASVG_H
#define MARTASVG_H

const char * svgString = \"""")

previousLine = ""

with open("../share/MartaSchematics.svg", "r") as f:
	for line in f:
		
		#if "S00BG" in line:
		#	line = line.replace('255,0,0', '@S00BG@')
		#if "S01BG" in line:
		#	line = line.replace('255,0,0', '@S01BG@')
		#if "S02BG" in line:
		#	line = line.replace('255,0,0', '@S02BG@')
		#if "S03BG" in line:
		#	line = line.replace('255,0,0', '@S03BG@')
		#if "S04BG" in line:
		#	line = line.replace('255,0,0', '@S04BG@')

		if "P00BG" in line:
			line = line.replace('255,0,0', '@P00BG@')
		if "P01BG" in line:
			line = line.replace('255,0,0', '@P01BG@')
		if "P02BG" in line:
			line = line.replace('255,0,0', '@P02BG@')
			
		if "P1GaugeBG" in line:
			line = line.replace('255,0,0', '@P1GaugeBG@')
		if "P2GaugeBG" in line:
			line = line.replace('255,0,0', '@P2GaugeBG@')
		if "SYSGaugeBG" in line:
			line = line.replace('255,0,0', '@SYSGaugeBG@')
		
		if "Pump1BG" in line:
			line = line.replace('255,0,0', '@Pump1BG@')
		if "Pump2BG" in line:
			line = line.replace('255,0,0', '@Pump2BG@')
			
		if "Valve1BG" in line:
			line = line.replace('255,0,0', '@Valve1BG@')
		if "Valve2BG" in line:
			line = line.replace('255,0,0', '@Valve2BG@')
		if "Valve3BG" in line:
			line = line.replace('255,0,0', '@Valve3BG@')

		if "VesselBG" in line:
			line = line.replace('255,0,0', '@VesselBG@')
		
		if "P1Curve" in previousLine:
			line = line.replace('255,0,0', '@P1Curve@')
		if "P2Curve" in previousLine:
			line = line.replace('255,0,0', '@P2Curve@')
		if "SYSCurve1" in previousLine:
			line = line.replace('255,0,0', '@SYSCurve1@')
		if "SYSCurve2" in previousLine:
			line = line.replace('255,0,0', '@SYSCurve2@')
		if "SYSCurve3" in previousLine:
			line = line.replace('255,0,0', '@SYSCurve3@')
		if "SYSCurve4" in previousLine:
			line = line.replace('255,0,0', '@SYSCurve4@')
		
		previousLine = line
		
		line = line.replace('"', '\\"')
		line = line.replace('\n', '')
		ofile.write (line)

ofile.write ("""\";
#endif
""")
