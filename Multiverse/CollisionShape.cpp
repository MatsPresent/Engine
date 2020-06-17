#include "MultiversePCH.h"
#include "CollisionShape.h"

#include <algorithm>	// copy, min, max, minmax, min_element, minmax_element
#include <cmath>	// abs, atan2, cos, sin, sqrt
#include <limits>	// numeric_limits
#include <utility>	// move, pair



enum RotationDirection
{
	anticlockwise = -1,
	colinear = 0,
	clockwise = 1
};

RotationDirection RotDir(const mv::vec2f& a, const mv::vec2f& b, const mv::vec2f& c)
{
	float val{ (b.y() - a.y()) * (c.x() - b.x()) - (b.x() - a.x()) * (c.y() - b.y()) };
	return static_cast<RotationDirection>((0.f < val) - (val < 0.f));
};


bool overlap(float amin, float amax, float bmin, float bmax, float& overlap) {
	float oab{ amax - bmin };
	float oba{ bmax - amin };
	bool retval{ oab >= 0.f && oba >= 0.f };
	overlap = (oab < oba ? -oab : oba); // add overlap to a to seperate
	return retval;
}




mv::CollisionShape<2>::CollisionShape()
	: _type{ Type::none }
{}

mv::CollisionShape<2>::CollisionShape(const Point& p)
	: _point(p), _type{ Type::point }
{}

mv::CollisionShape<2>::CollisionShape(const Line& l)
	: _line(l), _type{ Type::line }
{}

mv::CollisionShape<2>::CollisionShape(const Rectangle& r)
	: _rectangle(r), _type{ Type::rectangle }
{}

mv::CollisionShape<2>::CollisionShape(const Ellipse& e)
	: _ellipse(e), _type{ Type::ellipse }
{}

mv::CollisionShape<2>::CollisionShape(const Convex& x)
	: _convex(x), _type{ Type::convex }
{}

mv::CollisionShape<2>::CollisionShape(Convex&& x)
	: _convex(std::move(x)), _type{ Type::convex }
{}

mv::CollisionShape<2>::CollisionShape(const CollisionShape<2>& obj)
	: _type{ obj._type }
{
	switch (this->_type)
	{
	case Type::point:
		this->_point = obj._point;
		break;
	case Type::line:
		this->_line = obj._line;
		break;
	case Type::rectangle:
		this->_rectangle = obj._rectangle;
		break;
	case Type::ellipse:
		this->_ellipse = obj._ellipse;
		break;
	case Type::convex:
		this->_convex = obj._convex;
		break;
	}
}

mv::CollisionShape<2>::CollisionShape(CollisionShape<2>&& obj) noexcept
	: _type{ obj._type }
{
	switch (this->_type)
	{
	case Type::point:
		this->_point = std::move(obj._point);
		break;
	case Type::line:
		this->_line = std::move(obj._line);
		break;
	case Type::rectangle:
		this->_rectangle = std::move(obj._rectangle);
		break;
	case Type::ellipse:
		this->_ellipse = std::move(obj._ellipse);
		break;
	case Type::convex:
		this->_convex = std::move(obj._convex);
		break;
	}
}


mv::CollisionShape<2>::~CollisionShape() = default;


mv::CollisionShape<2>& mv::CollisionShape<2>::operator=(const CollisionShape<2>& obj)
{
	this->_type = obj._type;

	switch (this->_type)
	{
	case Type::point:
		this->_point = obj._point;
		break;
	case Type::line:
		this->_line = obj._line;
		break;
	case Type::rectangle:
		this->_rectangle = obj._rectangle;
		break;
	case Type::ellipse:
		this->_ellipse = obj._ellipse;
		break;
	case Type::convex:
		this->_convex = obj._convex;
		break;
	}
	return *this;
}

