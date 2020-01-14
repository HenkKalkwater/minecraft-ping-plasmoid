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

import QtQuick 2.1
import QtQuick.Layouts 1.1
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.extras 2.0
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents

import nl.netsoj.minecraftserverping 1.0 as MSP

Item {
	/**
	 * This object handles the pinging, and is part of th
	 */
	MSP.MinecraftServer {
		id: server
		name: plasmoid.configuration.name
		address: plasmoid.configuration.address
		port: plasmoid.configuration.port
		autoPing: false
		function validateAndRefresh() {
			if (plasmoid.configuration.address != "" && plasmoid.configuration.address != "example.org" && plasmoid.configuration.port != null) {
				refresh()
			}
		}
		
		onAddressChanged: {
			server.validateAndRefresh()
			pingingNewServer = true
		}
		onPortChanged: {
			server.validateAndRefresh()
			pingingNewServer = true
		}
		Component.onCompleted: firstDelayTimer.start()
	}
	function action_refresh() {
		server.validateAndRefresh()
	}
	Timer {
		id: firstDelayTimer
		interval: 1000
		onTriggered: server.validateAndRefresh()
	}
	// Poor people of example.org won't be able to monitor their minecraft server :/
	Plasmoid.configurationRequired: plasmoid.configuration.address == "" || plasmoid.configuration.address == "example.org" || plasmoid.configuration.port == null
	Plasmoid.configurationRequiredReason: qsTr("Please set an address and port for the Minecraft server to monitor")
	
	Timer {
		interval: plasmoid.configuration.refreshInterval * 1000
		repeat: true
		running: Plasmoid.configurationRequired
		onTriggered: server.validateAndRefresh()
		onIntervalChanged: {
			restart()
			server.validateAndRefresh()
		}
	}
	
	Plasmoid.switchHeight: theme.mSize(theme.defaultFont).height * 4
	Plasmoid.switchWidth: theme.mSize(theme.defaultFont).width * 30
	
    Plasmoid.fullRepresentation: MinecraftServerFullDelegate {
		name: server.name
		currentPlayers: server.currentPlayers
		maxPlayers: server.maxPlayers
		icon: server.icon
		motd: server.motd
		serverState: server.serverState
		error: server.error
		opacity: Plasmoid.configurationRequired ? 0.5 : 1
		//height: parent.height
		Layout.minimumWidth: implicitWidth
		Layout.minimumHeight: implicitHeight
	}
	
	Plasmoid.compactRepresentation: MinecraftServerIconDelegate {
		icon: server.icon
		currentPlayers: server.currentPlayers
		motd: server.motd
		serverState: server.serverState
		opacity: Plasmoid.configurationRequired ? 0.5 : 1
	}
	
   Plasmoid.preferredRepresentation: Plasmoid.fullRepresentation
   Plasmoid.toolTipItem: MinecraftServerFullDelegate {
	   name: server.name
		currentPlayers: server.currentPlayers
		maxPlayers: server.maxPlayers
		icon: server.icon
		motd: server.motd
		serverState: server.serverState
		error: server.error
		opacity: Plasmoid.configurationRequired ? 0.5 : 1
		//height: parent.height
		Layout.minimumWidth: implicitWidth
		Layout.minimumHeight: implicitHeight
	}
	
	Component.onCompleted: {
		plasmoid.setAction("refresh", qsTr("Refresh"), "view-refresh")
	}
}
