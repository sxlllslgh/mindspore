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
#include <string>
#include "transform/symbol/symbol_utils.h"
#include "transform/symbol/acl_prof_symbol.h"

namespace mindspore {
namespace transform {
aclprofCreateConfigFunObj aclprofCreateConfig_ = nullptr;
aclprofDestroyConfigFunObj aclprofDestroyConfig_ = nullptr;
aclprofFinalizeFunObj aclprofFinalize_ = nullptr;
aclprofInitFunObj aclprofInit_ = nullptr;
aclprofStartFunObj aclprofStart_ = nullptr;
aclprofStopFunObj aclprofStop_ = nullptr;

void LoadProfApiSymbol(const std::string &ascend_path) {
  std::string profiler_plugin_path = "lib64/libmsprofiler.so";
  auto handler = GetLibHandler(ascend_path + profiler_plugin_path);
  if (handler == nullptr) {
    MS_LOG(WARNING) << "Dlopen " << profiler_plugin_path << " failed!" << dlerror();
    return;
  }
  aclprofCreateConfig_ = DlsymAscendFuncObj(aclprofCreateConfig, handler);
  aclprofDestroyConfig_ = DlsymAscendFuncObj(aclprofDestroyConfig, handler);
  aclprofFinalize_ = DlsymAscendFuncObj(aclprofFinalize, handler);
  aclprofInit_ = DlsymAscendFuncObj(aclprofInit, handler);
  aclprofStart_ = DlsymAscendFuncObj(aclprofStart, handler);
  aclprofStop_ = DlsymAscendFuncObj(aclprofStop, handler);
  MS_LOG(INFO) << "Load acl prof api success!";
}

}  // namespace transform
}  // namespace mindspore
