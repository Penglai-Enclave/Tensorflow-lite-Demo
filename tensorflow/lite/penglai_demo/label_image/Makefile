# Find where we're running from, so we can store generated files here.
ifeq ($(origin MAKEFILE_DIR), undefined)
	TEMP_MAKEFILE_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
endif

MAKEFILE_DIR := $(TEMP_MAKEFILE_DIR)/../../../../../tensorflow/tensorflow/lite/tools/make
INCLUDES := \
-I. \
-I$(MAKEFILE_DIR)/../../../../ \
-I$(MAKEFILE_DIR)/../../../../../ \
-I$(MAKEFILE_DIR)/downloads/ \
-I$(MAKEFILE_DIR)/downloads/eigen \
-I$(MAKEFILE_DIR)/downloads/absl \
-I$(MAKEFILE_DIR)/downloads/gemmlowp \
-I$(MAKEFILE_DIR)/downloads/ruy \
-I$(MAKEFILE_DIR)/downloads/neon_2_sse \
-I$(MAKEFILE_DIR)/downloads/farmhash/src \
-I$(MAKEFILE_DIR)/downloads/flatbuffers/include \
-I$(MAKEFILE_DIR)/downloads/fp16/include \
-I$(MAKEFILE_DIR)/downloads/cpuinfo \
-I$(MAKEFILE_DIR)/downloads/cpuinfo/include \
-I$(MAKEFILE_DIR)/downloads/cpuinfo/src \
-I$(MAKEFILE_DIR)/downloads/cpuinfo/deps/clog/include \
-I$(OBJDIR)
# This is at the end so any globally-installed frameworks like protobuf don't
# override local versions in the source tree.
INCLUDES += -I/usr/local/include

CFLAGS := -O0 -DNDEBUG -DCPU_SETSIZE=__CPU_SETSIZE -fPIC $(EXTRA_CFLAGS)
CXXFLAGS := $(CFLAGS) --std=c++11 $(EXTRA_CXXFLAGS)
CXXFLAGS += -DTFLITE_WITHOUT_XNNPACK
LDOPTS := -L/usr/local/lib
ARFLAGS := -r
TARGET_TOOLCHAIN_PREFIX :=
CC_PREFIX :=

CXX := riscv64-unknown-linux-gnu-g++
CC := riscv64-unknown-linux-gnu-gcc
LINK = riscv64-unknown-linux-gnu-ld

GENDIR := $(MAKEFILE_DIR)/gen/linux_riscv64/
LIBDIR := $(GENDIR)lib/
LIB_NAME := libtensorflow-lite.a

PENGLAI_SDK = ../../../../../Penglai-Enclave-sPMP/sdk
SDK_LIB_DIR = $(PENGLAI_SDK)/lib
MUSL_LIB_DIR = $(PENGLAI_SDK)/musl/lib


LIB_PATH := $(LIBDIR)$(LIB_NAME)

LIBS := \
-lrt \
-lstdc++ \
-lpthread \
-lm \
-latomic \
-lgcc_eh \
-ldl \
-lc \
-lgcc \
-lgcc_eh \
-lc_add

APP=label_image
APP_LDS ?= ../app.lds
SDK_INCLUDE_DIR = $(SDK_LIB_DIR)/app/include
LDFLAGS = -static -L$(SDK_LIB_DIR) -L$(MUSL_LIB_DIR) -L. -lpenglai-enclave-eapp -lc -lrt -lstdc++fs -lsupc++ -lstdc++ -lpthread -lm -ldl -latomic

SDK_APP_LIB = $(SDK_LIB_DIR)/libpenglai-enclave-eapp.a
MUSL_LIBC = $(MUSL_LIB_DIR)/libc.a
GCC_LIB = $(SDK_LIB_DIR)/libgcc.a

all:
	$(CXX) -c label_image.cc $(CXXFLAGS) $(INCLUDES) -g -static -o label_image.o
	$(CC) -I$(SDK_INCLUDE_DIR) -c label_image_entry.c -g -o label_image_entry.o
	$(CXX) -c stub.cc $(CXXFLAGS) -g -static -o stub.o
	$(LINK) -L. -L$(SDK_LIB_DIR) -L$(MUSL_LIB_DIR) -g -static -o $(APP) label_image.o label_image_entry.o stub.o $(LIB_PATH) $(SDK_APP_LIB) $(MUSL_LIBC) $(GCC_LIB) $(LIBS) -T $(APP_LDS)
	chmod -x $(APP)

clean:
	rm -f *.o $(APP)

