#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#include <raylib-cpp/raylib-cpp.hpp>
#include <memory>

namespace typeset
{
	struct Word
	{
		int start;
		int end;
		float wordWidth;
	};

	struct LinebreakResult
	{
		std::vector<Word> words;
		std::vector<int> linebreaks;
	};

	class Paragraph
	{
	public:
		Paragraph(const std::string& str, std::shared_ptr<raylib::Font> font);
		LinebreakResult BuildLinebreaks(float maxLineSize, float fontSize);
	private:
		std::string str;
		std::shared_ptr<raylib::Font> font;
	};
}