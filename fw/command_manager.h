// Copyright 2015-2018 Josh Pieper, jjp@pobox.com.  All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "EventQueue.h"

#include "async_exclusive.h"
#include "async_stream.h"
#include "async_types.h"
#include "opaque_ptr.h"
#include "static_function.h"
#include "string_span.h"
#include "string_view.h"

/// This class presents a cmdline interface over an AsyncStream,
/// allowing multiple modules to register commands.
class CommandManager {
 public:
  /// @param queue is used to enqueue callbacks
  /// @param read_stream commands are read from this stream
  /// @param write_stream responses are written to this stream
  CommandManager(events::EventQueue* queue,
                 AsyncReadStream* read_stream,
                 AsyncExclusive<AsyncWriteStream>* write_stream);
  ~CommandManager();

  struct Response {
    AsyncWriteStream* stream = nullptr;
    ErrorCallback callback;

    Response(AsyncWriteStream* stream, ErrorCallback callback)
        : stream(stream), callback(callback) {}
    Response() {}
  };

  using CommandFunction = StaticFunction<void (const ::string_view&, const Response&)>;
  void Register(const ::string_view& name, CommandFunction);

  void AsyncStart();

 private:
  class Impl;
  OpaquePtr<Impl, 1024> impl_;
};
