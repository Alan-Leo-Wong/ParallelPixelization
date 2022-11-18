# Parallel Pixelation on CPU and GPU

由衷地感谢[滕老师](https://github.com/tengdj)的指导与帮助。

This is a project for learning parallel programming, which realizes pixelation of `.wkt` data on CPU and GPU platform.

1. On the CPU platform, this project realizes the parallel programming of `pThread` and `OpenMP`.
2. On the CPU platform,this project realizes the parallel programming of `CUDA`.

## Requirements
- [CMake>=3.22](https://cmake.org/)
- [Eigen3](https://eigen.tuxfamily.org)
- [CUDA Toolkit](https://developer.nvidia.com/cuda-toolkit)

## Installation

#### 1. Download from repo
```shell
$ git clone --recursive https://github.com/KMushaL/ParallelPixelization.git
```
#### 2. Build

---

**Configure Options**

- **USE_OPENMP**
  - This option is enabled by default. It enables `OpenMP` instead of `pThread`.
- **USE_CUDA** 
  - This option is enabled by default. If enables `CUDA` and to further generate the corresponding executable file.

---

Then run the follow shell to complete build.
```shell
$ cmake -S . -B [-D <option>=<value>]
$ cd build
$ make
```

#### 3. Run

There are some params can be passed when run the executable file.

<br>

>**CPU**

---

**Params**

- **in** 
  - Specify the path to the `.wkt` file, If no value is passed, the `bigpolygon.wkt` file in the `res` directory will be used.
  - e.g. `--in ${path to your file}`

- **ps** 
  - A `double` value specifying the pixel size, the default value is 10. 
  - e.g. `--ps 1`

- **th**
    - An `integer` value specifying the number of threads to run, the default value is 16.
    - e.g. `--th 8`

---

Then run the follow shell.
```shell
$ ./CPU_PIXEL.out [--<param> <value>]
```

<br>

>**GPU**

---

**Params**

- **in**
    - Specify the path to the `.wkt` file, If no value is passed, the `bigpolygon.wkt` file in the `res` directory will be used.
    - e.g. `--in ${path to your file}`

- **ps**
    - A `double` value specifying the pixel size, the default value is 1.
    - e.g. `--ps 1`

---

Then run the follow shell.
```shell
$ ./GPU_PIXEL.out [--<param> <value>]
```
## Test Results

There are some test results on my laptop, the configuration is as follows:

**CPU**: Intel 11th Gen Core i7-11800H

**GPU**: Nvidia Geforce RTX 3060(Laptop)

---

1. **pThread vs OpenMP** (pixel_size=1)

![](https://github.com/KMushaL/ParallelPixelization/blob/master/READEME.assests/pthread%20vs%20openmp.png)
    
---

2. **OpenMP vs CUDA** (pixel_size=0.05)

    OpenMP(16 threads): **17715.5s**

    CUDA: **19.9403s**

---

3. **Different size of block on CUDA**

   1. pixel_size=0.05

   2. pixel_size=0.01

---