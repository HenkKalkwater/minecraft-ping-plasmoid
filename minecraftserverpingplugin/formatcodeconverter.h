#ifndef FORMATCODECONVERTER_H
#define FORMATCODECONVERTER_H

#include <QDebug>
#include <QMap>
#include <QStack>
#include <QString>

enum ParseMode {
	NORMAL,
	ESCAPE
};
enum EscapeCharType {
	COLOR_BLACK = 0,
	COLOR_DARK_BLUE,
	COLOR_DARK_GREEN,
	COLOR_DARK_CYAN,
	COLOR_DARK_RED,
	COLOR_PURPLE,
	COLOR_GOLD,
	COLOR_GRAY,
	COLOR_DARK_GRAY,
	COLOR_BLUE,
	COLOR_BRIGHT_GREEN,
	COLOR_CYAN,
	COLOR_RED,
	COLOR_PINK,
	COLOR_YELLOW,
	COLOR_WHITE,
	
	MASK_STYLE = 0xf0,
	OBFUSCATED,
	BOLD,
	STRIKETROUGH,
	UNDERLINE,
	ITALIC,
	RESET,
	UNKNOWN
};

const QMap<EscapeCharType, QString> colorMap({
	{COLOR_BLACK, "#000000"},
	{COLOR_DARK_BLUE, "#0000aa"},
	{COLOR_DARK_GREEN, "#00aa00"},
	{COLOR_DARK_CYAN, "#00aaaa"},
	{COLOR_DARK_RED, "#aa0000"},
	{COLOR_PURPLE, "#aa00aa"},
	{COLOR_GOLD, "#ffaa00"},
	{COLOR_GRAY, "#aaaaaa"},
	{COLOR_DARK_GRAY, "#555555"},
	{COLOR_BLUE, "#5555ff"},
	{COLOR_BRIGHT_GREEN, "#55ff55"},
	{COLOR_CYAN, "#55ffff"},
	{COLOR_RED, "#ff5555"},
	{COLOR_PINK, "#ff55ff"},
	{COLOR_YELLOW, "#ffff55"},
	{COLOR_WHITE, "#ffffff"}
});
const QMap<QChar, EscapeCharType> escapeCharMap({
	{'0', COLOR_BLACK},
	{'1', COLOR_DARK_BLUE},
	{'2', COLOR_DARK_GREEN},
	{'3', COLOR_DARK_CYAN},
	{'4', COLOR_DARK_RED},
	{'5', COLOR_PURPLE},
	{'6', COLOR_GOLD},
	{'7', COLOR_GRAY},
	{'8', COLOR_DARK_GRAY},
	{'9', COLOR_BLUE},
	{'a', COLOR_BRIGHT_GREEN},
	{'b', COLOR_CYAN},
	{'c', COLOR_RED},
	{'d', COLOR_PINK},
	{'e', COLOR_YELLOW},
	{'f', COLOR_WHITE},
	{'k', OBFUSCATED},
	{'l', BOLD},
	{'m', STRIKETROUGH},
	{'n', UNDERLINE},
	{'o', ITALIC},
	{'r', RESET}
});

const QMap<EscapeCharType, QString> escapeCharOpenTagMap({
	{BOLD, "<b>"},
	{STRIKETROUGH, "<s>"},
	{UNDERLINE, "<u>"},
	{ITALIC, "<i>"}
});
const QMap<EscapeCharType, QString> escapeCharCloseTagMap({
	{BOLD, "</b>"},
	{STRIKETROUGH, "</s>"},
	{UNDERLINE, "</u>"},
	{ITALIC, "</i>"}
});

/**
 * Converts a Minecraft formatted code to HTML.
 * 
 * For Minecraft formatted code reference, see https://wiki.vg/Chat#Old_system
 */
QString formattingCodeToHTML(QString formattingCode);

#endif //FORMATCODECONVERTER_H
