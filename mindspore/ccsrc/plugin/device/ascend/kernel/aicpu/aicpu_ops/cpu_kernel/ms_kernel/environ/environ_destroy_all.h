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

#ifndef AICPU_KERNELS_NORMALIZED_ENVIRON_DESTORY_ALL_H_
#define AICPU_KERNELS_NORMALIZED_ENVIRON_DESTORY_ALL_H_

#include "inc/ms_cpu_kernel.h"

namespace aicpu {
class EnvironDestroyAllKernel : public CpuKernel {
 public:
  EnvironDestroyAllKernel() = default;
  ~EnvironDestroyAllKernel() = default;
  uint32_t Compute(CpuKernelContext &ctx) override;

 private:
  uint32_t ParseKernelParam(const CpuKernelContext &ctx) const;
};
}  // namespace aicpu
#endif  // AICPU_KERNELS_NORMALIZED_ENVIRON_DESTORY_ALL_H_
