import math
import numpy as np

import ROOT

# constants
mm_per_pixel = 0.0012
n_pixel_x = 2560
n_pixel_y = 1920

class Point2D:

    def __init__(self, xin, yin):
        self.x = xin
        self.y = yin

    def rotate_rad(self, angle_rad):
        x1 = (math.cos(angle_rad) * self.x) - (math.sin(angle_rad) * self.y)
        y1 = (math.sin(angle_rad) * self.x) + (math.cos(angle_rad) * self.y)

        return Point2D(x1, y1)

    def rotate_deg(self, angle_deg):
        return self.rotate_rad(angle_deg * math.pi / 180.)


    def __str__(self):
        return "(x=" + self.x + ", y=" + self.y + ")"

    def __add__(self, p0):
        return Point2D(self.x + p0.x, self.y + p0.y)


def from_pixels_to_distance_from_image_center(pixel_x, pixel_y):
    px = ( pixel_x - (n_pixel_x / 2.)) * mm_per_pixel
    py = (-pixel_y + (n_pixel_y / 2.)) * mm_per_pixel

    return Point2D(px, py)


if __name__ == "__main__":

    
    pr1_ang_deg = float(input("Input best-match angle (in deg) for PatRec on TL marker: "))
    pr1_x_pixel = int(input("Input x coordinate result (in pixels) of PatRec on TL marker (see printout on terminal): "))
    pr1_y_pixel = int(input("Input y coordinate result (in pixels) of PatRec on TL marker (see printout on terminal): "))
    pr1_x_stage = float(input("Input absolute x position (in mm) of motion stage for PatRec on TL marker: "))
    pr1_y_stage = float(input("Input absolute y position (in mm) of motion stage for PatRec on TL marker: "))

    pr2_ang_deg = float(input("Input best-match angle (in deg) for PatRec on TR marker: "))
    pr2_x_pixel = int(input("Input x coordinate result (in pixels) of PatRec on TR marker (see printout on terminal): "))
    pr2_y_pixel = int(input("Input y coordinate result (in pixels) of PatRec on TR marker (see printout on terminal): "))
    pr2_x_stage = float(input("Input absolute x position (in mm) of motion stage for PatRec on TR marker: "))
    pr2_y_stage = float(input("Input absolute y position (in mm) of motion stage for PatRec on TR marker: "))

    pr_mean_ang_deg = (pr1_ang_deg + pr2_ang_deg) / 2.


    # Start calculations

    d1 = from_pixels_to_distance_from_image_center(pr1_x_pixel, pr1_y_pixel)
    d2 = from_pixels_to_distance_from_image_center(pr2_x_pixel, pr2_y_pixel)

    def calc_angle_offset_diff(angle_offset):
        d1_mm = d1.rotate_deg(angle_offset) + Point2D(pr1_x_stage, pr1_y_stage)
        d2_mm = d2.rotate_deg(angle_offset) + Point2D(pr2_x_stage, pr2_y_stage)

        ang_deg_meas = math.atan((d2_mm.y - d1_mm.y) / (d2_mm.x - d1_mm.x)) * 180. / math.pi

        ang_deg_offset_new = ang_deg_meas - pr_mean_ang_deg - 90.

        ang_deg_offset_diff = abs(ang_deg_offset_new - angle_offset)

        return ang_deg_offset_diff


    best_angle = 999.
    best_angle_FOM = -1.

    dict_offset_diff = {}

    diff_vs_offset = ROOT.TGraph()
    diff_vs_offset_central = ROOT.TGraph()
    
    for offset in np.arange(-180, 180, 0.005):
        new_diff = calc_angle_offset_diff(offset)

        dict_offset_diff[offset] = new_diff
        if new_diff < best_angle_FOM or best_angle_FOM < 0.:
            best_angle = offset
            best_angle_FOM = new_diff

    print("  Camera frame angle in motion stage ref frame [deg] = " + str(best_angle) + "  (FOM= " + str(best_angle_FOM) + ")")

    # Plotting result

    for offset in dict_offset_diff:
        diff_vs_offset.SetPoint(diff_vs_offset.GetN(), offset, dict_offset_diff[offset])
        if abs(offset - best_angle) < 0.1:
            diff_vs_offset_central.SetPoint(diff_vs_offset_central.GetN(), offset, dict_offset_diff[offset])

    diff_vs_offset.GetXaxis().SetTitle("Offset angle [deg]")
    diff_vs_offset.GetYaxis().SetTitle("Computed difference from target [deg]")
    diff_vs_offset_central.GetXaxis().SetTitle("Offset angle [deg]")
    diff_vs_offset_central.GetYaxis().SetTitle("Computed difference from target [deg]")

    c1 = ROOT.TCanvas("c1","diff vs. offset",1200,600)
    c1.Divide(2,1)
    
    c1.cd(1)
    diff_vs_offset.Draw("AP")
    
    c1.cd(2)
    diff_vs_offset_central.SetMarkerStyle(23)
    diff_vs_offset_central.Draw("AP")
    
    c1.cd(2).SetLogy()

    c1.Update()

    input("\nPress enter to close")

    
