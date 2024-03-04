#include "pch.h"
#include "ImpColor.h"

ImpStructure::ImpColor::ImpColor(float r, float g, float b, float a)
	:r(r),g(g),b(b),a(a)
{}

ImpStructure::ImpColor::ImpColor()
	:r(1.f),g(1.f),b(1.f),a(1.f)
{}
