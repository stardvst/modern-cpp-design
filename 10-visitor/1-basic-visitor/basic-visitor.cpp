#include <iostream>

struct Paragraph;
struct Image;

class DocElementVisitor
{
public:
	virtual void visit(const Paragraph &paragraph) = 0;
	virtual void visit(const Image &image) = 0;
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

class Documment
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
	Documment document;
	document.AddElement(std::make_unique<Paragraph>());
	document.AddElement(std::make_unique<Image>());
	document.Accept(stats);
	stats.PrintStats();
}
