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

#ifndef AICPU_KERNELS_HAMMING_WINDOW_H_
#define AICPU_KERNELS_HAMMING_WINDOW_H_

#include "inc/ms_cpu_kernel.h"
#include "cpu_types.h"

namespace aicpu {
class HammingWindowCpuKernel : public CpuKernel {
 public:
  HammingWindowCpuKernel() = default;
  ~HammingWindowCpuKernel() = default;
  uint32_t Compute(CpuKernelContext &ctx) override;

 private:
  template <typename T>
  uint32_t HammingWindowCompute(const CpuKernelContext &ctx);
};
}  // namespace aicpu
#endif
