#pragma once

#include <Box2D/Box2D.h>
#include "p2dDebugDraw.hpp"
#include <array>
#include <memory>

inline float d2r(float degs) noexcept { return degs * b2_pi / 180.f; } // radians to degrees
inline float r2d(float rads) noexcept { return rads * 180.f / b2_pi; } // radians to degrees

namespace p2d
{
	namespace gv {
		static constexpr float const scale() noexcept { return 32.f; } // Physics world pixel scale
		static constexpr float const iscale() noexcept { return 1.f/scale(); }
		static constexpr float const pixels_per_meter() noexcept { return scale(); }
		static constexpr float const meters_per_pixel() noexcept { return iscale(); }

		static constexpr float const step() noexcept { return 1.f / 60.f; } // Step rate lock to 60 frames per second
		static constexpr float const istep() noexcept { return 1.f / 60.f; } // frames per second
		static constexpr float const fps() noexcept { return istep(); }
	}
	

	/*
	 * class: p2dObject
	 * purpose: wrapper for a b2Body* to be passed into a world
				, holds references to b2BodyDef,and b2FixtureDef
*/
	struct p2dObject
	{
		std::vector<std::shared_ptr<b2Body>> m_bodies;
		b2World& m_world_;
		p2dObject(b2World& world) : m_world_(world)
		{

		}

		std::weak_ptr<b2Body> addBody(b2BodyType type)
		{
			b2BodyDef bd;
			bd.type = type;
			bd.userData = this;
			return m_bodies.emplace_back(m_world_.CreateBody(&bd));
		}

		void addFixture(const b2Shape& s, float density)
		{
			m_body->CreateFixture(&s, density);
		}

		void addJoint(const b2JointDef& j)
		{
			m_body->GetWorld()->CreateJoint(&j);
		}

		~p2dObject()
		{
			m_body->GetWorld()->DestroyBody(m_body.get());
		}
	};

	class World
	{
		int32 velocity_iterations{ 8 };
		int32 position_iterations{ 3 };
		b2Vec2 gravity{0.f,0.f};

		b2World world_{gravity};
		const b2World & world() { return world_; }

		std::unique_ptr<b2Draw> debug_draw_;
	public:
		/* constructor: World
		 * Note: i_debug_draw takes ownership of the passed object, construct inplace
		 *
		*/
		World(b2Draw* i_debug_draw = { new b2GLDraw }) : debug_draw_(std::make_unique<b2Draw>(std::move(i_debug_draw))) {

			// Set up debug draw flags
			world_.SetDebugDraw(debug_draw_.get());
			uint32 flags = 0;
			flags += b2Draw::e_shapeBit;
			flags += b2Draw::e_jointBit;
			flags += b2Draw::e_aabbBit;
			flags += b2Draw::e_pairBit;
			flags += b2Draw::e_centerOfMassBit;
			debug_draw_->SetFlags(flags);
		};
		~World();

		void update()
		{
			world_.Step(gv::step(), velocity_iterations, position_iterations);
		}

		void debug_render()
		{
			world_.DrawDebugData();
		}
	private:

	};

}