This holds a python script for the calibration of the orientation of the camera frame w.r.t. the motion stage (`AngleOfCameraFrameInRefFrame_dA`).

To be executed interactively as follows:
 * Place a sensor on the assembly platform
 * Perform a pattern recognition (PatRec) on the top left marker
 * Enter ...
   * the best-match angle of the PatRec
   * the camera pixel coordinates (x/y) of the best-match location
   * the current stage position (x/y)
 * Move to the top right marker and perform another PatRec and enter the values as above:
   * the best-match angle of the PatRec
   * the camera pixel coordinates (x/y) of the best-match location
   * the current stage position (x/y)
 * The routine will provide the angle of the camera w.r.t. the motion stage, which can be applied in the assembly parameter files as `AngleOfCameraFrameInRefFrame_dA`
