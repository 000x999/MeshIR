#ifndef IRGRAPH_HPP 
#define IRGRAPH_HPP 
#include <cstddef>
#include "ir_core/ir_types.hpp"
#include "ir_core/ir_op_types.hpp"
#include "ir_alloc_core/ir_alloc.hpp"

namespace meshir_graph{
struct ir_graph{
  meshir_types::graph_id           id;
  memory::ir_arena                 graph_arena; 
  meshir_ops::ir_node             *graph_nodes;
  meshir_types::tensor_descriptor *graph_tensors;
  meshir_types::attr_entries      *graph_node_attrs;
  
  size_t                           num_graph_nodes;
  size_t                           num_graph_node_attrs;
  size_t                           num_graph_tensors;
  size_t                           node_capacity; 
  size_t                           tensor_capacity; 
  size_t                           attr_capacity; 
  
  
 /*
  * @brief Indexes graph node attributes from id's and keys.
  * @param graph_node_id current node id.
  * @param graph_node_key current node's key. 
  * @return Returns key value attribute pair. 
  */
  
  const meshir_types::type_attr *find_attr                        (meshir_types::node_id graph_node_id, meshir_types::attr_id graph_node_key) const noexcept;
  meshir_types::type_attr       *find_attr                        (meshir_types::node_id graph_node_id, meshir_types::attr_id graph_node_key)       noexcept;
 
  meshir_types::tensor_id        alloc_tensor_id                  (); 
  meshir_types::node_id          alloc_node_id                    (); 
  meshir_types::attr_id          alloc_attr_id                    ();  
  meshir_types::graph_id         alloc_graph_id                   (); 

  size_t                         get_graph_node_count             ();
  size_t                         get_graph_attr_count             (); 
  size_t                         get_graph_tensor_count           (); 
  size_t                         get_graph_arena_capacity_bytes   ();

  void                           ensure_node_capacity             (size_t required_size); 
  void                           ensure_tensor_capacity           (size_t required_size); 
  void                           ensure_attr_capacity             (size_t required_size); 
}; 

};// namespace 
#endif  
