/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef VRBROWSER_EXTERNALVR_H
#define VRBROWSER_EXTERNALVR_H

#include "vrb/MacroUtils.h"
#include "Controller.h"
#include "DeviceDelegate.h"
#include "Device.h"
#include <memory>
#include <string>
#include <vector>

namespace mozilla { namespace gfx { struct VRExternalShmem; } }

namespace crow {

class ExternalVR;
typedef std::shared_ptr<ExternalVR> ExternalVRPtr;

class ExternalVR : public ImmersiveDisplay {
public:
  enum class VRState {
    NotPresenting,
    Loading,
    LinkTraversal,
    Rendering
  };
  enum class VRBrowserType {
    Gecko,
    Servo
  };
  static ExternalVRPtr Create();
  mozilla::gfx::VRExternalShmem* GetSharedData();
  // DeviceDisplay interface
  void SetDeviceName(const std::string& aName) override;
  void SetCapabilityFlags(const device::CapabilityFlags aFlags) override;
  void SetFieldOfView(const device::Eye aEye, const double aLeftDegrees,
                      const double aRightDegrees,
                      const double aTopDegrees,
                      const double aBottomDegrees) override;
  void SetEyeOffset(const device::Eye aEye, const float aX, const float aY, const float aZ) override;
  void SetEyeResolution(const int32_t aX, const int32_t aY) override;
  void CompleteEnumeration() override;
  // ExternalVR interface
  void PushSystemState();
  void PullBrowserState();
  void SetCompositorEnabled(bool aEnabled);
  bool IsPresenting() const;
  VRState GetVRState() const;
  void PushFramePoses(const vrb::Matrix& aHeadTransform, const std::vector<Controller>& aControllers);
  bool WaitFrameResult();
  void GetFrameResult(int32_t& aSurfaceHandle, device::EyeRect& aLeftEye, device::EyeRect& aRightEye) const;
  void StopPresenting();
  void SetSourceBrowser(VRBrowserType aBrowser);
  ~ExternalVR();
protected:
  struct State;
  ExternalVR();
private:
  State& m;
  VRB_NO_DEFAULTS(ExternalVR)
};

}

#endif //VRBROWSER_EXTERNALVR_H
