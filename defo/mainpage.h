

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
&nbsp;&nbsp;\ref relaycard_subsection <BR>

\ref gui_sec <BR>
&nbsp;&nbsp;\ref gui_overview_subsec <BR>
&nbsp;&nbsp;\ref gui_online_subsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_measurementid_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_areas_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_display_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_refresh_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_manual_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_zoomandhisto_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_schedules_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_online_output_subsubsec <BR>
&nbsp;&nbsp;\ref gui_cooling_subsec <BR>
&nbsp;&nbsp;\ref gui_offline_subsec <BR>
&nbsp;&nbsp;\ref gui_advanced_subsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_advanced_basefolder_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_advanced_commentfield_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_advanced_parameters_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_advanced_chiller_subsubsec <BR>
&nbsp;&nbsp;&nbsp;&nbsp;\ref gui_advanced_power_subsubsec <BR>
&nbsp;&nbsp;\ref gui_viewer <BR>

\ref appendix_sec <BR>
&nbsp;&nbsp;\ref app_todo_subsec <BR>
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
offsets with respect to each other. In order to level these offsets,
the spline sets must be linked to a common reference frame, i.e. the
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
After cross mounting (\ref surfacecreation_subsec) the individual spline
sets are levelled to a common reference frame which, however, still
has few arbitrary parameters which cannot be determined by the measurement.
Besides a global offset along the z coordinate there may be remaining
rotations around all three axes. 

Subsequent to a deformation measurement, method DefoRecoSurface::removeGlobalOffset
is called to remove a global offset along the z coordinate by altering the
constants C of the individual splines such that the lowest point in the
surface has a height of zero.

Global tilts are supposed to be removed by method
DefoRecoSurface::removeTilt which is currently present as a skeleton but <SPAN
style="color:#ff0000"><B>not yet implemented</B></SPAN>.

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
(<tt>devices/Julabo</tt>). The chiller is supposed to be connected via
the left RS232 connector of the serial adapter card as seen from the
back of cmstkmodlab2. In order to obtain access to the corresponding
device file <tt>/dev/ttyS5</tt> the user must be member of the
<tt>dialout</tt> system group (or whatever group the file belongs to).

\subsection multimeter_subsection		5.3 Multimeter
pt100 temperature probes are read out via the Keithley 2700 multimeter.
The corresponding functionality is embedded in the <tt>Keithley2700</tt>
class (<tt>devices/Keithley</tt>). <SPAN style="color:#ff0000"><B>Not yet implemented!</B></span>

\subsection relaycard_subsection		5.4 Power relay card
Power switching for the light panels, the calibration LED system and
the camera is handled by a Conrad C-Control 8-fold relay card which is
connected via USB (onboard USB <> RS232 adapter). Access to the
hardware is provided by the <tt>ConradController</tt> class
(<tt>devices/Conrad</tt>) through the <tt>/dev/ttyUSB*</tt> device
files. The user must be member of the <tt>uucp</tt> system group (or
whatever group they belong to) to obtain access to these files.  Once
the communication is enabled (either manually or at startup, see \ref
gui_advanced_power_subsubsec) the relay card will - if possible - 
be automatically detected and initialized.

The following table indicates the association of the relay channels
(0..8) to the power devices as it is implemented in the GUI. A sketch
of the panel geometry can be found in the example configuration file
(\ref app_config_subsec).

<table border="1">
  <tr><th>Relay channel</th><td>0</td><td>1</td><td>2</td><td>3</td><td>4</td><td>5</td><td>6</td><td>7</td></tr>
  <tr><th>Device</th><td>panel 1</td><td>panel 2</td><td>panel 3</td><td>panel 4</td><td>panel 5</td>
	<td>LED system</td><td>camera</td><td>--</td></tr>
</table>

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
overview_sec, and \ref app_config_subsec).

\subsection gui_online_subsec                  6.2 Online tab

\image html screenshot_gui_online.gif "The online tab"

\subsubsection gui_online_measurementid_subsubsec      6.2.1 Measurement ID
The measurement ID serves as a common identification string for a
group of deformation measurements (including reference or calibration
measurements). In particular, all output for a given measurement,
manual or schedule driven, will be stored in a subfolder named after
the currently valid measurement ID. The measurement ID is initially
set to the default string <tt>defomeasurement-ddmmyy-hhmmss</tt>
(including date and time, see figure), however an arbitrary string can
also be chosen using the <tt>Edit</tt> button (the <tt>Default</tt>
button restores the default string). A new measurement ID should be
specified whenever a schedule is started (or restarted). The buttons
are disabled during reconstruction or schedule execution.

