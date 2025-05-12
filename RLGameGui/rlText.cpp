/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

-- Copyright (c) 2020-2024 Jeffery Myers
--
--This software is provided "as-is", without any express or implied warranty. In no event
--will the authors be held liable for any damages arising from the use of this software.

--Permission is granted to anyone to use this software for any purpose, including commercial
--applications, and to alter it and redistribute it freely, subject to the following restrictions:

--  1. The origin of this software must not be misrepresented; you must not claim that you
--  wrote the original software. If you use this software in a product, an acknowledgment
--  in the product documentation would be appreciated but is not required.
--
--  2. Altered source versions must be plainly marked as such, and must not be misrepresented
--  as being the original software.
--
--  3. This notice may not be removed or altered from any source distribution.

*/

#include "rlText.h"

#include "raymath.h"

#include <map>

#include "external/stb_rect_pack.h"     // Required for: ttf/bdf font rectangles packaging

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#include "external/stb_truetype.h"      // Required for: ttf font data reading

static rltFont DefaultFont;

static bool TextIsYFlipped = false;

void rltSetTextYFlip(bool flip)
{
	TextIsYFlipped = flip;
}

#define REUSE_DEFAULT_TEXTUREID 1

void LoadDefaultFont()
{
#define BIT_CHECK(a,b) ((a) & (1u << (b)))

	// NOTE: Using UTF-8 encoding table for Unicode U+0000..U+00FF Basic Latin + Latin-1 Supplement
	// Ref: http://www.utf8-chartable.de/unicode-utf8-table.pl

	int glyphCount = 224;   // Number of chars included in our default font
	DefaultFont.GlyphPadding = 0;   // Characters padding
	DefaultFont.BaseSize = 10;
	DefaultFont.DefaultNewlineOffset = 10;
	DefaultFont.DefaultSpacing = 1;

	// Default font is directly defined here (data generated from a sprite font image)
	// This way, we reconstruct Font without creating large global variables
	// This data is automatically allocated to Stack and automatically deallocated at the end of this function
	unsigned int defaultFontData[512] = {
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00200020, 0x0001b000, 0x00000000, 0x00000000, 0x8ef92520, 0x00020a00, 0x7dbe8000, 0x1f7df45f,
		0x4a2bf2a0, 0x0852091e, 0x41224000, 0x10041450, 0x2e292020, 0x08220812, 0x41222000, 0x10041450, 0x10f92020, 0x3efa084c, 0x7d22103c, 0x107df7de,
		0xe8a12020, 0x08220832, 0x05220800, 0x10450410, 0xa4a3f000, 0x08520832, 0x05220400, 0x10450410, 0xe2f92020, 0x0002085e, 0x7d3e0281, 0x107df41f,
		0x00200000, 0x8001b000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xc0000fbe, 0xfbf7e00f, 0x5fbf7e7d, 0x0050bee8, 0x440808a2, 0x0a142fe8, 0x50810285, 0x0050a048,
		0x49e428a2, 0x0a142828, 0x40810284, 0x0048a048, 0x10020fbe, 0x09f7ebaf, 0xd89f3e84, 0x0047a04f, 0x09e48822, 0x0a142aa1, 0x50810284, 0x0048a048,
		0x04082822, 0x0a142fa0, 0x50810285, 0x0050a248, 0x00008fbe, 0xfbf42021, 0x5f817e7d, 0x07d09ce8, 0x00008000, 0x00000fe0, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x000c0180,
		0xdfbf4282, 0x0bfbf7ef, 0x42850505, 0x004804bf, 0x50a142c6, 0x08401428, 0x42852505, 0x00a808a0, 0x50a146aa, 0x08401428, 0x42852505, 0x00081090,
		0x5fa14a92, 0x0843f7e8, 0x7e792505, 0x00082088, 0x40a15282, 0x08420128, 0x40852489, 0x00084084, 0x40a16282, 0x0842022a, 0x40852451, 0x00088082,
		0xc0bf4282, 0xf843f42f, 0x7e85fc21, 0x3e0900bf, 0x00000000, 0x00000004, 0x00000000, 0x000c0180, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x04000402, 0x41482000, 0x00000000, 0x00000800,
		0x04000404, 0x4100203c, 0x00000000, 0x00000800, 0xf7df7df0, 0x514bef85, 0xbefbefbe, 0x04513bef, 0x14414500, 0x494a2885, 0xa28a28aa, 0x04510820,
		0xf44145f0, 0x474a289d, 0xa28a28aa, 0x04510be0, 0x14414510, 0x494a2884, 0xa28a28aa, 0x02910a00, 0xf7df7df0, 0xd14a2f85, 0xbefbe8aa, 0x011f7be0,
		0x00000000, 0x00400804, 0x20080000, 0x00000000, 0x00000000, 0x00600f84, 0x20080000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0xac000000, 0x00000f01, 0x00000000, 0x00000000, 0x24000000, 0x00000f01, 0x00000000, 0x06000000, 0x24000000, 0x00000f01, 0x00000000, 0x09108000,
		0x24fa28a2, 0x00000f01, 0x00000000, 0x013e0000, 0x2242252a, 0x00000f52, 0x00000000, 0x038a8000, 0x2422222a, 0x00000f29, 0x00000000, 0x010a8000,
		0x2412252a, 0x00000f01, 0x00000000, 0x010a8000, 0x24fbe8be, 0x00000f01, 0x00000000, 0x0ebe8000, 0xac020000, 0x00000f01, 0x00000000, 0x00048000,
		0x0003e000, 0x00000f00, 0x00000000, 0x00008000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000038, 0x8443b80e, 0x00203a03,
		0x02bea080, 0xf0000020, 0xc452208a, 0x04202b02, 0xf8029122, 0x07f0003b, 0xe44b388e, 0x02203a02, 0x081e8a1c, 0x0411e92a, 0xf4420be0, 0x01248202,
		0xe8140414, 0x05d104ba, 0xe7c3b880, 0x00893a0a, 0x283c0e1c, 0x04500902, 0xc4400080, 0x00448002, 0xe8208422, 0x04500002, 0x80400000, 0x05200002,
		0x083e8e00, 0x04100002, 0x804003e0, 0x07000042, 0xf8008400, 0x07f00003, 0x80400000, 0x04000022, 0x00000000, 0x00000000, 0x80400000, 0x04000002,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00800702, 0x1848a0c2, 0x84010000, 0x02920921, 0x01042642, 0x00005121, 0x42023f7f, 0x00291002,
		0xefc01422, 0x7efdfbf7, 0xefdfa109, 0x03bbbbf7, 0x28440f12, 0x42850a14, 0x20408109, 0x01111010, 0x28440408, 0x42850a14, 0x2040817f, 0x01111010,
		0xefc78204, 0x7efdfbf7, 0xe7cf8109, 0x011111f3, 0x2850a932, 0x42850a14, 0x2040a109, 0x01111010, 0x2850b840, 0x42850a14, 0xefdfbf79, 0x03bbbbf7,
		0x001fa020, 0x00000000, 0x00001000, 0x00000000, 0x00002070, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x08022800, 0x00012283, 0x02430802, 0x01010001, 0x8404147c, 0x20000144, 0x80048404, 0x00823f08, 0xdfbf4284, 0x7e03f7ef, 0x142850a1, 0x0000210a,
		0x50a14684, 0x528a1428, 0x142850a1, 0x03efa17a, 0x50a14a9e, 0x52521428, 0x142850a1, 0x02081f4a, 0x50a15284, 0x4a221428, 0xf42850a1, 0x03efa14b,
		0x50a16284, 0x4a521428, 0x042850a1, 0x0228a17a, 0xdfbf427c, 0x7e8bf7ef, 0xf7efdfbf, 0x03efbd0b, 0x00000000, 0x04000000, 0x00000000, 0x00000008,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00200508, 0x00840400, 0x11458122, 0x00014210,
		0x00514294, 0x51420800, 0x20a22a94, 0x0050a508, 0x00200000, 0x00000000, 0x00050000, 0x08000000, 0xfefbefbe, 0xfbefbefb, 0xfbeb9114, 0x00fbefbe,
		0x20820820, 0x8a28a20a, 0x8a289114, 0x3e8a28a2, 0xfefbefbe, 0xfbefbe0b, 0x8a289114, 0x008a28a2, 0x228a28a2, 0x08208208, 0x8a289114, 0x088a28a2,
		0xfefbefbe, 0xfbefbefb, 0xfa2f9114, 0x00fbefbe, 0x00000000, 0x00000040, 0x00000000, 0x00000000, 0x00000000, 0x00000020, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00210100, 0x00000004, 0x00000000, 0x00000000, 0x14508200, 0x00001402, 0x00000000, 0x00000000,
		0x00000010, 0x00000020, 0x00000000, 0x00000000, 0xa28a28be, 0x00002228, 0x00000000, 0x00000000, 0xa28a28aa, 0x000022e8, 0x00000000, 0x00000000,
		0xa28a28aa, 0x000022a8, 0x00000000, 0x00000000, 0xa28a28aa, 0x000022e8, 0x00000000, 0x00000000, 0xbefbefbe, 0x00003e2f, 0x00000000, 0x00000000,
		0x00000004, 0x00002028, 0x00000000, 0x00000000, 0x80000000, 0x00003e0f, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000 };

	int charsHeight = 10;
	int charsDivisor = 1;    // Every char is separated from the consecutive by a 1 pixel divisor, horizontally and vertically

	int charsWidth[224] = { 3, 1, 4, 6, 5, 7, 6, 2, 3, 3, 5, 5, 2, 4, 1, 7, 5, 2, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 3, 4, 3, 6,
							7, 6, 6, 6, 6, 6, 6, 6, 6, 3, 5, 6, 5, 7, 6, 6, 6, 6, 6, 6, 7, 6, 7, 7, 6, 6, 6, 2, 7, 2, 3, 5,
							2, 5, 5, 5, 5, 5, 4, 5, 5, 1, 2, 5, 2, 5, 5, 5, 5, 5, 5, 5, 4, 5, 5, 5, 5, 5, 5, 3, 1, 3, 4, 4,
							1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
							1, 1, 5, 5, 5, 7, 1, 5, 3, 7, 3, 5, 4, 1, 7, 4, 3, 5, 3, 3, 2, 5, 6, 1, 2, 2, 3, 5, 6, 6, 6, 6,
							6, 6, 6, 6, 6, 6, 7, 6, 6, 6, 6, 6, 3, 3, 3, 3, 7, 6, 6, 6, 6, 6, 6, 5, 6, 6, 6, 6, 6, 6, 4, 6,
							5, 5, 5, 5, 5, 5, 9, 5, 5, 5, 5, 5, 2, 2, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3, 5 };

	// Re-construct image from defaultFontData and generate OpenGL texture
	//----------------------------------------------------------------------
	Image imFont = { MemAlloc(128 * 128 * 2),  // 2 bytes per pixel (gray + alpha)
		128,
		128,
		1,
		PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA
	};

	// Fill image.data with defaultFontData (convert from bit to pixel!)
	for (int i = 0, counter = 0; i < imFont.width * imFont.height; i += 32)
	{
		for (int j = 31; j >= 0; j--)
		{
			if (BIT_CHECK(defaultFontData[counter], j))
			{
				// NOTE: We are unreferencing data as short, so,
				// we must consider data as little-endian order (alpha + gray)
				((unsigned short*)imFont.data)[i + j] = 0xffff;
			}
			else
			{
				((unsigned char*)imFont.data)[(i + j) * sizeof(short)] = 0xFF;
				((unsigned char*)imFont.data)[(i + j) * sizeof(short) + 1] = 0x00;
			}
		}

		counter++;
	}

	if (REUSE_DEFAULT_TEXTUREID == 1)
		DefaultFont.Texture = GetFontDefault().texture;
	else
		DefaultFont.Texture = LoadTextureFromImage(imFont);

	// Reconstruct charSet using charsWidth[], charsHeight, charsDivisor, glyphCount
	//------------------------------------------------------------------------------

	// Allocate space for our characters info data
	// NOTE: This memory must be freed at end! --> Done by CloseWindow()

	auto& range = DefaultFont.Ranges.emplace_back();

	range.Start = 32;
	range.Glyphs.resize(glyphCount);
	int currentLine = 0;
	int currentPosX = charsDivisor;
	int testPosX = charsDivisor;

	for (int i = 0; i < glyphCount; i++)
	{
		range.Glyphs[i].Value = 32 + i;  // First char is 32

		range.Glyphs[i].SourceRect.x = (float)currentPosX;
		range.Glyphs[i].SourceRect.y = (float)(charsDivisor + currentLine * (charsHeight + charsDivisor));
		range.Glyphs[i].SourceRect.width = (float)charsWidth[i];
		range.Glyphs[i].SourceRect.height = (float)charsHeight;
		range.Glyphs[i].DestSize.x = (float)charsWidth[i];
		range.Glyphs[i].DestSize.y = (float)charsHeight;

		testPosX += (int)(range.Glyphs[i].SourceRect.width + (float)charsDivisor);

		if (testPosX >= imFont.width)
		{
			currentLine++;
			currentPosX = 2 * charsDivisor + charsWidth[i];
			testPosX = currentPosX;

			range.Glyphs[i].SourceRect.x = (float)charsDivisor;
			range.Glyphs[i].SourceRect.y = (float)(charsDivisor + currentLine * (charsHeight + charsDivisor));
		}
		else
		{
			currentPosX = testPosX;
		}
	}
	DefaultFont.LowestSourceRect = float(imFont.height);
	UnloadImage(imFont);
}

