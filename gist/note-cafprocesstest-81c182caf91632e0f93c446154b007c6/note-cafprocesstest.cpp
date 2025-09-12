#include <list>

#include "pch.h"

class CafProcess {
 public:
  enum class eState { kContinue, kError, kFatal };

 public:
  class CafProcessBASE {
   public:
    virtual eState Init() = 0;
    virtual eState Prepare() = 0;
    virtual eState Update() = 0;
    virtual eState Cleanup() = 0;
    virtual eState Release() = 0;
    virtual bool Recover() = 0;
    virtual std::unique_ptr<CafProcessBASE> Clone() const = 0;
  };

  template <class ParentT>
  class CafProcessModel : public CafProcessBASE {
    ParentT parent_;

   public:
    CafProcessModel(const ParentT& parent) : parent_(parent) {}
    eState Init() override { return parent_.Init(); }
    eState Prepare() override { return parent_.Prepare(); }
    eState Update() override { return parent_.Update(); }
    eState Cleanup() override { return parent_.Cleanup(); }
    eState Release() override { return parent_.Release(); }
    bool Recover() override { return parent_.Recover(); }

    std::unique_ptr<CafProcessBASE> Clone() const override {
      return std::make_unique<CafProcessModel>(*this);
    }
  };

  std::unique_ptr<CafProcessBASE> pimpl_;

  template <class ParentT>
  CafProcess(const ParentT& parent)
      : pimpl_(new CafProcessModel<ParentT>(parent)) {}

  CafProcess(const CafProcess& s) : pimpl_{s.pimpl_->Clone()} {}

  CafProcess(CafProcess&& s) : pimpl_{std::move(s.pimpl_)} {}

  CafProcess& operator=(const CafProcess& s) {
    pimpl_ = s.pimpl_->Clone();
    return *this;
  }

  CafProcess& operator=(CafProcess&& s) {
    pimpl_ = std::move(s.pimpl_);
    return *this;
  }

  eState Init() { return pimpl_->Init(); }
  eState Prepare() { return pimpl_->Prepare(); }
  eState Update() { return pimpl_->Update(); }
  eState Cleanup() { return pimpl_->Cleanup(); }
  eState Release() { return pimpl_->Release(); }
  bool Recover() { return pimpl_->Recover(); }
};

struct CafProcessObject {
  friend CafProcess;
  bool Init() { return true; }
  bool Prepare() { return true; }
  bool Update() { return true; }
  bool Cleanup() { return true; }
  bool Release() { return true; }
};

struct CafCore {
  std::list<CafProcess> procs_;

  CafProcess::eState Init() {
    using enum CafProcess::eState;
    CafProcess::eState state = kContinue;
    for (auto proc_it = procs_.begin(); proc_it != procs_.end(); proc_it++) {
      state = proc_it->Init();

      if (state == kError && !proc_it->Recover()) {
        state = kFatal;
      }

      if (state == kFatal) break;
    }
    return state;
  }

  eState Prepare() {
    for (auto& process : processes) {
      if (!process.Prepare()) return eState::kError;
    }
    return eState::kGood;
  }

  eState Update() {
    for (auto& process : processes) {
      if (!process.Update()) return eState::kError;
    }
    return eState::kGood;
  }

  eState Cleanup() {
    for (auto& process : processes) {
      if (!process.Cleanup()) return eState::kError;
    }
    return eState::kGood;
  }

  eState Release() {
    for (auto& process : processes) {
      if (!process.Release()) return eState::kError;
    }
    return eState::kGood;
  }
};

// Initializes the CafCore singleton, does nothing if already initialized.
static CafCore& InitCafCore() {
  static std::unique_ptr<CafCore> cafcore_ptr;
  if (cafcore_ptr == nullptr) {
    cafcore_ptr = std::make_unique<CafCore>();
  }
  return *cafcore_ptr;
}
// Same as above but a diffrent name to show intent of only accessing the
// singleton.
static CafCore& GetCafCore() { return InitCafCore(); }

class CideLauncher : public CafProcessObject {
 public:
  CideLauncher(CafCore& core) : core_() {}

  CafProcess::eState Init() {

  }


};
class CideMain : public CafProcessObject {};

TEST(TestCaseName, TestName) {
  auto& core = InitCafCore();

  core.processes.push_back(CafProcess(CideLauncher()));

  std::vector<CafProcess> processes;

  processes.push_back(CafProcess(CideLauncher()));
  processes.push_back(CafProcess(CideLauncher()));

  for (auto& process : processes) {
    EXPECT_TRUE(process.Init());
    EXPECT_TRUE(process.Prepare());
    EXPECT_TRUE(process.Update());
    EXPECT_TRUE(process.Cleanup());
    EXPECT_TRUE(process.Release());
  }

  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}