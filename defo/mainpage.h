

/*! \mainpage The reconstruction algorithms of the defo package

\section overview_sec              1. General overview
\image html defoScheme.gif "General surface reconstruction principle"


\section imagereco_sec             2. Raw image reconstruction

Images from the camera are stored as QT <a
href="http://doc.qt.nokia.com/4.3/qimage.html">QImage</a> objects and
further embedded in objects of type DefoRawImage, fitting them with
additional functionality. The origin of an image is located in the top
left corner, with the x-axis running horizontally to the right and the
y-axis vertically to the bottom. All positions and distances on the
image are handled in units of pixels.

\image html imageCoordinates.gif "Image origin and coordinate definition"

\subsection point_subsec             2.1 Point finding and seeding

Class DefoRecoImage performs the recognizing of reflected points in
the image and the reconstruction of their positions relative to the
origin in units of pixels. Within DefoRecoImage::reconstruct,
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
   estimate. <b>TODO: REJECT IF NPIX=0</b>

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

\subsection redpoint_subsec          2.2 The red point

The red point in the middle of the grid is identified using the method
DefoRecoImage::calculateReddishness. For all pixels within a square of
half width <tt>HALF_SQUARE_WIDTH</tt> around the reconstructed point
position the red/cyan fraction is determined and averaged. A point is
considered "red" if this average exceeds
<tt>REDDISHNESS_THRESHOLD</tt>. This tag is stored in the DefoPoint
object and can be queried using DefoPoint::isRed. DefoRecoImage does
not put any requirements on the number of points it tags as red.


\section surfacereco_sec           3. Surface reconstruction
\subsection units_subsec             3.1 Units and geometry
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
-# Finally, the average is calculated using the number of values that have been processed <b>TODO</b>

For deformations not exceeding a few hundred micron, the resulting
value estimates the true spacing of two adjacent points reflected from
a regular grid with an accuracy of 1-2 pixels (determined by
simulation). 



\subsection indexing_subsec          3.3 Point indexing

For the surface reconstruction step, the reconstructed reflected
points must be indexed, i.e. they are assigned a pair of integers
which indicate their position along x and y relative to the red dot in
the middle (which has indices 0,0 by definition). Indexing later allows
to match a point in any reconstructed image to its equivalent in the
reference image, such that its spatial deviation can be calculated.
Indexing is performed by the method DefoRecoSurface::indexPoints.

\image html defo_indexing1.gif "Principle of point indexing along a single coordinate starting from the red point in the middle"

Origin of the indexing step is the red point in the middle which is
therefore required to be present in the setup and must have been
recognized in the image. In the first step, starting from the red
point, the position of the adjacent point along the positive y
coordinate is estimated using the previously determined average
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

In the second step, and again starting with the red dot, the procedure
is repeated along the negative y direction to index the remaining
half of the field of reconstructed points.

\subsection slope_subsec             3.4 Surface slope determination

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
\frac{\delta h_1}{\delta x} (x_1) = s_1, h_1 = 0
\f]
where the last equation describes the freedom of choice of an initial
slope and height at the position of the first point ("integration
constants"). These equations can be solved for a given set of points
to obtain the parameters A, B and C for all splines in a spline
set. This functionality is embedded in: DefoSplineSetX::doFitZ and
DefoSplineSetY::doFitZ for point groups along x (rows) and along y
(columns), respectively. The height h of a spline set can be
evaluated at any position within its range (i.e. between the first and
the last point along the coordinate) using the DefoSplineSetBase::eval
method. 

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

\image html xySplineCombinationPrinciple.gif "Principle of mounting and averaging x and y splines to obtain the full reconstructed surface"

\subsection offsetsandtilts_subsec   3.7 Removal of global offsets and tilts
\subsection surfaceevaluation_subsec 3.8 Surface evaluation

\section surfaceformat_sec         4. Format of surface objects

A reconstructed surface is stored in an object of class DefoSurface,
containing the reconstructed points (DefoPointFields) and splines
(DefoSplineField) separately for the reconstruction along x and y, respectively.

Using the serialization functionality of the boost library 
these objects are written to the output directories as byte streams
with file extension: ".defosurface". Future versions: these files
can be read and surfaces viewed offline using the DefoViewer (TBD).

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

# "reddishness" (red/cyan adc ratio) above
# which a point is considered red
REDDISHNESS_THRESHOLD   2.1

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
