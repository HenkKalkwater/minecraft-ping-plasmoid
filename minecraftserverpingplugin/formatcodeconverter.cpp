/*
 * This file is part of MinecraftServerPing.
 * Copyright (C) 2020  Chris Josten <chris@netsoj.nl>
 * 
 * MinecraftServerPing is free software: you can redistribute it and/or modify
 * it under the terms of the lesser GNU General Public License as published by
 * the Free Software Foundation, version 3
 *
 * MinecraftServerPing is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with MinecraftServerPing. If not, see <https://www.gnu.org/licenses/>.
 */
#include "formatcodeconverter.h"

QString formattingCodeToHTML(QString formattedString) {
	QString parsedMotd;
	QStack<EscapeCharType> parseStack;
	ParseMode parseMode = NORMAL;
	const QChar escapeChar = L'§';
	const QString colorOpenTag = "<font color='%1'>";
	const QString colorCloseTag = "</font>";
	
	foreach (QChar character, formattedString) {
		switch(parseMode) {
			case NORMAL:
				if (character == escapeChar) {
					parseMode = ESCAPE;
				} else if (character == '\n') {
					parsedMotd += "<br />";
				} else {
					parsedMotd += character;
				}
				break;
			case ESCAPE:
				if (escapeCharMap.contains(character)) {
					EscapeCharType charType = escapeCharMap[character];
					if ((charType & MASK_STYLE) == MASK_STYLE) {
						if (escapeCharOpenTagMap.contains(charType)) {
							parsedMotd += escapeCharOpenTagMap[charType];
							parseStack.push(charType);
						} else if (charType == RESET) {
							while (!parseStack.isEmpty()) {
								EscapeCharType typeToRemove = parseStack.pop();
								if ((typeToRemove & MASK_STYLE) == MASK_STYLE) {
									if (escapeCharCloseTagMap.contains(typeToRemove)) {
										parsedMotd += escapeCharCloseTagMap[typeToRemove];
									}
								} else {
									parsedMotd += colorCloseTag;
								}
							}
						}
					
					} else {
						parsedMotd += colorOpenTag.arg(colorMap[charType]);
						parseStack.push(charType);
					}
				} else {
					parsedMotd += escapeChar;
					parsedMotd += character;
				}
			parseMode = NORMAL;
			break;
		}
	}
	qDebug() << "Parsed MOTD: " << parsedMotd;
	return parsedMotd;
}
