#include <iostream>

#include "DefoScriptableCamera.h"

DefoScriptableCamera::DefoScriptableCamera(
    DefoCameraModel* cameraModel
  , QObject *parent
) :
    QObject(parent)
  , cameraModel_(cameraModel)
{

}

void DefoScriptableCamera::test1() {
  std::cout << "sc: test1" << std::endl;
}

void DefoScriptableCamera::test2(float value) {
  std::cout << "sc: test2 " << value << std::endl;
}

float DefoScriptableCamera::getTemp1() {
  return 12.3456;
}
