#include "ir_core/ir_graph.hpp"
#include "ir_core/ir_types.hpp"
#include <cassert> 

const meshir_types::type_attr* meshir_graph::ir_graph::find_attr
(meshir_types::node_id graph_node_id,meshir_types::attr_id graph_node_key) const noexcept{
  
  for(size_t i = 0; i < num_graph_nodes; ++i){
    if(graph_nodes[i].node_id == graph_node_id){
      auto base_index = graph_nodes[i].attr_base_index; 
      auto attr_num   = graph_nodes[i].num_attrs; 
      assert(base_index <= num_graph_node_attrs                                 );
      assert(attr_num   <= num_graph_node_attrs - graph_nodes[i].attr_base_index);

      for(size_t j = base_index; j < base_index + attr_num ;++j){
        if(graph_node_attrs[j].attr_key == graph_node_key){
          return &graph_node_attrs[j].value; 
        }
      }
      return nullptr;
    }
  }  
  return nullptr;  
}

meshir_types::type_attr* meshir_graph::ir_graph::find_attr
(meshir_types::node_id graph_node_id,meshir_types::attr_id graph_node_key) noexcept{

  for(size_t i = 0; i < num_graph_nodes; ++i){
    if(graph_nodes[i].node_id == graph_node_id){
      auto base_index = graph_nodes[i].attr_base_index; 
      auto attr_num   = graph_nodes[i].num_attrs; 
      assert(base_index <= num_graph_node_attrs                                 );
      assert(attr_num   <= num_graph_node_attrs - graph_nodes[i].attr_base_index);

      for(size_t j = base_index; j < base_index + attr_num; ++j){
        if(graph_node_attrs[j].attr_key == graph_node_key){
          return &graph_node_attrs[j].value; 
        }
      }
      return nullptr;
    }
  }  
  return nullptr;
}

meshir_types::tensor_id meshir_graph::ir_graph::alloc_tensor_id(){

}
meshir_types::node_id meshir_graph::ir_graph::alloc_node_id(){

}
meshir_types::attr_id meshir_graph::ir_graph::alloc_attr_id(){

}
meshir_types::graph_id meshir_graph::ir_graph::alloc_graph_id(){

}

size_t meshir_graph::ir_graph::get_graph_node_count(){}
size_t meshir_graph::ir_graph::get_graph_attr_count(){}
size_t meshir_graph::ir_graph::get_graph_tensor_count(){}
size_t meshir_graph::ir_graph::get_graph_arena_capacity_bytes(){}

void meshir_graph::ir_graph::ensure_node_capacity(size_t required_size){}
void meshir_graph::ir_graph::ensure_tensor_capacity(size_t required_size){}
void meshir_graph::ir_graph::ensure_attr_capacity(size_t required_size){}
