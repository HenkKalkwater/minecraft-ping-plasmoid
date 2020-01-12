import QtQuick 2.2
import org.kde.plasma.configuration 2.0

ConfigModel {
	ConfigCategory {
		name: qsTr("Server")
		icon: "preferences-system-network"
		source: "configServer.qml"
	}
}
