/**
 * Copyright 2023-2024 Huawei Technologies Co., Ltd
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

#include "plugin/device/gpu/kernel/arrays/gather_d_grad_gpu_kernel.h"
#include <functional>
#include "mindspore/core/ops/array_ops.h"
#include "plugin/device/gpu/kernel/cuda_impl/cuda_ops/complex.h"
#include "plugin/device/gpu/hal/device/gpu_device_address.h"
#include "ops/auto_generate/gen_ops_name.h"

namespace mindspore {
namespace kernel {
template <typename T>
using Complex = mindspore::utils::Complex<T>;

template <typename T, typename S>
bool GatherDGradGpuKernelMod::LaunchKernel(const std::vector<KernelTensor *> &inputs,
                                           const std::vector<KernelTensor *> &workspace,
                                           const std::vector<KernelTensor *> &outputs) {
  auto index_addr = reinterpret_cast<T *>(inputs.at(kIndex2)->device_ptr());
  auto grad_addr = reinterpret_cast<S *>(inputs.at(kIndex3)->device_ptr());
  auto output_addr = reinterpret_cast<S *>(outputs.at(kIndex0)->device_ptr());
  auto cuda_stream = reinterpret_cast<cudaStream_t>(cuda_stream_);

  CHECK_CUDA_RET_WITH_ERROR_NOTRACE(cudaMemsetAsync(output_addr, 0, outputs[kIndex0]->size(), cuda_stream),
                                    "GatherGrad cudaMemSet Failed");

  auto index_size =
    static_cast<size_t>(std::accumulate(index_shapes_.begin(), index_shapes_.end(), 1, std::multiplies<int64_t>()));
  auto status = GatherGrad(index_addr, grad_addr, output_addr, dims_[0], dims_[1], dims_[2], dims_[3], dims_[4],
                           index_size, cuda_stream);
  CHECK_CUDA_STATUS(status, kernel_name_);
  return true;
}

bool GatherDGradGpuKernelMod::Init(const std::vector<KernelTensor *> &inputs,
                                   const std::vector<KernelTensor *> &outputs) {
  if (auto ret = MatchKernelFunc(kernel_name_, inputs, outputs); !ret) {
    return ret;
  }
  return true;
}

void GatherDGradGpuKernelMod::CalculateDim(int64_t dim_value) {
  int64_t rank = SizeToLong(output_shapes_.size());

  if (dim_value < 0) {
    dim_value += rank;
  }
  auto dim_size = static_cast<size_t>(dim_value);
  int64_t dim_before_axis_index = 1;
  for (size_t i = 0; i < dim_size; i++) {
    dim_before_axis_index *= index_shapes_[i];
  }
  auto dim_at_axis_index = static_cast<size_t>(index_shapes_[dim_size]);
  auto dim_at_axis_out = static_cast<size_t>(output_shapes_[dim_size]);
  int64_t dim_after_axis_out = 1;
  for (size_t i = dim_size + 1; i < output_shapes_.size(); i++) {
    dim_after_axis_out *= output_shapes_[i];
  }

  int64_t dim_after_axis_index = 1;
  for (size_t i = dim_size + 1; i < index_shapes_.size(); i++) {
    dim_after_axis_index *= index_shapes_[i];
  }

  dims_[kIndex0] = static_cast<size_t>(dim_before_axis_index);
  dims_[kIndex1] = dim_at_axis_index;
  dims_[kIndex2] = static_cast<size_t>(dim_after_axis_index);
  dims_[kIndex3] = dim_at_axis_out;
  dims_[kIndex4] = static_cast<size_t>(dim_after_axis_out);
}

int GatherDGradGpuKernelMod::Resize(const std::vector<KernelTensor *> &inputs,
                                    const std::vector<KernelTensor *> &outputs) {
  auto ret = KernelMod::Resize(inputs, outputs);
  index_shapes_ = inputs.at(kIndex2)->GetShapeVector();
  output_shapes_ = outputs.at(kIndex0)->GetShapeVector();
  grad_shapes_ = inputs.at(kIndex3)->GetShapeVector();

  if (output_shapes_.empty()) {
    output_shapes_ = ShapeVector{1};
  }

  if (index_shapes_.empty()) {
    index_shapes_ = ShapeVector{1};
  }

  auto dim = inputs[kIndex1]->GetValueWithCheck<int64_t>();
  CalculateDim(dim);
  return static_cast<int>(ret);
}

#define REG_INDEX(INPUT_DT, INDEX_DT, INPUT_T, INDEX_T)        \
  {                                                            \
    KernelAttr()                                               \
      .AddInputAttr(INPUT_DT)                                  \
      .AddInputAttr(kObjectTypeNumber, kNumberTypeInt64)       \
      .AddInputAttr(INDEX_DT)                                  \
      .AddInputAttr(INPUT_DT)                                  \
      .AddOutputAttr(INPUT_DT),                                \
      &GatherDGradGpuKernelMod::LaunchKernel<INDEX_T, INPUT_T> \
  }

#define GATHER_D_GRAD_V2_GPU_REGISTER(DT, T) \
  REG_INDEX(DT, kNumberTypeInt64, T, int64_t), REG_INDEX(DT, kNumberTypeInt32, T, int32_t)

const std::vector<std::pair<KernelAttr, GatherDGradGpuKernelMod::KernelRunFunc>> &GatherDGradGpuKernelMod::GetFuncList()
  const {
  static const std::vector<std::pair<KernelAttr, GatherDGradGpuKernelMod::KernelRunFunc>> func_list = {
    GATHER_D_GRAD_V2_GPU_REGISTER(kNumberTypeComplex64, Complex<float>),
    GATHER_D_GRAD_V2_GPU_REGISTER(kNumberTypeComplex128, Complex<double>),
    GATHER_D_GRAD_V2_GPU_REGISTER(kNumberTypeFloat16, half),
    GATHER_D_GRAD_V2_GPU_REGISTER(kNumberTypeFloat32, float),
    GATHER_D_GRAD_V2_GPU_REGISTER(kNumberTypeFloat64, double),
    GATHER_D_GRAD_V2_GPU_REGISTER(kNumberTypeInt8, int8_t),
    GATHER_D_GRAD_V2_GPU_REGISTER(kNumberTypeInt16, int16_t),
    GATHER_D_GRAD_V2_GPU_REGISTER(kNumberTypeInt32, int32_t),
    GATHER_D_GRAD_V2_GPU_REGISTER(kNumberTypeInt64, int64_t),
    GATHER_D_GRAD_V2_GPU_REGISTER(kNumberTypeUInt8, uchar),
    GATHER_D_GRAD_V2_GPU_REGISTER(kNumberTypeUInt16, uint16_t),
    GATHER_D_GRAD_V2_GPU_REGISTER(kNumberTypeUInt32, uint32_t),
    GATHER_D_GRAD_V2_GPU_REGISTER(kNumberTypeUInt64, uint64_t),
    GATHER_D_GRAD_V2_GPU_REGISTER(kNumberTypeBool, bool)};
  return func_list;
}

MS_KERNEL_FACTORY_REG(NativeGpuKernelMod, GatherDGradV2, GatherDGradGpuKernelMod);
}  // namespace kernel
}  // namespace mindspore
