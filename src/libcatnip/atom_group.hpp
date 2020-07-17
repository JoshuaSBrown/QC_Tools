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
   * Unit - non of the atoms appear in any of the other atom groups
   *
   * Unknown - some combination, may or may not have overlapping components
   *
   * Unassigned - has not yet been specified
   */
  enum class GroupType {
    Component, 
    Complex,
    Unit,
    Unknown,
    Unassigned
  };

  /**
   * @brief Stores the name of the atom group, the group type and the atoms in
   * the group
   *
   */
  class AtomGroup {
    private: 
      GroupType type_ = GroupType::Unassigned;
      std::string name_;
      std::vector<std::shared_ptr<Atom>> atoms_;
    public:
      AtomGroup(std::string group_name) : name_(group_name) {};
    
      std::string getName() const noexcept { return name_; }

      size_t size() const noexcept { return atoms_.size(); }

      std::vector<std::shared_ptr<Atom>>::iterator 
        begin() { return atoms_.begin(); }

      std::vector<std::shared_ptr<Atom>>::const_iterator 
        begin() const { return atoms_.begin(); }

      std::vector<std::shared_ptr<Atom>>::iterator 
        end() { return atoms_.end(); }
 
      std::vector<std::shared_ptr<Atom>>::const_iterator 
        end() const { return atoms_.end(); }

      std::shared_ptr<Atom> at(size_t ind) const {
        return atoms_.at(ind);
      }

      void add(std::shared_ptr<Atom> atom){
        atoms_.push_back(atom);
      } 

      void add(std::vector<std::shared_ptr<Atom>> atoms){
        atoms_.insert(atoms_.end(),atoms.begin(),atoms.end());
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
      std::vector<int> find(std::shared_ptr<Atom> atom);
     

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
