#ifndef ONEFLOW_CORE_ACTOR_BACKWARD_COMPUTE_ACTOR_H_
#define ONEFLOW_CORE_ACTOR_BACKWARD_COMPUTE_ACTOR_H_

#include "oneflow/core/actor/compute_actor.h"

namespace oneflow {

class NormalBackwardCompActor final : public CompActor {
 public:
  OF_DISALLOW_COPY_AND_MOVE(NormalBackwardCompActor);
  NormalBackwardCompActor() = default;
  ~NormalBackwardCompActor() = default;

  void VirtualCompActorInit(const TaskProto&) override;

 private:
  void ForEachCurCustomizedReadableRegst(std::function<void(const Regst*)>) override;
  void NormalProcessNaiveReadableRegstMsg(const std::deque<Regst*>&) override;
  void NormalProcessCustomizedReadableRegstMsg(const ActorMsg&) override;
  void Act() override;
  bool IsCustomizedReadReady() override;
  void AsyncReturnAllCustomizedReadableRegst() override;
  std::pair<bool, std::vector<std::string>> GetNaiveConsumedRegstDescName() override {
    return {false, {"activation", "data_tmp", "boxing_out", "121_out", "out_diff"}};
  }

  void AsyncReturnModelRegstUntilModelVersionIdEqual(int64_t model_version_id);
  void AsyncReturnModelRegstUntilLastPieceIdGreaterThan(int64_t piece_id);
  
  int64_t b121_out_regst_desc_id_;

  int64_t model_regst_desc_id_;
  int64_t model_tmp_regst_desc_id_;
  std::queue<Regst*> model_regst_queue_;
  Regst* model_tmp_regst_;

};

}  // namespace oneflow

#endif  // ONEFLOW_CORE_ACTOR_BACKWARD_COMPUTE_ACTOR_H_