\subsubsection gui_online_areas_subsubsec      6.2.2 Areas
The surface reconstruction process can be restricted to an area of
interest ("area"). When an area is defined, all image features outside
its boundaries are dismissed. Thus, an area is also useful for
omitting parts of an image which are affected by ambient light
effects, unwanted reflections or other nuisances. So far, only one
single rectangular area can be defined <B>which must involve a properly
reconstructible and recognizable blue point</B>.

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
<DT>TEMP</DT><DD>Opens a temperature dialog and sets the chiller target temperature to the specified value.
<SPAN style="color:#ff0000"><B>Not yet implemented!</B></span></DD>
<DT>CALIB</DT><DD>Switches off the light panels and powers the calibration LED system. Subsequently, an
image of the LEDs is taken and reconstructed, and the point positions are written to the
output folder in a plain text file. Afterwards, the power state of the light panels is
restored and the LEDs are shut down.</DD></DL>

Surface reconstruction (<i>DEFO</i>) can only be performed if an image has previously been processed
as a reference image. Apart from this, deleting an area will invalidate the reference. The spinbox in
the <i>Status</i> section indicates whether valid reference data is currently available and
surface reconstruction can be performed. Clicking the <tt>DEFO</tt> button while the spinbox is
not checked results in an error message.

Manual operation via these buttons is performed through calls to DefoMainWindow::handleAction
with the respective action item. The buttons are connected to the slots DefoMainWindow::manualFileRef, 
DefoMainWindow::manualFileDefo, DefoMainWindow::manualCalib and DefoMainWindow::manualTemp.

\subsubsection gui_online_zoomandhisto_subsubsec 6.2.6 Zoom and histogram buttons
The magnifying glass button allows for zooming in/out of the image currently displayed
on the online tab. <SPAN style="color:#ff0000"><B> Not yet implemented!</B></span>

When the histogram button is clicked, a window opens showing four
histograms of the pixel color RGB and grayscale distributions within
the selected area (or the whole image if no area is defined). The RGB
color space is statically defined according to the model described in
Qt's QColor documentation and thus typically does not match the
whitebalance of the image. A further click on the button closes the
window. Technically, the histogram button is connected to
DefoImageLabel::showHistogram.

\subsubsection gui_online_schedules_subsubsec  6.2.7 Schedules
For automated operation (e.g. repeated series or overnight measurements),
the defo GUI offers a programmable schedule. A schedule consists of
a list of entries, each comprising a command/action and - if required -
an associated value. All entries have a unique line number. There must not
be entries following an empty row as the latter will be interpreted
as the end of the schedule. For the same reason, an <tt>END</tt> command
at the schedule's end is not strictly required. Except for <tt>GOTO</tt>,
all schedule actions are handled by DefoMainWindow::handleAction.

Schedules can be started, paused/resumed and cancelled using the
respective buttons. The <tt>Clear</tt> button erases the schedule and
clears the table. Upon clicking the <tt>Verify</tt> button, the GUI
will check the schedule for inconsistencies such as syntax errors,
non-existing input files, invalid <tt>GOTO</tt> statements, etc. If no
error message is displayed, the schedule is considered executable.  It
is also possible to save a schedule or load it from a file, which is
by default placed under the <tt>schedules</tt> folder (extension:
<tt>.defoschedule</tt>). Schedule files can also be created or
modified manually with an editor; note that in a plain text file a
hyphen ("-") must be placed in the second column when an action takes
no value (example line in plain text schedule file: <B><tt>CALIB - </tt></B>).

Possible actions are the following, followed by the value in brackets ("-" if no value required):

<DL>
<DT><B><TT>SET -</TT></B></DT>
<DD>Loads an image from the camera and then pauses the schedule, thus allowing for
modifications of the setup, parameters, lighting, area definitions, etc. Manual refresh of the image is also possible.
Once everything is arranged properly, the schedule can be resumed using the PAUSE/RES button.</DD>

<DT><B><TT>REF -</TT></B></DT>
<DD>Capture an image from the camera, reconstruct the points and store the result
as the reference for future images, until a new reference image is taken.</DD>

<DT><B><TT>DEFO -</TT></B></DT><DD>Capture an image from the camera and reconstruct the surface relative to the
most recently taken reference image. </DD>

<DT><B><TT>FILE_SET &lt;image_file_path&gt;</TT></B></DT>
<DD>Load an image from a file and then pause the schedule, enabling manual image refresh,
thus allowing for modifications of the setup, parameters, lighting, area definitions, etc. Once everything is arranged properly,
the schedule can be resumed using the PAUSE/RES button.</DD>

