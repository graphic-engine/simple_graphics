#pragma once
#include <Windows.h>
#include <string>

namespace simple_graphics
{
	namespace windows
	{
		namespace direct3d
		{
			namespace v9
			{
				namespace engine_util
				{
					namespace converter
					{
						class TextConverter
						{
						public:
							static inline int multiByteToWideChar(const char *toConvert, LPWSTR &result)
							{
								int resultLength = MultiByteToWideChar(CP_ACP, 0, toConvert, -1, NULL, 0);
								result = (WCHAR *)malloc(resultLength * sizeof(wchar_t));
								MultiByteToWideChar(CP_ACP, 0, toConvert, -1, result, resultLength);
								return resultLength;
							}
							static inline int stringToWideChar(const std::string &toConvert, LPWSTR &result)
							{
								return multiByteToWideChar(toConvert.c_str(), result);
							}
						};
					}
				}
			}
		}
	}
}