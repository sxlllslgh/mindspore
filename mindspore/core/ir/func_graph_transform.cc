/**
 * This is the C++ adaptation and derivative work of Myia (https://github.com/mila-iqia/myia/).
 *
 * Copyright 2019-2021 Huawei Technologies Co., Ltd
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
#include "ir/func_graph_transform.h"

#include <memory>

#include "ir/primitive.h"
#include "ir/func_graph.h"

namespace mindspore {
const PrimitivePtr FuncGraphTransform::func_graph_prim_ = std::make_shared<Primitive>("FuncGraph");

FuncGraphTransform::FuncGraphTransform(const FuncGraphPtr &func_graph, const PrimitivePtr &prim,
                                       const CNodePtr &primal_cnode)
    : prim_(prim), func_graph_(FuncGraphWeakPtr(func_graph)), primal_cnode_(primal_cnode) {
  func_graph->set_reserved(true);
}
}  // namespace mindspore
