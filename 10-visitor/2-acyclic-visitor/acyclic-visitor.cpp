#include "doc-element-visitor.h"
#include "doc-element.h"

#include <iostream>

class Paragraph : public DocElement
{
public:
	void Accept(DocElementVisitor &visitor) override
	{
		if (auto paragraphVisitor = dynamic_cast<ParagraphVisitor *>(&visitor))
		{
			paragraphVisitor->visit(*this);
		}
	}

	int numChars = 5;
	int numWords = 1;
};

class Image : public DocElement
{
public:
	void Accept(DocElementVisitor &visitor) override
	{
		if (auto imageVisitor = dynamic_cast<ImageVisitor *>(&visitor))
		{
			imageVisitor->visit(*this);
		}
	}

	int width = 640;
	int height = 480;
};

class DocStats
	: public virtual ParagraphVisitor
	, public virtual ImageVisitor
{
public:
	void visit(const Paragraph &paragraph) override
	{
		m_numChars += paragraph.numChars;
		m_numWords += paragraph.numWords;
	}

	void visit(const Image &image) override
	{
		m_imageWidth += image.width;
		m_imageHeight += image.height;
	}

	void PrintStats()
	{
		std::cout << "Document Statistics:\n";
		std::cout << "Number of Characters: " << m_numChars << "\n";
		std::cout << "Number of Words: " << m_numWords << "\n";
		std::cout << "Total Image Width: " << m_imageWidth << "\n";
		std::cout << "Total Image Height: " << m_imageHeight << "\n";
	}

private:
	int m_numChars = 0;
	int m_numWords = 0;
	int m_imageWidth = 0;
	int m_imageHeight = 0;
};

int main()
{
	Paragraph paragraph;
	Image image;

	DocStats stats;
	paragraph.Accept(stats);
	image.Accept(stats);
	stats.PrintStats();
}
