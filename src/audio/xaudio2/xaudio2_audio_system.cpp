/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2013 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "rex/audio/xaudio2/xaudio2_audio_system.h"

#include "rex/audio/flags.h"
#include "rex/audio/xaudio2/xaudio2_audio_driver.h"

namespace rex::audio::xaudio2 {

std::unique_ptr<AudioSystem> XAudio2AudioSystem::Create(
    runtime::Processor* processor) {
  return std::make_unique<XAudio2AudioSystem>(processor);
}

XAudio2AudioSystem::XAudio2AudioSystem(runtime::Processor* processor)
    : AudioSystem(processor) {}

XAudio2AudioSystem::~XAudio2AudioSystem() {}

void XAudio2AudioSystem::Initialize() { AudioSystem::Initialize(); }

X_STATUS XAudio2AudioSystem::CreateDriver(size_t index,
                                          rex::thread::Semaphore* semaphore,
                                          AudioDriver** out_driver) {
  assert_not_null(out_driver);
  auto driver = new XAudio2AudioDriver(memory_, semaphore);
  if (!driver->Initialize()) {
    driver->Shutdown();
    return X_STATUS_UNSUCCESSFUL;
  }

  *out_driver = driver;
  return X_STATUS_SUCCESS;
}

void XAudio2AudioSystem::DestroyDriver(AudioDriver* driver) {
  assert_not_null(driver);
  auto xdriver = static_cast<XAudio2AudioDriver*>(driver);
  xdriver->Shutdown();
  assert_not_null(xdriver);
  delete xdriver;
}

}  // namespace rex::audio::xaudio2
