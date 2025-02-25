# Copyright 2022 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================

import numpy as np
import pytest

import mindspore as ms
from mindspore import Tensor
import mindspore.context as context
from mindspore.ops import operations as P


def test_float_tensor_api():
    """
    Feature: test float tensor API.
    Description: test float dtype tensor conversion.
    Expectation: the input and output shape should be same. output dtype should be float32.
    """
    dtype_op = P.DType()
    x = Tensor(np.ones([2, 3, 1]), ms.int32)
    output = x.float()
    assert x.shape == output.shape
    assert dtype_op(output) == ms.float32


@pytest.mark.level1
@pytest.mark.platform_arm_ascend_training
@pytest.mark.platform_x86_ascend_training
@pytest.mark.env_onecard
def test_float_tensor_api_modes():
    """
    Feature: test float tensor API for different modes.
    Description: test float dtype tensor conversion.
    Expectation: the input and output shape should be same. output dtype should be float32.
    """
    context.set_context(mode=context.GRAPH_MODE, device_target="Ascend")
    test_float_tensor_api()
    context.set_context(mode=context.PYNATIVE_MODE, device_target="Ascend")
    test_float_tensor_api()
