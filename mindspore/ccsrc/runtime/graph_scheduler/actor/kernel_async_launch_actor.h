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

#ifndef MINDSPORE_CCSRC_RUNTIME_FRAMEWORK_ACTOR_KERNEL_ASYNC_LAUNCH_ACTOR_H_
#define MINDSPORE_CCSRC_RUNTIME_FRAMEWORK_ACTOR_KERNEL_ASYNC_LAUNCH_ACTOR_H_

#include <vector>
#include <memory>

#include "runtime/graph_scheduler/actor/actor_common.h"
#include "kernel/kernel.h"
#include "runtime/hardware/device_context.h"
#include "include/backend/visible.h"

namespace mindspore {
namespace runtime {
class KernelActor;

class BACKEND_EXPORT KernelAsyncLaunchActor : public ActorBase {
 public:
  static std::shared_ptr<KernelAsyncLaunchActor> &GetInstance() {
    static std::shared_ptr<KernelAsyncLaunchActor> instance =
      std::shared_ptr<KernelAsyncLaunchActor>(new KernelAsyncLaunchActor());
    return instance;
  }
  ~KernelAsyncLaunchActor() override = default;

  void LaunchKernel(OpContext<DeviceTensor> *const context, KernelActor *kernel_actor);

  void Wait();

  Future<bool> OnTaskFinish();

 private:
  KernelAsyncLaunchActor() : ActorBase("KernelAsyncLaunchActor") {}
  DISABLE_COPY_AND_ASSIGN(KernelAsyncLaunchActor);
};
}  // namespace runtime
}  // namespace mindspore

#endif  // MINDSPORE_CCSRC_RUNTIME_FRAMEWORK_ACTOR_KERNEL_ASYNC_LAUNCH_ACTOR_H_
