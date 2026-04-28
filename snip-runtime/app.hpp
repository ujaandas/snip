#pragma once

#include <functional>
#include <iostream>
#include <utility>
#include <variant>

#include "cmd.hpp"
#include "msg.hpp"
#include "snip-core/cc_queue.hpp"
#include "snip-core/event_loop.hpp"
#include "snip-core/event_source.hpp"
#include "snip-core/ezpipe.hpp"
#include "snip-core/thread_pool.hpp"
#include "snip-editor/editor.hpp"
#include "snip-editor/state.hpp"
#include "snip-render/renderer.hpp"
#include "snip-render/translator.hpp"

namespace snip::runtime {

struct UpdateResult {
  editor::State newState;
  std::vector<Cmd> commands;
};

/*
  App represents Snip's runtime and provides an API for managing the
  application state and handling events. It provides a main event loop, a
  message passing system, and a simple command abstraction for running
  background tasks.

  We subclass App and implement the init, update, and render methods. Then call
  run() with an EventLoop instance to start the app.
*/
class App {
 private:
  core::CCQueue<Msg> msgQ{true};
  core::EzPipe wakePipe;

  core::ThreadPool pool{4};
  core::EventLoop* loop = nullptr;

  snip::editor::Editor editor;
  snip::render::Renderer renderer;
  snip::render::AnsiTranslator translator;

  editor::State& state;
  bool running = true;

 public:
  explicit App(editor::State& s) : state(s) {}
  ~App() = default;

  // T must be a supported runtime::Msg alternative.
  void post(Msg&& msg) {
    msgQ.ccpush(msg);
    wakePipe.write('!');
  }

  // Run the app inside an EventLoop
  void run(core::EventLoop& eventLoop) {
    loop = &eventLoop;

    // Register our mailbox source
    loop->addSource(messageSource());

    // Initial render
    std::cout << render(state) << std::flush;

    // Run init commands
    dispatch(init());

    loop->run();
  }

 private:
  std::vector<Cmd> init() { return editor.init(); };

  UpdateResult update(const editor::State& currentState, Msg msg) {
    auto result = editor.update(currentState, std::move(msg));
    return {std::move(result.newState), std::move(result.commands)};
  };

  std::string render(const editor::State& currentState) {
    const auto frame = renderer.render(editor.viewModel(currentState));
    return translator.translate(frame);
  };

  void quit() { running = false; }

  void dispatch(const std::vector<Cmd>& cmds) {
    for (const auto& cmd : cmds) {
      pool.enqueue([this, cmd]() {
        if (auto maybeMsg = cmd()) {
          post(std::move(*maybeMsg));
        }
      });
    }
  }

  // Event source used by the event loop
  core::EventSource messageSource() {
    core::EventSource src = core::EventSource::fromFd(wakePipe.getReadFd());

    src.onReadReady = [this]() {
      wakePipe.clear();

      Msg msg;
      // Message handler
      while (msgQ.ccawait(msg)) {
        if (std::holds_alternative<QuitMsg>(msg)) {
          quit();

          if (!running && loop) {
            loop->stop();
            break;
          }

          continue;
        }

        UpdateResult result = update(state, std::move(msg));
        state = result.newState;

        std::cout << render(state) << std::flush;

        dispatch(result.commands);

        if (!running && loop) {
          loop->stop();
        }
      }
    };

    return src;
  }
};
}  // namespace snip::runtime