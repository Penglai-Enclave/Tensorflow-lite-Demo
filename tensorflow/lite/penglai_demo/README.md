## Introduction

The penglai_demo directory contains tensorflow lite application that can be run in [Penglai](https://github.com/Penglai-Enclave/Penglai-Enclave-sPMP).

## Case: Running label_image with Penglai

### Build Tensorflow Lite

First, clone tensorflow, and place tensorflow and Penglai in the same level directory.

**Note**: The Penglai project's name should be *Penglai-Enclave-sPMP*.

    git clone https://github.com/Moonquakes/tensorflow.git
    cd tensorflow
    git checkout penglai

Then, download the dependency files for tensorflow lite.

    cd tensorflow/lite/tools/make/
    ./download_dependencies.sh

Last, build the risc-v version of tensorflow lite.

**Note**: You should make sure that there is a local riscv compilation chain so that you can use riscv64-unknown-linux-gnu-gcc and riscv64-unknown-linux-gnu-g++.

    ./build_riscv_lib.sh

You can also use penglai-sPMP docker image to build:

    cd ../../../../../
    docker run -v $(pwd):/home/penglai/penglai-enclave -w /home/penglai/penglai-enclave --rm -it ddnirvana/penglai-enclave:v0.5 tensorflow/tensorflow/lite/tools/make/build_riscv_lib.sh

### Build label_image demo with Penglai supports

**Note1**: Before you build label_image, you should make sure that Penglai has been built successfully.

**Note2**: The *MAX_ORDER* used in Penglai should be at least 16, you can refer to [large-demo-supports-20211026.patch](https://github.com/Penglai-Enclave/Penglai-Enclave-sPMP/blob/opensbi/patches/large-demo-supports-20211026.patch) for changing the *MAX_ORDER* in *mmzone.h*.

**Note3**: When build Penglai SDK, you should use:

    PENGLAI_SDK=$(pwd) CUSTOM_BRK_SIZE=4096*256 CUSTOM_MMAP_SIZE=4096*8192 make -j8

to customize parameters CUSTOM_BRK_SIZE and CUSTOM_MMAP_SIZE. You can find more details in [musl.config.md](https://github.com/Penglai-Enclave/penglai-sdk/blob/master/docs/musl.config.md).

Then, you can build label_image demo with Penglai supports:

    cd ../../penglai_demo/label_image
    make

You can also use penglai-sPMP docker image to build:

    docker run -v $(pwd):/home/penglai/penglai-enclave -w /home/penglai/penglai-enclave --rm -it ddnirvana/penglai-enclave:v0.5 tensorflow/tensorflow/lite/penglai_demo/label_image/scripts/build_label_image.sh

### Run label_image demo with Penglai supports
You can copy label_image to openEuler Qemu using scp.

**Note**: Before you copy label_image to openEuler Qemu, you should make sure that Penglai has been run successfully.

    scp -P 12055 label_image root@localhost:~/.

and then can run label_image like other Penglai demo.

    ./host label_image
