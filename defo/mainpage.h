

/*! \mainpage Defo Software User's Guide

<b><center>Reconstruction algorithms reference, peripheral devices and GUI</center></b>

\section contents_sec              Contents

\ref overview_sec <BR>
\ref imagereco_sec <BR>
&nbsp;&nbsp;\ref point_subsec <BR>
&nbsp;&nbsp;\ref bluepoint_subsec <BR>
\ref surfacereco_sec <BR>
&nbsp;&nbsp;\ref units_subsec <BR>
&nbsp;&nbsp;\ref spacing_subsec <BR>
&nbsp;&nbsp;\ref indexing_subsec <BR>
&nbsp;&nbsp;\ref slope_subsec <BR>
&nbsp;&nbsp;\ref splinecreation_subsec <BR>
&nbsp;&nbsp;\ref surfacecreation_subsec <BR>
&nbsp;&nbsp;\ref offsetsandtilts_subsec <BR>
&nbsp;&nbsp;\ref surfaceevaluation_subsec <BR>
\ref surfaceformat_sec <BR>
\ref devices_sec <BR>
&nbsp;&nbsp;\ref camera_subsec <BR>
&nbsp;&nbsp;\ref chiller_subsection <BR>
&nbsp;&nbsp;\ref multimeter_subsection <BR>
\ref gui_sec <BR>
&nbsp;&nbsp;\ref gui_overview_subsec <BR>
&nbsp;&nbsp;\ref gui_online_subsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_measurementid_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_areas_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_display_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_refresh_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_manual_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_schedules_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_output_subsubsec <BR>
&nbsp;&nbsp;\ref gui_cooling_subsec <BR>
&nbsp;&nbsp;\ref gui_offline_subsec <BR>
&nbsp;&nbsp;\ref gui_advanced_subsec <BR>
&nbsp;&nbsp;\ref gui_viewer <BR>
\ref appendix_sec <BR>
&nbsp;&nbsp;\ref app_config_subsec <BR>
&nbsp;&nbsp;\ref app_containers_subsec <BR>
&nbsp;&nbsp;\ref app_literature_subsec <BR>

<BR> <BR>

\section overview_sec              1. General overview
\image html defoScheme.gif "General surface reconstruction principle"

The algorithms of the defo package perform the 3D surface
reconstruction for the defo setup. The dots in the optical grid are
reflected by the surface under test (or device under test, DUT) and
observed by the camera. A deformation of the surface will result in a
displacement of the dots on the CCD chip from which the change of the
surface shape can be inferred.  All deformations are measured relative
to a reference state, i.e.  by comparing an image of the current state
to a reference image (e.g. at room temperature).

The reconstruction basically performs a numerical spatial integration
of the measured slope along the surface plane. As a result, global
translations, rotations or expansions/contractions of the surface are
indistinguishable from the integration constants and cannot
be measured without a potential unwanted bias.

The measurement principle implies that the DUT has a sufficiently well
reflecting surface. It is however not required to be perfectly
mirroring. In many cases, measures can be taken to obtain sufficient
reflectivity by surface finishing or the application of reflective
elements or foils. A slightly coarse surface, even with minor
scratches, is convenient, since most systematic effects cancel out in
the measurement which is always performed relative to a reference
state. Additionally, diffractive effects from inhomogenous surfaces
(silicon strip sensors) can typically be handled by proper adjustment
of the image reconstruction parameters (see \ref imagereco_sec, \ref
app_config_subsec).

All algorithms and the GUI rely on the presence of a configuration
file (see also \ref app_config_subsec) whose name and location are
consistently hard-coded in various places in calls to the
DefoConfigReader constructor. The default name is <tt>defo.cfg</tt>
and the default location is the current directory in which the
application has started. In the following, capitalized terms,
e.g. <tt>HALF_SQUARE_WIDTH</tt>, denote reconstruction parameters in the
configuration file. All file paths are given relative to the svn root
(<tt>cmstkmodlab/trunk/</tt>).


\section imagereco_sec             2. Raw image reconstruction

Images loaded from the camera or from a file are stored as QT <a
href="http://doc.qt.nokia.com/4.3/qimage.html">QImage</a> objects and
further embedded in objects of type DefoRawImage, fitting them with
additional functionality. The origin of an image is located in the top
left corner, with the x-axis running horizontally to the right and the
y-axis vertically to the bottom. All positions and distances on the
image are handled in units of pixels.

\image html imageCoordinates.gif "Image origin and coordinate definition"

\subsection point_subsec             2.1 Point finding and seeding

Class DefoRecoImage performs the recognition of reflected points in
the image and the reconstruction of their positions relative to the
image origin in units of pixels. Within DefoRecoImage::reconstruct,
reflected points are found by scanning all pixels in an image for
grayscale values above certain thresholds. Scanning is performed
row-by-row (along the positive x direction) starting from the topmost
row towards the bottom of the image. Thus, a reflected point will be
"approached" coming from the left, hitting its top left "foothill"
first. To reduce dead time during the scan, a three-step seeding
algorithm using 3 different thresholds is used:

-# When scanning over a pixel with a grayscale value above
<tt>STEP1_THRESHOLD</tt> ("step1 seed"), the scan is halted and the
vicinity of that pixel is investigated in the next step. Otherwise,
the scan continues.
-# The average of the grayscale values of the following 4 pixels is
calculated (indices relative to the step1 seed): <+2,+2>, <+2,+3>,
<+3,+2>, <+3,+3>.  If this average exceeds <tt>STEP2_THRESHOLD</tt>,
the seed is confirmed and the algorithm proceeds to the next
step. Otherwise, the step1 seed is dropped and the scan continues.
-# The step1 seed is passed to DefoRecoImage::averageSquare which
calculates the center-of-gravity of all pixel coordinates in a square
of half width <tt>HALF_SQUARE_WIDTH</tt> around the step1 seed using
their grayscale values as weights. Only pixels whose grayscale values
exceed <tt>STEP3_THRESHOLD</tt> are considered in this calculation.
This step is then repeated two times to improve the point coordinate
estimate by centering the square around the point.

\image html defo_scan.gif "Scanning over the pixels, seeding a point and reconstructing its position"

To speed up the process and to avoid multiple reconstruction of
points, a square-shaped area of half width <tt>HALF_SQUARE_WIDTH</tt>
around the reconstructed position of a point is stored in a veto
collection (DefoRecoImage::forbiddenAreas_). Pixels within any of the
squares in this collection will be overleaped during the scan as it
proceeds. Thus, the parameter <tt>HALF_SQUARE_WIDTH</tt> defines the
approximate minimum spacing of points in the image that can be handled
by the reconstruction algorithm.

The output of DefoRecoImage::reconstruct is of type DefoPointCollection,
containing the reconstructed point coordinates in the order as they were
found during the scan.

\subsection bluepoint_subsec          2.2 The blue point

The blue point is essential as an absolute reference within the image
(or - if defined - an active area) and required for indexing (\ref
indexing_subsec) Exactly one single point must be reconstructed as
"blue" in the image or area. If this requirement is not fulfilled, the
reconstruction of the current image will fail.  The proper
reconstruction of the blue point should be checked before starting a
schedule.

Blue points in the grid are identified using the method
DefoRecoImage::calculateBlueishness. For all pixels within a square of
half width <tt>HALF_SQUARE_WIDTH</tt> around a reconstructed point
position the blue/yellow fraction is determined and averaged. A point
is considered "blue" if this average exceeds the value of the
configuration parameter <tt>BLUEISHNESS_THRESHOLD</tt>. This tag is
stored in the DefoPoint object and can be queried using
DefoPoint::isBlue. DefoRecoImage itself does not put any requirements
on the number of points it tags as blue.



\section surfacereco_sec           3. Surface reconstruction

\subsection units_subsec             3.1 Units and geometry
\image html defo_geometry.gif "The geometry parameters of the deformation setup"

The setup geometry is characterized by the distances \f$L_c\f$ between
camera and DUT center and \f$L_g\f$, the distance between the middle
of the dot grid and the middle of the DUT. The NOMINAL_VIEWING_ANGLE of the
camera is defined as the angle between the vertical and \f$L_c\f$. The
setup is assumed to be symmetric, i.e. the angles between \f$L_c\f$
and \f$L_g\f$, respectively, and the vertical must be identical.

Internally, all spatial coordinates are processed in units of meters
and all angles are given in radian. Image processing related algorithms
measure distances in units of pixels.

\subsection spacing_subsec           3.2 Average spacing

The average spacing of a field of reconstructed reflected points is
defined as the pair of their average lattice constants along x and y
(in pixels, type double) and calculated by the function
DefoRecoSurface::determineAverageSpacing. This quantity is required
for indexing (\ref indexing_subsec). The average spacing is calculated
as follows:

-# The x and y point coodinates (in pixels) are separately filled in parallel vectors and sorted.
-# The differences of all x,y values wrt. their predecessors in the vectors are summed up.
To avoid summing the tiny differences of points in the same column (row),
their values along x (y) must exceed the threshold given by the parameter <tt>SPACING_ESTIMATE</tt>.
-# Finally, the average is calculated using the number of values that have been processed.

For deformations not exceeding a few hundred micron, the resulting
value estimates the true spacing of two adjacent points reflected from
a regular grid with an accuracy of 1-2 pixels (determined by
simulation). 



\subsection indexing_subsec          3.3 Point indexing

For the surface reconstruction step, the reconstructed reflected
points must be indexed, i.e. they are assigned a pair of integers
which indicate their position along x and y relative to the blue
(which has indices 0,0 by definition). Indexing later allows to match
a point in any reconstructed image to its equivalent in the reference
image, such that its spatial deviation can be calculated.  Indexing is
performed by the method DefoRecoSurface::indexPoints.

\image html defo_indexing1.gif "Principle of point indexing along a single coordinate starting from the blue point in the middle"

Origin of the indexing step is the single blue point in the image or
area which is therefore required to be present in the setup and must
have been recognized in the image. In the first step, starting from
the blue point, the position of the adjacent point along the positive
y coordinate is estimated using the previously determined average
spacing value (\ref spacing_subsec). Within a search window, i.e. a
circle with radius <tt>SPACING_ESTIMATE</tt>, the method
DefoRecoSurface::findClosestPoint then searches for an existing
reconstructed point around that position:

- if a point is found, the appropriate indices are attached to it
and its coordinates are taken as the starting point for estimating
the position of the next point using the average spacing;

- if no point is found, the index is skipped (creating a "gap" in the
point grid), and the coordinates of the estimated position
are taken as the starting point for estimating the position
of the next point.

This procedure continues until the last point in the column has been
found and indexed. At the same time, each of the points or gaps
(estimated position) is used as a starting point for finding the
indices of further points along the (orthogonal) x-axis
according to the same algorithm and in both directions.

In the second step, and again starting with the blue dot, the procedure
is repeated along the negative y direction to index the remaining
half of the field of reconstructed points.

\subsection slope_subsec             3.4 Surface slope determination

With the geometry parameters as defined in \ref units_subsec, the
local surface slope \f$\Delta S\f$ can be obtained as
\f[
\tan \{ 2\cdot \arctan(\Delta S) \} \cdot L_g(\delta) = \frac{\Delta d\cdot P}{f}\cdot (L_g(\delta)+L_c(\delta))
\f]
where \f$\Delta d\f$ denotes the spatial displacement of a point
position in the image with respect to the reference image, and P is
the pixel pitch on the CCD sensor. Since the point displacement angles
\f$\alpha\f$ are small, the above relation can further be
simplified. The distances \f$L_g\f$ and \f$L_c\f$ depend of the
viewing angle \f$\delta\f$ which has to be taken into account when the
measurement covers a large area. The corresponding corrections are
implemented in DefoRecoSurface::createZSplines (see correctionFactors
variable) but yet not fully tested.

\subsection splinecreation_subsec    3.5 Spline creation and fitting

Quadratic splines are used to calculate and interpolate h (height)
information between the reconstructed points. A quadratic spline is a
2nd order polynomial with three parameters A, B and C:
\f[
h(x) = A\cdot x^2 + B\cdot x + C
\f]
and mounted between two reconstructed points. A single spline is
represented by an instance of the class DefoSpline.

\image html defo_splineset.gif "a) A single DefoSpline connects 2 points;  b) a DefoSplineSetX consisting of 5 matched splines"

In the surface reconstruction step DefoRecoSurface::createZSplines,
points are grouped in ascending order according to their indices (\ref
indexing_subsec) in "rows" along the positive x coordinate and
"columns" along the positive y coordinate. A spline set is a group of
splines interconnecting such a row along x (DefoSplineSetX) or along y
(DefoSplineSetY). Given the continuity requirements for the height and
slope of the splines at the positions of the points, a spline set can
be fit to N points using the following equations:
\f[
h_j (x_{j+1}) = h_{j+1} (x_{j+1})\ \mbox{for}\ j = 1..N-1
\f]
\f[
\frac{\delta h_j}{\delta x} (x_{j+1}) = \frac{\delta h_{j+1}}{\delta x} (x_{j+1}) = h_{j+1} \ \mbox{for}\ j = 1..N-1
\f]
\f[
\frac{\delta h_1}{\delta x} (x_1) = \Delta S_1, h_1 = 0
\f]
where the last equation describes the freedom of choice of an initial
slope \f$\Delta S_1\f$ and height \f$h_1\f$ at the position of the
first point ("integration constants"). These equations can be solved
for a given set of points to obtain the parameters A, B and C for all
splines in a spline set. The measured slopes calculated from the point
displacements enter the equations as the derivatives \f$\delta
h/\delta x\f$. This functionality is embedded in:
DefoSplineSetX::doFitZ and DefoSplineSetY::doFitZ for point groups
along x (rows) and along y (columns), respectively. The height h of a
spline set can be evaluated at any position within its range
(i.e. between the first and the last point along the coordinate) using
the DefoSplineSetBase::eval method.

The result of the spline creation and fitting step is one instance of
class DefoSplineField, containing two vectors of spline sets
(DefoSplineSetXCollection/DefoSplineSetYCollection), one for the
fitted rows (splines along x) and one for the columns (splines along
y). Since rows and columns are orthogonal, any reconstructed point can
be part of one DefoSplineSetX and one DefoSplineSetY at the same
time. Thus, in principle, both DefoSplineCollections contain the full
3D surface information and can be queried for height information
independently.


\subsection surfacecreation_subsec   3.6 Cross-mounting of spline sets

Given the freedom of choice of initial slopes and heights at the
borders of the spline sets (\ref splinecreation_subsec), the
individually reconstructed spline sets generally have arbitrary height
offsets with respect to each other. In order to remove these offsets,
the spline sets must be levelled to a common reference frame, i.e. the
constants C of the individual splines have to be adjusted such that a
continuous surface shape is obtained. The levelling is done per spline
set, thus all splines in a set receive the same correction addend to
their parameter C. The final surface reference frame is defined as the
average of all combinations of local displacements of the orthogonal
spline sets along x and y.

\image html xySplineCombinationPrinciple.gif "Principle of mounting and averaging x and y splines to obtain the full reconstructed surface"

The figure illustrates the process of levelling which is performed in
the DefoRecoSurface::mountZSplines routine. Within a given reference
DefoSplineSetX (with points strung along a row), those points are
identified which the set shares with any of the orthogonal sets along
the columns (DefoSplineSetY). At each of these points, the difference
in height as evaluated from the two spline functions is determined and
summed for the respective spline set along y (a). This step is
subsequently repeated with all the spline sets along x while further
summing the height difference to the spline sets along y (b,c)

As a next step, the same procedure is performed orthogonally for the
spline sets along x with the sets along y serving as height reference
(d-f). Finally, for each spline set, the sum of all height offsets is
divided by the total number of comparisons, thus averaging all
possible local offsets. The result is a smooth surface (g).

\subsection offsetsandtilts_subsec   3.7 Removal of global offsets and tilts
\subsection surfaceevaluation_subsec 3.8 Surface evaluation

\section surfaceformat_sec         4. Format of surface objects

A reconstructed surface is stored in an object of class DefoSurface,
containing the reconstructed points (DefoPointFields) and splines
(DefoSplineField) separately for the reconstruction along x and y, respectively.

Using the serialization functionality of the boost library these
objects are written to the output directories as byte streams with
file extension: <tt>.defosurface</tt>. Future versions: these files
can be read and surfaces viewed offline using the DefoViewer (TBD).

\section devices_sec			       	5. Access to peripheral devices

\subsection camera_subsec		       	5.1 Camera

The camera is a Canon EOS550D, currently equipped with a 85mm zoom
lens. Access to the camera is provided by the DefoCamHandler class
which interfaces to <tt>EOS550DSyscall</tt> in the Canon subpackage
(<tt>devices/Canon</tt>). The latter uses the <a
href="http://sourceforge.net/projects/gphoto">gphoto2</a> executable
to control the camera and download images to the local file system.
Camera access via <tt>EOS550DSyscall</tt> is threaded to ensure
a smooth program flow while the communication with the device is
carried out.

Camera features and properties like possible apertures, shutter speeds
and iso settings are defined in the <tt>devices/Canon</tt>
subpackage. Due to some technical shortcomings<sup>1)</sup> in current versions of
QT, certain definitions have to be hard-coded also in the
DefoMainWindow.h class header. <i>Thus, whenever camera
properties change, this file must be adapted accordingly!</i> (or find a
better solution...) 

\htmlonly 
<font size="-1"><sup>1)</sup> Definitions are supposed to be imported
from <tt>devices/Canon/eosParameters.h</tt> but as of QT4.7 moc cannot
expand #include statements.</font>
\endhtmlonly

\subsection chiller_subsection			5.2 Chiller

A Julabo FP50 chiller is used as cooling plant. The corresponding
functionality for steering the chiller and reading the operation
parameters is embedded in the <tt>JulaboFP50</tt> class
(<tt>devices/Julabo</tt>).

\subsection multimeter_subsection		5.3 Multimeter

pt100 temperature probes are read out via the Keithley 2700 multimeter.
The corresponding functionality is embedded in the <tt>Keithley2700</tt>
class (<tt>devices/Keithley</tt>).

\section gui_sec                               6. GUI

The GUI window is divided into four tabs: measurement control
(<i>online</i>), chiller and temperature readout/control
(<i>cooling</i>), result visualization (<i>offline</i>) and
configuration (<i>advanced</i>).

\subsection gui_overview_subsec                6.1 Overview and invocation

The GUI is invoked by a call to the <tt>defo</tt> executable in the
<tt>defo</tt> folder. To set the proper shell environment for
execution, the files <tt>tkmodlabenv.sh (.csh)</tt> in the svn root
(<tt>cmstkmodlab/trunk</tt>) must have been loaded using the
<tt>source</tt> shell command. The GUI requires the presence of the
configuration file <tt>defo.cfg</tt> in the current directory,
i.e. the directory in which the application has started (see also \ref
overview_sec).

\subsection gui_online_subsec                  6.2 Online tab

\image html screenshot_gui_online.gif "The online tab"


\subsubsection gui_online_measurementid_subsubsec      6.2.1 Measurement ID

The measurement ID serves as a common identification string for a
group of deformation measurements (including reference
measurements). In particular, all output for a given measurement,
manual or schedule driven, will be stored in a subfolder named after
the currently valid measurement ID. The measurement ID is initially
set to the default string <tt>defomeasurement-ddmmyy-hhmmss</tt>
(including date and time, see figure), however an arbitrary string can
also be chosen using the <tt>Edit</tt> button (the <tt>Default</tt>
button restores the default string). The buttons are disabled during
reconstruction or schedule execution.

\subsubsection gui_online_areas_subsubsec      6.2.2 Areas

The surface reconstruction process can be restricted to an area of
interest ("area"). When an area is defined, all image features outside
its boundaries are dismissed. Thus, an area is also useful for
omitting parts of an image which are affected by ambient light
effects, unwanted reflections or other nuisances. So far, only
one single area can be defined which must involve a properly
reconstructed and recognized blue point.

An area can be defined by clicking the <tt>New</tt> button and
subsequent click-and-drag in the image to define its rectangular
boundaries from the top left to the bottom right. The boundaries are
displayed as a yellow dotted line. Areas are assigned names A1...An,
however, currently only one single area A1 is supported (thus the
<tt>no.</tt> spin box for area selection is not of particular use).

When an area is deleted, all reference information is invalidated
and a new reference image must be loaded and processed.

\subsubsection gui_online_display_subsubsec  6.2.3 Display options
The following information can be displayed as overlays on an image:
<DL>
<DT>Points</DT>
<DD>The reconstructed point positions are marked by a cross (3 px lines),
surrounded by a square of size <tt>HALF_SQUARE_WIDTH</tt> (see \ref point_subsec).
Crosses and squares are drawn in orange for normal points and in
blue for the blue point.</DD>
<DT>Areas</DT>
<DD>Areas are indicated as rectangles of yellow dotted lines which
show the area name in the top left corner.</DD>
<DT>Indices</DT>
<DD>If enabled, a pair of integers is drawn in magenta indicating the reconstructed indices
of each point relative to the blue point. The blue point has indices (0,0) by definition.</DD>
<DT>Splines</DT>
<DD>The reconstructed splines (see \ref splinecreation_subsec) are shown which
interconnect the reconstructed points and from which the surface slope
is determined. <SPAN style="color:#ff0000"><B>Not yet implemented!</B></span></DD>
<DT>Coords</DT>
<DD>Whether to draw a coordinate system at the bottom left corner of the image.</DD>
</DL>

\subsubsection gui_online_refresh_subsubsec  6.2.4 Refreshing images
Two buttons in this section allow for refreshing the current image display
either by capturing with the camera or from a file (a file dialog will open).
The camera button is not clickable when camera operation is globally
disabled (see \ref gui_advanced_subsec).

\subsubsection gui_online_manual_subsubsec  6.2.5 Manual operation and status
The <i>Manual</i> section of the online tab features three buttons for manual (non-scheduled)
point and surface reconstruction as well as  chiller temperature setting. Surface reconstruction
will be performed on the current image as displayed on the tab. 

<DL>
<DT>REF</DT><DD>Reconstruct the points in the current image and store the result
as the reference for future images, until a new reference image is taken.</DD>
<DT>DEFO</DT><DD>Reconstruct the points in the current image, perform surface reconstruction 
relative to the last reference image and display the results in the offline tab (\ref gui_offline_subsec).
</DD>
<DT>TEMP</DT><DD>Opens a temperature dialog and sets the chiller target temperature to the specified value.</DD>.
<SPAN style="color:#ff0000"><B>Not yet implemented!</B></span>
</DL>

Surface reconstruction (<i>DEFO</i>) can only be performed if an image has previously been processed
as a reference image. Apart from this, deleting an area will invalidate the reference. The spinbox in
the <i>Status</i> section indicates whether valid reference data is currently available and
surface reconstruction can be performed. Clicking the <tt>DEFO</tt> button while the spinbox is
not checked results in an error message.

\subsubsection gui_online_schedules_subsubsec  6.2.6 Schedules
For automated operation (e.g. repeated series or overnight measurements),
the defo GUI offers a programmable schedule. A schedule consists of
a list of entries, each comprising a command/action and - if required -
an associated value. All entries have a unique line number. Schedules can be
started, paused/resumed and cancelled using the respective buttons.
It is also possible to save a schedule or load it from a file, which is by
default placed under the <tt>schedules</tt> folder (extension: <tt>.defoschedule</tt>).
Possible actions are:

<DL>
<DT>SET</DT>
<DD>Loads an image from the camera and then pauses the schedule, thus allowing for
modifications of the setup, parameters, lighting, area definitions, etc. Manual refresh of the image is also possible.
Once everything is arranged properly, the schedule can be resumed using the PAUSE/RES button. <b>Value: none</b></DD>

<DT>REF</DT><DD>Capture an image from the camera, reconstruct the points and store the result
as the reference for future images, until a new reference image is taken. <b>Value: none</b> </DD>

<DT>DEFO</DT><DD>Capture an image from the camera and reconstruct the surface relative to the
most recently taken reference image. <b>Value: none</b></DD>

<DT>FILE_SET</DT><DD>Load an image from a file and then pause the schedule, enabling manual image refresh,
thus allowing for modifications of the setup, parameters, lighting, area definitions, etc. Once everything is arranged properly,
the schedule can be resumed using the PAUSE/RES button. <b>Value: image file path</b></DD>

<DT>FILE_REF</DT><DD>Load an image from file, reconstruct the points and store the result
as the reference for future images, until a new reference image is taken. <b>Value: image file path</b></DD>

<DT>FILE_DEFO</DT><DD>Load an image from file and reconstruct the surface relative to the
most recently taken reference image. <b>Value: image file path</b></DD>

<DT>TEMP</DT><DD>Set the chiller bath target temperature. <b>Value: target temperature in deg C.</b> <SPAN style="color:#ff0000"><B>Not yet implemented!</B></span> </DD>

<DT>SLEEP</DT><DD>Sleep for the specified amount of seconds, e.g. to allow the chiller to reach a
target temperature. When a schedule is paused during a <tt>SLEEP</tt> action, the sleep 
is also paused, i.e. after resuming the schedule the GUI will sit through the remaining number of seconds.
<b>Value: time in seconds.</b></DD>

<DT>GOTO</DT><DD>Jump to a schedule entry by line number (allowing program loops). <b>Value: schedule line number</b></DD>

<DT>END</DT><DD>Stop a schedule. Not explicitly required, but prevents error messages.<b>Value: none</b></DD>
</DL>

\subsubsection gui_online_output_subsubsec     6.2.7 Output files and folders


\subsection gui_cooling_subsec                 6.3 Cooling tab
\image html screenshot_gui_cooling.gif "The cooling tab"

\subsection gui_offline_subsec                 6.4 Offline tab
\image html screenshot_gui_offline.gif "The offline tab"

\subsection gui_advanced_subsec                6.5 Advanced tab
\image html screenshot_gui_advanced.gif "The advanced tab"

\subsection gui_viewer                         6.6 Defo Viewer


\section appendix_sec              Appendix

\subsection app_config_subsec        A.1 The configuration file

<PRE>
##################################
## DEFO main configuration file ##
##################################

# seeding thresholds [adc]
STEP1_THRESHOLD         40
STEP2_THRESHOLD         50
STEP3_THRESHOLD         80

# half width of seeding square [pixel]
HALF_SQUARE_WIDTH       10

# offsets for seeding square [pixel]
SQUARE_SEED_OFFSET_X    0
SQUARE_SEED_OFFSET_Y    0

# "blueishness" (blue/yellow adc ratio) above
# which a point is considered blue
BLUEISHNESS_THRESHOLD   2.1

# this is the spacing threshold
# above which two points are supposed to
# be on different splines [pixel]
SPACING_ESTIMATE        10

# points are put on the same spline along x (y)
# if their y (x) component differs from that
# of the previous one by not more than this: [pixel]
# (this option is deprecated)
SEARCH_PATH_HALF_WIDTH  10

# straight distance from grid to *center* of DUT [meter]
NOMINAL_GRID_DISTANCE   2.5179

# straight distance from ccd chip to *center* of DUT [meter]
NOMINAL_CAMERA_DISTANCE 2.5179

# angle under which the camera sees the DUT *center* [radian]
NOMINAL_VIEWING_ANGLE   0.119428926
##1.451367401

# pixel pitch on chip along X/Y [meter]
PIXEL_PITCH_X           0.00001
PIXEL_PITCH_Y           0.00001

# focal length of camera lens [meter]
LENS_FOCAL_LENGTH       0.12

# defines the verbosity of debug output (integer),
# 0 = silent, 3 = maximum verbosity
DEBUG_LEVEL             2

</PRE>
\subsection app_containers_subsec    A.2 Overview of the container classes
\subsection app_literature_subsec    A.3 Literature & links
- Diss. Stefan Koenig (RWTH): <a href="http://darwin.bth.rwth-aachen.de/opus3/volltexte/2003/642/">pdf</a>
- Note from H.-G. Moser (MPIM): <a href="http://wwwatlas.mppmu.mpg.de/~sct/welcomeaux/bowing/bowing_note.ps">ps</a>
*/
