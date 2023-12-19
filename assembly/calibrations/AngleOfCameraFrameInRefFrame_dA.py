import math
import numpy as np
import matplotlib.pyplot as plt

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
    px = (pixel_x - (n_pixel_x / 2.)) * mm_per_pixel
    py = (-pixel_y + (n_pixel_y / 2.)) * mm_per_pixel

    return Point2D(px, py)


if __name__ == "__main__":

    pr1_ang_deg = float(
        input("Input best-match angle (in deg) for PatRec on TL marker: "))
    pr1_x_pixel = int(input(
        "Input x coordinate result (in pixels) of PatRec on TL marker (see printout on terminal): "))
    pr1_y_pixel = int(input(
        "Input y coordinate result (in pixels) of PatRec on TL marker (see printout on terminal): "))
    pr1_x_stage = float(input(
        "Input absolute x position (in mm) of motion stage for PatRec on TL marker: "))
    pr1_y_stage = float(input(
        "Input absolute y position (in mm) of motion stage for PatRec on TL marker: "))

    pr2_ang_deg = float(
        input("Input best-match angle (in deg) for PatRec on TR marker: "))
    pr2_x_pixel = int(input(
        "Input x coordinate result (in pixels) of PatRec on TR marker (see printout on terminal): "))
    pr2_y_pixel = int(input(
        "Input y coordinate result (in pixels) of PatRec on TR marker (see printout on terminal): "))
    pr2_x_stage = float(input(
        "Input absolute x position (in mm) of motion stage for PatRec on TR marker: "))
    pr2_y_stage = float(input(
        "Input absolute y position (in mm) of motion stage for PatRec on TR marker: "))

    pr_mean_ang_deg = (pr1_ang_deg + pr2_ang_deg) / 2.

    # Start calculations

    d1 = from_pixels_to_distance_from_image_center(pr1_x_pixel, pr1_y_pixel)
    d2 = from_pixels_to_distance_from_image_center(pr2_x_pixel, pr2_y_pixel)

    def calc_angle_offset_diff(angle_offset):
        d1_mm = d1.rotate_deg(angle_offset) + Point2D(pr1_x_stage, pr1_y_stage)
        d2_mm = d2.rotate_deg(angle_offset) + Point2D(pr2_x_stage, pr2_y_stage)

        ang_deg_meas = math.atan(
            (d2_mm.y - d1_mm.y) / (d2_mm.x - d1_mm.x)) * 180. / math.pi

        ang_deg_offset_new = ang_deg_meas - pr_mean_ang_deg - 90.

        ang_deg_offset_diff = abs(ang_deg_offset_new - angle_offset)

        return ang_deg_offset_diff

    offset_arr = np.arange(-180, 180, 0.005)
    angle_diff_arr = np.zeros(offset_arr.size)

    for i, offset in enumerate(offset_arr):
        angle_diff_arr[i] = calc_angle_offset_diff(offset)

    best_angle_FOM = np.min(angle_diff_arr)
    best_angle_idx = np.argmin(angle_diff_arr)
    best_angle = offset_arr[best_angle_idx]

    print("  Camera frame angle in motion stage ref frame [deg] = " + str(
        best_angle) + "  (FOM= " + str(best_angle_FOM) + ")")

    # Plotting result

    fig, axs = plt.subplots(1, 2, figsize=(12, 6))
    axs[0].plot(offset_arr, angle_diff_arr, '.c')
    axs[0].set_xlabel("Offset angle [deg]")
    axs[0].set_ylabel("Computed difference from target [deg]")

    axs[1].plot(offset_arr, angle_diff_arr, '.c')
    axs[1].set(xlim=(best_angle-0.1, best_angle+0.1), ylim=(best_angle_FOM,
               np.max([angle_diff_arr[best_angle_idx-20], angle_diff_arr[best_angle_idx+20]])))
    axs[1].set_xlabel("Offset angle [deg]")
    axs[1].set_ylabel("Computed difference from target [deg]")

    plt.ion()
    plt.show()

    input("\nPress enter to close")
