#include "ir_alloc_core/ir_alloc.hpp"

size_t memory::ir_arena::align_forward(size_t ptr_address){
 if(ptr_address % ir_align == 0){
    return ptr_address;
  }else{
    return (ptr_address + (ir_align - (ptr_address % ir_align))); 
  }
}

memory::ir_arena::ir_arena(size_t ir_arena_size)
:
  ir_offset(0),
  ir_capacity(ir_arena_size),
  ir_buffer(new char[ir_arena_size])
{}

void   memory::ir_arena::ir_reset       () { ir_offset = 0;                    }
size_t memory::ir_arena::is_used        () { return ir_offset;                 }
size_t memory::ir_arena::is_remaining   () { return (ir_capacity - ir_offset); }

