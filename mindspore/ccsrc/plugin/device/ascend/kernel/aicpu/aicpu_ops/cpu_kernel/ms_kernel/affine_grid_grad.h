/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
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
#ifndef AICPU_KERNELS_NORMALIZED_Affine_Grid_Grad_H
#define AICPU_KERNELS_NORMALIZED_Affine_Grid_Grad_H

#include <Eigen/Dense>
#include <vector>
#include "inc/ms_cpu_kernel.h"
#include "cpu_types.h"
#include "utils/bcast.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;
namespace aicpu {
class AffineGridGradCpuKernel : public CpuKernel {
 public:
  AffineGridGradCpuKernel() = default;
  ~AffineGridGradCpuKernel() override = default;

 protected:
  uint32_t Compute(CpuKernelContext &ctx) override;

 private:
  template <typename T0>
  uint32_t AffineGridGradCheck(const CpuKernelContext &ctx);
  template <typename T, typename T0>
  uint32_t AffineGridGradCompute(const CpuKernelContext &ctx);
  template <typename T, typename T0>
  uint32_t AffineGridGradCompute_4D(const CpuKernelContext &ctx, bool align_corners);
  template <typename T0>
  Eigen::MatrixXf make_base_grid_4D(const CpuKernelContext &ctx, Eigen::VectorXd vecX, Eigen::VectorXd vecY);
  template <typename T, typename T0>
  uint32_t DoCompute_4D(const CpuKernelContext &ctx, Eigen::MatrixXf all);
  template <typename T, typename T0>
  uint32_t AffineGridGradCompute_5D(const CpuKernelContext &ctx, bool align_corners);
  template <typename T0>
  Eigen::MatrixXf make_base_grid_5D(const CpuKernelContext &ctx, Eigen::VectorXd vecX, Eigen::VectorXd vecY,
                                    Eigen::VectorXd vecZ);
  template <typename T, typename T0>
  uint32_t DoCompute_5D(const CpuKernelContext &ctx, Eigen::MatrixXf all);
};
}  // namespace aicpu
#endif
