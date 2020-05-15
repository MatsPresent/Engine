#include "MultiversePCH.h"
#include "Position.h"

void mv::Position::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}
