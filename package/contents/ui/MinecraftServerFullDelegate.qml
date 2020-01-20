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
import QtQuick 2.6
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.extras 2.0
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents

import nl.netsoj.minecraftserverping 1.0

Item {
	id: fullRoot
	clip: true
	implicitHeight: theme.mSize(theme.defaultFont).height * 4 + units.smallSpacing * 2
	implicitWidth: + theme.mSize(theme.defaultFont).width * 30 + theme.mSize(theme.defaultFont).height * 3 + 3 * units.smallSpacing
	property MinecraftServer minecraftServer
	property string name
	property string address
	property int port
	property string icon
	property int currentPlayers
	property int maxPlayers
	property string motd
	property int serverState
	property string error
	
	MinecraftServer {
		id: minecraftServer
	}
	Image {
		id: serverImage
		anchors.left: parent.left
		anchors.leftMargin: units.smallSpacing
		anchors.top: parent.top
		anchors.topMargin: units.smallSpacing
		anchors.bottomMargin: units.smallSpacing
		anchors.bottom: parent.bottom
		width: height
		fillMode: Image.PreserveAspectFit
		source: icon ? icon : "../images/no-icon.svgz"
	}
	Heading {
		id: serverName
		anchors.left: serverImage.right
		anchors.leftMargin: units.largeSpacing
		anchors.top: parent.top
		anchors.right: serverPlayers.left
		anchors.rightMargin: units.largeSpacing
		text: name
		elide: Text.ElideRight
	}
	
	PlasmaComponents.Label {
		id: serverPlayers
		anchors.right: parent.right
		anchors.rightMargin: units.smallSpacing
		anchors.baseline: serverName.baseline
		text: qsTr("??/??", "current and maximum amount of players unknown")
		Binding on text {
			when: fullRoot.serverState == MinecraftServer.ONLINE
			value: qsTr("%1 / %2", "current players/maximum amount of players").arg(currentPlayers).arg(maxPlayers)
		}
	}
	
	Motd {
		id: serverMotd
		anchors.top: serverName.bottom
		anchors.left: serverImage.right
		anchors.leftMargin: units.largeSpacing
		anchors.right: parent.right
		
		motd: fullRoot.motd
		serverState: fullRoot.serverState
		error: fullRoot.error
		address: fullRoot.address
		port: fullRoot.port
	}
}
