#include "TypesetEngine.hpp"

namespace typeset
{
	struct LinebreakNode
	{
		int index;
		int totalDemerits;
		std::shared_ptr<LinebreakNode> parent;

		LinebreakNode(int index, int totalDemerits, std::shared_ptr<LinebreakNode> parent)
		{
			this->index = index;
			this->totalDemerits = totalDemerits;
			this->parent = parent;
		}
	};

	template <typename T> int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	std::string trim(const std::string& str, const std::string REMOVE = " \n\r\t")
	{
		size_t first = str.find_first_not_of(REMOVE);

		if (std::string::npos == first)
		{
			return str;
		}

		size_t last = str.find_last_not_of(REMOVE);
		return str.substr(first, (last - first + 1));
	}

	Paragraph::Paragraph(const std::string& str, std::shared_ptr<raylib::Font> font)
	{
		this->str = trim(str);
		this->font = font;
	}

	LinebreakResult Paragraph::BuildLinebreaks(float maxLineSize, float fontSize)
	{
		float tolerance = 1;

		float normalSpaceWidth = fontSize / 3.f;
		float shrinkability = fontSize / 9.f; // y-sub-i
		float stretchability = fontSize / 6.f; // z-sub-i

		float minSpaceWidth = normalSpaceWidth - shrinkability;
		float maxSpaceWidth = normalSpaceWidth + stretchability;

		LinebreakResult result;
		std::shared_ptr<LinebreakNode> head;
		std::shared_ptr<LinebreakNode> tail;
		std::vector<std::shared_ptr<LinebreakNode>> activeNodes;
		std::vector<float> cumulativeLengths;

		tail = std::make_shared<LinebreakNode>(-1, 0, nullptr);
		head = tail;
		activeNodes.push_back(tail);

		float wordWidth = 0;
		float totalLength = 0;
		int lineGlueCount = 0;
		int wordStart = -1;

		for (int ci = 0; ci <= str.size(); ci++)
		{
			if (ci == str.size() || iswspace(str[ci]))
			{
				if (wordStart >= 0)
				{
					Word word{ wordStart, ci - 1, wordWidth };
					result.words.push_back(word);

					totalLength += wordWidth;
					cumulativeLengths.push_back(totalLength);

					wordWidth = 0;
					wordStart = -1;

					float bestActiveNodeScore = -1;
					std::shared_ptr<LinebreakNode> bestActiveNode;

					for (int nodeIdx = 0; nodeIdx < activeNodes.size(); nodeIdx++)
					{
						std::shared_ptr<LinebreakNode> activeNode = activeNodes[nodeIdx];

						float candidateLineWidth = cumulativeLengths.back();

						if (activeNode->index >= 0)
						{
							candidateLineWidth = candidateLineWidth - cumulativeLengths[activeNode->index];
						}

						float lineGlueCount = result.words.size() - (activeNode->index + 1) - 1;

						float availableShrink = lineGlueCount * shrinkability;
						float availableStretch = lineGlueCount * stretchability;

						float normalLineWidth = candidateLineWidth + (lineGlueCount * normalSpaceWidth);

						float adjustment = maxLineSize - normalLineWidth;
						float adjustmentRatio = (adjustment <= 0 ? adjustment / availableShrink : adjustment / availableStretch);

						if (ci != str.size())
						{
							if (adjustmentRatio < -1)
							{
								// Too long and cannot shrink enough, so breakpoint become inactive
								activeNodes.erase(activeNodes.cbegin() + nodeIdx);
								nodeIdx--;
								continue;
							}
							else if (adjustmentRatio > 1)
							{
								// Too short and cannot stretch enough, so skip this node
								continue;
							}
						}

						// Otherwise, potential break
						float badness = abs(adjustmentRatio);
						badness = badness * badness * badness * 100;

						// TODO: Consider penalty
						float penalty = 0;
						float consecutiveHyphenPenalty = 0;

						float linePenalty =  10;
						float demerit = (linePenalty + badness) * (linePenalty + badness) + sgn(penalty) * penalty * penalty + consecutiveHyphenPenalty;
						float totalDemerit = activeNode->totalDemerits + demerit;

						if (totalDemerit < bestActiveNodeScore)
						{
							bestActiveNodeScore = totalDemerit;
							bestActiveNode = activeNode;
						}
					}

					if (bestActiveNode != nullptr)
					{
						std::shared_ptr<LinebreakNode> newNode = std::make_shared<LinebreakNode>(result.words.size() - 1, bestActiveNodeScore, bestActiveNode);
						activeNodes.push_back(newNode);
						tail = newNode;
					}
				}
			}
			else
			{
				if (wordStart < 0)
				{
					wordStart = ci;
				}

				int glyphIndex = font->GetGlyphIndex(str[ci]);

				if (glyphIndex <= 0)
				{
					// Errorr!
				}

				wordWidth += font->glyphs[glyphIndex].advanceX * (fontSize / font->baseSize);
			}
		}

		while (tail != head)
		{
			result.linebreaks.push_back(tail->index);
			tail = tail->parent;
		}

		return result;
	}
}