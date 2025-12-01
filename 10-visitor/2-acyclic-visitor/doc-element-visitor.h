#pragma once

struct DocElement;
struct Paragraph;
struct Image;

struct DocElementVisitor
{
	virtual ~DocElementVisitor() = default;
};

class ParagraphVisitor : public virtual DocElementVisitor
{
public:
	virtual void visit(const Paragraph &paragraph) = 0;
};

class ImageVisitor : public virtual DocElementVisitor
{
public:
	virtual void visit(const Image &image) = 0;
};
