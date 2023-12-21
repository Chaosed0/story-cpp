#include "LinebreakNode.hpp"

#include <iostream>

namespace typeset
{
	struct LinebreakNode
	{
		int index;
		float totalDemerits;
		bool softHyphen;
		std::shared_ptr<LinebreakNode> parent;

		LinebreakNode(int index, float totalDemerits, bool softHyphen, std::shared_ptr<LinebreakNode> parent)
		{
			this->index = index;
			this->totalDemerits = totalDemerits;
			this->softHyphen = softHyphen;
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

	LinebreakBuilder::LinebreakBuilder(std::shared_ptr<raylib::Font> font, std::shared_ptr<HyphenDict> hyphenDict)
	{
		this->font = font;
		this->hyphenDict = hyphenDict;
	}

	struct CumulativeWordData
	{
		float totalWidth;
		int glueItemCount;
	};

	LinebreakResult LinebreakBuilder::BuildResult(const std::string& input, float maxLineSize, float fontSize)
	{
		std::string str = trim(input);
		float fontSizeMultiplier = fontSize / font->baseSize;
		int glyphIndex = font->GetGlyphIndex('-');
		float hyphenWidth = font->glyphs[glyphIndex].advanceX * fontSizeMultiplier;

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
		std::vector<CumulativeWordData> cumulativeWordDatas;

		tail = std::make_shared<LinebreakNode>(-1, 0.f, false, nullptr);
		head = tail;
		activeNodes.push_back(tail);

		std::vector<float> cumulativeCharWidths;
		int wordStart = -1;

		for (int ci = 0; ci <= str.size(); ci++)
		{
			if (ci == str.size() || iswspace(str[ci]))
			{
				if (wordStart >= 0)
				{
					// Reached glue item, but we have to hyphenate
					std::vector<Word> newWords;

					int hyphWordLength = ci - wordStart;
					std::vector<char> wordBuffer(hyphWordLength + 1, '\0');

					for (int i = 0; i < hyphWordLength; i++)
					{
						wordBuffer[i] = str[wordStart + i];
					}

					std::vector<char> hyphensBuffer(hyphWordLength + 5, '\0');
					char** rep = nullptr;
					int* pos = nullptr;
					int* cut = nullptr;

					// Run hyphenation algorithm
					hnj_hyphen_hyphenate2(hyphenDict.get(), wordBuffer.data(), hyphWordLength, hyphensBuffer.data(), NULL, &rep, &pos, &cut);
					std::string hyphens(hyphensBuffer.data());
					int lastHyphenIndex = -1;

					// Split word into hyphenated segments and add as words
					for (int i = 0; i < hyphens.size(); i++)
					{
						if (hyphens[i] % 2 != 0 || i == hyphens.size() - 1)
						{
							float wordWidth = cumulativeCharWidths[i];

							if (lastHyphenIndex >= 0)
							{
								wordWidth = wordWidth - cumulativeCharWidths[lastHyphenIndex];
							}

							Word word{ wordStart + lastHyphenIndex + 1, wordStart + i, wordWidth, i < hyphens.size() - 1};
							result.words.push_back(word);
							std::string beh = str.substr(word.start, word.end - word.start + 1);

							float totalWidth = cumulativeWordDatas.empty() ? wordWidth : cumulativeWordDatas.back().totalWidth + wordWidth;
							int newGlueItem = lastHyphenIndex == -1;
							int glueItemCount = cumulativeWordDatas.empty() ? 0 : cumulativeWordDatas.back().glueItemCount + newGlueItem;

							cumulativeWordDatas.push_back(CumulativeWordData{ totalWidth, glueItemCount });
							newWords.push_back(word);

							lastHyphenIndex = i;
						}
					}

					// Reset data for next word
					wordStart = -1;
					cumulativeCharWidths.clear();

					// Consider each segment as a potential breakpoint
					for (int i = 0; i < newWords.size(); i++)
					{
						Word word = newWords[i];
						int curWordLength = word.end - word.start + 1;

						float bestActiveNodeScore = FLT_MAX;
						std::shared_ptr<LinebreakNode> bestActiveNode;

						for (int nodeIdx = 0; nodeIdx < activeNodes.size(); nodeIdx++)
						{
							std::shared_ptr<LinebreakNode> activeNode = activeNodes[nodeIdx];

							CumulativeWordData curCumData = cumulativeWordDatas[cumulativeWordDatas.size() - newWords.size() + i];
							float candidateLineWidth = curCumData.totalWidth;
							int glueItemCount = curCumData.glueItemCount;

							if (activeNode->index >= 0)
							{
								CumulativeWordData activeCumData = cumulativeWordDatas[activeNode->index];
								candidateLineWidth = candidateLineWidth - activeCumData.totalWidth;
								glueItemCount = glueItemCount - activeCumData.glueItemCount;
							}

							if (word.softHyphen)
							{
								candidateLineWidth += hyphenWidth;
							}

							float availableShrink = glueItemCount * shrinkability;
							float availableStretch = glueItemCount * stretchability;

							float normalLineWidth = candidateLineWidth + (glueItemCount * normalSpaceWidth);

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

							float penalty = word.softHyphen ? 100000.f : 0.f;
							float consecutiveHyphenPenalty = activeNode->softHyphen ? 300000.f : 0.f;

							float linePenalty = 10;
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
							size_t resultWordsIdx = result.words.size() - newWords.size() + i;
							std::shared_ptr<LinebreakNode> newNode = std::make_shared<LinebreakNode>((int)resultWordsIdx, bestActiveNodeScore, word.softHyphen, bestActiveNode);
							activeNodes.push_back(newNode);
							tail = newNode;
						}
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

				float charWidth = font->glyphs[glyphIndex].advanceX * fontSizeMultiplier;
				float newWordWidth = cumulativeCharWidths.size() > 0 ? cumulativeCharWidths.back() + charWidth : charWidth;
				cumulativeCharWidths.push_back(newWordWidth);
			}
		}

		while (tail != head)
		{
			int linebreakIndex = tail->index;
			int glueItemCount = cumulativeWordDatas[linebreakIndex].glueItemCount;
			float wordWidth = cumulativeWordDatas[linebreakIndex].totalWidth;

			if (tail->parent != nullptr && tail->parent->index >= 0)
			{
				glueItemCount = glueItemCount - cumulativeWordDatas[tail->parent->index].glueItemCount;
				wordWidth = wordWidth - cumulativeWordDatas[tail->parent->index].totalWidth;

				if (!tail->parent->softHyphen)
				{
					// If the parent ends with a non-hyphen, discard the glue item at the end of its line
					glueItemCount--;
				}
			}

			// If we end with a hyphen, add the hyphen width to the total line width
			if (tail->softHyphen)
			{
				wordWidth += hyphenWidth;
			}

			result.linebreaks.push_back(LinebreakLocation{ linebreakIndex, glueItemCount, wordWidth });
			tail = tail->parent;
		}

		std::reverse(result.linebreaks.begin(), result.linebreaks.end());
		return result;
	}
}