const rltFont& rltGetDefaultFont()
{
	if (DefaultFont.Ranges.empty())
		LoadDefaultFont();

	return DefaultFont;
}

void rltGetStandardGlyphSet(rltGlyphSet& glyphSet)
{
	for (int i = 32; i < 128; i++)
		glyphSet.insert(i);
}

void rltAddRangeToGlyphSet(int start, int end, rltGlyphSet& glyphSet)
{
	for (int i = start; i <= end; i++)
		glyphSet.insert(i);
}

void rltAddGlyphSetFromString(std::string_view text, rltGlyphSet& glyphSet)
{
	for (size_t i = 0; i < text.size();)
	{
		int codepointByteCount = 0;
		int codepoint = GetCodepointNext(text.data() + i, &codepointByteCount);
		glyphSet.insert(codepoint);
		i += codepointByteCount;
	}
}

rltFont rltLoadFontTTF(std::string_view filePath, float fontSize, const rltGlyphSet* glyphSet, float* defaultSpacing)
{
	int dataSize = 0;
	unsigned char* fileData = LoadFileData(filePath.data(), &dataSize);

	auto font = rltLoadFontTTFMemory(fileData, dataSize, fontSize, glyphSet, defaultSpacing);

	UnloadFileData(fileData);

	return font;
}

