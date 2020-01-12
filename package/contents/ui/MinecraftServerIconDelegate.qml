import QtQuick 2.6

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.extras 2.0
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents

import nl.netsoj.minecraftserverping 1.0

Item {
	id: iconRoot
	property string icon
	property int currentPlayers
	property string motd
	property string error
	property int serverState
	
	Image {
		id: serverIcon
		anchors.top: parent.top
		anchors.left: parent.left
		height: Math.min(parent.width, parent.height)
		width: Math.min(parent.width, parent.height)
		source: icon ? icon : "../images/no-icon.svgz"
		
		Rectangle {
			visible: Math.max(parent.width, parent.height) > theme.mSize(theme.defaultFont).width * 3
			anchors.top: parent.top
			anchors.right: parent.right
			anchors.topMargin: -theme.mSize(theme.defaultFont).width * 1
			anchors.rightMargin: -theme.mSize(theme.defaultFont).width * 1
			width: theme.mSize(theme.defaultFont).width * 3
			height: theme.mSize(theme.defaultFont).width * 3
			radius: theme.mSize(theme.defaultFont).width * 1.5
			color: theme.buttonFocusColor
			
			PlasmaComponents.Label {
				anchors.centerIn: parent
				text: currentPlayers
			}
		}
	}
	
	Motd {
		anchors.left: serverIcon.right
		anchors.leftMargin: units.smallSpacing + theme.mSize(theme.defaultFont).width * 1.5
		anchors.verticalCenter: parent.verticalCenter
		anchors.right: parent.right
		visible: (parent.width - serverIcon.width) > (theme.mSize(theme.defaultFont).width * 20)
		maximumLineCount: 2
		wrapMode: Text.Wrap
		
		motd: iconRoot.motd
		serverState: iconRoot.serverState
		error: iconRoot.error
	}
	MouseArea {
		anchors.fill: parent
		onClicked: plasmoid.expanded = !plasmoid.expanded
	}
}
