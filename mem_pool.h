#pragma once

#include<vector>

#include "macros.h"

namespace common
{
    template<typename T>
    class MemPool final {
        public :
            // constructor
            explicit MemPool(std::size_t num_elems):store_(num_elems,{T(),true})
            {
                ASSERT(&(store_[0])==reinterpret_cast<const ObjectBlock*>(&(store_[0].object_)),
                "T object should be the first member of ObjectBlock."
                );
            }

            template<typename... Args>
            T *allocate(Args... args) noexcept {
                auto obj_block = &(store_[next_free_index_]);
                ASSERT(obj_block->is_free_,"Expected object block at index:"+std::to_string(next_free_index_));
                T* ret = &(obj_block->object_);
                ret = new(ret) T(args...);
                obj_block->is_free_ = false;

                updateNextFreeIndex();

                return ret;
            }

            auto deallocate(const T *elem) noexcept {
                const auto elem_index = (reinterpret_cast<ObjectBlock*>(elem) - &(store_[0]));
                ASSERT(elem_index>=0 && static_cast<size_t>(elem_index) < store_.size(),"Element being deallocated does not belong to this Memory pool.");
                ASSERT(!store_[0].is_free_,"Expected in-use ObjectBlock at index:" + std::to_string(elem_index));
                store_[elem_index].is_free_ = true;
            }

        private :

            void updateNextFreeIndex() noexcept {
                const auto initial_free_index = next_free_index_;
                while (!store_[next_free_index_].is_free_) {
                    next_free_index_++;
                    if (UNLIKELY(next_free_index_ == store_.size())) {
                        next_free_index_ = 0;
                    }
                    ASSERT(initial_free_index == next_free_index_, "Memory Pool is out of space.");
                }
            }

            struct ObjectBlock{
                T object_;
                bool is_free_=false;
            };
            std::vector<ObjectBlock> store_;
            std::size_t next_free_index_=0;
    };
} // namespace common




