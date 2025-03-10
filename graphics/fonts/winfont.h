/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GRAPHICS_WINFONT_H
#define GRAPHICS_WINFONT_H

#include "common/str.h"
#include "graphics/font.h"

namespace Common {
class SeekableReadStream;
class WinResources;
}

namespace Graphics {

struct WinFontDirEntry {
	WinFontDirEntry() : points(0) {}
	WinFontDirEntry(const Common::String &name, uint16 p) : faceName(name), points(p) {}

	// This is really just a simple identifier to match a directory entry with
	// If need-be, we can add other things to check such as italics and strikethrough, etc.
	Common::String faceName;
	uint16 points;
};

class WinFont : public Font {
public:
	WinFont();
	~WinFont();

	/**
	 * Open a font with a name in an FON file.
	 *
	 * If dirEntry is not given, the first font in the FONTDIR will be loaded
	 */
	bool loadFromFON(const Common::Path &fileName, const WinFontDirEntry &dirEntry = WinFontDirEntry());
	bool loadFromFON(Common::SeekableReadStream &stream, const WinFontDirEntry &dirEntry = WinFontDirEntry());

	/** Open a font from an FNT file */
	bool loadFromFNT(const Common::Path &fileName);

	/** Close this font */
	void close();

	// Font API
	int getFontHeight() const { return _pixHeight; }	//< pixels, not points - for points, see getFontSizeInPointsAtDPI()
	int getFontAscent() const { return _ascent; }
	int getMaxCharWidth() const { return _maxWidth; }
	Common::String getName() const { return _name; }
	int getCharWidth(uint32 chr) const;
	void drawChar(Surface *dst, uint32 chr, int x, int y, uint32 color) const;
	int getStyle() const;

	int getFontSizeInPointsAtDPI(const int dpi) const;

	static WinFont *scaleFont(const WinFont *src, int newSize);
private:
	bool loadFromEXE(Common::WinResources *exe, const Common::Path &fileName, const WinFontDirEntry &dirEntry);

	uint32 getFontIndex(Common::SeekableReadStream &stream, const WinFontDirEntry &dirEntry);
	Common::String getFONFontName(Common::SeekableReadStream &stream);
	bool loadFromFNT(Common::SeekableReadStream &stream);
	char indexToCharacter(uint16 index) const;
	uint16 characterToIndex(uint32 character) const;

	uint16 _pixHeight;
	uint16 _maxWidth;
	uint16 _ascent;
	uint16 _sizeInPoints;
	uint16 _dpi;
	byte _firstChar;
	byte _lastChar;
	byte _defaultChar;
	bool _italic;
	bool _strikethrough;
	bool _underline;
	uint16 _weight;
	Common::String _name;

	enum {
		kFontStyleRegular,
		kFontStyleBold = 1,
		kFontStyleItalic = 2,
		kFontStyleUnderline = 4,
	};

	uint16 _glyphCount;
	struct GlyphEntry {
		GlyphEntry() { bitmap = 0; charWidth = 0; offset = 0; }
		~GlyphEntry() { delete[] bitmap; }

		uint16 charWidth;
		uint32 offset;
		byte *bitmap;
	} *_glyphs;
};

} // End of namespace Graphics

#endif
