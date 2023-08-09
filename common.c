// Copyright 2023, Midnight Blue.
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

#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include "common.h"

uint32_t build_iv(FrameNumbers *f) {
    assert(1 <= f->tn  && f->tn  <= 4);
    assert(1 <= f->fn  && f->fn  <= 18);
    assert(1 <= f->mn  && f->mn  <= 60);
    assert(0 <= f->hn  && f->hn  <= 0xFFFF);
    assert(0 <= f->dir && f->dir <= 1); // 0 = downlink, 1 = uplink
    return ((f->tn - 1) | (f->fn << 2) | (f->mn << 7) | ((f->hn & 0x7FFF) << 13) | (f->dir << 28));
}
