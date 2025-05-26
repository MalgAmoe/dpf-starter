#include "DistrhoPlugin.hpp"
#include "DistrhoPluginInfo.h"
#include <cstdint>
#include "math.h"


START_NAMESPACE_DISTRHO

class TanDist : public Plugin {
public:
  TanDist() : Plugin(kParametersCount, 0, 0), gain{ 1.0, 1.0 } {}

protected:
  const char* getLabel() const override { return "TanDist"; }
  const char* getDescription() const override { return "Simple distortion plugin."; }
  const char* getMaker() const override { return "MalgAmoe"; }
  const char* getLicense() const override { return "MIT"; }
  uint32_t getVersion() const override { return d_version(1, 0, 0); }
  int64_t getUniqueId() const override { return d_cconst('A', 'E', 'I', 'O'); }

  void initParameter(uint32_t index, Parameter& parameter) override {
    switch (index) {
    case kGain:
      parameter.name = "Gain";
      parameter.symbol = "gain";
      parameter.ranges.def = 1.0f;
      parameter.ranges.min = 1.0f;
      parameter.ranges.max = 10.0f;
      parameter.hints = kParameterIsAutomatable;
      break;
    default:
      break;
    }
  }

  float getParameterValue(uint32_t index) const override {
    switch (index) {
    case kGain:
      return gain[0];
    default:
      return 0.0;
    }
  }

  void setParameterValue(uint32_t index, float value) override {
    switch (index) {
    case kGain:
      gain[0] = value;
      break;
    default:
      break;
    }
  }

  void run(const float** inputs, float** outputs, uint32_t frames) override {
    const float* const inL = inputs[0];
    const float* const inR = inputs[1];
    float* const outL = outputs[0];
    float* const outR = outputs[1];

    for (uint32_t i = 0; i < frames; i++) {
      gain[1] += (gain[0] - gain[1]) * 0.001f;
      const float quadraticGain = gain[1] * gain[1];

      outL[i] = tanh(inL[i] * quadraticGain);
      outR[i] = tanh(inR[i] * quadraticGain);
    }
  }

private:
  float gain[2];

  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TanDist);
};

Plugin* createPlugin() { return new TanDist(); }

END_NAMESPACE_DISTRHO