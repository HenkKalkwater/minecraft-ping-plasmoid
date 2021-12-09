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

#ifndef MINECRAFTSERVER_H
#define MINECRAFTSERVER_H

#include <QBuffer>
#include <QDataStream>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QStack>
#include <QTcpSocket>

#include "formatcodeconverter.h"

/**
 * Represents a Minecraft Server
 */
class MinecraftServer : public QObject {
	Q_OBJECT
	Q_ENUMS(ServerState)
public:
	enum ServerState {
		ONLINE,
		PINGING,
		ERROR,
		UNKNOWN
	};
	MinecraftServer(QObject *parent = nullptr);
	MinecraftServer(QString name, QString address, int port, QObject *parent = nullptr);
    ~MinecraftServer() override;

    /**
     * Copy constructor
     *
     * @param other TODO
     */
    MinecraftServer(const MinecraftServer& other);
	
	Q_PROPERTY(QString name MEMBER m_name NOTIFY nameChanged)
	Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
	Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
	Q_PROPERTY(int currentPlayers READ currentPlayers NOTIFY currentPlayersChanged STORED false)
	Q_PROPERTY(int maxPlayers READ maxPlayers NOTIFY maxPlayersChanged STORED false)
	Q_PROPERTY(ServerState serverState READ serverState NOTIFY serverStateChanged STORED false)
	Q_PROPERTY(QString error READ error NOTIFY errorChanged STORED false)
	Q_PROPERTY(QString motd READ motd NOTIFY motdChanged STORED false)
	Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(QStringList playerNamesSample READ playerNamesSample NOTIFY playerNamesSampleChanged)
	Q_PROPERTY(bool autoPing MEMBER m_autoPing NOTIFY autoPingChanged);
	
	/**
	 * Sets the address where the server is located.
	 * 
	 * Once the address and port is set, this class will automatically ping the server
	 * to retrieve the rest of the information.
	 */
	QString address() const { return this->m_address; }
	
	/**
	 * Sets the port to connect to
	 * 
	 * Once the address and port is set, this class will automatically ping the server
	 * to retrieve the rest of the information.
	 */
	void setAddress(QString newAddress);
	
	int port() const { return this->m_port; }
	void setPort(int newPort);
	
	/**
	 * @return the current count of players on the server.
	 */
	int currentPlayers() const { return this->m_currentPlayers; }

	QStringList playerNamesSample() const { return this->m_playerNamesSample; }
	
	/**
	 * @return the maximum amount of players on the server.
	 */
	int maxPlayers() const { return this->m_maxPlayers; }
	
	/**
	 * The state of the server:
	 * @returns one of the following enum values.
	 * UNKNOWN: no attempt to ping the server has been made yet.
	 * PINGING: the server is being pinged.
	 * ERROR: the server cannot be reached or an error in the protocol has occured
	 */
	ServerState serverState() const { return this->m_serverState; }
	
	/**
	 * Error string, valid if the serverState is ERROR.
	 */
	QString error() const { return this->m_error; }
	/**
	 * @return the message of the day (description) of the server, in HTML.
	 */
	QString motd() const { return this->m_motd; }
	/**
	 * @return the icon of the server, in the form of data:image/png;base64,\<data\>
	 */
	QString icon() const { return this->m_icon; }

signals:
	void nameChanged();
	void addressChanged(QString newAddress);
	void portChanged(int newPort);
	void currentPlayersChanged(int newCurrentPlayers);
	void maxPlayersChanged(int newMaxPlayers);
	void motdChanged(QString motd);
	void iconChanged();
	void serverStateChanged(ServerState newState);
	void errorChanged(QString newError);
	void autoPingChanged();
	void playerNamesSampleChanged();
	
public slots:
	/**
	 * Attempts to ping the server and updates its data.
	 * 
	 * This function will automatically be called whenever both the address and port are set for the first time or are updated.
	 */
	void refresh();
	
	void socketError(QAbstractSocket::SocketError error);
	void socketConnected();
	void socketReadyRead();

private:
	enum PingState {
		// Not pinging any server at this moment
		NOT_PINGING,
		// Request has been sent, waiting for the response length
		REQUEST_SENT,
		// Response length has been received, awaiting full response
		AWAITING_FULL_RESPONSE
	};
	/**
	 * Sets the MOTD of the server. Also parses the Minecraft Markup and converts it to HTML.
	 */
	void setMotd(QString motd);
	void setServerState(ServerState state);
	void setError(QString error);
	
	/**
	 * User-set name for the server. Has no other purposes than for being displayed.
	 */
	QString m_name;
	
	/**
	 * Address (hostname or IP) for the server to ping.
	 */
	QString m_address;
	
	/**
	 * Port that the Minecraft server running on is being pinged.
	 */
	int m_port;
	
	/**
	 * Message of the Day of the server, in HTML format (so that Qt can display it)
	 */
	QString m_motd;
	
	/**
	 * Base64 encoded PNG image in a data uri.
	 * 
	 * The Minecraft ping protocol sends it to us in this way and while its not the most
	 * space efficient way to store it, it is the easiest way to pass it to QML, without
	 * making an ImageProvider.
	 */
	QString m_icon;
	
	/**
	 * The amount of players currently on the server
	 */
	int m_currentPlayers;
	
	/**
	 * The maximum amount of players allowed on the server.
	 */
	int m_maxPlayers;
	//Not yet implemented: a list of players on the server
	//QStringList m_players;
	/**
	 * State of the server (as seen from the client side)
	 */
	ServerState m_serverState = UNKNOWN;
	
	/**
	 * Error message, in case m_serverState == ServerState.ERROR
	 */
	QString m_error;
	
	/**
	 * True if we should automatically ping the server if the address and/or
	 * port has changed.
	 */
	bool m_autoPing = false;
	
	/**
	 * Socket used to connect to the server.
	 */
	QTcpSocket socket;
	/**
	 * Buffer used to help constructing the packets.
	 */
	QBuffer packetBuffer;

    QStringList m_playerNamesSample;
	/**
	 * Small helper to help format data
	 */
	QDataStream socketDatastream;
	/**
	 * Stores the current state of the state machine used by the pinging procedure
	 */
	PingState pingState = PingState::NOT_PINGING;
	
	/**
	 * Length of the expected next response. We won't start parsing the response
	 * until it has fully arrived.
	 */
	qint32 responseLength = -1;
	
	/**
	 * Reset the socket and internal state machine.
	 */
	void resetInternalState();
	
	/**
	 * Parses the JSON string sent by the Minecraft server and updates its information.
	 */
	void parseResponseJSON(QByteArray &response);
	
	/**
	 * Writes a variable size integer.
	 * 
	 * @param dest The device to write to
	 * @param source The integer to write
	 */
	static void writeVarInt(QIODevice &dest, qint32 source);
	
	/**
	 * Reads a variable size integer.
	 * 
	 * If this fails, it won't automatically rollback the read operations on the QIODevice.
	 * @param source The device to read the integer from
	 * @param result The resulting integer
	 * @returns amount of bytes read, 0 if failed to parse varint.
	 */
	static int readVarInt(QIODevice &source, qint32 &result);
	
	/**
	 * True if address and/or port has been changed since previous ping.
	 * If this is set to true, previous state must be cleared.
	 */
	bool addressChangedSincePreviousPing = true;
};

#endif // MINECRAFTSERVER_H
