#pragma once

#include "base-visitor.h"

template <typename R = void>
class BaseVisitable
{
public:
	using ReturnType = R;
	virtual ~BaseVisitable() = default;
	virtual void Accept(BaseVisitor &visitor) = 0;

protected:
	template <typename T>
	static ReturnType AcceptImpl(T &element, BaseVisitor &visitor)
	{
		if (auto specificVisitor = dynamic_cast<Visitor<T, R> *>(&visitor))
		{
			return specificVisitor->visit(element);
		}
		return ReturnType{};
	}

#define DEFINE_VISITABLE()                                                                                                                 \
	virtual ReturnType Accept(BaseVisitor &visitor)                                                                                        \
	{                                                                                                                                      \
		return AcceptImpl(*this, visitor);                                                                                                 \
	}
};
