#pragma once

class DocElementVisitor;

class DocElement
{
public:
  virtual void Accept(DocElementVisitor &visitor) = 0;
  virtual ~DocElement() = default;
};
