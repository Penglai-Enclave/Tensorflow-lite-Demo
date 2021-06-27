/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "tensorflow/lite/optional_debug_tools.h"

#include <stddef.h>
#include <stdio.h>

#include <utility>
#include <vector>

#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
extern "C" void eapp_print(const char*s, ...);
namespace tflite {

void PrintIntVector(const std::vector<int>& v) {
  for (const auto& it : v) {
    eapp_print(" %d", it);
  }
  eapp_print("\n");
}

void PrintTfLiteIntVector(const TfLiteIntArray* v) {
  if (!v) {
    eapp_print(" (null)\n");
    return;
  }
  for (int k = 0; k < v->size; k++) {
    eapp_print(" %d", v->data[k]);
  }
  eapp_print("\n");
}

const char* TensorTypeName(TfLiteType type) {
  switch (type) {
    case kTfLiteNoType:
      return "kTfLiteNoType";
    case kTfLiteFloat32:
      return "kTfLiteFloat32";
    case kTfLiteInt32:
      return "kTfLiteInt32";
    case kTfLiteUInt32:
      return "kTfLiteUInt32";
    case kTfLiteUInt8:
      return "kTfLiteUInt8";
    case kTfLiteInt8:
      return "kTfLiteInt8";
    case kTfLiteInt64:
      return "kTfLiteInt64";
    case kTfLiteUInt64:
      return "kTfLiteUInt64";
    case kTfLiteString:
      return "kTfLiteString";
    case kTfLiteBool:
      return "kTfLiteBool";
    case kTfLiteInt16:
      return "kTfLiteInt16";
    case kTfLiteComplex64:
      return "kTfLiteComplex64";
    case kTfLiteComplex128:
      return "kTfLiteComplex128";
    case kTfLiteFloat16:
      return "kTfLiteFloat16";
    case kTfLiteFloat64:
      return "kTfLiteFloat64";
    case kTfLiteResource:
      return "kTfLiteResource";
    case kTfLiteVariant:
      return "kTfLiteVariant";
  }
  return "(invalid)";
}

const char* AllocTypeName(TfLiteAllocationType type) {
  switch (type) {
    case kTfLiteMemNone:
      return "kTfLiteMemNone";
    case kTfLiteMmapRo:
      return "kTfLiteMmapRo";
    case kTfLiteDynamic:
      return "kTfLiteDynamic";
    case kTfLiteArenaRw:
      return "kTfLiteArenaRw";
    case kTfLiteArenaRwPersistent:
      return "kTfLiteArenaRwPersistent";
    case kTfLitePersistentRo:
      return "kTfLitePersistentRo";
    case kTfLiteCustom:
      return "kTfLiteCustom";
  }
  return "(invalid)";
}

// Prints a dump of what tensors and what nodes are in the interpreter.
void PrintInterpreterState(Interpreter* interpreter) {
  eapp_print("Interpreter has %d tensors and %d nodes\n",
         interpreter->tensors_size(), interpreter->nodes_size());
  eapp_print("Inputs:");
  PrintIntVector(interpreter->inputs());
  eapp_print("Outputs:");
  PrintIntVector(interpreter->outputs());
  eapp_print("\n");
  for (size_t tensor_index = 0; tensor_index < interpreter->tensors_size() - 1;
       tensor_index++) {
    TfLiteTensor* tensor = interpreter->tensor(static_cast<int>(tensor_index));
    eapp_print("Tensor %d %s %s %s %d bytes (%d MB) ", tensor_index,
           tensor->name, TensorTypeName(tensor->type),
           AllocTypeName(tensor->allocation_type), tensor->bytes,
           (int)(static_cast<float>(tensor->bytes) / (1 << 20)));
    PrintTfLiteIntVector(tensor->dims);
  }
  eapp_print("\n");
  for (size_t node_index = 0; node_index < interpreter->nodes_size();
       node_index++) {
    const std::pair<TfLiteNode, TfLiteRegistration>* node_and_reg =
        interpreter->node_and_registration(static_cast<int>(node_index));
    const TfLiteNode& node = node_and_reg->first;
    const TfLiteRegistration& reg = node_and_reg->second;
    if (reg.custom_name != nullptr) {
      eapp_print("Node %d Operator Custom Name %s\n", node_index,
             reg.custom_name);
    } else {
      eapp_print("Node %d Operator Builtin Code %d %s\n", node_index,
             reg.builtin_code, EnumNamesBuiltinOperator()[reg.builtin_code]);
    }
    eapp_print("  Inputs:");
    PrintTfLiteIntVector(node.inputs);
    eapp_print("  Outputs:");
    PrintTfLiteIntVector(node.outputs);
    if (node.intermediates && node.intermediates->size) {
      eapp_print("  Intermediates:");
      PrintTfLiteIntVector(node.intermediates);
    }
    if (node.temporaries && node.temporaries->size) {
      eapp_print("  Temporaries:");
      PrintTfLiteIntVector(node.temporaries);
    }
  }
}

}  // namespace tflite