rltFont rltLoadFontTTFMemory(const void* data, size_t dataSize, float fontSize, const rltGlyphSet* glyphSet, float* defaultSpacing)
{
	rltFont font;

	font.BaseSize = fontSize;
	font.DefaultNewlineOffset = fontSize * 1.2f;
	font.DefaultSpacing = fontSize / 10.0f;
	font.GlyphPadding = 2;
	if (defaultSpacing)
		font.DefaultSpacing = *defaultSpacing;

	rltGlyphSet defaultSet;
	const std::set<int>* setTouse = glyphSet;
	if (!setTouse)
	{
		rltGetStandardGlyphSet(defaultSet);
		setTouse = &defaultSet;
	}
	stbtt_fontinfo fontInfo = { 0 };
	if (!stbtt_InitFont(&fontInfo, (unsigned char*)data, 0))
		return font;

	std::map<int, int> indexToCodepoint;

	float rasterScale = 1.0f;
	// if we are auto scaling for high DPI, load the font at the scaled size, so that the pixel map is the right size
	if (IsWindowState(FLAG_WINDOW_HIGHDPI))
		rasterScale = GetWindowScaleDPI().y;

	float effectivefontSize = fontSize * rasterScale;

	float scaleFactor = stbtt_ScaleForPixelHeight(&fontInfo, (float)effectivefontSize);

	// Calculate font basic metrics
	// NOTE: ascent is equivalent to font baseline
	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);

	font.Accent = (ascent * scaleFactor) / rasterScale;

	int lastCodePoint = -1000;

	rltGlyphRange* currentRange = nullptr;

	std::map<int, Image> glyphImages;

	for (auto& codepoint : *setTouse)
	{
		int index = stbtt_FindGlyphIndex(&fontInfo, codepoint);

		if (index <= 0)
			continue;

		indexToCodepoint[index] = codepoint;

		if (!currentRange || codepoint != lastCodePoint + 1)
		{
			currentRange = &font.Ranges.emplace_back();
			currentRange->Start = codepoint;
		}
		lastCodePoint = codepoint;
		auto& glyphInfo = currentRange->Glyphs.emplace_back();
		glyphInfo.Value = codepoint;

		Image glyphImage = { 0 };

		int offsetX = 0;
		int offsetY = 0;
		glyphImage.data = stbtt_GetCodepointBitmap(&fontInfo,
			scaleFactor,
			scaleFactor,
			codepoint,
			&glyphImage.width,
			&glyphImage.height,
			&offsetX,
			&offsetY);

		int advanceX = 0;
		stbtt_GetCodepointHMetrics(&fontInfo, codepoint, &advanceX, nullptr);
		glyphInfo.NextCharacterAdvance = (advanceX * scaleFactor) / rasterScale;

		// backplot the offsets into world space
		glyphInfo.Offset.x = offsetX / rasterScale;
		offsetY += int(ascent * scaleFactor);
		glyphInfo.Offset.y = offsetY / rasterScale;

		if (codepoint == 32)
		{
			if (glyphImage.data)
				MemFree(glyphImage.data);

			glyphImage.width = int(glyphInfo.NextCharacterAdvance);
			glyphImage.height = int(effectivefontSize);

			glyphImage.data = MemAlloc(glyphImage.width * glyphImage.height);
		}
		glyphImage.mipmaps = 1;
		glyphImage.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;

		glyphInfo.DestSize.x = glyphImage.width / rasterScale;
		glyphInfo.DestSize.y = glyphImage.height / rasterScale;

		// we will composite this later
		glyphImages.insert_or_assign(codepoint, glyphImage);
	}

	// kerning table

	auto tableSize = stbtt_GetKerningTableLength(&fontInfo);
	std::vector<stbtt_kerningentry> kerningTable(tableSize);

	stbtt_GetKerningTable(&fontInfo, kerningTable.data(), tableSize);

	for (auto& entry : kerningTable)
	{
		auto fromGlyphItr = indexToCodepoint.find(entry.glyph1);
		auto toGlyphItr = indexToCodepoint.find(entry.glyph2);

		if (fromGlyphItr == indexToCodepoint.end() || toGlyphItr == indexToCodepoint.end())
			continue;

		int fromCodepoint = fromGlyphItr->second;
		int toCodepoint = toGlyphItr->second;

		rltGlyphInfo* fromGlyph = nullptr;

		for (auto& range : font.Ranges)
		{
			if (fromCodepoint < range.Start || fromCodepoint >(range.Start + range.Glyphs.size()))
				continue;

			fromGlyph = &range.Glyphs[fromCodepoint - range.Start];
			break;
		}

		if (!fromGlyph)
			continue;

		fromGlyph->KerningInfo[toCodepoint] = (entry.advance * scaleFactor) / rasterScale;
	}

	// atlas generation

	Image fontAtlas = { 0 };

	std::map<int, Rectangle> glyphRects;

	// Calculate image size based on total glyph width and glyph row count
	int totalWidth = 0;
	int maxGlyphWidth = 0;

	for (auto& codepoint : *setTouse)
	{
		if (glyphImages.find(codepoint) == glyphImages.end())
			continue;

		if (glyphImages[codepoint].width > maxGlyphWidth)
			maxGlyphWidth = glyphImages[codepoint].width;

		totalWidth += glyphImages[codepoint].width + int(2 * font.GlyphPadding);
	}

	int paddedFontSize = int(effectivefontSize + 2 * font.GlyphPadding);
	// No need for a so-conservative atlas generation
	float totalArea = totalWidth * paddedFontSize * 1.2f;
	float imageMinSize = sqrtf(totalArea);
	int imageSize = (int)powf(2, ceilf(logf(imageMinSize) / logf(2)));

	if (totalArea < ((imageSize * imageSize) / 2))
	{
		fontAtlas.width = imageSize;    // Atlas bitmap width
		fontAtlas.height = imageSize / 2; // Atlas bitmap height
	}
	else
	{
		fontAtlas.width = imageSize;   // Atlas bitmap width
		fontAtlas.height = imageSize;  // Atlas bitmap height
	}

	fontAtlas.data = (unsigned char*)MemAlloc(fontAtlas.width * fontAtlas.height * 2);   // Create a bitmap to store characters (8 bpp)
	fontAtlas.format = PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA;
	fontAtlas.mipmaps = 1;

	//fill it with white
	memset(fontAtlas.data, 0, fontAtlas.width * fontAtlas.height * 2);

	int offsetX = int(font.GlyphPadding);
	int offsetY = int(font.GlyphPadding);

	// NOTE: Using simple packaging, one char after another
	for (auto& range : font.Ranges)
	{
		for (auto& glyphInfo : range.Glyphs)
		{
			auto& image = glyphImages[glyphInfo.Value];

			// Check remaining space for font.GlyphPadding
			if (offsetX >= (fontAtlas.width - image.width - 2 * font.GlyphPadding))
			{
				offsetX = int(font.GlyphPadding);

				// NOTE: Be careful on offsetY for SDF fonts, by default SDF
				// use an internal padding of 4 pixels, it means char rectangle
				// height is bigger than fontSize, it could be up to (fontSize + 8)
				offsetY += int(effectivefontSize + 2 * font.GlyphPadding);
			}

			glyphInfo.SourceRect = { float(offsetX), float(offsetY), float(image.width), float(image.height) };

			if (glyphInfo.SourceRect.y + glyphInfo.SourceRect.height > font.LowestSourceRect)
				font.LowestSourceRect = glyphInfo.SourceRect.y + glyphInfo.SourceRect.height;

			font.LastSourceRectX = glyphInfo.SourceRect.x + glyphInfo.SourceRect.width;

			// copy the image to just the alpha
			for (size_t y = 0; y < image.height; y++)
			{
				for (size_t x = 0; x < image.width; x++)
				{
					size_t sourceIndex = y * image.width + x;
					size_t destIndex = (offsetY + y) * fontAtlas.width + (offsetX + x);

					unsigned char* sourcePixel = ((unsigned char*)image.data) + sourceIndex;
					unsigned char* destPixel = ((unsigned char*)fontAtlas.data) + (destIndex * 2);

					// set the color channel to full white if the alpha is not 0, this way the alpha has a full color to blend with, not grays
					if (*sourcePixel > 0)
						*destPixel = 255;
					else
						*destPixel = 0;

					// set the alpha based on the glyph
					destPixel++;
					*destPixel = *sourcePixel;
				}
			}

			UnloadImage(image);
			image.data = nullptr;

			// Move atlas position X for next character drawing
			offsetX += int(glyphInfo.SourceRect.width + (2 * font.GlyphPadding));
		}
	}

	int rectSize = 3;
	ImageDrawRectangle(&fontAtlas, fontAtlas.width - rectSize, fontAtlas.height - rectSize, rectSize, rectSize, WHITE);

	Rectangle invalidRect = { fontAtlas.width - rectSize - effectivefontSize - font.GlyphPadding, fontAtlas.height - effectivefontSize - font.GlyphPadding , effectivefontSize, effectivefontSize };

	ImageDrawRectangleLines(&fontAtlas, invalidRect, 2, WHITE);

	float width = float(MeasureText("?", int(effectivefontSize)));
	ImageDrawText(&fontAtlas, "?", int(invalidRect.x + (effectivefontSize / 2 - width / 2)), int(invalidRect.y + 2), int(effectivefontSize), WHITE);

	font.InvalidGlyph.NextCharacterAdvance = fontSize + font.GlyphPadding;
	font.InvalidGlyph.Offset = Vector2Zeros;
	font.InvalidGlyph.SourceRect = invalidRect;
	font.InvalidGlyph.Value = -1;
	font.InvalidGlyph.DestSize.x = invalidRect.width / rasterScale;
	font.InvalidGlyph.DestSize.y = invalidRect.height / rasterScale;

	font.Texture = LoadTextureFromImage(fontAtlas);

	UnloadImage(fontAtlas);

	return font;
}

