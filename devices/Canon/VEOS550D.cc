#include "VEOS550D.h"

VEOS550D::VEOS550D(const char* port) {}
VEOS550D::~VEOS550D() {}

/// Returns the current index of given value in list, or -1 if not found.
int VEOS550D::indexOf(
    const VEOS550D::OptionList &list
  , const std::string &value
) {

  unsigned int i = list.size() - 1;
  // loop until found or smaller than 0
  while ( list[i] != value && i >= 0 )
    --i;

  return i;

}
