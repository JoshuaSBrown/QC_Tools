
#include "atom_group.hpp"
#include "atom.hpp"

// Standard includes
#include <vector>

namespace catnip {

  std::vector<int> AtomGroup::find(std::shared_ptr<Atom> atom) const {
    std::vector<int> indices;
    int index = 0;
    for ( const std::shared_ptr<Atom> & atom_ : atoms_){
      if ( *atom_ == (*atom)){
        indices.push_back(index);
      }
      ++index;
    }
    return indices;
  }

  std::vector<int> AtomGroup::findStrict(std::shared_ptr<Atom> atom) {
    std::vector<int> indices;
    int index = 0;
    for ( std::shared_ptr<Atom> & atom_ : atoms_){
      if ( atom_.get() == atom.get()){
        indices.push_back(index);
      }
      ++index;
    }
    return indices;
  }
}
