/**
 * Copyright 2019-2024 Huawei Technologies Co., Ltd
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

#ifndef MINDSPORE_CCSRC_TRANSFORM_GRAPH_IR_OP_DECLARE_QUANTIZE_OPS_DECLARE_H_
#define MINDSPORE_CCSRC_TRANSFORM_GRAPH_IR_OP_DECLARE_QUANTIZE_OPS_DECLARE_H_

#include "op_proto/inc/quantize_ops.h"
#include "op_proto/inc/fusion_ops.h"
#include "op_proto/inc/matrix_calculation_ops.h"
#include "transform/graph_ir/op_declare/op_declare_macro.h"
#include "utils/hash_map.h"

DECLARE_OP_ADAPTER(AscendQuant)
DECLARE_OP_USE_OUTPUT(AscendQuant)

DECLARE_OP_ADAPTER(AscendDequant)
DECLARE_OP_USE_OUTPUT(AscendDequant)

DECLARE_OP_ADAPTER(AscendAntiQuant)
DECLARE_OP_USE_OUTPUT(AscendAntiQuant)

DECLARE_OP_ADAPTER(QuantBatchMatmulV3)
DECLARE_OP_USE_OUTPUT(QuantBatchMatmulV3)

DECLARE_OP_ADAPTER(AscendAntiQuantV2)
DECLARE_OP_USE_OUTPUT(AscendAntiQuantV2)

DECLARE_OP_ADAPTER(WeightQuantBatchMatmulV2)
DECLARE_OP_USE_OUTPUT(WeightQuantBatchMatmulV2)
#endif  // MINDSPORE_CCSRC_TRANSFORM_GRAPH_IR_OP_DECLARE_QUANTIZE_OPS_DECLARE_H_
