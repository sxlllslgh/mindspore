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

#ifndef AICPU_KERNELS_NORMALIZED_SCATTERND_H_
#define AICPU_KERNELS_NORMALIZED_SCATTERND_H_

#include <string.h>

#include "inc/ms_cpu_kernel.h"
#include "cpu_types.h"
#include "utils/bcast.h"

namespace aicpu {
class ScatterNdCpuKernel : public CpuKernel {
 public:
  ScatterNdCpuKernel() = default;
  ~ScatterNdCpuKernel() override = default;
  uint32_t Compute(CpuKernelContext &ctx) override;

 private:
  template <typename data_type0>
  uint32_t DTYPE_CHOOSE(const CpuKernelContext &ctx);

  template <typename indices_type, typename shape_type, typename data_type0>
  uint32_t ScatterNdComputeRealKernel(const CpuKernelContext &ctx);
};
}  // namespace aicpu
#endif
