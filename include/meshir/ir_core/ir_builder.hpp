#ifndef IRBUILDER_HPP 
#define IRBUILDER_HPP 
#include <span> 
#include "ir_core/ir_graph.hpp" 
#include "ir_core/ir_op_types.hpp"
#include "ir_core/ir_types.hpp"

namespace meshir_builder{
struct ir_builder{
  meshir_graph::ir_graph &builder_graph; 
  
  explicit                ir_builder       (meshir_graph::ir_graph &graph);
  
  void                    init_build_graph (); 
  meshir_types::tensor_id create_tensor    (const meshir_types::tensor_descriptor &tensor                                       ); 
  meshir_types::node_id   create_node      (meshir_ops::op_table op_type, std::span<const meshir_types::tensor_id> node_inputs, 
                                            std::span<const meshir_types::tensor_id> node_outputs                               );
  void                    attach_attrs     (const meshir_types::node_id           &node, const meshir_types::attr_entries &attr );

}; 

}; //namespace 
#endif 
