/* SyncTicker library code is placed under the MIT license
 * Copyright (c) 2018 Stefan Staub
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "SyncTicker.h"

SyncTicker::SyncTicker() {
  // setup without a function or interval
  // can still be used by calling tick(), or setup() later
  setup(nullptr, 0, 0);
}

SyncTicker::SyncTicker(fptr _callback, uint32_t _timer, uint32_t _repeat, resolution_t _resolution) {
  setup(_callback, _timer, _repeat, _resolution);
}

SyncTicker::~SyncTicker() { }

void SyncTicker::setup(fptr _callback, uint32_t _timer, uint32_t _repeat, resolution_t _resolution) {
  this->resolution = _resolution;
  if (resolution == MICROS) {
    _timer = _timer * 1000;
  }
  this->timer = _timer;
  this->repeat = _repeat;
  this->callback = _callback;
  enabled = false;
  lastTime = 0;
  counts = 0;
}

void SyncTicker::start(uint32_t _timer=0) {
  if (_timer != 0) {
    this->interval(_timer);
  } else if (this->timer == 0)
    // do not start an uninitialized timer
    return;
  }

  if (resolution == MILLIS) {
    lastTime = millis();
  } else {
    lastTime = micros();
  }

  enabled = true;
  counts = 0;
  status = RUNNING;
}

void SyncTicker::resume() {
  if (resolution == MILLIS) {
    lastTime = millis() - diffTime;
  } else {
    lastTime = micros() - diffTime;
  }

  if (status == STOPPED) {
    counts = 0;
  }
  enabled = true;
  status = RUNNING;
}

void SyncTicker::stop() {
  enabled = false;
  counts = 0;
  status = STOPPED;
}

void SyncTicker::pause() {
  if (resolution == MILLIS) {
    diffTime = millis() - lastTime;
  } else {
    diffTime = micros() - lastTime;
  }

  enabled = false;
  status = PAUSED;
}

void SyncTicker::update() {
  if (tick()) {
    callback();
  }
}

bool SyncTicker::tick() {
  if (!enabled)  {
    return false;
  }

  if (resolution == MILLIS) {
    if ((millis() - lastTime) >= timer) {
      lastTime = millis();
      if (repeat - counts == 1) {
        enabled = false;
      }
      counts++;
      return true;
    }
  } else {
    if ((micros() - lastTime) >= timer) {
      lastTime = micros();
      if (repeat - counts == 1) enabled = false;
      counts++;
      return true;
    }
  }

  return false;
}

void SyncTicker::interval(uint32_t timer) {
  if (resolution == MICROS) {
    timer = timer * 1000;
  }

  this->timer = timer;
}

uint32_t SyncTicker::elapsed() {
  if (resolution == MILLIS) {
    return millis() - lastTime;
  } else {
    return micros() - lastTime;
  }
}

status_t SyncTicker::state() {
  return status;
}

uint32_t SyncTicker::counter() {
  return counts;
}
