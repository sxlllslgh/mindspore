/**
 * Copyright 2024 Huawei Technologies Co., Ltd
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
#ifndef AICPU_KERNELS_NORMALIZED_RANDOM_CHOICE_WITH_MASKL_H_
#define AICPU_KERNELS_NORMALIZED_RANDOM_CHOICE_WITH_MASKL_H_
#include "inc/ms_cpu_kernel.h"
#include "utils/eigen_tensor.h"
#include "utils/kernel_util.h"
#include "random/utils.h"

namespace aicpu {
class RandomChoiceWithMaskCpuKernel : public CpuKernel {
 public:
  RandomChoiceWithMaskCpuKernel() = default;
  ~RandomChoiceWithMaskCpuKernel() = default;
  uint32_t Compute(CpuKernelContext &ctx) override;

 private:
  uint32_t RandomChoiceWithMaskCompute(const CpuKernelContext &ctx);

  int64_t count_target_;
  int64_t seed_;
  int64_t seed2_;
  std::vector<int64_t> dims_;
};
}  // namespace aicpu
#endif  // AICPU_KERNELS_NORMALIZED_RANDOM_CHOICE_WITH_MASKL_H_
