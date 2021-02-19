//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) {}

LRUReplacer::~LRUReplacer() {}

bool LRUReplacer::Victim(frame_id_t *frame_id) {
  std::lock_guard<std::mutex> guard(mutex_);
  if (frame_table_.empty()) {
    return false;
  }
  *frame_id = lru_list_.back();
  lru_list_.pop_back();
  frame_table_.erase(*frame_id);
  return true;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
  std::lock_guard<std::mutex> guard(mutex_);
  auto table_it = frame_table_.find(frame_id);
  if (table_it == frame_table_.end()) {
    return;
  }
  lru_list_.erase(table_it->second);
  frame_table_.erase(table_it);
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
  std::lock_guard<std::mutex> guard(mutex_);
  auto table_it = frame_table_.find(frame_id);
  if (table_it != frame_table_.end()) {
    return;
  }
  lru_list_.emplace_front(frame_id);
  frame_table_[frame_id] = lru_list_.begin();
}

size_t LRUReplacer::Size() {
  std::lock_guard<std::mutex> guard(mutex_);
  return frame_table_.size();
}

}  // namespace bustub
