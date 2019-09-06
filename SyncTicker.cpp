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

SyncTicker::SyncTicker(fptr _callback, uint32_t _interval, int32_t _repeat, resolution_t _resolution) {
  setup(_callback, _interval, _repeat, _resolution);
}

SyncTicker::~SyncTicker() { }

void SyncTicker::setup(fptr _callback, uint32_t _interval, int32_t _repeat, resolution_t _resolution) {
  this->resolution = _resolution;
  this->repeat = _repeat;
  this->callback = _callback;
  setInterval(_interval);
  lastTime = 0;
  runs = 0;
  status = STOPPED;
}

bool SyncTicker::start(uint32_t _interval) {
  if (_interval != 0) {
    this->setInterval(_interval);
  }

  if (this->interval == 0) {
    return false; // do not start a timer without an interval
  } else if (status == RUNNING) {
    return true; // do nothing, already running
  }

  runs = 0;
  lastTime = now();
  status = RUNNING;
  return true;
}

void SyncTicker::resume() {
  lastTime = now();

  // account for partial intervals if we were paused and not stopped
  if (status == PAUSED) {
    lastTime -= pauseDiff;
  }

  status = RUNNING;
}

void SyncTicker::stop() {
  status = STOPPED;
}

void SyncTicker::pause() {
  pauseDiff = now() - lastTime;
  status = PAUSED;
}

bool SyncTicker::update() {
  if (!tick()) {
    return false;
  }

  callback();
  return true;
}

uint32_t SyncTicker::now() {
  return (resolution == MILLIS) ? millis() : micros();
}

bool SyncTicker::peek() {
  return status == RUNNING && elapsed() >= interval;
}

bool SyncTicker::tick() {
  if (!peek()) {
    return false;
  }

  runs++;
  lastTime = now();

  if (repeat != infinite && runs >= repeat) {
    stop();
  }

  return true;
}

void SyncTicker::setInterval(uint32_t _interval) {
  this->interval = (resolution == MICROS) ? _interval * 1000 : _interval;
}

uint32_t SyncTicker::elapsed() {
  return now() - lastTime;
}

status_t SyncTicker::state() {
  return status;
}

uint32_t SyncTicker::counter() {
  return runs;
}
