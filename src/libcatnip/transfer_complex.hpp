#pragma once
#ifndef _CATNIP_TRANSFER_COMPLEX_HPP
#define _CATNIP_TRANSFER_COMPLEX_HPP

// Local private includes
#include "atom_system.hpp"

// Third party includes
#include <eigen3/Eigen/Dense>

// Standard includes
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace catnip {

class TransferComplex {

  public:

    class Parameters {
      public:

        // It actually makes more sense to place these in the atom groups
        //const std::vector<std::unique_ptr<Eigen::MatrixXd>> monomers_coefs;
        //const std::unique_ptr<Eigen::MatrixXd> complex_coefs;
        /// Location of each of the homo_orbitals in each of the monomers
        //const std::vector<int> homo_orbitals;
        //HOMO_A;
        //const int HOMO_B;
        /// Atomic Orbital overlap matrix
        
        // The Overlap matrix can be added simply when it is needed  
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

    void unscramble(AtomSystem atm_sys);

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
