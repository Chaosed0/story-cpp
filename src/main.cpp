#include <unordered_map>
#include <optional>
#include <iostream>

#include <raylib-cpp/raylib-cpp.hpp>
#include <lua.hpp>
#include <rlgl.h>

#include "TypesetEngine.hpp"
#include "hyphen/hyphen.h"

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

	/*
	std::string str = std::string("In olden times when wishing still helped one, there ") +
		"lived a king whose daughters were all beautiful; and " +
		"the youngest was so beautiful that the sun itself, which " +
		"has seen so much, was astonished whenever it shone in " +
		"her face. Close by the king's castle lay a great dark " +
		"forest, and under an old lime-tree in the forest was a " +
		"well, and when the day was very warm, the king's child " +
		"went out into the forest and sat down by the side of the " +
		"cool fountain; and when she was bored she took a " +
		"golden ball, and threw it up on high and caught it; and " +
		"this ball was her favorite plaything.";
	*/

	std::string str = std::string("One of the most important operations necessary when text materials are prepared for ") +
		"printing or display is the task of dividing long paragraphs into individual lines. When " +
		"this job has been done well, people will not be aware of the fact that the words they " +
		"are reading have been arbitrarily broken apart and placed into a somewhat rigid and " +
		"unnatural rectangular framework; but if the job has been done poorly, readers will " +
		"be distracted by bad breaks that interrupt their train of thought. In some cases it " +
		"can be difficult to find suitable breakpoints; for example, the narrow columns often " +
		"used in newspapers allow for comparatively little flexibility, and the appearance of " +
		"mathematical formulas in technical text introduces special complications regardless " +
		"of the column width. But even in comparatively simple cases like the typesetting of " +
		"an ordinary novel, good line breaking will contribute greatly to the appearance and " +
		"desirability of the finished product. In fact, some authors actually write better material " +
		"when they are assured that it will look sufficiently beautiful when it appears in print. " +
		"lol lmao";

	float textWidth = textBoxWidth - textPadding * 2;
	int fontSize = 14;
	typeset::Paragraph paragraph(str, josefinSans, hyphenDict);
	typeset::LinebreakResult result = paragraph.BuildLinebreaks(textWidth, fontSize);

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

		int currentLine = 0;
		int wordInLine = 0;
		float currentLineWidth = 0;
		float lineHeight = 20;

		for (int i = 0; i < result.words.size(); i++)
		{
			typeset::Word word = result.words[i];
			raylib::Vector2 wordPosition(playAreaWidth - textBoxWidth + textPadding + currentLineWidth, textPadding + currentLine * lineHeight);

			std::string wordStr = str.substr(word.start, word.end - word.start + 1);

			if (word.softHyphen && currentLine < result.linebreaks.size() && i == result.linebreaks[currentLine].wordIndex)
			{
				wordStr += "-";
			}

			josefinSans->DrawText(wordStr, wordPosition, raylib::Vector2(0, 0), 0, 14, 0);

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
					float normalSpaceSize = fontSize / 3.;

					if (currentLine >= result.linebreaks.size() - 1)
					{
						currentLineWidth += normalSpaceSize;
					}
					else
					{
						currentLineWidth += (textWidth - result.linebreaks[currentLine].wordWidth) / (float)result.linebreaks[currentLine].glueItemCount;
					}
				}

				wordInLine++;
			}
		}

		DrawTexture(josefinSans->texture, 10, 10, WHITE);

		shader.EndMode();

		camera.EndMode();
		EndDrawing();
	}

	// cleanup
	CloseWindow();
	lua_close(luaState);
	return 0;
}