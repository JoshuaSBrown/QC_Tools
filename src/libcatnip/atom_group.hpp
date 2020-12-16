#pragma once
#ifndef CATNIP_ATOM_GROUP_HPP
#define CATNIP_ATOM_GROUP_HPP

// Third party libary includes
#include <eigen3/Eigen/Dense>

// Standard includes
#include <exception>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

namespace catnip {

  class Atom;
  
  /**
   * @brief Atom groups can either be a component or a complex
   *
   * Component - all atoms in the atom group have a matching atom in the 
   * complex, but do not share atoms between other components
   *
   * Complex - all atoms in this atom group have a matching atom in a component
   *
   * Island - non of the atoms appear in any of the other atom groups
   *
   * Unknown - some combination, may or may not have overlapping components
   *
   * Unassigned - has not yet been specified
   */
  enum class GroupType {
    Component, 
    Complex,
    Island,
    Unknown,
    Unassigned
  };

  enum class MatrixType {
    Overlap,
    Coefficients,
    MolecularOrbitalsEnergies
  };

  enum class FileType {
    pun,
    log
  };

  /**
   * @brief Atom group essentially packages anything associated with a file 
   *
   * Atom group is responsible for packaging the atoms associated with a 
   * group of atoms in one place. The group type can be assigned to the atom
   * group once it is known. Once the group type is known a bunch of other 
   * operations open up..
   *
   * Probably the core use of this class is to be able to compare atoms in the
   * group and ensure that they are equivalent. 
   *
   */
  class AtomGroup {
    private: 
      GroupType type_ = GroupType::Unassigned;
      std::string name_;
      // Files associated with this atom_group
      std::vector<std::string> file_names_;
      // Matries associated with the atom group
      std::vector<std::shared_ptr<Atom>> atoms_;
      std::unordered_map<FileType,std::string> files_;
      std::unordered_map<MatrixType,std::unique_ptr<Eigen::MatrixXd>> matrices_;
    public:
      AtomGroup() = default;
      AtomGroup(std::string group_name) : name_(group_name) {};
   
      void addFile(const std::string file_name) { file_names_.push_back(file_name); }
      
      void addMatrix(const MatrixType type, std::unique_ptr<Eigen::MatrixXd> mat) { matrices_[type] = std::move(mat); }

      // Attempt to load overlap matrix hopefully a log file exists which can do
      // this
      void loadOverlapMatrix() {};
      void loadMOCoefMatrix() {};

      std::string getName() const noexcept { return name_; }

      void addFile(FileType type, std::string file_name) {
        files_[type] = file_name;
      }

      size_t size() const noexcept { return atoms_.size(); }

      std::vector<std::shared_ptr<Atom>>::iterator 
        begin() { return atoms_.begin(); }

      std::vector<std::shared_ptr<Atom>>::const_iterator 
        begin() const { return atoms_.begin(); }

      std::vector<std::shared_ptr<Atom>>::iterator 
        end() { return atoms_.end(); }
 
      std::vector<std::shared_ptr<Atom>>::const_iterator 
        end() const { return atoms_.end(); }

      std::shared_ptr<Atom> & at(size_t ind) {
        return atoms_.at(ind);
      }

      const std::shared_ptr<Atom> & at(size_t ind) const {
        return atoms_.at(ind);
      }

      void add(std::shared_ptr<Atom> atom){
        atoms_.push_back(atom);
      } 

      void add(std::vector<std::shared_ptr<Atom>> atoms){
        atoms_.insert(atoms_.end(),atoms.begin(),atoms.end());
      } 

      bool is(const GroupType & type) const noexcept {
        return type == type_; 
      }

      void setType(const GroupType & type) noexcept {
        type_ = type;
      }
      const GroupType & getType() const noexcept { return type_; }

      /**
       * @brief Find the index of the atom in the group if it exists and return
       * the index, compares atoms 
       *
       * @param atom
       *
       * @return indeces of atoms that match
       */
      std::vector<int> find(std::shared_ptr<Atom> atom) const;

      /**
       * @brief Find the index of the atom in the group if it shares the provided
       * position  
       *
       * @param pos
       *
       * @return indeces of atoms that match the position
       */
      std::vector<int> find(const Eigen::Vector3d pos) const;

      /**
       * @brief Finds the exact matching atom in memory by looking at the address
       *
       * @param atom
       *
       * @return 
       */
      std::vector<int> findStrict(std::shared_ptr<Atom> atom);
  };


}  // namespace catnip
#endif  // CATNIP_ATOM_GROUP_HPP
