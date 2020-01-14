/*
 * This file is part of MinecraftServerPing.
 * Copyright (C) 2020  Chris Josten <chris@netsoj.nl>
 * 
 * MinecraftServerPing is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as published by
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
import QtQuick 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents

import nl.netsoj.minecraftserverping 1.0

PlasmaComponents.Label {
	property int serverState
	property string motd
	property string error
	property string address
	property int port
	
	id: serverMotd
	textFormat: Text.StyledText
	color: serverState == MinecraftServer.ERROR ? theme.negativeTextColor : theme.textColor
	wrapMode: Text.Wrap
	text: switch (serverState) {
		case MinecraftServer.UNKNOWN:
			return qsTr("No server address set")
			//return qsTr("Unknown") + " " + minecraftServer.address + ":"  + minecraftServer.port;
		case MinecraftServer.PINGING:
			return qsTr("Pinging") + " " + address + ":"  + port;
		case MinecraftServer.ONLINE:
			return motd;
		case MinecraftServer.ERROR:
			return error;
		default:
			return qsTr("??")
	}
}
