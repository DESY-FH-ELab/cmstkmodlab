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

const char * MartaSVGString = \"""")

previousLine = ""

with open("../share/MartaSchematics.svg", "r") as f:
	for line in f:
		
		if "AlarmButton" in line:
			line = line.replace('0,255,0', '@AlarmColor@')

		if "R507PumpBG" in line:
			line = line.replace('0,255,0', '@R507PumpColor@')
		
		if "CO2PumpBG" in line:
			line = line.replace('0,255,0', '@CO2PumpColor@')
		
		previousLine = line
		
		line = line.replace('  ', ' ')
		line = line.replace('  ', ' ')
		line = line.replace('  ', ' ')
		line = line.replace('  ', ' ')
		line = line.replace('  ', ' ')
		
		line = line.replace('"', '\\"')
		line = line.replace('\n', '')
		ofile.write (line)

ofile.write ("""\";
#endif
""")
