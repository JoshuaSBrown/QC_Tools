#pragma once
#ifndef _CATNIP_QC_FUNCTIONS_HPP
#define _CATNIP_QC_FUNCTIONS_HPP

#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

//#include "matrix.hpp"
#include <eigen3/Eigen/Dense>

namespace catnip {

class TransferComplex {
  private:
    Eigen::MatrixXd mat_1_Coef;
    Eigen::MatrixXd mat_2_Coef;
    Eigen::MatrixXd mat_P_Coef;
    // Stores the number of Molecular orbitals
    // and the HOMO for both monomer 1 and 2
    std::pair<int, int> Orbs1;
    std::pair<int, int> Orbs2;

    /**
     * \brief basis function overlap matrix
     **/
    Eigen::MatrixXd mat_S;
    /**
     * \brief Molecular orbital overlap matrix
     **/
    Eigen::MatrixXd S_AB;

    /**
     * \brief Non orthonormalized Hamiltonian matrix
     **/
    Eigen::MatrixXd Hamiltonian;

    /**
     * \brief Orthonormalized Hamiltonian Matrix
     **/
    Eigen::MatrixXd Hamiltonian_eff;

    int HOMO_Orb_;
    int LUMO_Orb_;
    Eigen::VectorXd vec_P_OE;
    // If unscrambaling is required
    bool unscrambled_;
    bool counterPoise_;

    /*
     * \brief Determine if the provided orbital is valid
     **/
    bool orbitalValid_(const std::pair<std::string,int> & orbital) const;

    void calculate_transfer_integral_();

    void printTransferIntegral_(const std::pair<std::string,int> & Orbital1,
                                const std::pair<std::string,int> & Orbital2) const;
  public:
    TransferComplex(
        Eigen::MatrixXd mat1Coef, 
        Eigen::MatrixXd mat2Coef, 
        Eigen::MatrixXd matPCoef,
        std::pair<int, int> MOs1, 
        std::pair<int, int> MOs2,
        Eigen::MatrixXd matS, 
        Eigen::VectorXd vecPOE, 
        bool cp);

    void unscramble(
        const Eigen::MatrixXd& coord_1_mat, 
        const Eigen::MatrixXd& coord_2_mat,
        const Eigen::MatrixXd& coord_P_mat, 
        const std::vector<int>& basisP,
        const std::vector<int>& basis2);

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
    void printTransferIntegral(
        const std::map<std::string,std::string> & orbital_type,
        const std::map<std::string,int> & orbnum) const;

    /**
     * \brief Print All info in matrix form
     **/
    void printAll() const;
};

std::unordered_map<int, std::pair<double, std::string>> findRank(
    Eigen::VectorXd& Orb_E_Alpha, Eigen::VectorXd& Orb_E_Beta);

// Reorganize the dimer coefficients to match up with the monomers
Eigen::MatrixXd organize_P_Coef(std::vector<int> matchDimerA,
                       std::vector<int> matchDimerB,
                       std::vector<int> basisFuncA, std::vector<int> basisFuncB,
                       std::vector<int> basisFuncDimer, Eigen::MatrixXd dimerCoef);

// Unscramble the coefficients of the dimer matrix
// Assumes that the match vectors describe swaps looking at a single
// instance of the dimerCoef matrix
Eigen::MatrixXd unscramble_Coef(
    const std::vector<int>& matchDimerA,
    const std::vector<int>& matchDimerB,
    const std::vector<int>& basisFuncDimer,
    const Eigen::MatrixXd & dimerCoef);

Eigen::MatrixXd unscramble_Coef(
    const std::vector<int>& matchDimerA,
    const std::vector<int>& basisFuncDimer,
    const Eigen::MatrixXd & dimerCoef);

// Reorganize the dimer overlap matrix to line up with the monomer
// coefficients.
/**
 *
 * Note S must not be a reference because it is altered during the unscrambling
 **/
Eigen::MatrixXd unscramble_S(
    const std::vector<int>& matchDimerA,
    const std::vector<int>& matchDimerB,
    const std::vector<int>& basisFuncDimer,
    Eigen::MatrixXd S);

/**
 *
 * Note S must not be a reference because it is altered during the unscrambling
 **/
Eigen::MatrixXd unscramble_S(
    const std::vector<int>& matchDimerA,
    const std::vector<int>& basisFuncDimer,
    Eigen::MatrixXd S);

Eigen::MatrixXd unscramble_OE(std::vector<int> matchDimerA,
                      std::vector<int> matchDimerB,
                      std::vector<int> basisFuncDimer, Eigen::VectorXd OE);

}  // namespace catnip
#endif  // _CATNIP_QC_FUNCTIONS_HPP
