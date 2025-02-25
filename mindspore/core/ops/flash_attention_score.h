/**
 * Copyright 2023 Huawei Technologies Co., Ltd
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
#ifndef MINDSPORE_CORE_OPS_FLASH_ATTENTION_SCORE_H_
#define MINDSPORE_CORE_OPS_FLASH_ATTENTION_SCORE_H_
#include <map>
#include <vector>
#include <memory>

#include "ops/base_operator.h"
#include "mindapi/base/types.h"
#include "ops/primitive_c.h"
#include "abstract/abstract_value.h"
#include "mindspore/core/ops/op_name.h"

namespace mindspore {
namespace ops {
constexpr auto kNameFlashAttentionScore = "FlashAttentionScore";
enum FlashAttentionScoreInputIndex : size_t {
  kFlashAttentionScoreInputQueryIndex = 0,
  kFlashAttentionScoreInputKeyIndex,
  kFlashAttentionScoreInputValueIndex,
  kFlashAttentionScoreInputRealShiftIndex,
  kFlashAttentionScoreInputDropMaskIndex,
  kFlashAttentionScoreInputPaddingMaskIndex,
  kFlashAttentionScoreInputAttnMaskIndex,
  kFlashAttentionScoreInputPrefixIndex,
  kFlashAttentionScoreInputsNum,
};
enum FlashAttentionScoreOutputIndex : size_t {
  kFlashAttentionScoreOutputSoftmaxMaxIndex = 0,
  kFlashAttentionScoreOutputSoftmaxSumIndex,
  kFlashAttentionScoreOutputSoftmaxOutIndex,
  kFlashAttentionScoreOutputAttentionOutIndex,
  kFlashAttentionScoreOutputsNum,
};
enum FlashAttentionScoreSparseMode : int64_t {
  kSparseDefaultMask = 0,
  kSparseAllMask,
  kSparseLeftDownCausal,
  kSparseRightDownCausal,
  kSparseBand,
  kSparsePrefix,
  kSparseGlobal,
  kSparseDilated,
  kSparseBlockLocal,
};
/// \brief FlashAttentionScore.
/// Refer to Python API @ref mindspore.ops.FlashAttentionScore for more details.
class MIND_API FlashAttentionScore : public BaseOperator {
 public:
  MIND_API_BASE_MEMBER(FlashAttentionScore);
  /// \brief Constructor.
  FlashAttentionScore() : BaseOperator(kNameFlashAttentionScore) {
    InitIOName({"query", "key", "value", "real_shift", "drop_mask", "padding_mask", "attn_mask", "prefix"},
               {"softmax_max", "softmax_sum", "softmax_out", "attention_out"});
  }
};
AbstractBasePtr FlashAttentionScoreInfer(const abstract::AnalysisEnginePtr &, const PrimitivePtr &primitive,
                                         const std::vector<AbstractBasePtr> &input_args);
using FlashAttentionScorePtr = std::shared_ptr<FlashAttentionScore>;
}  // namespace ops
}  // namespace mindspore

#endif  // MINDSPORE_CORE_OPS_FLASH_ATTENTION_SCORE_H_
