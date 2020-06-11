#include "MultiversePCH.h"
#include "Transform.h"

mv::Transform<2>::Transform()
	: translate{ 0.f, 0.f }, rotate{ 0.f }, scale{ 1.f, 1.f }
{}

bool mv::Transform<2>::operator==(const Transform<2>& rhs) const
{
	return this->translate == rhs.translate && this->rotate == rhs.rotate && this->scale == rhs.scale;
}

bool mv::Transform<2>::operator!=(const Transform<2>& rhs) const
{
	return !(*this == rhs);
}




mv::Transform<3>::Transform()
	: translate{ 0.f, 0.f, 0.f }, rotate{}, scale{ 1.f, 1.f, 1.f }
{}

bool mv::Transform<3>::operator==(const Transform<3>& rhs) const
{
	return this->translate == rhs.translate && this->rotate == rhs.rotate && this->scale == rhs.scale;
}

bool mv::Transform<3>::operator!=(const Transform<3>& rhs) const
{
	return !(*this == rhs);
}


template class mv::Transform<2>;
template class mv::Transform<3>;
