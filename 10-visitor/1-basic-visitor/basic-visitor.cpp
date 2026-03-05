#include <iostream>
#include <memory>
#include <vector>

struct Paragraph;
struct Image;
struct UnknownElement;
struct DocElement;

class DocElementVisitor
{
public:
	virtual void visit(const Paragraph &paragraph) = 0;
	virtual void visit(const Image &image) = 0;
//  virtual void visit(const UnknownElement &element) = 0;

	// catch-all visit method
	virtual void visit(const DocElement &) = 0;

	virtual ~DocElementVisitor() = default;
};

class DocElement
{
public:
	virtual void Accept(DocElementVisitor &visitor) = 0;
	virtual ~DocElement() = default;
};

struct Paragraph : public DocElement
{
	int numChars = 5;
	int numWords = 1;

	void Accept(DocElementVisitor &visitor) override
	{
		visitor.visit(*this);
	}
};

struct Image : public DocElement
{
	int width = 640;
	int height = 480;

	void Accept(DocElementVisitor &visitor) override
	{
		visitor.visit(*this);
	}
};

struct UnknownElement : public DocElement
{
	void Accept(DocElementVisitor &visitor) override
	{
		visitor.visit(*this);
	}
};

class DocStats : public DocElementVisitor
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

  // void visit(const UnknownElement &element) override
  // {
  //   std::cout << "Visited an unknown document element.\n";
  // }

	void visit(const DocElement &element) override
	{
		std::cout << "Visited a DocElement.\n";
	}

	void PrintStats()
	{
		std::cout << "Characters: " << m_numChars << "\n";
		std::cout << "Words: " << m_numWords << "\n";
		std::cout << "Image Width: " << m_imageWidth << "\n";
		std::cout << "Image Height: " << m_imageHeight << "\n";
	}

private:
	int m_numChars = 0;
	int m_numWords = 0;
	int m_imageWidth = 0;
	int m_imageHeight = 0;
};

class Document
{
public:
	void AddElement(std::unique_ptr<DocElement> element)
	{
		m_elements.push_back(std::move(element));
	}

	void Accept(DocElementVisitor &visitor)
	{
		for (auto &element : m_elements)
		{
			element->Accept(visitor);
		}
	}

private:
	std::vector<std::unique_ptr<DocElement>> m_elements;
};

int main()
{
	DocStats stats;
	Document document;
	document.AddElement(std::make_unique<Paragraph>());
	document.AddElement(std::make_unique<Image>());
	document.AddElement(std::make_unique<UnknownElement>());
	document.Accept(stats);
	stats.PrintStats();
}
