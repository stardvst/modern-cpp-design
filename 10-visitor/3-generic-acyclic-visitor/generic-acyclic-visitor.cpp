#include "base-visitable.h"
#include "base-visitor.h"

#include <iostream>

struct DocElement : public BaseVisitable<>
{
	DEFINE_VISITABLE()
};

struct Paragraph : public DocElement
{
	DEFINE_VISITABLE()
};

struct Image : public DocElement
{
	DEFINE_VISITABLE()
};

class InfoVisitor
	: public virtual Visitor<DocElement>
	, public virtual Visitor<Paragraph>
	, public virtual Visitor<Image>
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
