//
//  carfac.h
//  CARFAC Open Source C++ Library
//
//  Created by Alex Brandmeyer on 5/10/13.
//
// This C++ file is part of an implementation of Lyon's cochlear model:
// "Cascade of Asymmetric Resonators with Fast-Acting Compression"
// to supplement Lyon's upcoming book "Human and Machine Hearing"
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// *****************************************************************************
// Class: CARFAC
// *****************************************************************************
// The CARFAC class is the top-level class implementing the CAR-FAC C++ model.
// A CARFAC object knows how to design its details from a modest set of
// parameters, and knows how to process sound signals to produce "neural
// activity patterns" (NAPs) which are contained in a CARFACOutput object.
//
// The 'Design' method is used to intialize the CARFAC model, and is passed
// a set of CAR, IHC and AGC parameters along with sound file information
// (channels and sample rate).
//
// The two methods 'Run' and 'RunSegment' are responsible for
// processing sound signals. These both take two dimensional Eigen float arrays
// (samples x channels) as arguments and return CARFACOutput objects.

#ifndef CARFAC_Open_Source_C__Library_CARFAC_h
#define CARFAC_Open_Source_C__Library_CARFAC_h

#include "ear.h"
#include "carfac_output.h"

class CARFAC {
 public:
  // The 'Design' method takes a set of CAR, IHC and AGC parameters along with
  // arguments specifying the number of 'ears' (audio file channels) and sample
  // rate. This initializes a vector of 'Ear' objects -- one for mono, two for
  // stereo, or more.  Each 'Ear' includes various sub-objects representing the
  // parameters, designs (coeffs) ,and states of different parts of the CAR-FAC
  // model.
  void Design(int n_ears, int32_t fs, CARParams car_params, IHCParams ihc_params,
              AGCParams agc_params);
  // The 'Run' method processes an entire file with the current model, using
  // subsequent calls to the 'RunSegment' method
  CARFACOutput Run(FloatArray2d sound_data, bool open_loop, bool store_bm,
                   bool store_ohc, bool store_agc);
  // The 'RunSegment' method processes individual sound segments
  void RunSegment(FloatArray2d sound_data, CARFACOutput *seg_output,
                  bool open_loop, bool store_bm, bool store_ohc,
                  bool store_agc);
  
 private:
  void CrossCouple();
  void CloseAGCLoop();
  
  int n_ears_;  // This is the number of ears.
  int32_t fs_;  // This is our current sample rate.
  int n_ch_;  // This is the number of channels in the CARFAC model.
  FPType max_channels_per_octave_;
  // We store an array of Ear objects for mono/stereo/multichannel processing:
  std::vector<Ear> ears_;
};

#endif