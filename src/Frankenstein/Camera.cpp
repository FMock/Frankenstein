#include "Camera.h"
#include <GL/glew.h>

namespace {
	float clampf(float value, float lo, float hi) {
		if (hi < lo) return lo;          // world smaller than viewport on this axis
		if (value < lo) return lo;
		if (value > hi) return hi;
		return value;
	}
}

Camera::Camera() {}

Camera::Camera(int viewportWidth, int viewportHeight, float worldWidth, float worldHeight) :
	m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight),
	m_worldWidth(worldWidth), m_worldHeight(worldHeight)
{
}

void Camera::setViewport(int width, int height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;
}

void Camera::setWorldSize(float width, float height)
{
	m_worldWidth = width;
	m_worldHeight = height;
}

void Camera::centerOn(float worldX, float worldY)
{
	m_x = clampf(worldX - m_viewportWidth / 2.0f, 0.0f, m_worldWidth - m_viewportWidth);
	m_y = clampf(worldY - m_viewportHeight / 2.0f, 0.0f, m_worldHeight - m_viewportHeight);
}

void Camera::apply() const
{
	glTranslatef(-m_x, -m_y, 0.0f);
}
