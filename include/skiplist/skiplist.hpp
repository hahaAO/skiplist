#ifndef VGD_SKIPLIST_HPP
#define VGD_SKIPLIST_HPP

#include <atomic>
#include <cstddef>
#include <random>
namespace vgd {

inline bool one_in_four() {
  // There is a one-in-four chance of an increase
  static std::minstd_rand random_seed(time(NULL));
  static std::uniform_int_distribution<size_t> dis1(1, 4);
  if (dis1(random_seed) == 4) {
    return true;
  }
  return false;
}

template <typename Key_T>
class skiplist {
 private:
  struct Node;
  Node* head;

  static constexpr size_t kMaxHeight = 12;

  std::atomic<int> max_height_;  // Height of the entire list

 public:
  skiplist();

  skiplist(const skiplist&) = delete;
  skiplist& operator=(const skiplist&) = delete;

  void Insert(const Key_T& key);

  bool Contains(const Key_T& key) const;
};

template <typename Key_T>
skiplist<Key_T>::skiplist() : head(nullptr), max_height_(1) {}


template <typename Key_T>
struct skiplist<Key_T>::Node {
  Key_T value;

  Node* next_node;
  Node* next_level_node;
};


}  // namespace vgd

#endif  // VGD_SKIPLIST_HPP
