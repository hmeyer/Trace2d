#ifndef VECTORIMAGEAREA_H
#define VECTORIMAGEAREA_H

#include "imageArea.h"
#include "VectorShape.h"


class VectorImageArea : public ImageArea
{
  public:
  void addVector( const VectorShape &vect );
  void clearVectors( void ) { m_vectors.clear(); }
protected:
  virtual void drawContent(CairoContextPointer c);
  std::list< VectorShape > m_vectors;
};

#endif // VECTORIMAGEAREA_H
