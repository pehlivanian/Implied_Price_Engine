#ifndef __VISITOR_HPP__
#define __VISITOR_HPP__

class DataElement;

class Visitor
{
public:
  inline void visit(DataElement* d) { generate(d); }
  virtual ~Visitor() {}
private:
  virtual void generate(DataElement*) = 0;
protected:
  Visitor() {}
};

#endif