<DT><B><TT>FILE_REF &lt;image_file_path&gt;</TT></B></DT><DD>Load an image from file, reconstruct the points and store the result
as the reference for future images, until a new reference image is taken.</DD>

<DT><B><TT>FILE_DEFO &lt;image_file_path&gt;</TT></B></DT><DD>Load an image from file and reconstruct the surface relative to the
most recently taken reference image.</DD>

<DT><B><TT>TEMP &lt;target_temperature_in_degC&gt;</TT></B></DT>
<DD>Set the chiller bath target temperature.<SPAN style="color:#ff0000"> <B>Not
yet implemented!</B></span> </DD>

<DT><B><TT>CALIB -</TT></B></DT>
<DD>Switches off the light panels and powers the calibration LED system. Subsequently, an
image of the LEDs is taken and reconstructed, and the point positions are written to the
output folder in a plain text file. Afterwards, the power state of the light panels is
restored and the LEDs are shut down.</DD>

<DT><B><TT>SLEEP &lt;time_in_seconds&gt;</TT></B></DT>
<DD>Sleep for the specified amount of seconds, e.g. to allow the chiller to reach a
target temperature. When a schedule is paused during a <tt>SLEEP</tt> action, the sleep 
is also paused, i.e. after resuming the schedule the GUI will sit through the remaining number of seconds.
</DD>

<DT><B><TT>GOTO &lt;schedule_line_number&gt;</TT></B></DT><DD>Jump to a schedule entry by line number, allowing program loops. Line numbers start at 1,
as displayed in the schedule table. Technically, this case is caught and handled by class DefoSchedule and
not by DefoMainWindow.</DD>

<DT><B><TT>END -</TT></B></DT>
<DD>Stop a schedule. Not explicitly required since an empty row has the same effect, but is more clear and prevents error messages.</DD>
</DL>

Technically, class DefoSchedule has the functionality for storing defo
schedules, schedule bookkeeping as well as delivering single action
items (DefoSchedule::scheduleItem) to the caller DefoMainWindow upon
request via the DefoSchedule::pollAction slot. The interpretation of
the action items and the invocation of the respective action is done
within DefoMainWindow::handleAction which comprises one C++ switch
case for each action item type. One exception is the GOTO action which
is caught and handled by DefoSchedule itself.

\subsubsection gui_online_output_subsubsec     6.2.8 Output files and folders
All output is stored under the <i>output base folder</i> which can be
specified on the advanced tab. Default is the subdirectory
<tt>out</tt> in the folder in which the application is started.

Once a manual measurement is initiated via the manual buttons, or when
a schedule is started, a <i> measurement group subfolder</i> with the
measurement ID (\ref gui_online_measurementid_subsubsec) as its name
is created under the base folder. Each action - manual or schedule
driven - that produces output during a measurement (SET, REF, DEFO,
FILE_SET, FILE_REF, FILE_DEFO, CALIB) will place its files in a within
that measurement folder in a separate subdirectory whose name is of
the form: <tt>action.ddmmyy-hhmmss</tt>. The following listing shows
an example of the directory structure:

