#pragma once

struct BaseVisitor
{
	virtual ~BaseVisitor() = default;
};

template <typename T, typename R = void>
class Visitor : public virtual BaseVisitor
{
public:
	using ReturnType = R; // available for use in derived visitors
	virtual ReturnType visit(const T &element) = 0;
};
