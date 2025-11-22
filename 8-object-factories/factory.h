#pragma once

#include "typeinfo.h"

#include <exception>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>

template <typename IdentifierType, class ProductType>
class DefaultFactoryError
{
public:
	class Exception : public std::exception
	{
	public:
		Exception(IdentifierType id) : m_id(id)
		{
		}

		const char *what() const noexcept override
		{
			return "Unknown Type";
		}

		const IdentifierType &GetId() const
		{
			return m_id;
		}

	private:
		IdentifierType m_id;
	};

protected:
	static ProductType *OnUnknownType(const IdentifierType &id)
	{
		throw Exception(id);
	}
};

template <class AbstractProduct, typename IdentifierType, class ProductCreator = std::function<AbstractProduct *()>,
  template <class, class> class FactoryErrorPolicy = DefaultFactoryError>
class Factory : public FactoryErrorPolicy<IdentifierType, AbstractProduct>
{
public:
	bool Register(IdentifierType id, ProductCreator creator)
	{
		return m_creators.insert(std::make_pair(id, creator)).second;
	}

	bool Unregister(IdentifierType id)
	{
		return m_creators.erase(id) == 1;
	}

	AbstractProduct *CreateObject(IdentifierType id)
	{
		auto it = m_creators.find(id);
		if (it != m_creators.end())
			return it->second();

		return this->OnUnknownType(id);
	}

private:
	std::unordered_map<IdentifierType, ProductCreator> m_creators;
};

template <class AbstractProduct, class ProductCreator = std::function<AbstractProduct *(AbstractProduct *)>,
  template <class, class> class FactoryErrorPolicy = DefaultFactoryError>
class CloneFactory : public FactoryErrorPolicy<Loki::TypeInfo, AbstractProduct>
{
public:
	AbstractProduct *CreateObject(AbstractProduct *prototype)
	{
		if (!prototype)
			return nullptr;

		auto it = m_creators.find(Loki::TypeInfo(typeid(*prototype)));
		if (it != m_creators.end())
			return it->second(prototype);

		return this->OnUnknownType(Loki::TypeInfo(typeid(*prototype)));
	}

	bool Register(const Loki::TypeInfo &type, ProductCreator creator)
	{
		return m_creators.insert(std::make_pair(type, creator)).second;
	}

	bool Unregister(const Loki::TypeInfo &type)
	{
		return m_creators.erase(type) == 1;
	}

private:
	std::unordered_map<Loki::TypeInfo, ProductCreator> m_creators;
};
