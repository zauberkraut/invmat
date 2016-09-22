name = acmi
root = $(shell pwd -P)
src = $(root)/src
build = $(root)/build
bin = $(root)/$(name)
testbin = $(build)/test

cc = gcc
nvcc = nvcc

main = $(build)/main.o
modules = $(build)/util.o $(build)/mat.o $(build)/invert.o $(build)/linalg.o \
          $(build)/kernels.obj $(build)/mmio.o
tests = $(build)/test.o

oflags = -O3
wflags = -Wall -Werror
cflags = -std=c99 $(oflags) $(wflags) -I/usr/local/cuda/include \
         -D_POSIX_C_SOURCE=200809L -mf16c
libs = -lm -llapacke -lopenblas -lpthread -L/usr/local/cuda/lib64 -lcudart \
       -lcublas -lstdc++

$(bin): $(main) $(modules)
	$(cc) -o $(@) $(^) $(libs)

$(build)/%.o: $(src)/%.c
	@mkdir -p $(build)
	$(cc) -o $(@) $(cflags) -c $(<)

$(build)/%.obj: $(src)/%.cu
	@mkdir -p $(build)
	$(nvcc) -o $(@) $(oflags) -Xcompiler "$(wflags)" -c $(<)

debug: oflags = -O0 -g
debug: $(bin)

$(testbin): $(tests) $(modules)
	$(cc) -o $(@) $(^) -lcmocka $(libs)

.PHONY: test clean
test: $(testbin)
	$(^)

clean:
	rm -fr $(build) $(bin)
