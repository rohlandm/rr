/* -*- Mode: C++; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

#include "BaseFileMonitor.h"

#include "Flags.h"
#include "ReplaySession.h"
#include "ReplayTask.h"
#include "Session.h"
#include "log.h"

namespace rr {
    

Switchable BaseFileMonitor::will_write(Task* t) {
  char buf[256];
  snprintf(buf, sizeof(buf) - 1, "[rr %d %d]", t->tgid(), t->trace_time());
  ssize_t len = strlen(buf);
  if (write(original_fd, buf, len) != len) {
      ASSERT(t, false) << "Couldn't write to " << original_fd;
  }

  return PREVENT_SWITCH;
}

void BaseFileMonitor::did_write(Task* t, const std::vector<Range>& ranges,
                             LazyOffset&) {
  if (!t->session().is_replaying()) {
    return;
  }
//   auto rt = static_cast<ReplayTask*>(t);
  for (auto& r : ranges) {
    auto bytes = t->read_mem(r.data.cast<uint8_t>(), r.length);
    if (bytes.size() !=
        (size_t)write(original_fd, bytes.data(), bytes.size())) {
      ASSERT(t, false) << "Couldn't write to " << original_fd;
    }
  }
}

} // namespace rr