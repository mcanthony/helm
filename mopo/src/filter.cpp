/* Copyright 2013-2015 Matt Tytel
 *
 * mopo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mopo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mopo.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "filter.h"

#include <cmath>

namespace {
  const mopo::mopo_float COEFFICIENT_FILTERING = 0.9;
} // namespace

namespace mopo {

  Filter::Filter() : Processor(Filter::kNumInputs, 1) {
    current_type_ = kNumTypes;
    current_cutoff_ = 0.0;
    current_resonance_ = 0.0;

    target_in_0_ = 1.0;
    target_in_1_ = target_in_2_ = 0.0;
    target_out_1_ = target_out_2_ = 0.0;

    in_0_ = 1.0;
    in_1_ = in_2_ = 0.0;
    out_1_ = out_2_ = 0.0;

    past_in_1_ = past_in_2_ = past_out_1_ = past_out_2_ = 0.0;
  }

  std::complex<mopo_float> Filter::getResponse(mopo_float frequency) {
    static const std::complex<mopo_float> one(1.0, 0.0);
    const mopo_float phase_delta = 2.0 * PI * frequency / sample_rate_;
    const std::complex<mopo_float> freq_tick1 = std::polar(1.0, -phase_delta);
    const std::complex<mopo_float> freq_tick2 = std::polar(1.0, -2 * phase_delta);

    return (target_in_0_ * one + target_in_1_ * freq_tick1 + target_in_2_ * freq_tick2) /
           (one + target_out_1_ * freq_tick1 + target_out_2_ * freq_tick2);
  }

  void Filter::process() {
    current_type_ = static_cast<Type>(inputs_[kType]->at(0));
    computeCoefficients(current_type_, inputs_[kCutoff]->at(0),
                                       inputs_[kResonance]->at(0));

    int i = 0;
    if (inputs_[kReset]->source->triggered &&
        inputs_[kReset]->source->trigger_value == kVoiceReset) {
      int trigger_offset = inputs_[kReset]->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        outputs_[0]->buffer[i] = tick(i);

      reset();
    }
    for (; i < buffer_size_; ++i)
      outputs_[0]->buffer[i] = tick(i);
  }

  inline mopo_float Filter::tick(int i) {
    mopo_float input = inputs_[kAudio]->at(i);
    in_0_ = INTERPOLATE(target_in_0_, in_0_, COEFFICIENT_FILTERING);
    in_1_ = INTERPOLATE(target_in_1_, in_1_, COEFFICIENT_FILTERING);
    in_2_ = INTERPOLATE(target_in_2_, in_2_, COEFFICIENT_FILTERING);
    out_1_ = INTERPOLATE(target_out_1_, out_1_, COEFFICIENT_FILTERING);
    out_2_ = INTERPOLATE(target_out_2_, out_2_, COEFFICIENT_FILTERING);

    mopo_float out = input * in_0_ + past_in_1_ * in_1_ + past_in_2_ * in_2_ -
                     past_out_1_ * out_1_ - past_out_2_ * out_2_;
    past_in_2_ = past_in_1_;
    past_in_1_ = input;
    past_out_2_ = past_out_1_;
    past_out_1_ = out;
    return out;
  }

  inline void Filter::reset() {
    past_in_1_ = past_in_2_ = past_out_1_ = past_out_2_ = 0;
  }

} // namespace mopo