#pragma once

#include <string>
#include <algorithm>
#include "LinebreakNode.hpp"

#undef DrawText

struct Paragraph
{
	std::string str;
	typeset::LinebreakResult result;

	raylib::Vector2 Draw(std::shared_ptr<raylib::Font> font, raylib::Vector2 origin, float maxWidth, float fontSize, float lineHeight, raylib::Color col = WHITE)
	{
		int currentLine = 0;
		int wordInLine = 0;
		float currentLineWidth = 0;
		float maxLineWidth = 0;

		for (int i = 0; i < result.words.size(); i++)
		{
			typeset::Word word = result.words[i];
			raylib::Vector2 wordPosition(origin.x + currentLineWidth, origin.y + currentLine * lineHeight);

			std::string wordStr = str.substr(word.start, word.end - word.start + 1);

			if (word.softHyphen && currentLine < result.linebreaks.size() && i == result.linebreaks[currentLine].wordIndex)
			{
				wordStr += "-";
			}

			font->DrawText(wordStr, wordPosition, raylib::Vector2(0, 0), 0, fontSize, 0, col);

			if (currentLine < result.linebreaks.size() &&
				i == result.linebreaks[currentLine].wordIndex)
			{
				maxLineWidth = std::max(currentLineWidth + word.wordWidth, maxLineWidth);

				currentLine++;
				wordInLine = 0;
				currentLineWidth = 0;
			}
			else
			{
				currentLineWidth += word.wordWidth;

				if (!word.softHyphen)
				{
					float spaceSize = fontSize / 3.f;

					if (result.linebreaks.size() > 0 && currentLine < result.linebreaks.size() - 1)
					{
						// Not the last line, so justify
						spaceSize = (maxWidth - result.linebreaks[currentLine].wordWidth) / (float)result.linebreaks[currentLine].glueItemCount;
					}

					currentLineWidth += spaceSize;
				}

				maxLineWidth = std::max(currentLineWidth, maxLineWidth);
				wordInLine++;
			}
		}

		if (result.linebreaks.size() == 0)
		{
			currentLine = 1;
		}

		return raylib::Vector2(maxLineWidth, currentLine * lineHeight);
	}
};
