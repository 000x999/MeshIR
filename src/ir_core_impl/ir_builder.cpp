#include "ir_core/ir_builder.hpp"

meshir_builder::ir_builder::ir_builder(meshir_graph::ir_graph &graph)
: builder_graph(graph)
{}

void meshir_builder::ir_builder::init_build_graph(){
  builder_graph.reset_graph();
}

meshir_types::tensor_id meshir_builder::ir_builder::create_tensor(const meshir_types::tensor_descriptor &tensor){
  const size_t required_tensor_size = builder_graph.get_graph_tensor_count() + 1; 
  builder_graph.ensure_tensor_capacity(required_tensor_size);

  const meshir_types::tensor_id current_tensor_id       = builder_graph.alloc_tensor_id();
  meshir_types::tensor_descriptor current_stored_tensor = tensor; 
  current_stored_tensor.id                              = current_tensor_id;
  builder_graph.write_tensor(current_tensor_id, current_stored_tensor); 
  
  return current_tensor_id; 
}

meshir_types::node_id meshir_builder::ir_builder::create_node(meshir_ops::op_table op_type, std::span<const meshir_types::tensor_id> node_inputs, 
                                                              std::span<const meshir_types::tensor_id> node_outputs){
  const size_t required_node_size = builder_graph.get_graph_node_count() + 1; 
  builder_graph.ensure_node_capacity(required_node_size);

  const meshir_types::node_id current_node_id = builder_graph.alloc_node_id();
  auto input_ptr                              = builder_graph.store_tensor_ids(node_inputs); 
  auto output_ptr                             = builder_graph.store_tensor_ids(node_outputs);
  
  meshir_ops::ir_node current_stored_node {
    .op_type          = op_type, 
    .node_inputs      = input_ptr, 
    .node_outputs     = output_ptr,
    .node_id          = current_node_id,
    .num_node_outputs = node_outputs.size(),
    .num_node_inputs  = node_inputs.size(), 
    .num_attrs        = 0,
    .attr_base_index  = builder_graph.get_graph_attr_count()
  };
  
  builder_graph.write_node(current_node_id, current_stored_node);
  return current_node_id; 
}

void meshir_builder::ir_builder::attach_attrs(const meshir_types::node_id &node, const meshir_types::attr_entries &attr){
  builder_graph.attach_attr(node, attr); 
}
