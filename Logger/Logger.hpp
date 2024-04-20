#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <chrono>

class Logger {
 public:
  void TimeStartImport();
  void TimeEndImport();

  void TimeStartRender();
  void TimeEndRender();

  void TimeStartExport();
  void TimeEndExport();

  void LogTimeDurationImport();
  void LogTimeDurationRender();
  void LogTimeDurationExport();

 private:
  using TimePoint = std::chrono::high_resolution_clock::time_point;

  TimePoint timeStartImport;
  TimePoint timeEndImport;
  TimePoint timeStartRender;
  TimePoint timeEndRender;
  TimePoint timeStartExport;
  TimePoint timeEndExport;

  void LogTimeDuration(std::string message, TimePoint start, TimePoint end);
};

#endif  // LOGGER_HPP
