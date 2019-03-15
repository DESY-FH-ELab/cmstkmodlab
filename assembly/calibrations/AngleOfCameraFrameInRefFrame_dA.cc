#include <iostream>
#include <cstdlib>
#include <cmath>

#include <TFile.h>
#include <TGraph.h>

/////////////////////////////////////////////////////////////////////////////////

// MEASUREMENTS
const double ANGDEG_PATREC = -2.2;

const int PR1_PIXEL_X = 929;
const int PR1_PIXEL_Y = 780;

const double PR1_MOTION_STAGE_POS_X = -46.5506;
const double PR1_MOTION_STAGE_POS_Y = -100.940;

const int PR2_PIXEL_X = 1711;
const int PR2_PIXEL_Y = 1253;

const double PR2_MOTION_STAGE_POS_X = 46.7431;
const double PR2_MOTION_STAGE_POS_Y = -151.628;

// SENSOR, IMAGE SPECIFICATIONS
const double ANGDEG_REFERC = -27.313500; // PS-s
//const double ANGDEG_REFERC = -26.706931; // PS-p

const double MM_PER_PIXEL = 0.0012;

const int IMAGE_PIXEL_WIDTH  = 2560;
const int IMAGE_PIXEL_HEIGHT = 1920;

// ANALYSIS PARAMETERS
const double ANGDEG_OFFSET_MIN  = -180.;
const double ANGDEG_OFFSET_MAX  = +180.;
const double ANGDEG_OFFSET_STEP = 0.005;

// OUTPUT OPTIONS
const std::string OUTPUT_TFILE_PATH = "AngleOfCameraFrameInRefFrame_dA.root";
const std::string TGRAPH_NAME       = "AngleOfCameraFrameInRefFrame_dA";

const int VERBOSE = 0;

/////////////////////////////////////////////////////////////////////////////////

class Point2D
{
 public:
  explicit Point2D(const double x, const double y): x_(x), y_(y) {}
  virtual ~Point2D() {}

  Point2D rotate_deg(const double) const;
  Point2D rotate_rad(const double) const;

  Point2D operator+(const Point2D&) const;

  void print() const;

  double x() const { return x_; }
  double y() const { return y_; }

 protected:
  double x_;
  double y_;
};

Point2D Point2D::rotate_deg(const double ang_deg) const
{
  const double ang_rad = ang_deg * (M_PI / 180.);

  return this->rotate_rad(ang_rad);
}

Point2D Point2D::rotate_rad(const double ang_rad) const
{
  const double x1 = (std::cos(ang_rad) * this->x()) - (std::sin(ang_rad) * this->y());
  const double y1 = (std::sin(ang_rad) * this->x()) + (std::cos(ang_rad) * this->y());

  return Point2D(x1, y1);
}

void Point2D::print() const
{
  std::cout << "(x=" << x_ << ", y=" << y_ << ")" << std::endl;

  return;
}

Point2D Point2D::operator+(const Point2D& p0) const
{
  return Point2D((this->x() + p0.x()), (this->y() + p0.y()));
}

Point2D from_pixels_to_distance_from_image_center(const int pixel_x, const int pixel_y)
{
  const double dx = double( pixel_x - (IMAGE_PIXEL_WIDTH  / 2.)) * MM_PER_PIXEL;
  const double dy = double(-pixel_y + (IMAGE_PIXEL_HEIGHT / 2.)) * MM_PER_PIXEL;

  return Point2D(dx, dy);
}

int main(int /* argc */, char** /* argv */)
{
  const Point2D d1 = from_pixels_to_distance_from_image_center(PR1_PIXEL_X, PR1_PIXEL_Y);
  const Point2D d2 = from_pixels_to_distance_from_image_center(PR2_PIXEL_X, PR2_PIXEL_Y);

  TFile ofile(OUTPUT_TFILE_PATH.c_str(), "recreate");

  TGraph g_offset;
  g_offset.SetName(TGRAPH_NAME.c_str());

  float best_angle = 999.;
  float best_angle_FOM = -1.0; // positive-def

  for(float angdeg_offset = ANGDEG_OFFSET_MIN; angdeg_offset <= ANGDEG_OFFSET_MAX; angdeg_offset += ANGDEG_OFFSET_STEP)
  {
    if(VERBOSE > 2){ std::cout << "* ANGLE_REFERC [deg] = " << ANGDEG_REFERC << std::endl; }
    if(VERBOSE > 2){ std::cout << "* ANGLE_PATREC [deg] = " << ANGDEG_PATREC << std::endl; }

    const Point2D d1_mm = d1.rotate_deg(angdeg_offset);
    const Point2D d2_mm = d2.rotate_deg(angdeg_offset);

    if(VERBOSE > 3){ std::cout << "* PatRec #1: center-to-bestmatch distance [mm] = "; d1_mm.print(); }
    if(VERBOSE > 3){ std::cout << "* PatRec #2: center-to-bestmatch distance [mm] = "; d2_mm.print(); }

    const Point2D PR1 = d1_mm + Point2D(PR1_MOTION_STAGE_POS_X, PR1_MOTION_STAGE_POS_Y);
    const Point2D PR2 = d2_mm + Point2D(PR2_MOTION_STAGE_POS_X, PR2_MOTION_STAGE_POS_Y);

    if(VERBOSE > 3){ std::cout << "* PatRec #1: best-match position [mm] = "; PR1.print(); }
    if(VERBOSE > 3){ std::cout << "* PatRec #2: best-match position [mm] = "; PR2.print(); }

    const double angdeg_measur = std::atan((PR2.y() - PR1.y()) / (PR2.x() - PR1.x())) * (180. / M_PI);

    if(VERBOSE > 3){ std::cout << "* measured angle from PR1-best-match to PR2-best-match [deg] = " << angdeg_measur << std::endl; }

    const double angdeg_offset_new = angdeg_measur - ANGDEG_REFERC - ANGDEG_PATREC - 90.0;

    if(VERBOSE > 2){ std::cout << "* camera-offset angle assumed [deg] = " << angdeg_offset     << std::endl; }
    if(VERBOSE > 2){ std::cout << "* camera-offset angle derived [deg] = " << angdeg_offset_new << std::endl; }

    const double angdeg_offset_diff = std::abs(angdeg_offset_new - angdeg_offset);

    if(VERBOSE > 2){ std::cout << "* camera-offset angle, abs-difference \"derived-assumed\" = " << angdeg_offset_diff << std::endl; }

    if(VERBOSE > 2){ std::cout << "-----------------------------------------------------------------------------------" << std::endl << std::endl; }

    if((angdeg_offset_diff < best_angle_FOM) || (best_angle_FOM < 0.))
    {
      best_angle     = angdeg_offset;
      best_angle_FOM = angdeg_offset_diff;
    }

    g_offset.SetPoint(g_offset.GetN(), angdeg_offset, angdeg_offset_diff);
  }

  if(VERBOSE > -1)
  {
    std::cout << "* camera frame angle in motion stage reference frame [deg] = " << best_angle << " (FOM=" << best_angle_FOM << ")" << std::endl;
  }

  ofile.cd();
  g_offset.Write();

  ofile.Close();

  return 0;
}