void rltUnloadFont(rltFont* font)
{
	if (font == &DefaultFont)
		return;

	UnloadTexture(font->Texture);

	font->Ranges.clear();
}

const rltGlyphInfo* rtlGetFontGlyph(const rltFont* font, int id)
{
	for (auto& range : font->Ranges)
	{
		if (id < range.Start || id >(range.Start + range.Glyphs.size()))
			continue;

		return &range.Glyphs[id - range.Start];
	}

	return &font->InvalidGlyph;
}

const rltGlyphInfo* GetGlyphForCodePoint(const char* data, size_t& index, Vector2& currentPos, const rltFont* font, float scale)
{
	int codepointByteCount = 0;
	int codepoint = GetCodepointNext(data + index, &codepointByteCount);
	index += codepointByteCount;

	if (codepoint < 32)
	{
		// control character
		if (codepoint == 10)
		{
			currentPos.x = 0;
			currentPos.y += font->DefaultNewlineOffset * scale;
		}
		return nullptr;
	}

	return rtlGetFontGlyph(font, codepoint);
}

void DrawGlyph(const rltGlyphInfo* glyph, const Vector2& position, Vector2& currentPos, Color tint, const rltFont* font, float scale)
{
	if (glyph)
	{
		float offsetY = position.y + currentPos.y + (glyph->Offset.y * scale);

		Rectangle srcRect = glyph->SourceRect;
		if (TextIsYFlipped)
		{
			srcRect.height *= -1;
			offsetY = position.y + currentPos.y; // TODO, should the offset be computed here?
		}
		Rectangle destRect = { position.x + currentPos.x + (glyph->Offset.x * scale), offsetY , glyph->DestSize.x * scale , glyph->DestSize.y * scale };

		DrawTexturePro(font->Texture, srcRect, destRect, Vector2Zeros, 0, tint);
		currentPos.x += destRect.width;

		currentPos.x += font->DefaultSpacing * scale;
	}
}

