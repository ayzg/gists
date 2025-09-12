#pragma once

#include "boost/variant.hpp"
#include <unordered_map>
#include <utility>
#include <memory>
#include <vector>
#include <variant>

class ComponentBase
{
public:
	virtual ~ComponentBase() = 0;
};

class EntityBase
{
	virtual ~EntityBase() = 0;
};


template <typename T, typename... TArgs>
T& createComponent(TArgs&&... mArgs)
{
	static_assert(std::is_base_of<ComponentBase, T>::value, "T must inherit from ComponentBase");
	auto* new_comp(new T(std::forward<TArgs>(mArgs)...));
	return *new_comp;
}

template<typename T, template <typename T, typename... TArgs>typename... TComponents>
T& createEntity(TComponents&&... cArgs)
{
	auto* new_entity(new T(std::forward<TComponents>(cArgs)...))
}

class EntityKernel
{
	std::vector<std::unique_ptr<EntityBase>> vector;

	template <class T>
	T get(int id){

	}
};

class RealEntity : public EntityBase
{

};

class RealComponent : public ComponentBase
{
public:
	RealComponent(int i, int j) {};
	~RealComponent() = default;
	int data;
	int operator()(int message) const
	{
		return message;
	}
};


void test()
{
	auto c = createComponent<RealComponent>;
}




using TypeInfoRef = std::reference_wrapper<const std::type_info>;
struct TypeInfoHasher {
	std::size_t operator()(TypeInfoRef code) const
	{
		return code.get().hash_code();
	}
};
struct TypeInfoHasherEqualTo {
	bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const
	{
		return lhs.get() == rhs.get();
	}
};

using component_wptr_id = std::pair<std::size_t, std::weak_ptr<Component>>;
using component_wptr_id_vector = std::vector<component_wptr_id>;

class ComponentIdentifier
{
	std::unordered_map<TypeInfoRef, component_wptr_id_vector, TypeInfoHasher, TypeInfoHasherEqualTo> component_map;
	std::map<std::size_t, TypeInfoRef> unique_cid_map;

	const std::size_t get_unique_cid(const TypeInfoRef& type) noexcept
	{
		static std::size_t last_cid{ 0u };
		unique_cid_map[last_cid] = type;
		return last_cid++;
	}

	bool exists(std::size_t id) { return (unique_cid_map.count(id) != 0); }

	const component_wptr_id & insert(const std::shared_ptr<Component> & new_component)
	{
		const auto & c_type(typeid(new_component.get()));
		if (component_map.count(c_type) != 0)
		{
			component_map.at(c_type).push_back(
				std::make_pair<std::size_t, std::weak_ptr<Component>>(get_unique_cid(c_type), std::weak_ptr(new_component)));
		}
		else
		{
			component_map[c_type] = component_wptr_id_vector
			{ std::make_pair<std::size_t,std::weak_ptr<Component>>(get_unique_cid(c_type),std::weak_ptr(new_component)) };
		}

		return *(--component_map[c_type].cend());
	}
	const std::weak_ptr<Component> & get_by_id(std::size_t id)
	{
		if (exists(id))
		{
			return component_map[unique_cid_map.at(id)].
		}
	}

	template<typename T>
	void remove_by_type()
	{
		static_assert(std::is_base_of<Component, T>::value, "Attemping to remove a non-component type from component identity map.");
		component_map.erase(typeid(T));
	}
};
