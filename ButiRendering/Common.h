#pragma once

#ifdef BUTIRENDERING_EXPORTS
#define BUTIRENDERING_API __declspec(dllexport)
#else
#define BUTIRENDERING_API __declspec(dllimport)
#endif