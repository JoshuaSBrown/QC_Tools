
#pragma once
#ifndef _CATNIP_ATOM_HPP
#define _CATNIP_ATOM_HPP


#include "elements.hpp"

#include <eigen3/Eigen/Dense>

namespace catnip {

  class Atom {
    private:
      Element element_;
      Eigen::Vector3d xyz_;
      int basis_func_count_ = -1;
    public: 
      Atom(Element element, double x, double y, double z) : element_(element), xyz_(x,y,z) {};

      void setBasisFuncCount(int count) {
        basis_func_count_ = count;
      }

      int getBasisFuncCount() const noexcept { return basis_func_count_; }

      bool equal( Atom & atm2 ) const {
        if ( element_ == atm2.element_ ){
          if ( xyz_.isApprox(atm2.xyz_)) {
            return true;
          }
        }
        return false;
      }
  };


}

#endif // _CATNIP_ATOM_HPP
