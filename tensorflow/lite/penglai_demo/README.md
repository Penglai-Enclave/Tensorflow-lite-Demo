## Introduction

The penglai_demo directory contains tensorflow lite application that can be run in [Penglai](https://github.com/Penglai-Enclave/Penglai-Enclave-sPMP).

## Case: Running label_image with Penglai

### Build Tensorflow Lite

First, clone tensorflow, and place tensorflow and Penglai in the same level directory.

    git clone https://github.com/Moonquakes/tensorflow.git
    cd tensorflow
    git checkout penglai

Then, download the dependency files for tensorflow lite.

    cd tensorflow/lite/tools/
    ./download_dependencies.sh

Last, build the risc-v version of tensorflow lite.

    ./build_riscv_lib.sh

### Build label_image demo with Penglai supports

    cd ../../penglai_demo/label_image
    make

**Note1**: Before you build label_image, you should make sure that Penglai has been built successfully.

**Note2**: The *MAX_ORDER* used in Penglai should be at least 16, you can refer to [patches/large-demo-supports-20211026.patch](https://github.com/Penglai-Enclave/Penglai-Enclave-sPMP/blob/opensbi/patches/large-demo-supports-20211026.patch) for changing the *MAX_ORDER* in *mmzone.h*. 

### Run label_image demo with Penglai supports
You can copy label_image to openEuler Qemu using scp,

    scp -p 12055 label_image root@localhost:~/.

and then can run label_image like other Penglai demo.

    ./host label_image

**Note**: Before you copy label_image to openEuler Qemu, you should make sure that Penglai has been run successfully.
