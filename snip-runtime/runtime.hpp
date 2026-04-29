#pragma once

#include "cmd.hpp"
#include "msg.hpp"
#include "snip-core/cc_queue.hpp"
#include "snip-core/event_loop.hpp"
#include "snip-core/event_source.hpp"
#include "snip-core/ezpipe.hpp"
#include "snip-core/thread_pool.hpp"
#include "snip-editor/state.hpp"

namespace snip::runtime {

struct UpdateResult {
  editor::State newState;
  std::vector<Cmd> commands;
};

/*
  Runtime represents Snip's runtime and provides an API for managing the
  application state and handling events. It provides a main event loop, a
  message passing system, and a simple command abstraction for running
  background tasks.
*/
class Runtime {
 private:
  core::CCQueue<Msg> msgQ{true};
  core::EzPipe wakePipe;

  core::ThreadPool pool{4};
  core::EventLoop* loop = nullptr;

  editor::State& state;
  bool running = true;

 public:
  explicit Runtime(editor::State& s);
  virtual ~Runtime() = default;

  // T must be a supported runtime::Msg alternative.
  void post(Msg&& msg);

  // Run the app inside an EventLoop
  void run(core::EventLoop& eventLoop);

 protected:
  virtual std::vector<Cmd> init() = 0;

  virtual UpdateResult update(const editor::State& currentState, Msg msg) = 0;

  virtual std::string render(const editor::State& currentState) = 0;

 private:
  void dispatch(const std::vector<Cmd>& cmds);

  // Event source used by the event loop
  core::EventSource messageSource();
};
}  // namespace snip::runtime