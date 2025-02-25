# Copyright 2020 Huawei Technologies Co., Ltd
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

import mindspore.context as context
import mindspore.ops.operations as P
from mindspore.common.tensor import Tensor
from mindspore.nn import Cell
from mindspore.train import Model
import mindspore
import pytest


context.set_context(mode=context.GRAPH_MODE, device_target="Ascend")


class Net(Cell):
    def __init__(self, begin, end, stride):
        super(Net, self).__init__()
        self.stridedslice = P.StridedSlice()
        self.begin = begin
        self.end = end
        self.stride = stride

    def construct(self, input_):
        x = self.stridedslice(input_, self.begin, self.end, self.stride)
        return x


def me_stridedslice(input1, begin, end, stride, mstype=None):
    input_me = Tensor(input1)
    if mstype == mindspore.bfloat16:
        input_me = Tensor(input1, mindspore.bfloat16)
    net = Net(begin, end, stride)
    net.set_train()
    model = Model(net)
    output = model.predict(input_me)
    if mstype == mindspore.bfloat16:
        print(output.float().asnumpy())
    else:
        print(output.asnumpy())


def test_stridedslice_input_2d():
    input_ = np.random.randn(5, 5).astype(np.int32)
    begin = (0, 0)
    end = (2, 2)
    stride = (1, 1)

    me_stridedslice(input_, begin, end, stride)


def test_stridedslice_input_3d():
    input_ = np.random.randn(5, 5, 5).astype(np.float32)
    begin = (0, 0, 0)
    end = (3, 3, 3)
    stride = (1, 1, 1)
    me_stridedslice(input_, begin, end, stride)

@pytest.mark.level1
@pytest.mark.platform_arm_ascend910b_training
@pytest.mark.env_onecard
def test_stridedslice_input_3d_bf16():
    """
    Feature: test fast_gelu functional API.
    Description: Operation selects input is Tensor with bfloat16 type.
    Expectation: execute without error.
    """
    input_ = np.random.randn(5, 5, 5).astype(np.float32)
    begin = (0, 0, 0)
    end = (3, 3, 3)
    stride = (1, 1, 1)
    me_stridedslice(input_, begin, end, stride, mstype=mindspore.bfloat16)
