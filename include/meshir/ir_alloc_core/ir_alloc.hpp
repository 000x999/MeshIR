#ifndef IRALLOC_HPP
#define IRALLOC_HPP
#include <cstddef> 
#include <memory> 
#include <iostream>
#include <new>

namespace memory{
class ir_arena{
private: 
  static constexpr size_t ir_align  = 32; 
  size_t                  ir_offset;
  size_t                  ir_capacity;
  std::unique_ptr<char[]> ir_buffer;

  size_t align_forward ( size_t ptr_address );
  
public:
  explicit ir_arena    ( size_t nn_arena_size );

  ir_arena             ( const ir_arena& )  = delete;
  ir_arena operator=   ( const ir_arena& )  = delete;

  template<typename T> 
  T*     ir_alloc     ( size_t ir_alloc_size ){
    uintptr_t ir_addr      = reinterpret_cast<uintptr_t>(ir_buffer.get()); 
    uintptr_t current_addr = ir_addr + ir_offset; 

    size_t    padding      = 0;

    if(current_addr % ir_align != 0){
      padding = ir_align - (current_addr % ir_align); 
    }

    size_t bytes_needed = (ir_alloc_size * sizeof(T)); 
    if(ir_offset + padding + bytes_needed > ir_capacity){
      std::cout << "arena size: " << ir_capacity<< '\n';
      std::cout << "total needed size: " << ir_offset + padding + bytes_needed << '\n'; 
      throw std::bad_alloc(); 
    }
    T* ir_aligned_ptr = reinterpret_cast<T*>(ir_buffer.get() + ir_offset + padding); 
    ir_offset        += padding + bytes_needed;

    return ir_aligned_ptr; 
  }

  void   is_reset     ();
  size_t is_used      ();
  size_t is_remaining ();
};
};
#endif 


