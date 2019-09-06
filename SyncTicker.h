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

#ifndef SYNC_TICKER_H
#define SYNC_TICKER_H

#include "Arduino.h"

/** SyncTicker internal resolution
 *
 * @param MICROS (default), measure milliseconds timings using microsecond precision, max is 70 minutes
 * @param MILLIS measure millisecond timings using millisecond precision, for longer cycles over 70 minutes
 * @param MICROS_MICROS, measure microseconds using microsecond precision
 *
 */
enum resolution_t {MICROS, MILLIS, MICROS_MICROS};

/** SyncTicker status
 *
 * @param STOPPED default, ticker is stopped
 * @param RUNNING ticker is running
 * @param PAUSED ticker is paused
 *
 */
enum status_t {STOPPED, RUNNING, PAUSED};

#define infinite -1

typedef void (*fptr)();

class SyncTicker {

public:
  /** create a SyncTicker object
   *
   * @param callback the name of the function to call
   * @param _interval interval length in ms or us
   * @param repeat default -1 -> endless, repeat > 0 -> number of repeats
   * @param resolution default MICROS for tickers under 70min, use MILLIS for tickers over 70 min
   *
   */
  SyncTicker(fptr callback = nullptr, uint32_t _interval = 0, int32_t repeat = infinite, resolution_t resolution = MICROS);

  /** destructor for the SyncTicker object
   *
   */
  ~SyncTicker();

  /** configure the SyncTicker object
   *
   * @param callback the name of the function to call
   * @param _interval interval length in ms or us
   * @param repeat default -1 -> endless, repeat > 0 -> number of repeats
   * @param resolution default MICROS for tickers under 70min, use MILLIS for tickers over 70 min
   *
   */
  void setup(fptr callback = nullptr, uint32_t _interval = 0, int32_t repeat = infinite, resolution_t resolution = MICROS);

  /** start the ticker
   *
   */
  bool start(uint32_t _interval = 0);

  /** resume the ticker. If not started, it will start it.
   *
   */
  void resume();

  /** pause the ticker
   *
   */
  void pause();

  /** stops the ticker
   *
   */
  void stop();

  /** must to be called in the main loop(), it will check the SyncTicker, and if necessary, will run the callback
   *
   */
  bool update();

  /**
   * check whether or not it is time for a tick
   * @return true if ticker is ready to tick
   */
  bool peek();

  /** same as update(), except will NOT call any callback
   *  instead it will return a bool whether or not it is time to execute code
   */
  bool tick();

  /**
   * @brief set the interval timer
   *
   * @param timer interval length in ms or us
   */
  void setInterval(uint32_t _interval);

  /** actual ellapsed time
   *
   * @returns the elapsed time after the last tick
   *
   */
  uint32_t elapsed();

  /** get the state of the ticker
   *
   * @returns the state of the ticker: STOPPED, RUNNING or PAUSED
   */
  status_t state();

  /** get the numbers of executed repeats
   *
   * @returns the number of executed repeats
   *
   */
  uint32_t counter();

private:
  uint32_t interval;
  int32_t repeat;
  uint32_t runs;
  resolution_t resolution = MICROS;
  status_t status = STOPPED;
  fptr callback;
  uint32_t lastTime;
  uint32_t pauseDiff;
  uint32_t now();
};

#endif
