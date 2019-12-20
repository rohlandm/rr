/* -*- Mode: C++; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

#ifndef RR_BASE_FILE_MONITOR_H_
#define RR_BASE_FILE_MONITOR_H_

#include "FileMonitor.h"

namespace rr {

/**
 * A FileMonitor to track writes to files used by the tracee
 */
class BaseFileMonitor : public FileMonitor {
public:
  /**
   * Create the BaseFileMonitor
   */
  BaseFileMonitor(int original_fd) : original_fd(original_fd) {}

  // virtual Type type() override { return Base; }

  /**
   * Make writes blocking (see StdioMonitor)
   */
  virtual Switchable will_write(Task* t) override;

  /**
   * During replay echo writes
   */
  virtual void did_write(Task* t, const std::vector<Range>& ranges,
                         LazyOffset&) override;

private:
  int original_fd;
};
} // namespace rr

#endif /* RR_BASE_FILE_MONITOR_H_ */