#ifndef HUAWEIP2P_TIMESTAMP_H
#define HUAWEIP2P_TIMESTAMP_H
#include<boost/operators.hpp>
#include<string>
#include<stdint.h>

namespace huaweip2p
{

class Timestamp: boost::less_than_comparable<Timestamp>
{
 public:
  /// Constructs an invalid Timestamp

  Timestamp()
    : microSecondsSinceEpoch_(0)
  {
  }
  /// Constructs a Timestamp at a specific time
  explicit Timestamp(int64_t microSecondsSinceEpoch);
  std::string toString() const;
  std::string toFormattedString() const;
  bool valid() const { return microSecondsSinceEpoch_ > 0; }

  int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
  time_t secondsSinceEpoch() const
  {
    return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
  }
  static Timestamp now();
  static Timestamp invalid();
  static const int kMicroSecondsPerSecond = 1000 * 1000;
 private:
  int64_t microSecondsSinceEpoch_;
};
inline bool operator<(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}
inline bool operator==(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}
inline double timeDifference(Timestamp high, Timestamp low)
{
  int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
  return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}
inline Timestamp addTime(Timestamp timestamp, double seconds)
{
  int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}
}
#endif
