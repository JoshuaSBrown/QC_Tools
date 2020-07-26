
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "index_map.hpp"

#include "atom_group_container.hpp"

using namespace catnip;

TEST_CASE("Index Map","[unit]") {

  AtomGroupContainer atom_cont;
  IndexMap ind_map(atom_cont);

}