<pre>
.
`-- defomeasurement-300412-161339
    |-- file_defo.300412-161349
    |   |-- defoimage_raw.jpg
    |   |-- points.txt
    |   `-- surface.defosurface
    |-- file_ref.300412-161345
    |   |-- points.txt
    |   `-- refimage_raw.jpg
    `-- file_set.300412-161344
        `-- setimage_raw.jpg
</pre>

The following output files can be found in the action subfolders:
<DL>
<DT><tt><B>*image_raw.jpg</B></tt></DT>
<DD>A copy of the raw input image that was used for reconstruction, or has been loaded
from camera/file during a SET / FILE_SET action.</DD>
<DT><tt><B>points.txt</B></tt></DT>
<DD>Text file listing all reconstructed points, their position (in pixels),
their indices and the reconstructed point color (white or blue).</DD>
<DT><tt><B>calibpoints.txt</B></tt></DT>
<DD>Text file for LED calibration listing all reconstructed points, their position (in pixels),
as well as their color composition (RGB).</DD>
<DT><tt><B>surface.defosurface</B></tt></DT>
<DD>A reconstructed surface file (see \ref surfaceformat_sec).</DD>
</DL>

\subsection gui_cooling_subsec                 6.3 Cooling tab
\image html screenshot_gui_cooling.gif "The cooling tab"

\subsection gui_offline_subsec                 6.4 Offline tab
\image html screenshot_gui_offline.gif "The offline tab"

\subsection gui_advanced_subsec                6.5 Advanced tab
\image html screenshot_gui_advanced.gif "The advanced tab"

\subsubsection gui_advanced_basefolder_subsubsec     6.5.1 Output base folder
This field allows to specify the subdirectory under which all output
is stored. Subfolders will automatically be created under the base folder
for each individual measurement (see \ref gui_online_measurementid_subsubsec).

\subsubsection gui_advanced_commentfield_subsubsec   6.5.2 Comment field
Manual input for text comments & log messages. All contents of this
text field are continously written to the file <tt>comment.txt</tt>
in the current <i>measurementId</i> directory under the <i>output base folder</i>.

\subsubsection gui_advanced_parameters_subsubsec   6.5.3 Algorithm parameters
The inputs in the  <i>Raw image reconstruction</i>, <i>Surface reconstruction</i> and
<i>Geometry</i> groups allow for setting the various configuration
parameters and are initialized from the configuration file (\ref app_config_subsec)
on startup. After modifying the parameters manually, the <tt>Apply</tt> button
must be clicked to make these changes persistent.

\subsubsection gui_advanced_chiller_subsubsec 6.5.4 Chiller
The chiller group comprises the communication switch as well as the
steering controls for the chiller control parameters. Checking the
<tt>Enabled</tt> checkbox in the <tt>Communication</tt> field will
trigger an attempt to initialize the chiller communication (see \ref
chiller_subsection) and - upon successful handshake - upload the
controller parameters as specified in the configuration file
(<tt>CHILLER_PARAMETER_XP</tt>, <tt>CHILLER_PARAMETER_TN</tt>,
</tt>CHILLER_PARAMETER_TV</tt>). When the configuration parameter
<tt>CHILLER_COMM_WHEN_START</tt> is set to true, the chiller will be
automatically initialized at program startup. Unchecking the checkbox
will terminate communication.

The controller parameters Xp, Tv, Tn can be modified at any time when
communication is enabled. The <tt>Apply</tt> buton must be hit
to make the changes persistent.

\subsubsection gui_advanced_power_subsubsec   6.5.5 Power options
The items in the power group serve as the control interface to the
Conrad relay card.  Software control can be enabled via the
<tt>Enabled</tt> check box which will trigger auto-detection of the
card through the <tt>ttyUSB</tt> device driver files in the
<tt>/dev</tt> directory. Once the communication with the device is
established, the steering buttons will be enabled, the
<tt>CommPort</tt> display shows the device file, and the current state
of the power channels is indicated by the color of the buttons. In
the above screenshot, light panels 1 and 3 as well as the LED system
are powered while panels 2, 4, 5 and the camera are switched off.

If the <tt>CONRAD_COMM_WHEN_START</tt> configuration option is set to
<tt>true</tt>, software control will be enabled at program startup. In
that case, the desired initial power states of the light panels, the
LED system and the camera can be specified via the
<tt>PANEL_STATE_WHEN_START</tt>, <tt>LEDS_POWER_WHEN_START</tt> and
<tt>CAMERA_POWER_WHEN_START</tt> configuration options. The respective
channels will then be powered immediately at startup.

Individual light panel power can be switched via the light panel
buttons 1..5 (yellow = on, black = off); the two buttons in the <tt>All
panels</tt> group power on/off all panels at the same time.  The LED
system and the camera can be toggled via the respective buttons (green
= on, gray = off).

\subsection gui_viewer                         6.6 Defo Viewer


\section appendix_sec              Appendix

\subsection app_todo_subsec        A.1 TODO
<UL>
<LI> put in a DefoTogglePushButton for chiller circulator on/off
<LI> advanced tab: load cfg from file
</UL>

\subsection app_config_subsec        A.2 The configuration file

All algorithms and the GUI rely on the presence of a configuration
file whose name and location are consistently hard-coded in
various places in calls to the DefoConfigReader constructor.
The default name is <tt>defo.cfg</tt> and the default location
is the current directory in which the application has started.

Parameters are specified in "name value" format where name and value
must be given on the same line and separated by at least one space or
tab. Lines starting with a hash mark (#) are considered as
comments and will be ignored.

<PRE>

##################################
## DEFO MAIN CONFIGURATION FILE ##
##################################


# seeding thresholds [adc]
STEP1_THRESHOLD		35
STEP2_THRESHOLD		37
STEP3_THRESHOLD		50

# half width of seeding square [pixel]
HALF_SQUARE_WIDTH	15

# "blueishness" (blue/yellow adc ratio)
# above which a point is considered blue
BLUEISHNESS_THRESHOLD	0.8

# this is the spacing threshold
# above which two points are supposed to
# be on different splines [pixel]
SPACING_ESTIMATE	25

# points are put on the same spline along x (y)
# if their y (x) component differs from that
# of the previous one by not more than this: [pixel]
SEARCH_PATH_HALF_WIDTH	8

# straight distance from grid to *center* of DUT [meter]
NOMINAL_GRID_DISTANCE	1.802

# straight distance from ccd chip to *center* of DUT [meter]
NOMINAL_CAMERA_DISTANCE 1.822

# angle under which the camera sees the DUT *center* [radian]
# camera: atan(145mm/1816mm) = 0.079677
# grid:   atan(123mm/1798mm) = 0.068303
NOMINAL_VIEWING_ANGLE	0.079677
##1.451367401

# pixel pitch on chip along X/Y [meter]
# Chip size from Canon EOS 550D Specs: CMOS-Sensor (22.3mm x 14.9mm)
# Maximum image size: 5184 x 3456
# PIXEL_PITCH_X = 22.3e-3m/5184 = 4.302e-06m
# PIXEL_PITCH_Y = 14.9e-3m/3456 = 4.311e-06m
PIXEL_PITCH_X		4.302e-06
PIXEL_PITCH_Y		4.311e-06

# focal length of camera lens [meter]
LENS_FOCAL_LENGTH	0.085

# camera options;
# for possible values see end of this file;
CAMERA_SHUTTERSPEED	1s
CAMERA_APERTURE		F8
CAMERA_ISO		100
CAMERA_WHITEBALANCE	yet_undefined...
# camera connection enable upon program startup? [true/false]
# this does not switch the camera power!
# (see relay_on_when_start)
CAMERA_ON_WHEN_START	false

# chiller parameters
CHILLER_PARAMETER_XP	0.1
CHILLER_PARAMETER_TN	3
CHILLER_PARAMETER_TV	0

# should the rs232 chiller communication
# be enabled at startup? [true/false]
CHILLER_COMM_WHEN_START	true

# should the conrad relay communication
# be enabled at startup? [true/false]
# active devices will be powered immediately
CONRAD_COMM_WHEN_START	false

# light panel power state on program startup;
# this is a 5-digit string consisting of [1/0]
# indicating the power state for panels 1-5, sketch below
# (only effective if CONRAD_COMM_WHEN_START = true)
#
# panel                 12345
#----------------------------
PANEL_STATE_WHEN_START	11111

# calibration LEDs powered at startup? [true/false]
# (only effective if CONRAD_COMM_WHEN_START = true)
LEDS_POWER_WHEN_START	false

# should the camera be powered
# at program startup? [true/false]
(only effective if CONRAD_COMM_WHEN_START = true)
CAMERA_POWER_WHEN_START	true

# defines the verbosity of debug output [0-3],
# 0 = silent, 3 = maximum verbosity
DEBUG_LEVEL		0


##################################
##     END OF CONFIGURATION     ##
##################################



# some aux information:

## current possible values for CAMERA_SHUTTERSPEED: 
# 30s 25s 20s 15s 13s 10s 8s 6s 5s 4s 3.2s 2.5s 2s 1.6s 1.3s 1s 0.8s
# 0.6s 0.5s 0.4s 0.3s 1/4s 1/5s 1/6s 1/8s 1/10s 1/13s 1/15s 1/20s
# 1/25s 1/30s 1/40s 1/50s 1/60s 1/80s 1/100s 1/125s 1/160s 1/200s
# 1/250s 1/320s 1/400s 1/500s 1/640s 1/800s 1/1000s 1/1250s 1/1600s
# 1/2000s 1/2500s 1/3200s 1/4000s
#
## current possible values for CAMERA_APERTURE
# F5.6 F6.3 F7.1 F8 F9 F10 F11 F13 F14 F16 F18 F20 F22 F25 F29 F32 F36
#
## current possible values for CAMERA_ISO
# AUTO 100 200 400 800 1600 3200 6400

# panel numbering for PANEL_STATE_WHEN_START, 
# as seen from underneath the panel:
# ---------------
# |   |     |   |
# |   |  1  |   |
# |   |     |   |
# | 3 |-----| 4 |
# |   |     |   |
# |   |  2  |   |
# |   |     |   |
# ---------------
# |      5      |
# ---------------

</PRE>

\subsection app_containers_subsec    A.2 Overview of the container classes
\subsection app_literature_subsec    A.3 Literature & links
- Diss. Stefan Koenig (RWTH): <a href="http://darwin.bth.rwth-aachen.de/opus3/volltexte/2003/642/">pdf</a>
- Note from H.-G. Moser (MPIM): <a href="http://wwwatlas.mppmu.mpg.de/~sct/welcomeaux/bowing/bowing_note.ps">ps</a>
*/
