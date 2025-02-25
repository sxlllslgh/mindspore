/**
 * Copyright 2021 Jilin University
 * Copyright 2020 Huawei Technologies Co., Ltd.
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

#ifndef AICPU_KERNELS_NORMALIZED_ADJUST_HUE_H_
#define AICPU_KERNELS_NORMALIZED_ADJUST_HUE_H_

#include "inc/ms_cpu_kernel.h"

namespace aicpu {
struct ComputeOptions {
  const Tensor *input;
  const Tensor *delta;
  Tensor *output;

  int64_t channel_count;
};
struct HsvTuple {
  float h;
  float s;
  float v;
};
struct RgbTuple {
  float r;
  float g;
  float b;
};

class AdjustHueCpuKernel final : public CpuKernel {
 public:
  AdjustHueCpuKernel() = default;
  ~AdjustHueCpuKernel() override = default;

 protected:
  uint32_t Compute(CpuKernelContext &ctx) override;

 private:
  template <typename T>
  uint32_t DoCompute(const CpuKernelContext &ctx, const ComputeOptions &options);
  template <typename T>
  uint32_t DoComputeHalf(const CpuKernelContext &ctx, const ComputeOptions &options);
};
}  // namespace aicpu
#endif