int getDigit(const char input)
{
	switch (input)
	{
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'a':
	case 'A': return 10;
	case 'b':
	case 'B': return 11;
	case 'c':
	case 'C': return 12;
	case 'd':
	case 'D': return 13;
	case 'e':
	case 'E': return 14;
	case 'f':
	case 'F': return 15;
	}
	return 0;
}

unsigned char GetHexValue(const char* ptr, size_t& offset)
{
	return getDigit(ptr[offset++]) * 16 + getDigit(ptr[offset++]);
}

Color ProcessColorSequence(std::string_view text, size_t& index, Color currentColor)
{
	if (text.data()[index] == '\a')
	{
		// escape sequence
		index++;

		if (index < text.size() - 1)
		{
			if (text.data()[index] == '#')
			{
				index++;
				if (index < text.size() - 8)
				{
					currentColor.r = GetHexValue(text.data(), index);
					currentColor.g = GetHexValue(text.data(), index);
					currentColor.b = GetHexValue(text.data(), index);
					currentColor.a = GetHexValue(text.data(), index);
				}
			}
		}
	}

	return currentColor;
}

void rltDrawText(std::string_view text, float size, const Vector2& position, Color tint, const rltFont* font)
{
	const rltFont* fontToUse = &rltGetDefaultFont();
	if (font)
		fontToUse = font;

	Vector2 currentPos{ 0,0 };

	float scale = size / fontToUse->BaseSize;

	Color tintToUse = tint;

	const rltGlyphInfo* lastGlyph = nullptr;

	for (size_t i = 0; i < text.size();)
	{
		tintToUse = ProcessColorSequence(text, i, tintToUse);
		const rltGlyphInfo* glyph = GetGlyphForCodePoint(text.data(), i, currentPos, fontToUse, scale);

		if (lastGlyph && glyph && !lastGlyph->KerningInfo.empty())
		{
			auto itr = lastGlyph->KerningInfo.find(glyph->Value);
			if (itr != lastGlyph->KerningInfo.end())
			{
				if (currentPos.x > 0)
					currentPos.x += itr->second * scale;
			}
		}

		DrawGlyph(glyph, position, currentPos, tintToUse, fontToUse, scale);

		lastGlyph = glyph;
	}
}

