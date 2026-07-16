// Copyright 2025-present the zvec project
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

#include <memory>
#include <zvec/db/doc.h>
#include <zvec/db/status.h>

namespace zvec {

class CollectionImpl;

class DocIterator {
  // Pimpl: implementation holds Arrow types (RecordBatchReader, RecordBatch).
  // Forward-declared here (private) so the public constructor can name it.
  struct Impl;

 public:
  using Ptr = std::shared_ptr<DocIterator>;

  // Passkey idiom: Passkey's constructor is private and CollectionImpl is its
  // friend, so only CollectionImpl can construct a DocIterator. This lets
  // CollectionImpl use std::make_shared while keeping construction controlled.
  struct Passkey {
   private:
    Passkey() {}
    friend class CollectionImpl;
  };

  // Called by CollectionImpl::CreateIterator
  DocIterator(Passkey, std::unique_ptr<Impl> impl);

  // !has_value() → error
  // has_value() && value() == nullptr → EOF
  // has_value() && value() != nullptr → success
  Result<Doc::Ptr> Next();

  void Close();

  ~DocIterator();

 private:
  // CollectionImpl builds the iterator (constructs Impl and fills its fields).
  friend class CollectionImpl;

  std::unique_ptr<Impl> impl_;
};

}  // namespace zvec
