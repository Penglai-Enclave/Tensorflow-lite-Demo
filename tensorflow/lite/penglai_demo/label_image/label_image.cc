/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

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
#include <cstdio>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include "tensorflow/lite/builtin_op_data.h"
#include "tensorflow/lite/string_util.h"
#include "model_data.cc"
#include "image_data.cc"
#include "labels.cc"
extern "C" void eapp_print(const char*s, ...);

#define TFLITE_MINIMAL_CHECK(x)                              \
  if (!(x)) {                                                \
    eapp_print("Error at %s:%d\n", __FILE__, __LINE__); \
  }

extern "C" int __main_label_image();
int __main_label_image() {
  // Load model
  std::unique_ptr<tflite::FlatBufferModel> model =
      tflite::FlatBufferModel::BuildFromBuffer((const char *) imported_model, imported_model_size);
  TFLITE_MINIMAL_CHECK(model != nullptr);
  eapp_print("Load model successfully\n");

  // Build the interpreter with the InterpreterBuilder.
  // Note: all Interpreters should be built with the InterpreterBuilder,
  // which allocates memory for the Intrepter and does various set up
  // tasks so that the Interpreter can read the provided model.
  tflite::ops::builtin::BuiltinOpResolver resolver;
  eapp_print("Construct BuiltinOpResolver successfully\n");
  tflite::InterpreterBuilder builder(*model, resolver);
  eapp_print("Construct InterpreterBuilder successfully\n");
  std::unique_ptr<tflite::Interpreter> interpreter;
  eapp_print("Construct Interpreter successfully\n");
  builder.AddInterpreter(&interpreter);
  // builder(&interpreter);
  eapp_print("AddInterpreter successfully\n");
  TFLITE_MINIMAL_CHECK(interpreter != nullptr);
  eapp_print("Check interpreter != nullptr successfully\n");

  // Allocate tensor buffers.
  TFLITE_MINIMAL_CHECK(interpreter->AllocateTensors() == kTfLiteOk);
  eapp_print("Interpreter allocateTensors successfully\n");

  // tflite::PrintInterpreterState(interpreter.get());
  // TFLITE_MINIMAL_CHECK(interpreter->Invoke() == kTfLiteOk);
  // eapp_print("TFLITE_MINIMAL_CHECK(interpreter->Invoke() == kTfLiteOk);\n");
  // Fill input buffers
  // TODO(user): Insert code to fill input tensors.
  // Note: The buffer of the input tensor with index `i` of type T can
  // be accessed with `T* input = interpreter->typed_input_tensor<T>(i);`
  {
  TfLiteTensor *input_tensor = interpreter->tensor(interpreter->inputs()[0]);
  switch (input_tensor->type) {
      case kTfLiteFloat32: {
        int input0 = interpreter->inputs()[0];

        TfLiteIntArray* dims = interpreter->tensor(input0)->dims;
        int wanted_height = dims->data[1];
        int wanted_width = dims->data[2];
        int wanted_channels = dims->data[3];
        auto number_of_pixels = wanted_height * wanted_width * wanted_channels;

        uint8_t* input = interpreter->typed_tensor<uint8_t>(input0);
        for (int i = 0; i < number_of_pixels; i++) {
          input[i] = imported_image[i] / 255.f;
        }
        break;
      }
      case kTfLiteUInt8: {
        int input0 = interpreter->inputs()[0];

        TfLiteIntArray* dims = interpreter->tensor(input0)->dims;
        int wanted_height = dims->data[1];
        int wanted_width = dims->data[2];
        int wanted_channels = dims->data[3];
        auto number_of_pixels = wanted_height * wanted_width * wanted_channels;

        uint8_t* input = interpreter->typed_tensor<uint8_t>(input0);
        for (int i = 0; i < number_of_pixels; i++) {
          input[i] = static_cast<uint8_t>(imported_image[i]);
        }
        break;
      }
  }
  }
  eapp_print("Set inputs successfully\n");

  // Run inference
  int number_of_warmup_runs = 10;
  for (int i = 0; i < number_of_warmup_runs; i++){
    TFLITE_MINIMAL_CHECK(interpreter->Invoke() == kTfLiteOk);
    eapp_print("The number of warmup runs is %d\n", i + 1);
  }
  eapp_print("Finish warmup runs successfully\n");

  int loop_count = 10;
  for (int i = 0; i < loop_count; i++){
    TFLITE_MINIMAL_CHECK(interpreter->Invoke() == kTfLiteOk);
    eapp_print("The number of loop runs is %d\n", i + 1);
  }
  eapp_print("Finish loop runs successfully\n");

  // tflite::PrintInterpreterState(interpreter.get());

  // Read output buffers
  // TODO(user): Insert getting data out code.
  // Note: The buffer of the output tensor with index `i` of type T can
  // be accessed with `T* output = interpreter->typed_output_tensor<T>(i);`
  TfLiteTensor *output_tensor = interpreter->tensor(interpreter->outputs()[0]);
  eapp_print("Get outputs successfully\n");

  unsigned int max = output_tensor->data.uint8[0];
  unsigned int index = 0;
  for(int i = 1; i < 1001; i++){
    if(output_tensor->data.uint8[i] > max){
      max = output_tensor->data.uint8[i];
      index = i;
    }
  }
  eapp_print("The input image's index is %d, and label is %s\n", index, imported_labels[index]);

  return 0;
}
