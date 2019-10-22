#include "oneflow/core/kernel/reduce_max_kernel.h"
#include "oneflow/core/ndarray/ndarray_util.h"

namespace oneflow {

template<DeviceType device_type, typename T>
void ReduceMaxKernel<device_type, T>::ForwardDataContent(
    const KernelCtx& ctx, std::function<Blob*(const std::string&)> BnInOp2Blob) const {
  const Blob* in_blob = BnInOp2Blob("in");
  Blob* out_blob = BnInOp2Blob("out");
  Blob* fw_tmp_blob = BnInOp2Blob("fw_tmp");
  const ReduceMaxOpConf& conf = this->op_conf().reduce_max_conf();
  const Shape& reduced_shape =
      conf.axis().empty()
          ? Shape::Ones(in_blob->shape().NumAxes())
          : in_blob->shape().CreateReducedShape({conf.axis().begin(), conf.axis().end()});
  NdarrayUtil<device_type, T>::ReduceMax(
      ctx.device_ctx, XpuVarNdarray<T>(reduced_shape, out_blob->mut_dptr<T>()),
      XpuVarNdarray<const T>(in_blob, in_blob->shape().NumAxes()),
      XpuVarNdarray<T>(fw_tmp_blob, in_blob->shape().NumAxes()));
}

ADD_DEFAULT_KERNEL_CREATOR_WITH_GPU_HALF(OperatorConf::kReduceMaxConf, ReduceMaxKernel,
                                         ARITHMETIC_DATA_TYPE_SEQ);

}  // namespace oneflow