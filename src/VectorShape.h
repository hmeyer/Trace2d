#ifndef VECTORSHAPE_H
#define VECTORSHAPE_H

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
typedef std::list< Segment > Curve;    

class VectorShape: public Curve
{
  public:
};

typedef std::list< VectorShape > VectorShapeList;

#endif // VECTORSHAPE_H
