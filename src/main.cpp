#if defined(_WIN32)           
	#define NOGDI             // All GDI defines and routines
	#define NOUSER            // All USER defines and routines
#endif

#include <Windows.h> // or any library that uses Windows.h

#if defined(_WIN32)           // raylib uses these names as function parameters
	#undef near
	#undef far
	#undef min
	#undef max
	#undef PlaySound
#endif

#include <unordered_map>
#include <optional>
#include <iostream>
#include <fstream>
#include <string>

#include <spdlog/spdlog.h>
#include <raylib-cpp/raylib-cpp.hpp>
#include <lua.hpp>
#include <ink/story.h>
#include <ink/runner.h>
#include <ink/globals.h>
#include <ink/choice.h>

#include "Paragraph.hpp"
#include "LinebreakNode.hpp"
#include "hyphen/hyphen.h"
#include "tweeny/tweeny-3.2.0.h"

struct Story
{
	const float CHARACTERS_PER_SECOND = 30;
	const float CHOICE_FADE_IN_DURATION = 0.2f;
	const float CHOICE_FADE_IN_DELAY = 0.15f;

	ink::runtime::runner runner;
	ink::runtime::globals globals;
	typeset::LinebreakBuilder linebreakBuilder;
	float maxWidth;
	float fontSize;

	std::vector<Paragraph> mainTextParagraphs;
	std::vector<Paragraph> storyChoices;

	tweeny::tween<int> _mainParagraphTween;
	std::vector<tweeny::tween<float>> _choiceTweens;

	Story(ink::runtime::story* story, std::shared_ptr<raylib::Font> font, std::string hyphenDictFile, float maxWidth, float fontSize)
		: linebreakBuilder(font, std::shared_ptr<HyphenDict>(hnj_hyphen_load(hyphenDictFile.c_str())))
	{
		this->maxWidth = maxWidth;
		this->fontSize = fontSize;

		globals = story->new_globals();
		runner = story->new_runner(globals);
		runner->move_to(ink::hash_string("Fall"));
		Refresh();
	}

	void Choose(int index)
	{
		runner->choose(index);
		Refresh();
	}

	void Update(float deltaTime)
	{
		int deltaTicks = (int)std::round(deltaTime * 1000.f);

		if (_mainParagraphTween.progress() < 1)
		{
			_mainParagraphTween.step(deltaTicks);
		}
		else
		{
			for (auto& tween : _choiceTweens)
			{
				tween.step(deltaTicks);
			}
		}
	}

	void Refresh()
	{
		mainTextParagraphs.clear();
		storyChoices.clear();

		int totalCharacters = 0;

		while (runner->can_continue())
		{
			std::string str = runner->getline();

			typeset::LinebreakResult typeset = linebreakBuilder.BuildResult(str, maxWidth, fontSize);
			Paragraph paragraph{ str, typeset };
			mainTextParagraphs.push_back(paragraph);

			for (int i = 0; i < paragraph.result.words.size(); i++)
			{
				totalCharacters += paragraph.result.words[i].end - paragraph.result.words[i].start + 1;
			}
		}

		_mainParagraphTween = tweeny::from(0).to(totalCharacters + 1).during((int)std::round(totalCharacters / CHARACTERS_PER_SECOND * 1000.f));
		_choiceTweens.clear();

		if (runner->has_choices())
		{
			for (ink::size_t i = 0; i < runner->num_choices(); i++)
			{
				const ink::runtime::choice* choice = runner->get_choice(i);
				std::string str = choice->text();
				typeset::LinebreakResult typeset = linebreakBuilder.BuildResult(str, maxWidth, fontSize);
				Paragraph paragraph{ str, typeset };
				storyChoices.push_back(paragraph);

				auto tween = tweeny::from(0.f)
					.to(0.f).during((int)std::round(CHOICE_FADE_IN_DELAY * i * 1000.f))
					.to(1.f).during((int)std::round(CHOICE_FADE_IN_DURATION * 1000.f));
				_choiceTweens.push_back(tween);
			}
		}
	}
};

