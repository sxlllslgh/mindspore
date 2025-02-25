/**
 * Copyright 2021 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AICPU_KERNELS_NORMALIZED_ACOSH_GRAD_H_
#define AICPU_KERNELS_NORMALIZED_ACOSH_GRAD_H_

#include <complex>

#include "inc/ms_cpu_kernel.h"
#include "utils/bcast.h"

namespace aicpu {
class AcoshGradCpuKernel : public CpuKernel {
 public:
  AcoshGradCpuKernel() = default;
  ~AcoshGradCpuKernel() override = default;

 protected:
  uint32_t Compute(CpuKernelContext &ctx) override;

 private:
  static uint32_t AcoshGradParamCheck(const CpuKernelContext &ctx);

  template <typename T>
  static uint32_t AcoshGradCompute(const CpuKernelContext &ctx);

  template <typename T>
  static uint32_t AcoshGradComputeComplex(const CpuKernelContext &ctx);

  template <typename T>
  static uint32_t AcoshGradComputeFP16(const CpuKernelContext &ctx);

  template <typename T>
  static void SpecialCompute(int64_t start, int64_t end, const T *input1, const T *input2, T *output);

  template <typename T>
  static void SpecialComputeComplex(int64_t start, int64_t end, const T *input1, const T *input2, T *output);

  template <typename T>
  static void SpecialComputeFP16(int64_t start, int64_t end, const T *input1, const T *input2, T *output);
};
}  // namespace aicpu
#endif
