#include "base-visitable.h"
#include "base-visitor.h"

#include <iostream>

struct DocElement : public BaseVisitable<>
{
	ReturnType Accept(BaseVisitor &visitor) override { return BaseVisitable::AcceptThis(*this, visitor); }
};

struct Paragraph : public DocElement
{
	ReturnType Accept(BaseVisitor &visitor) override { return BaseVisitable::AcceptThis(*this, visitor); }
};

struct Image : public DocElement
{
	ReturnType Accept(BaseVisitor &visitor) override { return BaseVisitable::AcceptThis(*this, visitor); }
};

class InfoVisitor
	: public Visitor<DocElement>
	, public Visitor<Paragraph>
	, public Visitor<Image>
{
public:
	void visit(const DocElement &element) override
	{
		std::cout << "Visited a DocElement.\n";
	}

	void visit(const Paragraph &paragraph) override
	{
		std::cout << "Visited a Paragraph.\n";
	}

	void visit(const Image &image) override
	{
		std::cout << "Visited an Image.\n";
	}
};

int main()
{
	DocElement docElement;
	Paragraph paragraph;
	Image image;

	InfoVisitor info;
	docElement.Accept(info);
	paragraph.Accept(info);
	image.Accept(info);

	return 0;
}
