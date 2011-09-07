/*buckets class for helping keep track of bucket information
  on the hash.h file structure
*/
#ifndef _buckets_h
#define _buckets_h
class Buckets
{
 private:

 public:
  int bucketQty;
  int bucketSize;
  int bucketMin;
  int bucketMax;
  
  Buckets()
    : bucketQty(0), bucketSize(0), bucketMin(0), bucketMax(0)
    {}
  
  void maxAndMin()
    {
      if(bucketQty < 2) bucketMin = bucketMax = bucketSize;
      else if(bucketSize < bucketMin) bucketMin = bucketSize;
      else if(bucketSize > bucketMax) bucketMax = bucketSize;
    }
};

#endif
