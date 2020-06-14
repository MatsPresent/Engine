#pragma once
#include "setup.h"

#include <initializer_list> // initializer_list
#include <type_traits>	// enable_if, is_same

#include "Vector.h"
#include "Matrix.h"


namespace mv
{
	template <uint dims>
	class CollisionShape;

	template <>
	class CollisionShape<2>
	{
	public:
		enum class Type
		{
			point,	// cannot collide with points or lines, even if they are in the exact same location
			line,
			rectangle,
			ellipse,
			convex,
			none
		};

		struct Point
		{
			vec2f p0;


			bool collides(const Point& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;
		};
		struct Line
		{
			vec2f p0;
			vec2f p1;


			bool collides(const Point& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;
			bool collides(const Line& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;
		};
		class Rectangle
		{
		private:
			vec2f _lower_xy;
			vec2f _upper_xy;
			float _cos_a;
			float _sin_a;

		public:
			Rectangle(const vec2f& lower_xy, const vec2f& upper_xy, float angle = 0.f);


			bool collides(const Point& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;
			bool collides(const Line& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;
			bool collides(const Rectangle& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;


			const vec2f& lower_xy() const;
			const vec2f& upper_xy() const;
			float lower_x() const;
			float lower_y() const;
			float upper_x() const;
			float upper_y() const;

			float angle() const;

			mat3f apply_rotation(const mat3f& m) const;
		};
		class Ellipse
		{
		private:
			vec2f _centre;
			vec2f _radii;
			float _cos_a;
			float _sin_a;

		public:
			Ellipse(const vec2f& centre, const vec2f& radii, float angle = 0.f);


			bool collides(const Point& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;
			bool collides(const Line& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;
			bool collides(const Rectangle& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;
			bool collides(const Ellipse& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;


			const vec2f& centre() const;
			const vec2f& radii() const;

			float angle() const;

			mat3f apply_rotation(const mat3f& m) const;
			mat3f apply_transform(const mat3f& m) const;
		};
		class Convex
		{
		private:
			vec2f* _vertices;
			unsigned int _vertex_count;

		public:
			Convex();
			explicit Convex(std::initializer_list<vec2f> il);
			template <typename InputIterator>
			Convex(InputIterator first, InputIterator last);
			Convex(const Convex& obj);
			Convex(Convex&& obj) noexcept;

			~Convex();

			Convex& operator=(const Convex& obj);
			Convex& operator=(Convex&& obj) noexcept;


			bool collides(const Point& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;
			bool collides(const Line& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;
			bool collides(const Rectangle& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;
			bool collides(const Ellipse& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;
			bool collides(const Convex& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;


			const vec2f& operator[](unsigned int i) const;
			vec2f& operator[](unsigned int i);

			unsigned int size() const;
		};

	private:
		union
		{
			Point _point;
			Line _line;
			Rectangle _rectangle;
			Ellipse _ellipse;
			Convex _convex;
		};
		Type _type;

	public:
		CollisionShape();
		CollisionShape(const Point& p);
		CollisionShape(const Line& l);
		CollisionShape(const Rectangle& r);
		CollisionShape(const Ellipse& e);
		CollisionShape(const Convex& x);
		CollisionShape(Convex&& x);
		CollisionShape(const CollisionShape<2>& other);
		CollisionShape(CollisionShape<2>&& other) noexcept;

		~CollisionShape();	// defaulted in HitBox.cpp because of implicit deletion from undefined member class destructors

		CollisionShape& operator=(const CollisionShape<2>& other);
		CollisionShape& operator=(CollisionShape<2>&& other) noexcept;


		bool collides(const CollisionShape<2>& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const;
		bool collides(const CollisionShape<2>& other, const mat3f& t0, const mat3f& t1) const;


		const Point& as_point() const;
		const Line& as_line() const;
		const Rectangle& as_rectangle() const;
		const Ellipse& as_ellipse() const;
		const Convex& as_convex() const;

		Type type() const;
	};

	template <>
	class CollisionShape<3>
	{
	public:
		CollisionShape() = default;

		bool collides(const CollisionShape&, const mat4f&, const mat4f&, vec3f&) const
		{
			return false;
		}
		bool collides(const CollisionShape&, const mat4f&, const mat4f&) const
		{
			return false;
		}
	};
}

#include "CollisionShape.inl"