mv::CollisionShape<2>& mv::CollisionShape<2>::operator=(CollisionShape<2>&& obj) noexcept
{
	this->_type = obj._type;

	switch (this->_type)
	{
	case Type::point:
		this->_point = std::move(obj._point);
		break;
	case Type::line:
		this->_line = std::move(obj._line);
		break;
	case Type::rectangle:
		this->_rectangle = std::move(obj._rectangle);
		break;
	case Type::ellipse:
		this->_ellipse = std::move(obj._ellipse);
		break;
	case Type::convex:
		this->_convex = std::move(obj._convex);
		break;
	}
	return *this;
}



bool mv::CollisionShape<2>::collides(const CollisionShape& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const
{
	switch (this->_type)
	{
	case Type::point:
		switch (other._type)
		{
		case Type::point:
			return this->_point.collides(other._point, t0, t1, mtv);
		} break;
	case Type::line:
		switch (other._type)
		{
		case Type::point:
			return this->_line.collides(other._point, t0, t1, mtv);
		case Type::line:
			return this->_line.collides(other._line, t0, t1, mtv);
		} break;
	case Type::rectangle:
		switch (other._type)
		{
		case Type::point:
			return this->_rectangle.collides(other._point, t0, t1, mtv);
		case Type::line:
			return this->_rectangle.collides(other._line, t0, t1, mtv);
		case Type::rectangle:
			return this->_rectangle.collides(other._rectangle, t0, t1, mtv);
		} break;
	case Type::ellipse:
		switch (other._type)
		{
		case Type::point:
			return this->_ellipse.collides(other._point, t0, t1, mtv);
		case Type::line:
			return this->_ellipse.collides(other._line, t0, t1, mtv);
		case Type::rectangle:
			return this->_ellipse.collides(other._rectangle, t0, t1, mtv);
		case Type::ellipse:
			return this->_ellipse.collides(other._ellipse, t0, t1, mtv);
		} break;
	case Type::convex:
		switch (other._type)
		{
		case Type::point:
			return this->_convex.collides(other._point, t0, t1, mtv);
		case Type::line:
			return this->_convex.collides(other._line, t0, t1, mtv);
		case Type::rectangle:
			return this->_convex.collides(other._rectangle, t0, t1, mtv);
		case Type::ellipse:
			return this->_convex.collides(other._ellipse, t0, t1, mtv);
		case Type::convex:
			return this->_convex.collides(other._convex, t0, t1, mtv);
		} break;
	case Type::none:
		mtv = vec2f{ 0.f, 0.f };
		return false;
	}

	bool retval{ other.collides(*this, t1, t0, mtv) };
	mtv *= -1;
	return retval;
}

bool mv::CollisionShape<2>::collides(const CollisionShape& other, const mat3f& t0, const mat3f& t1) const
{
	vec2f mtvDummy;
	return this->collides(other, t0, t1, mtvDummy);
}



const mv::CollisionShape<2>::Point& mv::CollisionShape<2>::as_point() const
{
	return this->_point;
}

const mv::CollisionShape<2>::Line& mv::CollisionShape<2>::as_line() const
{
	return this->_line;
}

const mv::CollisionShape<2>::Rectangle& mv::CollisionShape<2>::as_rectangle() const
{
	return this->_rectangle;
}

const mv::CollisionShape<2>::Ellipse& mv::CollisionShape<2>::as_ellipse() const
{
	return this->_ellipse;
}

const mv::CollisionShape<2>::Convex& mv::CollisionShape<2>::as_convex() const
{
	return this->_convex;
}


mv::CollisionShape<2>::Type mv::CollisionShape<2>::type() const
{
	return this->_type;
}




bool mv::CollisionShape<2>::Point::collides(const Point&, const mat3f&, const mat3f&, vec2f& mtv) const
{
	mtv = vec2f{ 0.f, 0.f };
	return false;
}




bool mv::CollisionShape<2>::Line::collides(const Point&, const mat3f&, const mat3f&, vec2f& mtv) const
{
	mtv = vec2f{ 0.f, 0.f };
	return false;
}

bool mv::CollisionShape<2>::Line::collides(const Line& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const
{
	vec2f a0{ t0 * vec3f{ this->p0, 1.f } };
	vec2f a1{ t0 * vec3f{ this->p1, 1.f } };
	vec2f b0{ t1 * vec3f{ other.p0, 1.f } };
	vec2f b1{ t1 * vec3f{ other.p1, 1.f } };
	float oA, oB;

	vec2f aAxis{ (a1 - a0).cross().normalise() };
	float ap{ aAxis.dot(a0) };
	std::pair<float, float> bpMinMax{ std::minmax(aAxis.dot(b0), aAxis.dot(b1)) };
	if (!overlap(ap, ap, bpMinMax.first, bpMinMax.second, oA)) {
		mtv = vec2f{ 0.f, 0.f };
		return false;
	}

	vec2f bAxis{ (b1 - b0).cross().normalise() };
	float bp{ bAxis.dot(b0) };
	std::pair<float, float> apMinMax{ std::minmax(bAxis.dot(a0), bAxis.dot(a1)) };
	if (!overlap(apMinMax.first, apMinMax.second, bp, bp, oB)) {
		mtv = vec2f{ 0.f, 0.f };
		return false;
	}

	mtv = std::abs(oA) < std::abs(oB) ? aAxis * oA : bAxis * oB;
	return true;
}




mv::CollisionShape<2>::Rectangle::Rectangle(const vec2f& lower_xy, const vec2f& upper_xy, float angle)
	: _lower_xy{ lower_xy }, _upper_xy{ upper_xy }, _cos_a{ std::cos(angle) }, _sin_a{ std::sin(angle) }
{}



bool mv::CollisionShape<2>::Rectangle::collides(const Point& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const
{
	mat3f t0r{ this->apply_rotation(t0) };
	mat3f t{ t0r.inverse() * t1 };
	vec2f p{ t * vec3f{ other.p0, 1.f } };

	float o[2];
	if (!overlap(this->lower_x(), this->upper_x(), p.x(), p.x(), o[0]) ||
		!overlap(this->lower_y(), this->upper_y(), p.y(), p.y(), o[1])) {
		mtv = vec2f{ 0.f, 0.f };
		return false;
	}

	unsigned int i{ static_cast<unsigned int>(std::abs(o[0]) > std::abs(o[1])) };
	mtv = static_cast<vec2f>(t0r * vec3f{ vec2f::basis()[i] * o[i], 1.f }) - static_cast<vec2f>(t0r.get_column(2));
	return true;
}

bool mv::CollisionShape<2>::Rectangle::collides(const Line& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const
{
	mat3f t0r{ this->apply_rotation(t0) };
	mat3f ta{ t0r.inverse() * t1 };
	vec2f b0{ ta * vec3f{ other.p0, 1.f } };
	vec2f b1{ ta * vec3f{ other.p1, 1.f } };

	float oax, oay, ob; // overlap values

	// rect axes
	if (!overlap(this->lower_x(), this->upper_x(), std::min(b0.x(), b1.x()), std::max(b0.x(), b1.x()), oax) ||
		!overlap(this->lower_y(), this->upper_y(), std::min(b0.y(), b1.y()), std::max(b0.y(), b1.y()), oay)) {
		mtv = vec2f{ 0.f, 0.f };
		return false;
	}

	// line axis
	vec2f a[4]{ this->lower_xy(), vec2f{ this->upper_x(), this->lower_y() }, this->upper_xy(), vec2f{ this->lower_x(), this->upper_y() } };
	b0 = t1 * vec3f{ other.p0, 1.f };
	b1 = t1 * vec3f{ other.p1, 1.f };
	vec2f line_axis{ (b1 - b0).cross().normalise() };
	float bp{ line_axis.dot(b0) }; // normal axis projection is the same for b0 and b1, like the y axis projection of a horizontal line
	float ap[4];
	for (unsigned int i{ 0 }; i < 4; ++i) {
		ap[i] = line_axis.dot(vec2f{ t0r * vec3f{ a[i], 1.f } });
	}
	std::pair<float*, float*> ap_min_max{ std::minmax_element(ap, ap + 4) };
	if (!overlap(*ap_min_max.first, *ap_min_max.second, bp, bp, ob)) {
		mtv = vec2f{ 0.f, 0.f };
		return false;
	}

	// get mtv
	if (std::abs(ob) < std::abs(oax) && std::abs(ob) < std::abs(oay)) { // shortest distance is over line axis
		mtv = line_axis * ob;
	}
	else if (std::abs(oax) < std::abs(oay)) {
		mtv = t0r * vec3f{ mtv.basis()[0] * oax, 1.f } - t0r.get_column(2);
	}
	else {
		mtv = t0r * vec3f{ mtv.basis()[1] * oay, 1.f } - t0r.get_column(2);
	}
	return true;
}

bool mv::CollisionShape<2>::Rectangle::collides(const Rectangle& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const
{
	mat3f t0r{ this->apply_rotation(t0) };
	mat3f t1r{ other.apply_rotation(t1) };
	float o[4];

	mat3f ta{ t1r.inverse() * t0r };
	vec2f pa[4]{ this->lower_xy(), vec2f{ this->upper_x(), this->lower_y() },
		this->upper_xy(), vec2f{ this->lower_x(), this->upper_y() } };
	vec2f mina{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
	vec2f maxa{ -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };
	for (vec2f& v : pa) {
		v = ta * vec3f{ v, 1.f };
		mina.x() = std::min(mina.x(), v.x());
		mina.y() = std::min(mina.y(), v.y());
		maxa.x() = std::max(maxa.x(), v.x());
		maxa.y() = std::max(maxa.y(), v.y());
	}
	if (!overlap(mina.x(), maxa.x(), other.lower_x(), other.upper_x(), o[0]) ||
		!overlap(mina.y(), maxa.y(), other.lower_y(), other.upper_y(), o[1])) {
		mtv = vec2f{ 0.f, 0.f };
		return false;
	}

	mat3f tb{ t0r.inverse() * t1r };
	vec2f pb[4]{ other.lower_xy(), vec2f{ other.upper_x(), other.lower_y() },
		other.upper_xy(), vec2f{ other.lower_x(), other.upper_y() } };
	vec2f minb{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
	vec2f maxb{ -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };
	for (vec2f& v : pb) {
		v = tb * vec3f{ v, 1.f };
		minb.x() = std::min(minb.x(), v.x());
		minb.y() = std::min(minb.y(), v.y());
		maxb.x() = std::max(maxb.x(), v.x());
		maxb.y() = std::max(maxb.y(), v.y());
	}
	if (!overlap(this->lower_x(), this->upper_x(), minb.x(), maxb.x(), o[2]) ||
		!overlap(this->lower_y(), this->upper_y(), minb.y(), maxb.y(), o[3])) {
		mtv = vec2f{ 0.f, 0.f };
		return false;
	}

	std::ptrdiff_t i{ std::min_element(o, o + 4, [](float lhs, float rhs) { return std::abs(lhs) < std::abs(rhs); }) - o };
	mtv = static_cast<vec2f>((i < 2 ? t1r : t0r) * vec3f{ vec2f::basis()[i % 2] * o[i], 1.f }) -
		static_cast<vec2f>((i < 2 ? t1r : t0r).get_column(2));
	return true;
}



const mv::vec2f& mv::CollisionShape<2>::Rectangle::lower_xy() const
{
	return this->_lower_xy;
}

const mv::vec2f& mv::CollisionShape<2>::Rectangle::upper_xy() const
{
	return this->_upper_xy;
}

float mv::CollisionShape<2>::Rectangle::lower_x() const
{
	return this->_lower_xy.x();
}

float mv::CollisionShape<2>::Rectangle::lower_y() const
{
	return this->_lower_xy.y();
}

float mv::CollisionShape<2>::Rectangle::upper_x() const
{
	return this->_upper_xy.x();
}

float mv::CollisionShape<2>::Rectangle::upper_y() const
{
	return this->_upper_xy.y();
}


float mv::CollisionShape<2>::Rectangle::angle() const
{
	return std::atan2(this->_sin_a, this->_cos_a);
}


mv::mat3f mv::CollisionShape<2>::Rectangle::apply_rotation(const mat3f& m) const
{
	return mat3f{
		m[0][0] * this->_cos_a + m[0][1] * this->_sin_a, m[0][0] * -this->_sin_a + m[0][1] * this->_cos_a, m[0][2],
		m[1][0] * this->_cos_a + m[1][1] * this->_sin_a, m[1][0] * -this->_sin_a + m[1][1] * this->_cos_a, m[1][2],
		0.f, 0.f, 1.f
	};
}




mv::CollisionShape<2>::Ellipse::Ellipse(const vec2f& centre, const vec2f& radii, float angle)
	: _centre{ centre }, _radii{ radii }, _cos_a{ std::cos(angle) }, _sin_a{ std::sin(angle) }
{}



bool mv::CollisionShape<2>::Ellipse::collides(const Point& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const
{
	mat3f t0t{ this->apply_transform(t0) };
	mat3f ta{ t0t.inverse() * t1 };
	vec2f p0{ ta * vec3f{ other.p0, 1.f } };

	float sqrm{ p0.squared_magnitude() };
	bool retval{ sqrm <= 1.f };
	mtv = (retval ? static_cast<vec2f>(t0t * vec3f{ p0, 1.f } - t0t * vec3f{ p0 / std::sqrt(sqrm), 1.f }) : vec2f{ 0.f, 0.f });
	return retval;
}

bool mv::CollisionShape<2>::Ellipse::collides(const Line& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const
{
	mat3f t0t{ this->apply_transform(t0) };
	mat3f ta{ t0t.inverse() * t1 };
	vec2f p0{ ta * vec3f{ other.p0, 1.f } };
	vec2f p1{ ta * vec3f{ other.p1, 1.f } };
	float lineMag{ static_cast<float>((p1 - p0).magnitude()) };
	vec2f lineNorm{ (p1 - p0) / lineMag };
	float isecProj{ lineNorm.dot(-p0) };
	vec2f isecPt{ isecProj < 0 ? p0 : (isecProj > lineMag ? p1 : lineNorm * isecProj + p0) };

	float sqrm{ isecPt.squared_magnitude() };
	bool retval{ sqrm <= 1.f };
	mtv = (retval ? static_cast<vec2f>(t0t * vec3f{ isecPt, 1.f } -t0t * vec3f{ isecPt / std::sqrt(sqrm), 1.f }) : vec2f{ 0.f, 0.f });
	return retval;
}

bool mv::CollisionShape<2>::Ellipse::collides(const Rectangle& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const
{
	static const float min_sqr_mag{ 1.f / 65536.f }; // distance small enough to be considered zero

	mat3f t0t{ this->apply_transform(t0) };
	mat3f ta{ t0t.inverse() * other.apply_rotation(t1) };
	vec2f p[4]{ { ta * vec3f{ other.lower_xy(), 1.f } },
		{ ta * vec3f{ other.upper_x(), other.lower_y(), 1.f } },
		{ ta * vec3f{ other.upper_xy(), 1.f } },
		{ ta * vec3f{ other.lower_x(), other.upper_y(), 1.f } }, };

	// the ellipse's centre within the rectangle
	vec2f o{ 0.f, 0.f };
	RotationDirection p01o{ RotDir(p[0], p[1], o) };
	bool a_in_b{ (p01o == RotationDirection::colinear) ||
		((p01o != -RotDir(p[1], p[2], o)) && (p01o != -RotDir(p[2], p[3], o)) && (p01o != -RotDir(p[3], p[0], o))) };

	// any of the rectangle's edges are at least partially inside the ellipse
	float closest_sqrm{ std::numeric_limits<float>::infinity() };
	vec2f closest_pt;
	for (unsigned int i{ 0 }; i < 4; ++i) {
		unsigned int j{ (i + 1) % 4 };
		float line_mag{ static_cast<float>((p[j] - p[i]).magnitude()) };
		vec2f line_norm{ (p[j] - p[i]) / line_mag };
		float isec_proj{ line_norm.dot(-p[i]) };
		vec2f isec_pt{ isec_proj < 0 ? p[i] : (isec_proj > line_mag ? p[j] : line_norm * isec_proj + p[i]) };
		float sqrm{ isec_pt.squared_magnitude() };
		if (sqrm < closest_sqrm) {
			closest_sqrm = sqrm;
			if (sqrm >= min_sqr_mag) {
				closest_pt = isec_pt;
			}
			else {
				mtv = p[0];
				for (unsigned int k{ 1 }; k < 4; ++k) {
					mtv += p[k];
				}
				mtv.normalise();
				return true;
			}
		}
	}

	if (a_in_b || closest_sqrm <= 1.f) {
		mtv = t0t.get_column(2) - t0t * vec3f{ ((a_in_b ? -closest_pt : closest_pt) / std::sqrt(closest_sqrm)) - closest_pt, 1.f } ;
		return true;
	}
	else {
		mtv = vec2f{ 0.f, 0.f };
		return false;
	}
}

bool mv::CollisionShape<2>::Ellipse::collides(const Ellipse& other, const mat3f& t0, const mat3f& t1, vec2f& mtv) const
{
	static const unsigned int precision{ 8u };
	static const float min_sqr_mag{ 1.f / 65536.f }; // distance small enough to be considered zero

	mat3f t0t{ this->apply_transform(t0) };
	mat3f t1t{ other.apply_transform(t1) };
	mat3f ta{ t1t.inverse() * t0t };
	mat3f tb{ t0t.inverse() * t1t };

	// check if center of a is inside ellipse b
	bool a_in_b{ static_cast<vec2f>(ta * vec3f{ this->centre(), 1.f }).squared_magnitude() < 1.f };

	float angle{ 0.f };
	float delta_a{ static_cast<float>(2. * pi / 3.) };
	vec2f p[3]{ vec2f{}, static_cast<vec2f>(tb * vec3f{ 1.f, 0.f, 1.f }), vec2f{} };
	float sqrms[3]{ 0.f, p[1].squared_magnitude(), 0.f };

	for (unsigned int i{ 0u }; i < precision; ++i, delta_a /= 2.f) {
		p[0] = static_cast<vec2f>(tb * vec3f{ std::cos(angle - delta_a), std::sin(angle - delta_a), 1.f });
		p[2] = static_cast<vec2f>(tb * vec3f{ std::cos(angle + delta_a), std::sin(angle + delta_a), 1.f });
		sqrms[0] = p[0].squared_magnitude();
		sqrms[2] = p[2].squared_magnitude();

		std::ptrdiff_t closest{ std::min_element(sqrms, sqrms + 3) - sqrms };
		p[1] = p[closest];
		sqrms[1] = sqrms[closest];
		angle += (closest - 1) * delta_a;
	}

	if (a_in_b || sqrms[1] <= 1.f) {
		mtv = ((a_in_b ? -p[1] : p[1]) / std::sqrt(sqrms[1])) - p[1];
		mtv = t0t.get_column(2) - t0t * vec3f{ mtv, 1.f };
		return true;
	}
	else {
		mtv = vec2f{ 0.f, 0.f };
		return false;
	}
}



const mv::vec2f& mv::CollisionShape<2>::Ellipse::centre() const
{
	return this->_centre;
}

const mv::vec2f& mv::CollisionShape<2>::Ellipse::radii() const
{
	return this->_radii;
}


float mv::CollisionShape<2>::Ellipse::angle() const
{
	return std::atan2(this->_sin_a, this->_cos_a);
}


mv::mat3f mv::CollisionShape<2>::Ellipse::apply_rotation(const mat3f& m) const
{
	return mat3f{
		m[0][0] * this->_cos_a + m[0][1] * this->_sin_a, m[0][0] * -this->_sin_a + m[0][1] * this->_cos_a, m[0][2],
		m[1][0] * this->_cos_a + m[1][1] * this->_sin_a, m[1][0] * -this->_sin_a + m[1][1] * this->_cos_a, m[1][2],
		0.f, 0.f, 1.f
	};
}

mv::mat3f mv::CollisionShape<2>::Ellipse::apply_transform(const mat3f& m) const
{
	return m * mat3f{
		this->_radii.x() * this->_cos_a, this->_radii.y() * -this->_sin_a, this->_centre.x(),
		this->_radii.x() * this->_sin_a, this->_radii.y() * this->_cos_a , this->_centre.y(),
		0.f, 0.f, 1.f
	};
}




mv::CollisionShape<2>::Convex::Convex()
	: _vertices{ nullptr }, _vertex_count{ 0 }
{}

mv::CollisionShape<2>::Convex::Convex(std::initializer_list<vec2f> il)
	: _vertices{ new vec2f[il.size()]{} }, _vertex_count{ static_cast<unsigned int>(il.size()) }
{
	std::copy(il.begin(), il.end(), this->_vertices);
}

mv::CollisionShape<2>::Convex::Convex(const Convex& obj)
	: _vertices{ new vec2f[obj._vertex_count]{} }, _vertex_count{ obj._vertex_count }
{
	std::copy(obj._vertices, obj._vertices + obj._vertex_count, this->_vertices);
}

mv::CollisionShape<2>::Convex::Convex(Convex&& obj) noexcept
	: _vertices{ obj._vertices }, _vertex_count{ obj._vertex_count }
{
	obj._vertices = nullptr;
	obj._vertex_count = 0;
}


mv::CollisionShape<2>::Convex::~Convex()
{
	delete[] this->_vertices;
	this->_vertices = nullptr;
	this->_vertex_count = 0;
}


mv::CollisionShape<2>::Convex& mv::CollisionShape<2>::Convex::operator=(const Convex& obj)
{
	if (this != &obj) {
		delete[] this->_vertices;
		this->_vertices = new vec2f[obj._vertex_count]{};
		for (unsigned int i{ 0 }; i < this->_vertex_count; ++i) {
			this->_vertices[i] = obj._vertices[i];
		}
		this->_vertex_count = obj._vertex_count;
	}
	return *this;
}

mv::CollisionShape<2>::Convex& mv::CollisionShape<2>::Convex::operator=(Convex&& obj) noexcept
{
	if (this != &obj) {
		delete[] this->_vertices;
		this->_vertices = obj._vertices;
		this->_vertex_count = obj._vertex_count;
		obj._vertices = nullptr;
		obj._vertex_count = 0;
	}
	return *this;
}



bool mv::CollisionShape<2>::Convex::collides(const Point&, const mat3f&, const mat3f&, vec2f&) const
{
	return false;
}

bool mv::CollisionShape<2>::Convex::collides(const Line&, const mat3f&, const mat3f&, vec2f&) const
{
	return false;
}

bool mv::CollisionShape<2>::Convex::collides(const Rectangle&, const mat3f&, const mat3f&, vec2f&) const
{
	return false;
}

bool mv::CollisionShape<2>::Convex::collides(const Ellipse&, const mat3f&, const mat3f&, vec2f&) const
{
	return false;
}

bool mv::CollisionShape<2>::Convex::collides(const Convex&, const mat3f&, const mat3f&, vec2f&) const
{
	return false;
}



const mv::vec2f& mv::CollisionShape<2>::Convex::operator[](unsigned int i) const
{
	return this->_vertices[i];
}

mv::vec2f& mv::CollisionShape<2>::Convex::operator[](unsigned int i)
{
	return this->_vertices[i];
}


unsigned int mv::CollisionShape<2>::Convex::size() const
{
	return this->_vertex_count;
}



template class mv::CollisionShape<2>;
template class mv::CollisionShape<3>;
