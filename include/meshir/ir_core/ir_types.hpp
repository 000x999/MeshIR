#ifndef IRTYPES_HPP
#define IRTYPES_HPP 
#include <string> 
#include <variant> 
#include <vector>
#include <cstddef>

namespace meshir_types{
struct attr_id{ 
  size_t id;
  bool operator==(const meshir_types::attr_id &id_in) const{ 
    return (this->id == id_in.id);
  }
};
struct node_id{
  size_t id; 
  bool operator==(const meshir_types::node_id &id_in) const{ 
    return (this->id == id_in.id);
  }
}; 
struct tensor_id{
  size_t id; 
  bool operator==(const meshir_types::tensor_id &id_in) const{ 
    return (this->id == id_in.id);
  }
}; 
struct graph_id{
  size_t id;
  bool operator==(const meshir_types::graph_id &id_in) const{ 
    return (this->id == id_in.id);
  }
}; 

struct int_attr    { int                value; }; 
struct float_attr  { float              value; };
struct bool_attr   { bool               value; }; 
struct string_attr { std::string        value; };
struct ilist_attr  { std::vector<int>   value; };
struct flist_attr  { std::vector<float> value; };

using type_attr = std::variant<int_attr, float_attr, bool_attr, string_attr, ilist_attr, flist_attr>;

struct attr_entries{
  attr_id   attr_key; 
  type_attr value; 
};


struct tensor_descriptor{
  enum tensor_domain { VERTEX_TENSOR, FACE_TENSOR, EDGE_TENSOR, SCALAR_TENSOR }; 
  
  tensor_domain            domain;
  size_t                   tensor_dim; // Can be V, F, or E :: Will be resolved from mesh metadata;  
  size_t                   tensor_channels; 
  meshir_types::tensor_id  id; 
  std::string              tensor_name;
};



}; //namespace  
#endif 
