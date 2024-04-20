#include "Logger.hpp"

#include <iostream>

void Logger::TimeStartImport() {
  timeStartImport = std::chrono::high_resolution_clock::now();
}
void Logger::TimeEndImport() {
  timeEndImport = std::chrono::high_resolution_clock::now();
}

void Logger::TimeStartRender() {
  timeStartRender = std::chrono::high_resolution_clock::now();
}
void Logger::TimeEndRender() {
  timeEndRender = std::chrono::high_resolution_clock::now();
}

void Logger::TimeStartExport() {
  timeStartExport = std::chrono::high_resolution_clock::now();
}
void Logger::TimeEndExport() {
  timeEndExport = std::chrono::high_resolution_clock::now();
}

void Logger::LogTimeDurationImport() {
  LogTimeDuration("Import", timeStartImport, timeEndImport);
}
void Logger::LogTimeDurationRender() {
  LogTimeDuration("Render", timeStartRender, timeEndRender);
}
void Logger::LogTimeDurationExport() {
  LogTimeDuration("Export", timeStartExport, timeEndExport);
}

void Logger::LogTimeDuration(std::string message, TimePoint start,
                             TimePoint end) {
  using namespace std::chrono;

  duration<double, std::milli> duration(end - start);

  auto h = duration_cast<hours>(duration);
  auto m = duration_cast<minutes>(duration - h);
  auto s = duration_cast<seconds>(duration - h - m);
  auto ms = duration_cast<milliseconds>(duration - h - m - s);

  std::cout << message << ": ";

  auto empty = true;

  if (auto hours = h.count()) {
    std::cout << hours << "h ";
    empty = false;
  }

  if (auto minutes = m.count()) {
    std::cout << minutes << "m ";
    empty = false;
  }

  if (auto seconds = s.count()) {
    std::cout << seconds << "s ";
    empty = false;
  }

  if (auto milliseconds = ms.count()) {
    std::cout << milliseconds << "ms";
    empty = false;
  }

  if (empty) {
    std::cout << "0ms";
  }

  std::cout << std::endl;
}
