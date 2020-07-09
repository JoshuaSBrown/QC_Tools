#pragma once
#ifndef _CATNIP_TRANSFER_COMPLEX_HPP
#define _CATNIP_TRANSFER_COMPLEX_HPP

#include "atom_group_container.hpp"

#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

//#include "matrix.hpp"
#include <eigen3/Eigen/Dense>

namespace catnip {

class TransferComplex {

  public:

    class Parameters {
      public:
        const std::vector<std::unique_ptr<Eigen::MatrixXd>> monomers_coefs;
        const std::unique_ptr<Eigen::MatrixXd> complex_coefs;
        /// Location of each of the homo_orbitals in each of the monomers
        const std::vector<int> homo_orbitals;
        //HOMO_A;
        //const int HOMO_B;
        /// Atomic Orbital overlap matrix
        const std::unique_ptr<Eigen::MatrixXd> S_AO;
        const std::unique_ptr<Eigen::VectorXd> complex_orbital_energies;
        /// Counter poise flag
        bool counter_poise = false;
        /// Calculate all couplings
        bool calculate_all = false;
    };

  private:

    Parameters * params_;

  public:

    TransferComplex(Parameters * params) : params_(params) {};

    void unscramble(AtomGroupContainer atom_groups);

    // Orbital type and a map of the corresponding number
    // E.g.
    // orbital type    orbital number
    // "mon1" "LUMO"    "mon1" -3
    // "mon2" "HOMO"    "mon2"  0
    //
    //    monomer1 LUMO-3
    //    monomer2 HOMO
    void calcJ();

    /**
     * \brief Print the transfer integral specified
     **/
//    void printTransferIntegral(
 //       const std::map<std::string,std::string> & orbital_type,
  //      const std::map<std::string,int> & orbnum) const;

    /**
     * \brief Print All info in matrix form
     **/
   // void printAll() const;

};

}  // namespace catnip
#endif  // _CATNIP_TRANSFER_COMPLEX_HPP
