#!/usr/bin/python

ofile = open('PumpStationSVG.h', 'w')

ofile.write("""/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef PUMPSTATIONSVG_H
#define PUMPSTATIONSVG_H

const char * svgString = \"""")

with open("../../share/PumpStationSchematic.svg", "r") as f:
	for line in f:
		
		if "S00BG" in line:
			line = line.replace('255,0,0', '@S00BG@')
		if "S01BG" in line:
			line = line.replace('255,0,0', '@S01BG@')
		if "S02BG" in line:
			line = line.replace('255,0,0', '@S02BG@')
		if "S03BG" in line:
			line = line.replace('255,0,0', '@S03BG@')
		if "S04BG" in line:
			line = line.replace('255,0,0', '@S04BG@')

		if "P00BG" in line:
			line = line.replace('255,0,0', '@P00BG@')
		if "P01BG" in line:
			line = line.replace('255,0,0', '@P01BG@')
		if "P02BG" in line:
			line = line.replace('255,0,0', '@P02BG@')
		
		line = line.replace('"', '\\"')
		line = line.replace('\n', '')
		ofile.write (line)

ofile.write ("""\";
#endif
""")