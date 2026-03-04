#include "ir_core/ir_graph.hpp"
#include "ir_core/ir_types.hpp"

const meshir_types::type_attr* meshir_graph::ir_graph::find_attr(meshir_types::node_id graph_node_id,meshir_types::attr_id graph_node_key) const noexcept{return nullptr;}

meshir_types::type_attr* meshir_graph::ir_graph::find_attr(meshir_types::node_id graph_node_id,meshir_types::attr_id graph_node_key) noexcept{return nullptr;}

meshir_types::tensor_id meshir_graph::ir_graph::alloc_tensor_id(){}
meshir_types::node_id meshir_graph::ir_graph::alloc_node_id(){}
meshir_types::attr_id meshir_graph::ir_graph::alloc_attr_id(){}
meshir_types::graph_id meshir_graph::ir_graph::alloc_graph_id(){}

size_t meshir_graph::ir_graph::get_graph_node_count(){}
size_t meshir_graph::ir_graph::get_graph_attr_count(){}
size_t meshir_graph::ir_graph::get_graph_tensor_count(){}
size_t meshir_graph::ir_graph::get_graph_arena_capacity_bytes(){}

void meshir_graph::ir_graph::ensure_node_capacity(size_t required_size){}
void meshir_graph::ir_graph::ensure_tensor_capacity(size_t required_size){}
void meshir_graph::ir_graph::ensure_attr_capacity(size_t required_size){}
