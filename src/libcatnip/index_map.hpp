

#include "transfer_complex.hpp"

#include "atom.hpp"
#include "atom_group.hpp"

namespace catnip {

  class IndexMap {
    public:

      IndexMap(AtomGroupContainer atom_groups);
      // The component rows and colums in their current locations in  
      // in the full system matrix of the complex
      std::map<int,std::vector<int>> row_col_init;

      // Map indicate where the rows and cols should go in the final state
      // Each int in the vector represents the locatin in the full system matrix
      std::map<int,std::vector<int>> row_col_final;

      // Searches the init row for the provided row and returns the group it is
      // in as well as its current index
      std::pair<int,int> findLocation(int row);
  };


}  // namespace catnip
