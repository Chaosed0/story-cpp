#include <unordered_map>
#include <iostream>

#include <raylib-cpp/raylib-cpp.hpp>
#include <lua.hpp>
#include <rlgl.h>

#include "TypesetEngine.hpp"

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

	float textWidth = 420;
	float textPadding = 10;

    raylib::Shader shader = LoadShader(0, TextFormat("assets/shader/font/sdf_glsl%i.fs", GLSL_VERSION));
	raylib::Rectangle rect(playAreaWidth - textWidth, 0, textWidth, playAreaHeight);

	int filesize;
	unsigned char* fileData = LoadFileData("assets/font/JosefinSans-Regular.ttf", &filesize);

	// SDF font generation from TTF font
	std::shared_ptr<raylib::Font> josefinSans = std::make_shared<raylib::Font>();
	josefinSans->baseSize = 16;
	josefinSans->glyphCount = 95;
	josefinSans->glyphs = LoadFontData(fileData, filesize, 16, 0, 0, FONT_SDF);
	raylib::Image atlas =  GenImageFontAtlas(josefinSans->glyphs, &josefinSans->recs, 95, 16, 0, 1);
	josefinSans->texture = LoadTextureFromImage(atlas);
	UnloadImage(atlas);

	UnloadFileData(fileData);

	std::string str = std::string("In olden times when wishing still helped one, there ") +
		"lived a king whose daughters were all beautiful; and " +
		"the youngest was so beautiful that the sun itself, which " +
		"has seen so much, was astonished whenever it shone in " +
		"her face. Close by the king’s castle lay a great dark " +
		"forest, and under an old lime-tree in the forest was a " +
		"well, and when the day was very warm, the king’s child " +
		"went out into the forest and sat down by the side of the " +
		"cool fountain; and when she was bored she took a " +
		"golden ball, and threw it up on high and caught it; and " +
		"this ball was her favorite plaything.";

	typeset::Paragraph paragraph(str, josefinSans);
	typeset::LinebreakResult result = paragraph.BuildLinebreaks(textWidth - textPadding*2, 14);

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

		/*
		int currentLine = 0;
		int wordInLine = 0;
		float currentLineWidth = 0;
		float lineHeight = 20;

		for (int i = 0; i < result.words.size(); i++)
		{
			typeset::Word word = result.words[i];
			raylib::Vector2 wordPosition(playAreaWidth - textWidth + textPadding + currentLineWidth, textPadding + currentLine * lineHeight);
			josefinSans->DrawText(str.substr(word.start, word.end - word.start + 1), wordPosition, raylib::Vector2(0, 0), 0, 14, 0);

			if (currentLine < result.linebreaks.size() &&
				i == result.linebreaks[currentLine])
			{
				currentLine++;
				wordInLine = 0;
				currentLineWidth = 0;
			}
			else
			{
				const float spaceWidth = 14 / 3.;
				wordInLine++;
				currentLineWidth += word.wordWidth + spaceWidth;
			}
		}
		*/
		josefinSans->DrawText(str, raylib::Vector2(playAreaWidth - textWidth + textPadding, textPadding), raylib::Vector2(0, 0), 0, 64, 0);
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