int main()
{
	lua_State* luaState = luaL_newstate();
	luaL_openlibs(luaState);

	auto status = luaL_dofile(luaState, "assets/lua/config.lua");
	if (status)
	{
		std::cerr << "Couldn't load lua file lua/config.lua: " << lua_tostring(luaState, -1);
	}

	std::cout << "load file status: " << status << std::endl;

	int windowWidth = 1024;
	int windowHeight = 768;

	lua_getglobal(luaState, "config");
	lua_getfield(luaState, 1, "resolution");
	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	windowWidth = (int)lua_tonumber(luaState, 3);
	windowHeight = (int)lua_tonumber(luaState, 4);
	lua_settop(luaState, 0);

	// set up the window
	raylib::Window window(windowWidth, windowHeight, "window");

	window.SetTargetFPS(60);

	float playAreaWidth = 1080;
	float playAreaHeight = (float)windowHeight / windowWidth * playAreaWidth;
	raylib::Rectangle playArea(-playAreaWidth / 2, -playAreaHeight / 2, playAreaWidth, playAreaHeight);
	raylib::Camera2D camera(raylib::Vector2(0, 0), raylib::Vector2(0, 0), 0, (float)windowWidth / playAreaWidth);

	float textBoxWidth = 420;
	float textPadding = 10;
	float textWidth = textBoxWidth - textPadding * 2;
	float fontSize = 14;

    raylib::Shader shader(TextFormat("assets/shader/font/sdf_glsl%i.vs", GLSL_VERSION), TextFormat("assets/shader/font/sdf_glsl%i.fs", GLSL_VERSION));
	raylib::Rectangle storyContainerRect(playAreaWidth - textBoxWidth, 0, textBoxWidth, playAreaHeight);

	int filesize;
	unsigned char* fileData = LoadFileData("assets/font/JosefinSans-Regular.ttf", &filesize);

	// SDF font generation from TTF font
	std::shared_ptr<raylib::Font> josefinSans = std::make_shared<raylib::Font>();
	josefinSans->baseSize = 32;
	josefinSans->glyphCount = 95;
	josefinSans->glyphs = LoadFontData(fileData, filesize, 32, 0, 0, FONT_SDF);
	raylib::Image atlas = GenImageFontAtlas(josefinSans->glyphs, &josefinSans->recs, 95, 32, 0, 1);
	josefinSans->texture = LoadTextureFromImage(atlas);
	SetTextureFilter(josefinSans->texture, TEXTURE_FILTER_BILINEAR);    // Required for SDF font
	atlas.Unload();

	UnloadFileData(fileData);

	std::ifstream inkStream;
	inkStream.open("assets/ink/MageSchool.bin");
	ink::runtime::story* inkStory = ink::runtime::story::from_file("assets/ink/MageSchool.bin");

	Story story(inkStory, josefinSans, "assets/hyph_en_US.dic", textWidth, fontSize);

	// game loop
	while (!window.ShouldClose())
	{
		float deltaTime = window.GetFrameTime();
		raylib::Vector2 mousePos = camera.GetScreenToWorld(raylib::Mouse::GetPosition());

		window.BeginDrawing();
		window.ClearBackground(BLACK);
		camera.BeginMode();

		storyContainerRect.Draw(raylib::Color(20, 20, 20));
		if (storyContainerRect.CheckCollision(mousePos) &&
			raylib::Mouse::IsButtonPressed(0) &&
			story._mainParagraphTween.progress() < 1)
		{
			story._mainParagraphTween.seek(story._mainParagraphTween.duration());
		}

		story.Update(deltaTime);
		shader.SetValue(shader.GetLocation("shownCharacterCount"), &story._mainParagraphTween.peek(), SHADER_UNIFORM_INT);
		//spdlog::info("{0} {1}", story._mainParagraphTween.peek(), window.GetFrameTime());

		shader.BeginMode();

		float lineHeight = fontSize * 1.2f;
		float currentLineStart = 0;
		bool isOverChoice = false;

		for (int i = 0; i < story.mainTextParagraphs.size(); i++)
		{
			raylib::Vector2 start = raylib::Vector2{ playAreaWidth - textBoxWidth + textPadding, textPadding + currentLineStart };
			raylib::Vector2 size = story.mainTextParagraphs[i].Draw(josefinSans, start, textWidth, (float)fontSize, lineHeight);
			currentLineStart += size.y + lineHeight;
		}

		shader.EndMode();

		for (int i = 0; i < story.storyChoices.size(); i++)
		{
			const float CHOICE_FADE_IN_SHIFT = 15.f;
			const int ALL_CHARS_SHOWN = 100000;
			shader.SetValue(shader.GetLocation("shownCharacterCount"), &ALL_CHARS_SHOWN, SHADER_UNIFORM_INT);

			shader.BeginMode();

			raylib::Vector2 start = raylib::Vector2{ playAreaWidth - textBoxWidth + textPadding, textPadding + currentLineStart };
			start.x += tweeny::easing::sinusoidalOutEasing::run(story._choiceTweens[i].progress(), CHOICE_FADE_IN_SHIFT, 0.f);

			raylib::Color col = ORANGE;
			col.a = story._choiceTweens[i].progress() * UCHAR_MAX;

			raylib::Vector2 size = story.storyChoices[i].Draw(josefinSans, start, textWidth, (float)fontSize, lineHeight, col);
			currentLineStart += size.y;

			raylib::Rectangle bounding(start.x, start.y, size.x, size.y);

			//spdlog::info("{0} {1} {2} {3} {4} {5} {6}", start.x, start.y, size.x, size.y, mousePos.x, mousePos.y, bounding.CheckCollision(mousePos));

			if (bounding.CheckCollision(mousePos) && story._choiceTweens[i].progress() > 0.5f)
			{
				isOverChoice = true;

				if (raylib::Mouse::IsButtonPressed(0))
				{
					story.Choose(i);
				}
			}

			shader.EndMode();
		}

		DrawTexture(josefinSans->texture, 10, 10, raylib::WHITE);

		camera.EndMode();
		window.EndDrawing();

		if (isOverChoice)
		{
			raylib::Mouse::SetCursor(MOUSE_CURSOR_POINTING_HAND);
		}
		else
		{
			raylib::Mouse::SetCursor(MOUSE_CURSOR_DEFAULT);
		}
	}

	// cleanup
	window.Close();
	lua_close(luaState);
	return 0;
}