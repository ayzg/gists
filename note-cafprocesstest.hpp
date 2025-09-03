#pragma once
#pragma once
#include <concepts>
#include <memory>

namespace caf {

class CafProcess {
 public:
  class CafProcessBASE {
   public:
    virtual bool Init() = 0;
    virtual bool Prepare() = 0;
    virtual bool Update() = 0;
    virtual bool Cleanup() = 0;
    virtual bool Release() = 0;
    virtual std::unique_ptr<CafProcessBASE> Clone() const = 0;
  };

  template <class ParentT>
  class CafProcessModel : public CafProcessBASE {
    ParentT parent_;

   public:
    CafProcessModel(const ParentT& parent) : parent_(parent) {}
    bool Init() override { return parent_.Init(); }
    bool Prepare() override { return parent_.Prepare(); }
    bool Update() override { return parent_.Update(); }
    bool Cleanup() override { return parent_.Cleanup(); }
    bool Release() override { return parent_.Release(); }

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

  bool Init() { return pimpl_->Init(); }
  bool Prepare() { return pimpl_->Prepare(); }
  bool Update() { return pimpl_->Update(); }
  bool Cleanup() { return pimpl_->Cleanup(); }
  bool Release() { return pimpl_->Release(); }
};

}  // namespace caf
