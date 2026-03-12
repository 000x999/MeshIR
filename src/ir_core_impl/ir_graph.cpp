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
  assert(num_graph_tensors < tensor_capacity);   
  // return a new stable tensor handle
  // id = num_graph_tensors (before incrementing)
  // increment tensor count 
  // enforce num_graph_tensors <= tensor_capacity
  meshir_types::tensor_id current_tensor_id;
  current_tensor_id.id = num_graph_tensors;  
  num_graph_tensors++; 
  return current_tensor_id; 
}

meshir_types::node_id meshir_graph::ir_graph::alloc_node_id(){
  assert(num_graph_nodes < node_capacity); 
  meshir_types::node_id current_node_id; 
  current_node_id.id = num_graph_nodes; 
  num_graph_nodes++; 
  return current_node_id; 
}

meshir_types::attr_id meshir_graph::ir_graph::alloc_attr_key_id(const std::string &name){
  if(attr_key_table.contains(name)){
    return attr_key_table[name]; 
  }

  meshir_types::attr_id current_key_id;
  current_key_id.id    = next_attr_key_id;
  next_attr_key_id++; 
  attr_key_table[name] = current_key_id;

  return current_key_id; 
} 

meshir_types::graph_id meshir_graph::ir_graph::alloc_graph_id(){
  static size_t next_graph_id = 1; 
  if(id.id != 0){
    return id; 
  }
  id.id = next_graph_id;
  next_graph_id++; 
  return id; 
}

size_t meshir_graph::ir_graph::get_graph_node_count(){
  return num_graph_nodes;
}

size_t meshir_graph::ir_graph::get_graph_attr_count(){
  return num_graph_node_attrs;
}

size_t meshir_graph::ir_graph::get_graph_tensor_count(){
  return num_graph_tensors;
}

size_t meshir_graph::ir_graph::get_graph_arena_capacity_bytes(){
  auto tensor_size = tensor_capacity * sizeof(meshir_types::tensor_descriptor);
  auto node_size   = node_capacity   * sizeof(meshir_ops::ir_node);
  auto attr_size   = attr_capacity   * sizeof(meshir_types::attr_entries); 
  
  auto total_size  = tensor_size + node_size + attr_size; 
  return total_size; 
}

void meshir_graph::ir_graph::ensure_node_capacity(size_t required_size){
}

void meshir_graph::ir_graph::ensure_tensor_capacity(size_t required_size){
}

void meshir_graph::ir_graph::ensure_attr_capacity(size_t required_size){
}

