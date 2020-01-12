import QtQuick 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents

import nl.netsoj.minecraftserverping 1.0

PlasmaComponents.Label {
	property int serverState
	property string motd
	property string error
	
	id: serverMotd
	textFormat: Text.StyledText
	color: serverState == MinecraftServer.ERROR ? theme.negativeTextColor : theme.textColor
	wrapMode: Text.Wrap
	text: switch (serverState) {
		case MinecraftServer.UNKNOWN:
			return qsTr("Please configure this widget")
			//return qsTr("Unknown") + " " + minecraftServer.address + ":"  + minecraftServer.port;
		case MinecraftServer.PINGING:
			return qsTr("Pinging") + " " + minecraftServer.address + ":"  + minecraftServer.port;
		case MinecraftServer.ONLINE:
			return motd;
		case MinecraftServer.ERROR:
			return error;
		default:
			return qsTr("??")
	}
}
