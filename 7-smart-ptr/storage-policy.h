#pragma once

template <class T>
class DefaultStorage
{
public:
	using StoredType = T *;
	using PointerType = T *;
	using ReferenceType = T &;

	DefaultStorage() = default;
	DefaultStorage(const StoredType &pointee) : m_pointee{pointee}
	{
	}

	PointerType operator->() const
	{
		return m_pointee;
	}

	ReferenceType operator*() const
	{
		return *m_pointee;
	}

	friend inline PointerType Get(const DefaultStorage &storage)
	{
		return storage.m_pointee;
	}

	friend inline PointerType GetPointer(const DefaultStorage &storage)
	{
		return storage.m_pointee;
	}

	friend inline StoredType &GetStored(DefaultStorage &storage)
	{
		return storage.m_pointee;
	}

	friend inline const StoredType &GetStored(const DefaultStorage &storage)
	{
		return storage.m_pointee;
	}

	friend inline ReferenceType GetReference(DefaultStorage &storage)
	{
		return *storage.m_pointee;
	}

	friend inline const StoredType &GetReference(const DefaultStorage &storage)
	{
		return *storage.m_pointee;
	}

	friend inline void Set(DefaultStorage &storage, const StoredType &pointee)
	{
		storage.m_pointee = pointee;
	}

protected:
	static StoredType Default()
	{
		return nullptr;
	}

	void Destroy()
	{
		delete m_pointee;
	}

private:
	StoredType m_pointee{Default()};
};
