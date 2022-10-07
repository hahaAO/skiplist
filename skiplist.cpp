#include "skiplist.hpp"

#include <atomic>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <random>
#include <set>
#include <thread>
#include <vgd_tools/timer.hpp>

#include "le_skiplist.hpp"

const int kTestNumber = 100000;
struct A {
  A(int ra) : a(ra), b(0){};
  A(int ra, double rb) : a(ra), b(rb){};

  int a;
  double b;
  std::ostream& operator<<(std::ostream& os) {
    return os << "A: " << a << " " << b << std::endl;
  }
  friend std::ostream& operator<<(const A& l, std::ostream& os) {
    return os << "A: " << l.a << " " << l.b << std::endl;
  }
  friend std::ostream& operator<<(std::ostream& os, const A& l) {
    return os << "A: " << l.a << " " << l.b << std::endl;
  }

  bool operator==(const A& r) { return a == r.a && b == r.b; }
  friend bool operator==(const A& l, const A& r) {
    return l.a == r.a && l.b == r.b;
  }

  bool operator>(const A& r) { return a > r.a && b > r.b; }
  friend bool operator>(const A& l, const A& r) {
    return l.a > r.a && l.b > r.b;
  }
  bool operator<(const A& r) { return !(*this > r) && !(*this == r); }
  friend bool operator<(const A& l, const A& r) {
    return !(l > r) && !(l == r);
  }
};

int main() {
  leveldb::SkipList<A> ss;
  std::set<A> ssa;
  // auto e = new int(3);
  // auto e2 = new int(4);

  std::default_random_engine rr;
  std::uniform_int_distribution<int> dis1(1, 1000);
  std::uniform_real_distribution<double> disd(1, 123321);

  // ss.Insert({2, 3.0});
  // std::cout << ss.Contains({2, 3.0}) << std::endl;
  // std::cout << ss.Contains({3, 3.0}) << std::endl;

  // int p = 0;
  std::atomic<int> thread_number = 0;
  for (size_t i = 0; i < 10000; ++i) {
    A aa(dis1(rr), disd(rr));
    bool should_remove = leveldb::one_in_four();
    bool should_multithread =
        (leveldb::one_in_four() &&
         thread_number.load(std::memory_order_relaxed) < 100);
    // std::cout << thread_number.load(std::memory_order_relaxed) << std::endl;

    if (should_multithread) {
      thread_number.fetch_add(1, std::memory_order_relaxed);
      std::thread t1([&, aa, should_remove]() {
        ss.Insert(aa);
        if (should_remove) {
          ss.Delete(aa);
        }
        thread_number.fetch_sub(1, std::memory_order_relaxed);
      });
      t1.detach();
    } else {
      ss.Insert(aa);
    }

    ssa.insert(aa);

    if (should_remove) {
      ssa.erase(aa);
      if (!should_multithread) {
        ss.Delete(aa);
      }
    }
  }

  for (size_t i = 0; i < 100000; ++i) {
    A aa(dis1(rr), disd(rr));
    bool s_h = ss.Contains(aa);
    auto it = ssa.find(aa);
    bool sa_h = (it != ssa.end());

    assert(s_h == sa_h);
  }
  {
    vgd::Timer t("1");
    for (size_t i = 0; i < kTestNumber; ++i) {
      A aa(dis1(rr), disd(rr));
      if (!ss.Contains(aa)) {
        ss.Insert(aa);
      }
    }
  }
  {
    vgd::Timer t("11");
    for (size_t i = 0; i < kTestNumber; ++i) {
      A aa(dis1(rr), disd(rr));
      ss.Contains(aa);
    }
  }

  {
    vgd::Timer t("2");
    for (size_t i = 0; i < kTestNumber; ++i) {
      A aa(dis1(rr), disd(rr));
      ssa.insert(aa);
    }
  }
  {
    vgd::Timer t("22");
    for (size_t i = 0; i < kTestNumber; ++i) {
      A aa(dis1(rr), disd(rr));
      auto it = ssa.find(aa);
      bool sa_h = (it != ssa.end());
    }
  }
  // std::cout << "p: " << p << std::endl;
  std::cout << "Hello, from skiplist!\n" << std::endl;
}
