#pragma once

#include "checking-policy.h"
#include "conversion-policy.h"
#include "ownership-policy.h"
#include "storage-policy.h"

template <bool flag, typename T, typename U>
struct Select
{
	using Result = T;
};

template <typename T, typename U>
struct Select<false, T, U>
{
	using Result = U;
};

// Helper to clone based on ownership policy type
template <bool isDestructive, typename OP, typename SP>
struct CloneHelper;

template <typename OP, typename SP>
struct CloneHelper<false, OP, SP>
{
	template <typename SmartPtrType>
	static void DoClone(SmartPtrType *self, typename SP::StoredType &lhs, const typename SP::StoredType &rhs)
	{
		Set(*self, self->Clone(rhs));
	}
};

template <typename OP, typename SP>
struct CloneHelper<true, OP, SP>
{
	template <typename SmartPtrType>
	static void DoClone(SmartPtrType *self, typename SP::StoredType &lhs, typename SP::StoredType &rhs)
	{
		Set(*self, OP::Clone(rhs));
	}
};

template <typename T, template <class> class OwnershipPolicy = RefCounted, class ConversionPolicy = DisallowConversion,
  template <class> class CheckingPolicy = AssertCheck, template <class> class StoragePolicy = DefaultStorage>
class SmartPtr
	: public OwnershipPolicy<typename StoragePolicy<T>::PointerType>
	, public ConversionPolicy
	, public CheckingPolicy<typename StoragePolicy<T>::StoredType>
	, public StoragePolicy<T>
{
	using SP = StoragePolicy<T>;
	using OP = OwnershipPolicy<typename SP::PointerType>;
	using CP = ConversionPolicy;
	using KP = CheckingPolicy<typename SP::StoredType>;

public:
	struct NeverMatched;

	using PointerType = typename SP::PointerType;
	using StoredType = typename SP::StoredType;
	using ReferenceType = typename SP::ReferenceType;

	using CopyArg = typename Select<OP::destructiveCopy, SmartPtr, const SmartPtr &>::Result;
	using ImplicitArg = const StoredType &;
	using ExplicitArg = typename Select<false, const StoredType &, NeverMatched>::Result;

public:
	SmartPtr()
	{
		KP::OnDefault(Get(*this));
	}

	explicit SmartPtr(ExplicitArg p) : SP(p)
	{
		KP::OnInit(Get(*this));
	}

	SmartPtr(ImplicitArg p) : SP(p)
	{
		KP::OnInit(Get(*this));
	}

	SmartPtr(CopyArg &rhs) : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
	{
		// For RefCounted, Clone is a protected member function; for others it's static
		if constexpr (!OP::destructiveCopy)
		{
			// RefCounted, DeepCopy, NoCopy - Clone takes const reference
			Set(*this, this->Clone(Get(rhs)));
		}
		else
		{
			// DestructiveCopy - Clone is static and takes non-const reference
			Set(*this, OP::Clone(GetStored(rhs)));
		}
	}

	~SmartPtr()
	{
		if (OP::Release(Get(*static_cast<SP *>(this))))
		{
			SP::Destroy();
		}
	}

	PointerType operator->()
	{
		KP::OnDereference(Get(*this));
		return SP::operator->();
	}

	PointerType operator->() const
	{
		KP::OnDereference(Get(*this));
		return SP::operator->();
	}

	ReferenceType operator*()
	{
		KP::OnDereference(Get(*this));
		return SP::operator*();
	}

	ReferenceType operator*() const
	{
		KP::OnDereference(Get(*this));
		return SP::operator*();
	}

	bool operator!() const // Enables "if (!sp) ..."
	{
		return Get(*this) == nullptr;
	}
};
