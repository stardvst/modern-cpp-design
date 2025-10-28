#pragma once

#include "functor-impl.h"

template <typename ParentFunctor, typename PointerToObj, typename PointerToMemFn>
class MemFunHandler : public FunctorImpl<typename ParentFunctor::ResultType, typename ParentFunctor::ParamList>
{
public:
	using ResultType = typename ParentFunctor::ResultType;

	MemFunHandler(const PointerToObj &obj, PointerToMemFn memFn) : m_obj(obj), m_memFn(memFn)
	{
	}

	DEFINE_CLONE_FUNCTOR_IMPL(MemFunHandler)

	ResultType operator()()
	{
		return (m_obj->*m_memFn)();
	}
	ResultType operator()(typename ParentFunctor::Param1 p1)
	{
		return (m_obj->*m_memFn)(p1);
	}
	ResultType operator()(typename ParentFunctor::Param1 p1, typename ParentFunctor::Param2 p2)
	{
		return (m_obj->*m_memFn)(p1, p2);
	}
	ResultType operator()(typename ParentFunctor::Param1 p1, typename ParentFunctor::Param2 p2, typename ParentFunctor::Param3 p3)
	{
		return (m_obj->*m_memFn)(p1, p2, p3);
	}

private:
	PointerToObj m_obj;
	PointerToMemFn m_memFn;
};
