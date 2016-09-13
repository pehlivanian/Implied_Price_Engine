#ifndef __DATA_ELEMENT_HPP__
#define __DATA_ELEMENT_HPP__

#include "Visitor.hpp"

class DataElement
{
public:
  inline void accept(Visitor* v) { v->visit(this); }
  virtual ~DataElement() {}
protected:
  DataElement() {}
};

#endif
