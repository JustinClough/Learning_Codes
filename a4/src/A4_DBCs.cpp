#include "A4_DBCs.hpp"
#include "A4_Disc.hpp"
#include "A4_LinAlg.hpp"

#include <Teuchos_ParameterList.hpp>
#include <string>

namespace A4 {

using Teuchos::getValue;
using Teuchos::Array;

void apply_dbcs(
    Disc* d,
    LinAlg* la,
    ParameterList const& p)
{
  auto sublist = p.sublist( "dirichlet bcs");
  std::vector<apf::Node> nodes;
  auto K = la->owned->K;
  auto f = la->owned->F->get1dViewNonConst();
  Array<LO> col_indices;
  Array<ST> col_entries;
  ST diag_entry = 1.0;
  size_t num_entries;

  for( auto it = sublist.begin(); it != sublist.end(); ++it)
  {
    auto entry = sublist.entry(it);
    // Format is: {0, xmin, 0.0} (spatial_dof, node_set_name, value)
    auto a = getValue<Array<std::string> >(entry);
    int s_dof = std::stoi(a[0]);
    std::string nodeSet = a[1];
    double value = std::stod(a[2]);

    nodes = d->get_nodes( nodeSet);
    for( int i = 0; i<(int)nodes.size(); ++i)
    {
      auto node = nodes[i];
      auto row = d->get_owned_lid( node, s_dof);

      num_entries = K->getNumEntriesInLocalRow(row);
      col_indices.resize(num_entries);
      col_entries.resize(num_entries);
      K->getLocalRowCopy(row, col_indices(), col_entries(), num_entries);

      for (size_t c = 0; c < num_entries; ++c)
      {
        if( !(col_indices[c] == row))
        {
          col_entries[c] = 0.0;
        }
        else
        {
          diag_entry = col_entries[c];
        }
      }
      K->replaceLocalValues(row, col_indices(), col_entries());
      f[row] = diag_entry*value;
    }
  }
  return;
}

}
