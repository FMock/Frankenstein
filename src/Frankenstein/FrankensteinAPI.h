#pragma once

/* FrankensteinAPI.h
 * Defines the FRANKENSTEIN_API macro used to export/import the engine's public
 * symbols across the DLL boundary. The engine project defines FRANKENSTEIN_EXPORTS
 * (see Frankenstein.vcxproj), so it builds with __declspec(dllexport); consumers
 * (e.g. TestApp) do not define it and therefore get __declspec(dllimport).
 */

#ifdef FRANKENSTEIN_EXPORTS
	#define FRANKENSTEIN_API __declspec(dllexport)
#else
	#define FRANKENSTEIN_API __declspec(dllimport)
#endif