void rltDrawTextJustified(std::string_view text, float size, const Vector2& position, Color tint, rltAllignment allignment, const rltFont* font)
{
	auto bounds = rltMeasureText(text, size, font);

	Vector2 origin = position;
	switch (allignment)
	{
	default:
	case rltAllignment::Left:
		break;

	case rltAllignment::Center:
		// origin is the center
		origin.x -= bounds.x / 2;
		break;

	case rltAllignment::Right:
		// origin is the right
		origin.x -= bounds.x;
		break;
	}
	rltDrawText(text, size, origin, tint, font);
}

float rltDrawTextWrapped(std::string_view text, float size, const Vector2& position, float width, Color tint, const rltFont* font)
{
	const rltFont* fontToUse = &rltGetDefaultFont();
	if (font)
		fontToUse = font;

	Vector2 currentPos = Vector2Zeros;

	float scale = size / fontToUse->BaseSize;

	const rltGlyphInfo* lastGlyph = nullptr;

	Color tintToUse = tint;
	for (size_t i = 0; i < text.size();)
	{
		tintToUse = ProcessColorSequence(text, i, tintToUse);

		const auto* glyph = GetGlyphForCodePoint(text.data(), i, currentPos, fontToUse, scale);

		float glyphWidth = size;
		if (glyph)
			glyphWidth = glyph->SourceRect.width * scale;

		if (currentPos.x + position.x + glyphWidth + fontToUse->DefaultSpacing * scale > width)
		{
			currentPos.x = 0;
			currentPos.y += fontToUse->DefaultNewlineOffset * scale;
		}

		if (lastGlyph && glyph && !lastGlyph->KerningInfo.empty())
		{
			auto itr = lastGlyph->KerningInfo.find(glyph->Value);
			if (itr != lastGlyph->KerningInfo.end())
			{
				if (currentPos.x > 0)
					currentPos.x += itr->second * scale;
			}
		}

		DrawGlyph(glyph, position, currentPos, tintToUse, fontToUse, scale);

		lastGlyph = glyph;
	}

	return currentPos.y + fontToUse->DefaultNewlineOffset * scale;
}

