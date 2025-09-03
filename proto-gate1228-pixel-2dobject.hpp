#pragma once
#include <Box2D/Box2D.h>
#include "p2dDebugDraw.hpp"
#include <vector>
#include <memory>

/*
	 * class: p2dObject
	 * purpose: wrapper for a b2Body* to be passed into a world
				, holds references to b2BodyDef,and b2FixtureDef
*/
namespace ns_p2dObject {

	template<typename T> using shared_ptr = std::shared_ptr<T>;
	template<typename T> using weak_ptr = std::weak_ptr<T>;
	template<typename T> using vector = std::vector<T>;
	using body = b2Body;
	using fixture = b2Fixture;
	using joint = b2Joint;
	using body_def = b2Body;
	using fixture_def = b2Fixture;
	using joint_def = b2Joint;

	class p2dObject
	{
		vector<shared_ptr<body>> m_bodies;
		vector<shared_ptr<fixture>> m_fixtures;
		vector<shared_ptr<joint>> m_joints;
		b2World& m_world_;

		explicit p2dObject(b2World& world);
		~p2dObject();

		weak_ptr<body> addBody(b2BodyType type);
		weak_ptr<fixture> addFixture(size_t body_idx, const b2FixtureDef& s, float density);
		template<typename JType = b2JointDef> 
		weak_ptr<JType> addJoint(const JType&& j);

	};

	template<typename JType>
	weak_ptr<JType> p2dObject::addJoint(const JType&& j)
	{

		static_assert(std::is_base_of<b2JointDef, JType>());
		return static_cast<weak_ptr<JType>>(std::static_pointer_cast<JType>
			(m_joints.emplace_back(m_world_.CreateJoint(&j))));
	}
}