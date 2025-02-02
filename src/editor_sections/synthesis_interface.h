/* Copyright 2013-2015 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef SYNTHESIS_INTERFACE_H
#define SYNTHESIS_INTERFACE_H

#include "JuceHeader.h"
#include "helm_engine.h"

#include "articulation_section.h"
#include "delay_section.h"
#include "envelope_section.h"
#include "extra_mod_section.h"
#include "feedback_section.h"
#include "filter_section.h"
#include "formant_section.h"
#include "lfo_section.h"
#include "noise_section.h"
#include "oscillator_section.h"
#include "reverb_section.h"
#include "sub_section.h"
#include "step_sequencer_section.h"
#include "stutter_section.h"
#include "volume_section.h"

#include "modulation_button.h"

class SynthesisInterface  : public SynthSection {
  public:
    SynthesisInterface(mopo::control_map controls);
    ~SynthesisInterface();
  
    void paintBackground(Graphics& g) override;
    void resized();

  private:
    ScopedPointer<EnvelopeSection> amplitude_envelope_section_;
    ScopedPointer<ArticulationSection> articulation_section_;
    ScopedPointer<DelaySection> delay_section_;
    ScopedPointer<EnvelopeSection> extra_envelope_section_;
    ScopedPointer<ExtraModSection> extra_mod_section_;
    ScopedPointer<FeedbackSection> feedback_section_;
    ScopedPointer<EnvelopeSection> filter_envelope_section_;
    ScopedPointer<FilterSection> filter_section_;
    ScopedPointer<FormantSection> formant_section_;
    ScopedPointer<LfoSection> mono_lfo_1_section_;
    ScopedPointer<LfoSection> mono_lfo_2_section_;
    ScopedPointer<NoiseSection> noise_section_;
    ScopedPointer<OscillatorSection> oscillator_section_;
    ScopedPointer<LfoSection> poly_lfo_section_;
    ScopedPointer<ReverbSection> reverb_section_;
    ScopedPointer<StepSequencerSection> step_sequencer_section_;
    ScopedPointer<StutterSection> stutter_section_;
    ScopedPointer<SubSection> sub_section_;
    ScopedPointer<VolumeSection> volume_section_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthesisInterface)
};

#endif // SYNTHESIS_INTERFACE_H
