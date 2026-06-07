#pragma once

#include "FrankensteinAPI.h"

/* Camera.h
 * A simple 2D camera/viewport. The camera defines a rectangular view into a
 * (potentially larger) world. `apply()` issues the OpenGL modelview translation
 * that scrolls the world under the fixed-size viewport, and `centerOn()` moves
 * the camera to keep a target centered while clamping to the world bounds so the
 * view never scrolls past the edge of the world.
 */
class FRANKENSTEIN_API Camera
{
public:
	Camera();
	Camera(int viewportWidth, int viewportHeight, float worldWidth, float worldHeight);

	void setViewport(int width, int height);
	void setWorldSize(float width, float height);

	// Center the view on (worldX, worldY), clamped so the viewport stays inside
	// the world. If the world is smaller than the viewport on an axis, that axis
	// is pinned to 0.
	void centerOn(float worldX, float worldY);

	// Translate the current (modelview) matrix so world-space drawing appears
	// shifted by the camera position. Call after glLoadIdentity().
	void apply() const;

	float getX() const { return m_x; }
	float getY() const { return m_y; }

private:
	float m_x = 0.0f;
	float m_y = 0.0f;
	int   m_viewportWidth = 0;
	int   m_viewportHeight = 0;
	float m_worldWidth = 0.0f;
	float m_worldHeight = 0.0f;
};
