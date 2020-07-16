
#include "atom_group.hpp"
#include "atom.hpp"

namespace catnip {

  int AtomGroup::findIndex(std::shared_ptr<Atom> atom) {
    int index = 0;
    for ( std::shared_ptr<Atom> & atom_ : atoms_){
      if ( *atom_ == (*atom)){
        return index;
      }
      ++index;
    }
    return -1;
  }
}
