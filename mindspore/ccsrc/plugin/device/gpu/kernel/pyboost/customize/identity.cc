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

#include "plugin/device/gpu/kernel/pyboost/customize/identity.h"
#include <memory>
#include <utility>
#include "plugin/device/gpu/hal/device/gpu_device_manager.h"
#include "mindspore/ccsrc/kernel/pyboost/customize/identity.h"

namespace mindspore {
namespace kernel {
namespace pyboost {
tensor::TensorPtr IdentityGPUCustomize(const std::shared_ptr<OpRunner> &op, const TensorPtr &x_tensor) {
  MS_LOG(DEBUG) << "Identity call start";
  auto stream = device::gpu::GPUDeviceManager::GetInstance().GetStream(op->stream_id());
  IdentityCustomize(op, x_tensor, stream);
  static auto sync = MsContext::GetInstance()->get_param<bool>(MS_CTX_ENABLE_PYNATIVE_SYNCHRONIZE);
  if (sync && !op->device_context()->device_res_manager_->SyncAllStreams()) {
    MS_LOG(EXCEPTION) << "SyncStream failed for op Identity.";
  }
  MS_LOG(DEBUG) << "Identity call end";
  return op->output(0);
}
}  // namespace pyboost
}  // namespace kernel
}  // namespace mindspore
