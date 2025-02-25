/**
 * Copyright 2022 Huawei Technologies Co., Ltd
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

#ifndef MINDSPORE_MINDSPORE_CCSRC_PIPELINE_PYNATIVE_FORWARD_DO_CAST_PYBOOST_H_
#define MINDSPORE_MINDSPORE_CCSRC_PIPELINE_PYNATIVE_FORWARD_DO_CAST_PYBOOST_H_

#include <vector>
#include <string>
#include <memory>
#include <tuple>
#include <utility>
#include "pipeline/pynative/forward/cast_base.h"
#include "frontend/operator/composite/do_signature.h"
#include "ir/cell.h"

namespace mindspore {
namespace pynative {
static constexpr auto kCast = "Cast";

class PyBoostCastOperation : public CastBaseOperation {
 public:
  PyBoostCastOperation() = default;
  ~PyBoostCastOperation() = default;

  template <typename... InputArgs>
  auto DoMixPrecisionCast(const FrontendOpRunInfoPtr &op_run_info, const InputArgs &... input_args) {
    // Mixed precision conversion tensors which has cast dtype
    if (op_run_info->async_status.disable_mix_precision) {
      return std::make_tuple(input_args...);
    }
    size_t index = sizeof...(input_args);
    auto decrease_index_fn = [&index]() { return --index; };
    // Notice, the input_args of variadic template in make_tuple obtaining is reverse
    auto ret = std::make_tuple(SetTensorMixPrecisionCast(op_run_info, input_args, decrease_index_fn())...);
    return ret;
  }

  template <typename T>
  T SetTensorMixPrecisionCast(const FrontendOpRunInfoPtr &op_run_info, const T &t, size_t index) const {
    MS_EXCEPTION_IF_NULL(t);
    MS_LOG(DEBUG) << "Get input type " << typeid(t).name();
    return t;
  }

  template <typename TupleInput>
  std::optional<ValuePtr> VisitHelp(size_t i, const TupleInput &input_args, std::index_sequence<>) {
    return std::get<0>(input_args);
  }

  template <typename TupleInput, size_t... I>
  std::optional<ValuePtr> VisitHelp(size_t i, const TupleInput &input_args, std::index_sequence<I...>) {
    constexpr size_t index = std::index_sequence<I...>::size() - 1;
    return i == index ? std::get<index>(input_args) : VisitHelp(i, input_args, std::make_index_sequence<index>{});
  }

  template <typename TupleInput>
  std::optional<ValuePtr> Visit(size_t i, const TupleInput &input_args) {
    return VisitHelp(i, input_args, std::make_index_sequence<std::tuple_size<TupleInput>::value>{});
  }

  template <typename TupleInput>
  void GetEachTypeMaxType(const FrontendOpRunInfoPtr &op_run_info, const std::vector<size_t> &same_type_index,
                          const std::vector<SignatureEnumDType> &dtypes, const TupleInput &input_args,
                          mindspore::HashMap<SignatureEnumDType, std::pair<TypeId, bool>> *dst_type) {
    MS_EXCEPTION_IF_NULL(op_run_info);
    constexpr size_t index_size = 2;
    const auto &type = dtypes[same_type_index.front()];
    if (type == SignatureEnumDType::kDTypeEmptyDefaultValue || same_type_index.size() < index_size) {
      return;
    }
    int64_t priority = INT_MIN;
    TypeId max_type = TypeId::kTypeUnknown;
    bool has_scalar_float32 = false;
    bool has_scalar_int64 = false;
    bool has_tensor_int8 = false;
    // The indexes value has tensor input
    bool has_tensor_input = false;
    // Find the maximum priority of the same dtype
    for (size_t index : same_type_index) {
      const auto &optional_v = Visit(index, input_args);
      if (!optional_v) {
        continue;
      }
      auto v = optional_v.value();
      if (v->template isa<tensor::Tensor>()) {
        has_tensor_input = true;
        auto arg = v->template cast<tensor::TensorPtr>();
        TypeId arg_type_id = arg->data_type();
        auto type_priority = prim::type_map.find(arg_type_id);
        if (type_priority == prim::type_map.end()) {
          continue;
        }
        if (arg_type_id == kNumberTypeInt8) {
          has_tensor_int8 = true;
        }
        int64_t cur_priority = type_priority->second;
        if (op_run_info->source_type[index] != ops::OP_DTYPE::DT_BEGIN) {
          cur_priority = cur_priority - kLowerPriority;
          if (arg_type_id == kNumberTypeFloat32) {
            has_scalar_float32 = true;
          }
          if (arg_type_id == kNumberTypeInt32 || arg_type_id == kNumberTypeInt64) {
            has_scalar_int64 = true;
          }
        }
        if (cur_priority > priority) {
          max_type = type_priority->first;
          priority = cur_priority;
        }
      } else if (v->template isa<FloatImm>()) {
        has_scalar_float32 = true;
      } else if ((!v->template isa<BoolImm>() && v->template isa<IntegerImm>())) {
        has_scalar_int64 = true;
      }
    }

    max_type = JudgeMaxType(max_type, has_scalar_float32, has_scalar_int64, has_tensor_int8);
    MS_EXCEPTION_IF_NULL(dst_type);
    (*dst_type)[type] = std::make_pair(max_type, has_tensor_input);
  }

  template <size_t N, typename TupleInput>
  std::enable_if_t<N == 1> GetDstTypeForEachType(
    const FrontendOpRunInfoPtr &op_run_info, const std::vector<std::vector<size_t>> &same_type_table,
    const std::vector<SignatureEnumDType> &dtypes, const TupleInput &input_args,
    mindspore::HashMap<SignatureEnumDType, std::pair<TypeId, bool>> *dst_type) {
    GetEachTypeMaxType(op_run_info, same_type_table[N - 1], dtypes, input_args, dst_type);
  }

  template <size_t N, typename TupleInput>
  std::enable_if_t<N != 1> GetDstTypeForEachType(
    const FrontendOpRunInfoPtr &op_run_info, const std::vector<std::vector<size_t>> &same_type_table,
    const std::vector<SignatureEnumDType> &dtypes, const TupleInput &input_args,
    mindspore::HashMap<SignatureEnumDType, std::pair<TypeId, bool>> *dst_type) {
    GetEachTypeMaxType(op_run_info, same_type_table[N - 1], dtypes, input_args, dst_type);
    GetDstTypeForEachType<N - 1>(op_run_info, same_type_table, dtypes, input_args, dst_type);
  }

  // Implicit transform
  template <size_t N, typename... InputArgs>
  auto DoImplicitCast(const FrontendOpRunInfoPtr &op_run_info, const std::vector<std::vector<size_t>> &same_type_table,
                      const std::tuple<InputArgs...> &input_args) {
    MS_EXCEPTION_IF_NULL(op_run_info);
    std::vector<SignatureEnumDType> dtypes;
    mindspore::HashMap<SignatureEnumDType, std::pair<TypeId, bool>> dst_type;
    const auto &it = implicit_cast_map_.find(op_run_info->base_op_run_info.op_name);
    if (it == implicit_cast_map_.end()) {
      // Get current inputs signatures
      bool has_dtype_sig = GetSignatureType(op_run_info->signatures, &dtypes);
      if (!has_dtype_sig) {
        PrimSignature sig_value{has_dtype_sig, {}, {}};
        implicit_cast_map_[op_run_info->base_op_run_info.op_name] = sig_value;
        return input_args;
      }
      PrimSignature sig_value{has_dtype_sig, dtypes, {}};
      implicit_cast_map_[op_run_info->base_op_run_info.op_name] = sig_value;
    } else {
      dtypes = it->second.dtypes;
    }

    GetDstTypeForEachType<N>(op_run_info, same_type_table, dtypes, input_args, &dst_type);
    return SetImplicitCast(op_run_info, dst_type, dtypes, input_args, std::make_index_sequence<sizeof...(InputArgs)>{});
  }

 private:
  template <typename TupleInput, size_t... N>
  auto SetImplicitCast(const FrontendOpRunInfoPtr &op_run_info,
                       const mindspore::HashMap<SignatureEnumDType, std::pair<TypeId, bool>> &dst_type,
                       const std::vector<SignatureEnumDType> &dtypes, const TupleInput &input_args,
                       std::index_sequence<N...>) const {
    MS_EXCEPTION_IF_NULL(op_run_info);
    return std::make_tuple(DoSignatureCast(op_run_info, dst_type, dtypes, N, std::get<N>(input_args))...);
  }

  template <typename Item>
  Item DoSignatureCast(const FrontendOpRunInfoPtr &op_run_info,
                       const mindspore::HashMap<SignatureEnumDType, std::pair<TypeId, bool>> &dst_type,
                       const std::vector<SignatureEnumDType> &dtypes, size_t index, const Item &t) const {
    // No need to implicit cast if no dtype.
    const auto &signature = op_run_info->signatures;
    if (dtypes.empty() || dtypes[index] == SignatureEnumDType::kDTypeEmptyDefaultValue) {
      return t;
    }
    auto it = dst_type.find(dtypes[index]);
    if (it == dst_type.end() || it->second.first == kTypeUnknown) {
      return t;
    }

    TypeId arg_type_id = kTypeUnknown;
    if (t->template isa<tensor::MetaTensor>()) {
      const auto &arg = t->template cast<tensor::MetaTensorPtr>();
      arg_type_id = arg->data_type();
    }
    // Implicit cast
    bool is_same_type = false;
    if (arg_type_id != kTypeUnknown) {
      is_same_type = (prim::type_map.find(arg_type_id) == prim::type_map.end() || arg_type_id == it->second.first);
    }
    if (signature[index].rw == SignatureEnumRW::kRWWrite && arg_type_id != kTypeUnknown && !is_same_type) {
      prim::RaiseExceptionForConvertRefDtype(op_run_info->op_grad_info->op_prim, TypeIdToMsTypeStr(arg_type_id),
                                             TypeIdToMsTypeStr(it->second.first), index);
    }
    if (is_same_type) {
      return t;
    }

    if (IsValueTypeInvalid(t)) {
      std::string type_str = t->type() == nullptr ? "None, value is \"" + t->ToString() + "\"" : t->type()->ToString();
      MS_EXCEPTION(TypeError) << "For '" << op_run_info->op_grad_info->op_prim->name() << "', the " << (index + 1)
                              << "th input " << signature[index].name << " can not be implicitly converted. "
                              << "Its type is " << type_str << ". Only support Tensor or Scalar.";
    }
    MS_LOG(DEBUG) << "Implicit cast for " << op_run_info->base_op_run_info.op_name << " " << index
                  << "th input, and to type " << TypeIdToType(it->second.first)->ToString();
    // Has tensor input
    return DoAutoCast(op_run_info, it->second, index, t);
  }

  template <typename Item>
  std::optional<Item> DoSignatureCast(const FrontendOpRunInfoPtr &op_run_info,
                                      const mindspore::HashMap<SignatureEnumDType, std::pair<TypeId, bool>> &dst_type,
                                      const std::vector<SignatureEnumDType> &dtypes, size_t index,
                                      const std::optional<Item> &t) const {
    if (!t.has_value()) {
      return std::nullopt;
    }
    return std::make_optional(DoSignatureCast(op_run_info, dst_type, dtypes, index, t.value()));
  }

  template <class Item>
  bool IsValueTypeInvalid(const Item &v) const {
    MS_EXCEPTION_IF_NULL(v);
    return !v->template isa<tensor::Tensor>() && !v->template isa<tensor::CSRTensor>() &&
           !v->template isa<IntegerImm>() && !v->template isa<FloatImm>() && !v->template isa<BoolImm>();
  }

  //  template <class Item, class = typename std::enable_if<std::is_same<Item, tensor::Tensor>::value, Item>::type>
  template <class Item>
  Item DoAutoCast(const FrontendOpRunInfoPtr &op_run_info, const std::pair<TypeId, bool> &dst_type, size_t index,
                  const Item &t) const {
    MS_EXCEPTION_IF_NULL(t);
    MS_LOG(DEBUG) << "Get input type " << typeid(t).name();
    return t;
  }

  ValuePtr DoAutoCast(const FrontendOpRunInfoPtr &op_run_info, const std::pair<TypeId, bool> &dst_type, size_t index,
                      const ValuePtr &v) const;
  tensor::TensorPtr DoAutoCast(const FrontendOpRunInfoPtr &op_run_info, const std::pair<TypeId, bool> &dst_type,
                               size_t index, const tensor::TensorPtr &t) const;
  ValuePtr SetTensorMixPrecisionCast(const FrontendOpRunInfoPtr &op_run_info, const ValuePtr &v, size_t index) const;
  tensor::TensorPtr SetTensorMixPrecisionCast(const FrontendOpRunInfoPtr &op_run_info, const tensor::TensorPtr &t,
                                              size_t index) const;
  std::optional<tensor::TensorPtr> SetTensorMixPrecisionCast(const FrontendOpRunInfoPtr &op_run_info,
                                                             const std::optional<tensor::TensorPtr> &t,
                                                             size_t index) const;
  ValueTuplePtr SetTensorMixPrecisionCast(const FrontendOpRunInfoPtr &op_run_info, const ValueTuplePtr &v_tuple,
                                          size_t index) const;
  ValueListPtr SetTensorMixPrecisionCast(const FrontendOpRunInfoPtr &op_run_info, const ValueListPtr &v_list,
                                         size_t index) const;
  ValuePtrList SetSeqMixPrecisionCast(const FrontendOpRunInfoPtr &op_run_info, const ValueSequencePtr &v_seq,
                                      size_t index) const;
};
using PyBoostCastOperationPtr = std::shared_ptr<PyBoostCastOperation>;

}  // namespace pynative
}  // namespace mindspore
#endif  // MINDSPORE_MINDSPORE_CCSRC_PIPELINE_PYNATIVE_FORWARD_DO_CAST_PYBOOST_H_
