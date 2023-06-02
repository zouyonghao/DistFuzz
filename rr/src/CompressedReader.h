/* -*- Mode: C++; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

#ifndef RR_COMPRESSED_READER_H_
#define RR_COMPRESSED_READER_H_

#include <pthread.h>
#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

#include "ScopedFd.h"

namespace rr {

/**
 * CompressedReader opens an input file written by CompressedWriter
 * and reads data from it. Currently data is decompressed by the thread that
 * calls read().
 */
class CompressedReader {
public:
  CompressedReader(const std::string& filename);
  CompressedReader(const CompressedReader& aOther);
  ~CompressedReader();
  bool good() const { return !error; }
  bool at_end() const {
    const_cast<CompressedReader*>(this)->process_skip();
    return eof && buffer_read_pos == buffer.size();
  }
  // Returns true if successful. Otherwise there's an error and good()
  // will be false.
  bool read(void* data, size_t size);
  // Returns pointer/size of some buffered data. Does not change the state.
  // Returns zero size if at EOF.
  bool get_buffer(const uint8_t** data, size_t* size);
  // Advances the read position by the given size.
  void skip(size_t size) {
    buffer_skip_bytes += size;
  }
  void rewind();
  void close();

  /**
   * Save the current position. Nested saves are not allowed.
   */
  void save_state();
  /**
   * Restore previously saved position.
   */
  void restore_state();
  /**
   * Discard saved position
   */
  void discard_state();

  /**
   * Gathers stats on the file stream. These are independent of what's
   * actually been read.
   */
  uint64_t uncompressed_bytes() const;
  uint64_t compressed_bytes() const;

protected:
  void process_skip();
  bool refill_buffer(size_t* skip_bytes = nullptr);

  /* Our fd might be the dup of another fd, so we can't rely on its current file
     position.
     Instead track the current position in fd_offset and use pread. */
  uint64_t fd_offset;
  std::shared_ptr<ScopedFd> fd;
  bool error;
  bool eof;
  std::vector<uint8_t> buffer;
  // within `buffer`
  size_t buffer_read_pos;
  size_t buffer_skip_bytes;

  bool have_saved_state;
  bool have_saved_buffer;
  uint64_t saved_fd_offset;
  std::vector<uint8_t> saved_buffer;
  size_t saved_buffer_read_pos;
};

} // namespace rr

#endif /* RR_COMPRESSED_READER_H_ */
