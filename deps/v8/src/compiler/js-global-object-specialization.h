// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_COMPILER_JS_GLOBAL_OBJECT_SPECIALIZATION_H_
#define V8_COMPILER_JS_GLOBAL_OBJECT_SPECIALIZATION_H_

#include "src/base/flags.h"
#include "src/compiler/graph-reducer.h"

namespace v8 {
namespace internal {

// Forward declarations.
class CompilationDependencies;
class TypeCache;


namespace compiler {

// Forward declarations.
class CommonOperatorBuilder;
class JSGraph;
class JSOperatorBuilder;
class SimplifiedOperatorBuilder;


// Specializes a given JSGraph to a given global object, potentially constant
// folding some {JSLoadGlobal} nodes or strength reducing some {JSStoreGlobal}
// nodes.
class JSGlobalObjectSpecialization final : public AdvancedReducer {
 public:
  // Flags that control the mode of operation.
  enum Flag {
    kNoFlags = 0u,
    kDeoptimizationEnabled = 1u << 0,
  };
  typedef base::Flags<Flag> Flags;

  JSGlobalObjectSpecialization(Editor* editor, JSGraph* jsgraph, Flags flags,
                               MaybeHandle<Context> native_context,
                               CompilationDependencies* dependencies);

  Reduction Reduce(Node* node) final;

 private:
  Reduction ReduceJSLoadGlobal(Node* node);
  Reduction ReduceJSStoreGlobal(Node* node);

  // Retrieve the global object from the given {node} if known.
  MaybeHandle<JSGlobalObject> GetGlobalObject(Node* node);

  struct ScriptContextTableLookupResult;
  bool LookupInScriptContextTable(Handle<JSGlobalObject> global_object,
                                  Handle<Name> name,
                                  ScriptContextTableLookupResult* result);

  Graph* graph() const;
  JSGraph* jsgraph() const { return jsgraph_; }
  Isolate* isolate() const;
  CommonOperatorBuilder* common() const;
  JSOperatorBuilder* javascript() const;
  SimplifiedOperatorBuilder* simplified() const;
  Flags flags() const { return flags_; }
  MaybeHandle<Context> native_context() const { return native_context_; }
  CompilationDependencies* dependencies() const { return dependencies_; }

  JSGraph* const jsgraph_;
  Flags const flags_;
  MaybeHandle<Context> native_context_;
  CompilationDependencies* const dependencies_;
  TypeCache const& type_cache_;

  DISALLOW_COPY_AND_ASSIGN(JSGlobalObjectSpecialization);
};

DEFINE_OPERATORS_FOR_FLAGS(JSGlobalObjectSpecialization::Flags)

}  // namespace compiler
}  // namespace internal
}  // namespace v8

#endif  // V8_COMPILER_JS_GLOBAL_OBJECT_SPECIALIZATION_H_
