#pragma once
#ifndef _CATNIP_ORGANIZER_HPP
#define _CATNIP_ORGANIZER_HPP


#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

//#include "matrix.hpp"
#include <eigen3/Eigen/Dense>

namespace catnip {

  /**
   * @brief The purpose of this class is to be able to organize basis functions
   * with the correct atoms in a molecule
   */
  class Organizer {


    public:
      void unscramble(AtomGroupContainer cont);
  };
}  // namespace catnip
#endif  // _CATNIP_ORGANIZER_HPP
