
#pragma once
#ifndef _CATNIP_ATOM_HPP
#define _CATNIP_ATOM_HPP

// Private local includes
#include "elements.hpp"

// Third party includes
#include <eigen3/Eigen/Dense>

// Standard includes
#include <bits/stdc++.h> 

namespace catnip {

  /**
   * @brief Atom Class
   *
   * This class is responsible for packaging basic atomic information including
   *    atomic coordinates
   *    element
   *    the number of basis functions
   */
  class Atom {
    private:
      // Element should not be able to change
      const Element element_;
      Eigen::Vector3d xyz_;
      int basis_func_count_ = -1;
    public: 
      Atom(Element element, double x, double y, double z) : element_(element), xyz_(x,y,z) {};

      Element getElement() const noexcept { return element_; }

      Eigen::Vector3d getPos() const noexcept { return xyz_; }

      void setBasisFuncCount(int count) {
        basis_func_count_ = count;
      }

      int getBasisFuncCount() const noexcept { return basis_func_count_; }

      bool operator==( const Atom & atm2 ) const {
        if ( element_ == atm2.element_ ){
          if ( xyz_.isApprox(atm2.xyz_)) {
            return true;
          }
        }
        return false;
      }

      bool operator!=( const Atom & atm2 ) const {
        return !(*this == atm2);
      }
  };

  double round(double N, double n);
}

namespace std {

  template <>
    struct hash<catnip::Atom>
    {
      std::size_t operator()(const catnip::Atom& atom) const
      {
        Eigen::Vector3d pos = atom.getPos();
        return ((((std::hash<catnip::Element>()(atom.getElement())
              ^ (hash<double>()(catnip::round(pos(0),4)) << 1)) >> 1)
          ^ (hash<double>()(catnip::round(pos(1),4)) << 1)) >> 1)
          ^ (hash<double>()(catnip::round(pos(2),4)) << 1);
      }
    };
}
#endif // _CATNIP_ATOM_HPP
