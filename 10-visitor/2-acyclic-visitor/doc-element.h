#pragma once

class DocElementVisitor;

class DocElement
{
public:
  virtual void Accept(DocElementVisitor &visitor) = 0;
};
