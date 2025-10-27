#pragma once

#include "functor-impl.h"

template <typename ParentFunctor, typename Fun>
class FunctorHandler : public FunctorImpl<typename ParentFunctor::ResultType, typename ParentFunctor::ParamList>
{
public:
	using ResultType = typename ParentFunctor::ResultType;

	DEFINE_CLONE_FUNCTOR_IMPL(FunctorHandler)

	FunctorHandler(const Fun &fun) : m_fun(fun)
	{
	}

	ResultType operator()()
	{
		return m_fun();
	}

	ResultType operator()(typename ParentFunctor::Param1 p1)
	{
		return m_fun(p1);
	}

	ResultType operator()(typename ParentFunctor::Param1 p1, typename ParentFunctor::Param2 p2)
	{
		return m_fun(p1, p2);
	}

	ResultType operator()(typename ParentFunctor::Param1 p1, typename ParentFunctor::Param2 p2, typename ParentFunctor::Param3 p3)
	{
		return m_fun(p1, p2, p3);
	}

private:
	Fun m_fun;
};
