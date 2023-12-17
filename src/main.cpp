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
#include <ink/choice.h>

#include "Paragraph.hpp"
#include "LinebreakNode.hpp"
#include "hyphen/hyphen.h"

struct Story
{
	ink::runtime::runner runner;
	typeset::LinebreakBuilder linebreakBuilder;
	float maxWidth;
	float fontSize;

	std::vector<Paragraph> paragraphs;
	std::vector<Paragraph> storyChoices;

	Story(ink::runtime::story* story, std::shared_ptr<raylib::Font> font, std::string hyphenDictFile, float maxWidth, float fontSize)
		: linebreakBuilder(font, std::shared_ptr<HyphenDict>(hnj_hyphen_load(hyphenDictFile.c_str())))
	{
		this->maxWidth = maxWidth;
		this->fontSize = fontSize;

		runner = story->new_runner();
		runner->move_to(ink::hash_string("Fall"));
		Refresh();
	}

	void Choose(int index)
	{
		runner->choose(index);
		Refresh();
	}

	void Refresh()
	{
		paragraphs.clear();
		storyChoices.clear();

		while (runner->can_continue())
		{
			std::string str = runner->getline();
			typeset::LinebreakResult typeset = linebreakBuilder.BuildResult(str, maxWidth, fontSize);
			Paragraph paragraph{ str, typeset };
			paragraphs.push_back(paragraph);
		}

		if (runner->has_choices())
		{
			for (ink::size_t i = 0; i < runner->num_choices(); i++)
			{
				const ink::runtime::choice* choice = runner->get_choice(i);
				std::string str = choice->text();
				typeset::LinebreakResult typeset = linebreakBuilder.BuildResult(str, maxWidth, fontSize);
				Paragraph paragraph{ str, typeset };
				storyChoices.push_back(paragraph);
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

    raylib::Shader shader(0, TextFormat("assets/shader/font/sdf_glsl%i.fs", GLSL_VERSION));
	raylib::Rectangle rect(playAreaWidth - textBoxWidth, 0, textBoxWidth, playAreaHeight);

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
	UnloadImage(atlas);

	UnloadFileData(fileData);

	std::ifstream inkStream;
	inkStream.open("assets/ink/MageSchool.bin");
	ink::runtime::story* inkStory = ink::runtime::story::from_file("assets/ink/MageSchool.bin");

	Story story(inkStory, josefinSans, "assets/hyph_en_US.dic", textWidth, fontSize);

	// game loop
	while (!window.ShouldClose())
	{
		float deltaTime = window.GetFrameTime();

		window.BeginDrawing();
		window.ClearBackground(BLACK);
		camera.BeginMode();

		rect.Draw(raylib::Color(20, 20, 20));

		shader.BeginMode();

		float lineHeight = fontSize * 1.2f;
		float currentLineStart = 0;
		bool isOverChoice = false;
		raylib::Vector2 mousePos = camera.GetScreenToWorld(raylib::Mouse::GetPosition());

		for (int i = 0; i < story.paragraphs.size(); i++)
		{
			raylib::Vector2 start = raylib::Vector2{ playAreaWidth - textBoxWidth + textPadding, textPadding + currentLineStart };
			raylib::Vector2 size = story.paragraphs[i].Draw(josefinSans, start, textWidth, (float)fontSize, lineHeight);
			currentLineStart += size.y + lineHeight;
		}

		for (int i = 0; i < story.storyChoices.size(); i++)
		{
			raylib::Vector2 start = raylib::Vector2{ playAreaWidth - textBoxWidth + textPadding, textPadding + currentLineStart };
			raylib::Vector2 size = story.storyChoices[i].Draw(josefinSans, start, textWidth, (float)fontSize, lineHeight, ORANGE);
			currentLineStart += size.y;

			raylib::Rectangle bounding(start.x, start.y, size.x, size.y);

			spdlog::info("{0} {1} {2} {3} {4} {5} {6}", start.x, start.y, size.x, size.y, mousePos.x, mousePos.y, bounding.CheckCollision(mousePos));

			if (bounding.CheckCollision(mousePos))
			{
				isOverChoice = true;

				if (raylib::Mouse::IsButtonPressed(0))
				{
					story.Choose(i);
				}
			}
		}

		shader.EndMode();

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