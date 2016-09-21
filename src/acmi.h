/* acmi.h

   Internal header for the ACMI project. */

#ifndef ACMI_H
#define ACMI_H

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum { MAX_MAT_DIM = 32768, MAX_ELEM_SIZE = 8 };

struct Mat_;
typedef struct Mat_* Mat;
struct Double2 { double hi, lo; };
union Elem;

// util.c
void setVerbose(bool b);
void debug(const char* msg, ...);
void warn(const char* msg, ...);
void fatal(const char* msg, ...);
double mibibytes(size_t size);

// mat.c
Mat MatNew(int n, int elemSize, bool dev);
Mat MatBuild(Mat m);
void MatFree(Mat m);
void MatClear(Mat m);
int MatN(Mat m);
int64_t MatN2(Mat m);
int MatElemSize(Mat m);
size_t MatSize(Mat m);
size_t MatPitch(Mat m);
void* MatElems(Mat m);
void* MatCol(Mat m, int col);
bool MatDev(Mat m);
double MatTrace(Mat m);
void MatToDev(Mat m);
void MatToHost(Mat m);
void MatPromote(Mat m);
void MatDemote(Mat m);
double MatGet(Mat m, int row, int col);
void MatPut(Mat m, int row, int col, double elem);
Mat MatLoad(const char* path, int elemSize, bool attrOnly);
void MatWrite(Mat m, const char* path);
Mat MatNewRand(int n, int elemSize, double maxElem, bool symm, bool real,
               bool neg, bool diagDom);
void MatDebug(Mat m);
double ElemVal(union Elem* e, int size);
void ElemSet(union Elem* e, int size, double val);

// invert.c
double altmanInvert(Mat mA, Mat *mRp, int convOrder, double errLimit,
                    int msLimit, double convRateLimit);

// blas.c
void cublasInit();
void cublasShutDown();
void transpose(double alpha, Mat mA, Mat mT);
void gemm(double alpha, Mat mA, Mat mB, double beta, Mat mC);
void geam(double alpha, Mat mA, double beta, Mat mB, Mat mC);
void setDiag(Mat mA, double alpha);
double norm(Mat mA);
double normSubFromI(Mat mA);
void add3I(Mat mA);

// kernels.c
size_t cuMemAvail();
void* cuMalloc(size_t size);
void cuFree(void* p);
void cuClear(void* p, size_t size);
void cuUpload(void* devDst, const void* hostSrc, size_t size);
void cuDownload(void* hostDst, const void* devSrc, size_t size);
void cuPin(void* p, size_t size);
void cuUnpin(void* p);
void cuPromote(void* dst, void* src, int srcElemSize, int64_t n2);
void cuDemote(uint16_t* dst, float* src, int64_t n2);
void cuSetDiag(void* elems, double alpha, int n, int elemSize);
double cuNorm(void* elems, int64_t n2, int elemSize);
double cuNormSubFromI(void* elems, int n, int elemSize);
void cuAdd3I(void* elems, int n, int elemSize);

double halfToDouble(uint16_t h);
uint16_t doubleToHalf(double d);

#endif
