#include "ir_core/ir_graph.hpp"
#include "ir_core/ir_types.hpp"
#include <cassert>
#include <cstring>
#include <climits>
#include <memory>

meshir_graph::ir_graph::ir_graph(size_t arena_size)
:
  graph_arena           (arena_size), 
  graph_nodes           (nullptr), 
  graph_tensors         (nullptr), 
  graph_node_attrs      (nullptr),
  graph_arena_size      (arena_size), 
  num_graph_nodes       (0), 
  num_graph_node_attrs  (0), 
  num_graph_tensors     (0),
  node_capacity         (0),
  tensor_capacity       (0), 
  attr_capacity         (0), 
  next_attr_key_id      (0) 
{}

meshir_graph::ir_graph::~ir_graph(){ reset_graph(); }

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
  // Returns immediately if required_size <= current_capacity 
  // Otherwise, we need to choose a new capacity  
  // Allocate the new storage from arena for that table 
  // Copy/move existing elements into new storage 
  // Update pointers + capacity 
  
  assert(required_size >= num_graph_nodes);  
  if(required_size <= node_capacity){
    // noop
    return; 
  }
  size_t old_capacity      = node_capacity; 
  size_t policy_capacity   = std::max<size_t>(16           , old_capacity * 2); 
  size_t new_node_capacity = std::max<size_t>(required_size, policy_capacity );
  
  meshir_ops::ir_node *new_nodes = graph_arena.ir_alloc<meshir_ops::ir_node>(new_node_capacity);
  
  if(graph_nodes != nullptr){
    // Copy everything from graph_nodes into a container with a larger size; 
    memcpy(new_nodes, graph_nodes, num_graph_nodes * sizeof(meshir_ops::ir_node)); 
  }
  node_capacity = new_node_capacity; 
  // Point graph_nodes to the new new_nodes mem block 
  graph_nodes   = new_nodes;
}

void meshir_graph::ir_graph::ensure_tensor_capacity(size_t required_size){
  assert(required_size >= num_graph_tensors);
  if(required_size <= tensor_capacity){
    return; 
  }

  size_t old_capacity        = tensor_capacity; 
  size_t policy_capacity     = std::max<size_t>(16           , old_capacity * 2); 
  size_t new_tensor_capacity = std::max<size_t>(required_size, policy_capacity ); 
  
  meshir_types::tensor_descriptor *new_tensors = graph_arena.ir_alloc<meshir_types::tensor_descriptor>(new_tensor_capacity);  
  if(graph_tensors != nullptr && num_graph_tensors > 0){
    for(size_t i = 0; i < num_graph_tensors; ++i){
      // Element wise movement ops on tensor_descriptors
      new (&new_tensors[i]) meshir_types::tensor_descriptor(std::move(graph_tensors[i]));   
    }
  }

  tensor_capacity = new_tensor_capacity; 
  graph_tensors   = new_tensors; 
}

void meshir_graph::ir_graph::ensure_attr_capacity(size_t required_size){
  assert(required_size >= num_graph_node_attrs); 
  if(required_size <= attr_capacity){
    return; 
  }

  size_t old_capacity      = attr_capacity; 
  size_t policy_capacity   = std::max<size_t>(16           , old_capacity * 2); 
  size_t new_attr_capacity = std::max<size_t>(required_size, policy_capacity );
  
  meshir_types::attr_entries *new_attrs = graph_arena.ir_alloc<meshir_types::attr_entries>(new_attr_capacity); 

  if(graph_node_attrs != nullptr && num_graph_node_attrs > 0){
    for(size_t i = 0; i < num_graph_node_attrs; ++i){
      new (&new_attrs[i]) meshir_types::attr_entries(std::move(graph_node_attrs[i])); 
    }
  }

  attr_capacity = new_attr_capacity; 
  graph_node_attrs = new_attrs; 
}

const meshir_types::tensor_id* meshir_graph::ir_graph::store_tensor_ids(std::span<const meshir_types::tensor_id> ids){
  if(ids.empty()){
    return nullptr; 
  }
  auto *storage = graph_arena.ir_alloc<meshir_types::tensor_id>(ids.size());
  std::memcpy(storage, ids.data(), ids.size() * sizeof(meshir_types::tensor_id));
  return storage; 
}

void meshir_graph::ir_graph::attach_attr(const meshir_types::node_id &node_id, const meshir_types::attr_entries &attr){
  assert(node_id.id < num_graph_nodes); 
  auto &current_node = graph_nodes[node_id.id];
  
  const size_t begin_index = current_node.attr_base_index; 
  const size_t end_index   = begin_index + current_node.num_attrs; 

  assert(begin_index <= num_graph_node_attrs); 
  assert(end_index   <= num_graph_node_attrs); 

  for(size_t i = begin_index; i < end_index; ++i){
    if(graph_node_attrs[i].attr_key == attr.attr_key){
      graph_node_attrs[i].value = attr.value; 
      return; 
    }
  }
  
  ensure_attr_capacity(num_graph_node_attrs + 1); 
  if(current_node.num_attrs == 0){
    current_node.attr_base_index = num_graph_node_attrs; 
  }else{
    assert(current_node.attr_base_index + current_node.num_attrs == num_graph_node_attrs); 
  }
  new(&graph_node_attrs[num_graph_node_attrs]) meshir_types::attr_entries(attr); 
  ++num_graph_node_attrs; 
  ++current_node.num_attrs;
}

void meshir_graph::ir_graph::write_tensor(meshir_types::tensor_id id, const meshir_types::tensor_descriptor &tensor){
  assert(id.id < num_graph_tensors);
  new(&graph_tensors[id.id]) meshir_types::tensor_descriptor(tensor); 
}

void meshir_graph::ir_graph::write_node(meshir_types::node_id id, const meshir_ops::ir_node &node){
  assert(id.id < num_graph_nodes);
  new(&graph_nodes[id.id]) meshir_ops::ir_node(node); 
}

void meshir_graph::ir_graph::reset_graph(){
  if(graph_tensors != nullptr){
    std::destroy_n(graph_tensors, num_graph_tensors); 
  } 

  if(graph_node_attrs != nullptr){
    std::destroy_n(graph_node_attrs, num_graph_node_attrs); 
  } 

  graph_arena.ir_reset();
  attr_key_table.clear();

  next_attr_key_id     = 0;
  num_graph_nodes      = 0;
  num_graph_node_attrs = 0;
  num_graph_tensors    = 0;
  node_capacity        = 0; 
  tensor_capacity      = 0; 
  attr_capacity        = 0; 
}