Vector2 rltMeasureText(std::string_view text, float size, const rltFont* font)
{
	const rltFont* fontToUse = &rltGetDefaultFont();
	if (font)
		fontToUse = font;

	Vector2 currentPos{ 0,0 };

	float scale = size / fontToUse->BaseSize;

	float maxWidth = 0;

	for (size_t i = 0; i < text.size();)
	{
		ProcessColorSequence(text, i, WHITE);
		const auto* glyph = GetGlyphForCodePoint(text.data(), i, currentPos, fontToUse, scale);

		if (glyph)
		{
			currentPos.x += glyph->DestSize.x * scale;
			currentPos.x += font->DefaultSpacing * scale;
		}

		if (currentPos.x > maxWidth)
			maxWidth = currentPos.x;
	}

	currentPos.y += font->DefaultNewlineOffset * scale;
	currentPos.x = maxWidth;
	return currentPos;
}

bool rltFontHasCodepoint(rltFont* font, int codepoint)
{
	if (!font)
		return false;

	auto* glyph = rtlGetFontGlyph(font, codepoint);

	return glyph != &font->InvalidGlyph;
}

bool rltFontHasAllGlyphsInString(rltFont* font, std::string_view text)
{
	if (!font)
		return false;

	Vector2 currentPos = Vector2Zeros;

	for (size_t i = 0; i < text.size();)
	{
		ProcessColorSequence(text, i, WHITE);
		const auto* glyph = GetGlyphForCodePoint(text.data(), i, currentPos, font, 1);

		if (glyph != &font->InvalidGlyph)
			return false;
	}
	return true;
}

