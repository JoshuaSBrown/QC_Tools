#pragma once
#ifndef _CATNIP_ATOM_GROUP_HPP
#define _CATNIP_ATOM_GROUP_HPP

#include <exception>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

#include <eigen3/Eigen/Dense>

namespace catnip {

  class Atom;
  
  /**
   * @brief Atom groups can either be a component or a complex
   */
  enum class GroupType {
    PotentialComponent, // The atoms in all the components == the num atoms in the complex
    Component, // All atoms in the components have a matching atom in the complex
    Complex,
    Unknown
  };

  enum class FileType {
    pun,
    log
  };

  enum class MatrixType {
    Overlap,
    Coefficients,
    MolecularOrbitalEnergies
  };
  /**
   * @brief Stores the name of the atom group, the group type and the atoms in
   * the group
   *
   */
  class AtomGroup {
    private: 
      GroupType type_ = GroupType::Unknown;
      std::string name_;
      std::vector<std::shared_ptr<Atom>> atoms_;
      std::unordered_map<FileType,std::string> files_;
      std::unordered_map<MatrixType,std::unique_ptr<Eigen::MatrixXd>> matrices_;
    public:
      AtomGroup(std::string group_name) : name_(group_name) {};

      void addFile(FileType type, std::string file_name) {
        files_[type] = file_name;
      }

      size_t size() const noexcept { return atoms_.size(); }

      std::vector<std::shared_ptr<Atom>>::iterator 
        begin() { return atoms_.begin(); }

      std::vector<std::shared_ptr<Atom>>::iterator 
        end() { return atoms_.end(); }
 
      std::shared_ptr<Atom> at(size_t ind) const {
        return atoms_.at(ind);
      }

      void add(std::shared_ptr<Atom> atom){
        atoms_.push_back(atom);
      } 

      void add(std::vector<std::shared_ptr<Atom>> atoms){
        atoms_.insert(atoms_.end(),atoms.begin(),atoms.end());
      } 

      size_t numberOfAtoms() const noexcept {
        return atoms_.size();
      }

      void setType(const GroupType & type) noexcept {
        type_ = type;
      }
      GroupType getType() const noexcept { return type_; }

      /**
       * @brief Find the index of the atom in the group if it exists and return
       * the index
       *
       * @param atom
       *
       * @return index if found -1 if not found 
       */
      int findIndex(std::shared_ptr<Atom> atom);
  };


}  // namespace catnip
#endif  // _CATNIP_ATOM_GROUP_HPP
