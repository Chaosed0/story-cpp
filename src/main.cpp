#include <unordered_map>
#include <optional>
#include <iostream>
#include <fstream>
#include <string>

#include <raylib-cpp/raylib-cpp.hpp>
#include <lua.hpp>
#include <rlgl.h>
#include <ink/story.h>
#include <ink/runner.h>
#include <ink/choice.h>

#include "TypesetEngine.hpp"
#include "hyphen/hyphen.h"

struct Paragraph
{
	std::string str;
	typeset::LinebreakResult result;

	float Draw(std::shared_ptr<raylib::Font> font, raylib::Vector2 origin, raylib::Vector2 size, float fontSize, float lineHeight)
	{
		int currentLine = 0;
		int wordInLine = 0;
		float currentLineWidth = 0;

		for (int i = 0; i < result.words.size(); i++)
		{
			typeset::Word word = result.words[i];
			raylib::Vector2 wordPosition(origin.x + currentLineWidth, origin.y + currentLine * lineHeight);

			std::string wordStr = str.substr(word.start, word.end - word.start + 1);

			if (word.softHyphen && currentLine < result.linebreaks.size() && i == result.linebreaks[currentLine].wordIndex)
			{
				wordStr += "-";
			}

			font->DrawText(wordStr, wordPosition, raylib::Vector2(0, 0), 0, fontSize, 0);

			if (currentLine < result.linebreaks.size() &&
				i == result.linebreaks[currentLine].wordIndex)
			{
				currentLine++;
				wordInLine = 0;
				currentLineWidth = 0;
			}
			else
			{
				currentLineWidth += word.wordWidth;

				if (!word.softHyphen)
				{
					float spaceSize = fontSize / 3.;

					if (currentLine < result.linebreaks.size() - 1)
					{
						// Not the last line, so justify
						spaceSize = (size.x - result.linebreaks[currentLine].wordWidth) / (float)result.linebreaks[currentLine].glueItemCount;
					}

					currentLineWidth += spaceSize;
				}

				wordInLine++;
			}
		}

		return (currentLine + 1) * lineHeight;
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

	SetTargetFPS(60);

	float playAreaWidth = 1080;
	float playAreaHeight = (float)windowHeight / windowWidth * playAreaWidth;
	raylib::Rectangle playArea(-playAreaWidth / 2, -playAreaHeight / 2, playAreaWidth, playAreaHeight);
	raylib::Camera2D camera(raylib::Vector2(0, 0), raylib::Vector2(0, 0), 0, (float)windowWidth / playAreaWidth);

	float textBoxWidth = 420;
	float textPadding = 10;
	float textWidth = textBoxWidth - textPadding * 2;
	int fontSize = 14;

    raylib::Shader shader = LoadShader(0, TextFormat("assets/shader/font/sdf_glsl%i.fs", GLSL_VERSION));
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

	std::shared_ptr<HyphenDict> hyphenDict(hnj_hyphen_load("assets/hyph_en_US.dic"));

	std::ifstream inkStream;
	inkStream.open("assets/ink/MageSchool.bin");
	std::unique_ptr<ink::runtime::story> story(ink::runtime::story::from_file("assets/ink/MageSchool.bin"));

	ink::runtime::runner runner(story->new_runner());
	typeset::LinebreakBuilder linebreakBuilder(josefinSans, hyphenDict);
	std::vector<Paragraph> paragraphs;
	runner->move_to(ink::hash_string("Fall"));

	while (runner->can_continue())
	{
		std::string str = runner->getline();
		typeset::LinebreakResult typeset = linebreakBuilder.BuildResult(str, textWidth, fontSize);
		Paragraph paragraph{ str, typeset };
		paragraphs.push_back(paragraph);
	}

	// game loop
	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();

		// update

		// drawing
		BeginDrawing();
		ClearBackground(BLACK);
		camera.BeginMode();

		rect.Draw(raylib::Color(20, 20, 20));

		shader.BeginMode();

		float lineHeight = fontSize * 1.2f;
		float currentLineStart = 0;

		for (int i = 0; i < paragraphs.size(); i++)
		{
			paragraphs[i].Draw(josefinSans, raylib::Vector2{ playAreaWidth - textBoxWidth + textPadding, textPadding + currentLineStart }, raylib::Vector2{ textWidth, playAreaHeight - textPadding * 2 }, fontSize, fontSize * 1.2f);
			currentLineStart += lineHeight;
		}

		shader.EndMode();

		DrawTexture(josefinSans->texture, 10, 10, WHITE);

		camera.EndMode();
		EndDrawing();
	}

	// cleanup
	CloseWindow();
	lua_close(luaState);
	return 0;
}