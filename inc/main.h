#include "flatbuffers/flatbuffers.h"

// use unique_ptr implementation from flatbuffers since STDPort does not implement it
namespace std {
  using flatbuffers::unique_ptr;
}