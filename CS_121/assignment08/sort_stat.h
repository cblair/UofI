#ifndef _sort_stat_h
#define _sort_stat_h

class Stats
{
 public:
  int find_mins;
  int shifts;
  int copies;
  int swaps;
  int comparisons;
  int pivots;
  Stats()
    : find_mins(0), shifts(0), copies(0), swaps(0),
    comparisons(0), pivots(0)
    {}
};

#endif
