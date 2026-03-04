#ifndef IROPTYPES_HPP 
#define IROPTYPES_HPP 
#include "ir_types.hpp"
#include <cstddef> 

namespace meshir_ops{
/**
* @brief Table for all supported op types.
*/
enum op_table{
  LAPLACIAN_APPLY, SHIFTED_LAPLACIAN_APPLY, GRAD      , L2NORM, 
  DIV            , AXPY                   , SCALE     , COPY, 
  DOT            , NORMALIZE              , CLAMP_VERT
};

struct ir_node{
  op_table                        op_type;
  const meshir_types::tensor_id  *node_inputs;
  size_t                          num_node_inputs; 
  const meshir_types::tensor_id  *node_outputs;
  size_t                          num_node_outputs; 
  meshir_types::node_id           node_id;
  size_t                          num_attrs; 
  size_t                          attr_base_index; 
};

}; //namespace

#endif 