bool GlyphLocationIsValid(rltFont* font, Rectangle& rectangle)
{
	float right = rectangle.x + rectangle.width + font->GlyphPadding;
	float bottom = rectangle.y + rectangle.height + font->GlyphPadding;

	float cornerY = font->InvalidGlyph.SourceRect.y - font->GlyphPadding;
	float cornerX = font->InvalidGlyph.SourceRect.x - font->GlyphPadding;

	// we have plenty of space
	if (right <= font->Texture.width && bottom < cornerY)
		return true;

	// we are over the right side
	if (right > font->Texture.width)
	{
		// move down to the left
		rectangle.y = font->LowestSourceRect + font->GlyphPadding;
		rectangle.x = font->GlyphPadding;

		// we are out of room
		if (rectangle.y + rectangle.height > font->Texture.height)
			return false;

		// it's too big to even fit
		if (rectangle.x + rectangle.width + font->GlyphPadding > font->Texture.width)
			return false;

		// it can fit
		return true;
	}

	return true;
}

bool rltAddGlpyhToFont(rltFont* font, int codepoint, Image& glpyhImage, const Vector2& offeset, float advance)
{
	if (!font)
		return false;

	auto& lastRect = font->Ranges.back().Glyphs.back().SourceRect;

	Rectangle nextSourceRect = { font->LastSourceRectX + lastRect.width + font->GlyphPadding , lastRect.y, float(glpyhImage.width), float(glpyhImage.height) };

	if (!GlyphLocationIsValid(font, nextSourceRect))
		return false;

	Image bitmap = LoadImageFromTexture(font->Texture);

	rltGlyphInfo newGlyph;

	newGlyph.Value = codepoint;
	newGlyph.SourceRect = nextSourceRect;
	newGlyph.DestSize.x = newGlyph.SourceRect.width;
	newGlyph.DestSize.y = newGlyph.SourceRect.height;

	if (nextSourceRect.y + nextSourceRect.height > font->LowestSourceRect)
		font->LowestSourceRect = nextSourceRect.y + nextSourceRect.height;

	font->LastSourceRectX = nextSourceRect.x + nextSourceRect.width;

	newGlyph.Offset = offeset;
	newGlyph.NextCharacterAdvance = advance;
	if (newGlyph.NextCharacterAdvance < 0)
		newGlyph.NextCharacterAdvance = font->DefaultSpacing;

	ImageDraw(&bitmap,
		glpyhImage,
		Rectangle{ 0,0, float(glpyhImage.width), float(glpyhImage.height) },
		newGlyph.SourceRect,
		WHITE);

	UpdateTexture(font->Texture, bitmap.data);
	UnloadImage(bitmap);

	for (auto itr = font->Ranges.begin(); itr != font->Ranges.end(); itr++)
	{
		if (codepoint < itr->Start)
		{
			// we have to insert a new item before this one
			rltGlyphRange range;
			range.Start = codepoint;
			range.Glyphs.push_back(newGlyph);
			font->Ranges.insert(itr, range);
			return true;
		}
		else if (codepoint == itr->Start + itr->Glyphs.size())
		{
			itr->Glyphs.push_back(newGlyph);
			return true;
		}

		rltGlyphRange range;
		range.Start = codepoint;
		range.Glyphs.push_back(newGlyph);
		font->Ranges.push_back(range);
		return true;
	}

	return false;
}