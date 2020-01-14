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
