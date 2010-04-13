#ifndef VECTORSHAPE_H
#define VECTORSHAPE_H

#include <vector>
#include <list>

struct Point2D {
  double x;
  double y;
};
enum SegmentType {
  BezierSegment,
  CornerSegment
};
struct Segment {
  SegmentType type;
  Point2D u;
  Point2D w;
  Point2D endPoint;
};
typedef std::vector< Segment > Curve;    

class VectorShape: public Curve
{
  public:
};

#endif // VECTORSHAPE_H
