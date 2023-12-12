#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#include <raylib-cpp/raylib-cpp.hpp>
#include <memory>

#include "hyphen/hyphen.h"

namespace typeset
{
	struct Word
	{
		int start;
		int end;
		float wordWidth;
		bool softHyphen;
	};

	struct LinebreakLocation
	{
		int wordIndex;
		int glueItemCount;
		float wordWidth;
	};

	struct LinebreakResult
	{
		std::vector<Word> words;
		std::vector<LinebreakLocation> linebreaks;
	};

	class Paragraph
	{
	public:
		Paragraph(const std::string& str, std::shared_ptr<raylib::Font> font, std::shared_ptr<HyphenDict> hyphenDict);
		LinebreakResult BuildLinebreaks(float maxLineSize, float fontSize);
	private:
		std::string str;
		std::shared_ptr<raylib::Font> font;
		std::shared_ptr<HyphenDict> hyphenDict;
	};
}