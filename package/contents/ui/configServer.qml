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
import QtQuick 2.12
import QtQuick.Controls 2.12
import org.kde.kirigami 2.5 as Kirigami
import org.kde.plasma.core 2.0 as PlasmaCore

Item {
	id: page
	property alias cfg_name: name.text
	property alias cfg_address: address.text
	property alias cfg_port: port.value
	property alias cfg_refreshInterval: refreshIntervalField.value
	Kirigami.FormLayout {
		anchors.left: parent.left
        anchors.right: parent.right
		
		TextField {
			id: name
			Kirigami.FormData.label: i18nc("@label:textbox Name of the Minecraft server", "Name:")
		}
		TextField {
			id: address
			Kirigami.FormData.label: i18nc("@label:textbox Address as in IP address", "Address:")
		}
		SpinBox {
			id: port
			Kirigami.FormData.label: i18nc("@label:spinbox Port as in TCP port.", "Port:")
			from: 1
			to: 49151
		}
		
		SpinBox {
			id: refreshIntervalField
			Kirigami.FormData.label: i18n("Refresh interval (hh:mm):")
			from: 0
			to: 99999999
			valueFromText: function(text){
				var parts = text.split(":")
				var hours = Number(parts[0]) * 3600
				var minutes = Number(parts[1]) * 60
				var seconds = Number(parts[2])
				return hours + minutes + seconds
			}
			textFromValue: function(value) {
				var temp = value
				var hours = Math.floor(temp / 3600)
				if (hours < 10) hours = "0" + hours
					temp %= 3600
				var minutes = Math.floor(temp / 60)
				if (minutes < 10) minutes = "0" + minutes
					temp %= 60
				if (temp < 10) temp = "0" + temp
				return hours + ":" + minutes + ":" + temp
			}
		}
	}
}
