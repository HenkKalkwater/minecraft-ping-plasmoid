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

#include "minecraftserver.h"

MinecraftServer::MinecraftServer(QObject *parent) : MinecraftServer("","", 0, parent) {
	qDebug() << "Empty constructor called";
}

MinecraftServer::MinecraftServer(QString name, QString address, int port, QObject *parent)
	: QObject(parent), m_name(name), m_address(address), m_port(port), m_currentPlayers(-1), m_maxPlayers(-1), socketDatastream(&packetBuffer) {
	connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
			this, &MinecraftServer::socketError);
	connect(&socket, &QAbstractSocket::connected, this, &MinecraftServer::socketConnected);
	connect(&socket, &QAbstractSocket::readyRead, this, &MinecraftServer::socketReadyRead);
	packetBuffer.open(QIODevice::ReadWrite);
}


MinecraftServer::~MinecraftServer() {};

MinecraftServer::MinecraftServer(const MinecraftServer& other)
	: MinecraftServer(other.m_name, other.m_address, other.m_port, other.parent()){
		qDebug() << "Copy constructor called";
}

void MinecraftServer::setAddress(QString newAddress) { 
	if (newAddress != this->m_address) {
		this->addressChangedSincePreviousPing = true;
	}
	this->m_address = newAddress;
	emit addressChanged(newAddress);
	if (this->m_port != 0 && !this->m_address.isEmpty() && this->m_autoPing) {
		this->refresh();
	} else {
		qDebug() << "Port not yet set";
	}
}

void MinecraftServer::setPort(int newPort)  { 
	if (newPort != this->m_port) {
		this->addressChangedSincePreviousPing = true;
	}
	this->m_port = newPort; 
	emit portChanged(newPort);
	if (this->m_port != 0 && !this->m_address.isEmpty() && this->m_autoPing) {
		this->refresh();
	} else {
		qDebug() << "Address not yet set";
	}
}

void MinecraftServer::setServerState(MinecraftServer::ServerState state) {
	this->m_serverState = state;
	emit serverStateChanged(state);
}

void MinecraftServer::setError(QString error) {
	this->resetInternalState();
	setServerState(ServerState::ERROR);
	this->m_error = error;
	emit errorChanged(error);
}

void MinecraftServer::setMotd(QString motd) {
	QString parsedMotd = formattingCodeToHTML(motd);
	this->m_motd = parsedMotd;
	emit motdChanged(parsedMotd);
}

void MinecraftServer::socketError(QAbstractSocket::SocketError error) {
	switch(error) {
		case QAbstractSocket::SocketError::HostNotFoundError:
			setError(tr("Host not found"));
			break;
		case QAbstractSocket::SocketError::RemoteHostClosedError:
			if (this->m_serverState != ONLINE) {
				// The remote host closed the connection, but we haven't got a response.
				setError(tr("The server unexpectedly closed the connection"));
			}
			break;
		default:
			setError(tr("Error: %1").arg(socket.errorString()));
			break;
	}
	this->resetInternalState();
	qInfo() << "Error while connecting: " << this->m_error;
}

void MinecraftServer::socketConnected() {
	qDebug() << "Socket connected";
	
	// Write data state packet
	writeVarInt(packetBuffer, 0x00);
	writeVarInt(packetBuffer, -1);
	QByteArray hostName = this->m_address.toUtf8();
	writeVarInt(packetBuffer, hostName.size());
	packetBuffer.write(hostName);
	socketDatastream << static_cast<quint16>(this->m_port);
	writeVarInt(packetBuffer, 0x01);
	
	//Write data to socket
	writeVarInt(socket, packetBuffer.pos());
	socket.write(packetBuffer.data());
	
	// Write size of handshake packet
	writeVarInt(socket, 1);
	writeVarInt(socket, 0);
	socket.flush();
	qDebug() << "---------[START PARSING SOCKET DATA]------------";
	this->pingState = PingState::REQUEST_SENT;
}

void MinecraftServer::socketReadyRead() {
	bool canStillParse = true;
	qDebug() << "Bytes available: " << socket.bytesAvailable();
	while (canStillParse && socket.bytesAvailable() > 0) {
		switch(this->pingState) {
			case REQUEST_SENT:
				qDebug() << "Request sent. Expecting a varInt";
				socket.startTransaction();
				if (readVarInt(socket, this->responseLength)) {
					qDebug() << "Response lenght: " << this->responseLength;
					this->pingState = AWAITING_FULL_RESPONSE;
					socket.commitTransaction();
				} else {
					qDebug() << "Not a full integer";
					socket.rollbackTransaction();
					canStillParse = false;
				}
				break;
			case AWAITING_FULL_RESPONSE:
				qDebug() << "Awaiting full response.";
				if (socket.bytesAvailable() < this->responseLength) {
					qDebug() << "Not enough bytes received";
					canStillParse = false;
				} else {
					qDebug() << "Reading packet id";
					//QByteArray data = socket.read(this->responseLength);
					qint32 packetId;
					qint32 bytesRead = readVarInt(socket, packetId);
					if (!bytesRead) {
						this->setError("Could not parse packet");
						canStillParse = false;
						break;
					}
					if (packetId != 0x00) {
						this->setError(tr("Could not parse response: invalid packet id"));
						canStillParse = false;
					} else {
						qint32 stringLength;
						qDebug() << "Reading string size";
						qint32 bytesRead2 = readVarInt(socket, stringLength);
						if (!bytesRead2) {
							this->setError(tr("Could not parse response: malformed variable-sized integer"));
							canStillParse = false;
							break;
						};
						if (stringLength != (this->responseLength - bytesRead - bytesRead2)) {
							this->setError(tr("Could not parse response: invalid packet size (%1, expected %2)").arg(stringLength, this->responseLength - bytesRead - bytesRead2));
							canStillParse = false;
							break;
						}
						QByteArray data = socket.read(stringLength);
						this->parseResponseJSON(data);
						this->setServerState(ONLINE);
						this->pingState = NOT_PINGING;
						this->resetInternalState();
						socket.close();
					}
				}
				break;
			default:
				qWarning() << "Socket response parser landed in invalid state";
				canStillParse = false;
				this->resetInternalState();
				break;
		}
	}
}

void MinecraftServer::refresh() {
	qDebug() << "Refreshing server";
	if (this->m_serverState == ServerState::PINGING) return;
	if (this->addressChangedSincePreviousPing) {
		qDebug() << "Address changed since previous ping";
		this->addressChangedSincePreviousPing = false;
		this->m_motd = "";
		emit motdChanged("");
		this->m_currentPlayers = -1;
		emit currentPlayersChanged(-1);
		this->m_maxPlayers = -1;
		emit currentPlayersChanged(-1);
		this->m_icon = "";
		emit iconChanged();
	}
	
	this->setServerState(ServerState::PINGING);
	this->socket.connectToHost(this->m_address, this->m_port);
}

void MinecraftServer::parseResponseJSON(QByteArray &response) {
	qDebug () << response;
	QJsonDocument doc = QJsonDocument::fromJson(response);
	if (doc.isNull()) {
		this->setError(tr("Could not parse response: malformed JSON"));
		return;
	}
	if (!doc.isObject()) {
		this->setError(tr("Could not parse response: expected object as root JSON object"));
		return;
	}
	QJsonObject root = doc.object();
	if (root.isEmpty()) {
		this->setError(tr("Server still booting"));
		return;
	}
	if (root.contains("description") && root["description"].isObject() 
		&& root["description"].toObject().contains("text") && root["description"].toObject()["text"].isString()) {
		this->setMotd(root["description"].toObject()["text"].toString());
	} else if (root.contains("description") && root["description"].isString()){
		this->setMotd(root["description"].toString());
	} else {
		this->setError(tr("Could not parse response: could not extract MOTD"));
	}
	if (root.contains("favicon") && root["favicon"].isString()) {
		this->m_icon = root["favicon"].toString();
		emit this->iconChanged();
	} else {
		this->m_icon = nullptr;
		emit this->iconChanged();
	}
	if (root.contains("players") && root["players"].isObject()) {
		QJsonObject players = root["players"].toObject();
		if (players.contains("online") && players["online"].isDouble()) {
			this->m_currentPlayers = static_cast<int>(players["online"].toDouble());
			emit this->currentPlayersChanged(this->m_currentPlayers);
		}
		if (players.contains("max") && players["max"].isDouble()) {
			this->m_maxPlayers = static_cast<int>(players["max"].toDouble());
			emit this->maxPlayersChanged(this->m_maxPlayers);
		}
	}
}


void MinecraftServer::writeVarInt(QIODevice &dest, qint32 source) {
	//qDebug() << "writeVarInt(source: " << Qt::bin << source << ")";
	quint8 count = 0;
	do {
		quint8 temp = static_cast<quint8>(source & 0b01111111);
		source = static_cast<quint32>(source) >> 7;
		if (source != 0) {
			temp |= 0b10000000;
		}
		//qDebug() << Qt::bin << "temp: " << temp << ", source: " << source;
		dest.putChar(temp);
		count++;
	}
	while (source != 0 && count < 5);
}

int MinecraftServer::readVarInt(QIODevice &source, int &result) {
	int numRead = 0;
	quint8 read;
	result = 0;
	do {
		if (!source.getChar(reinterpret_cast<char *>(&read))) { 
			qDebug() << "Could not execute getChar";
			return 0;
		}
		
		qint32 value = (read & 0b01111111);
		qDebug() << Qt::hex << value;
		result |= (static_cast<quint32>(value) << (7 * numRead));
		
		numRead++;
		if (numRead > 5) {
			qDebug() << "varInt too long";
			return 0;
		}
	} while ((read & 0b10000000) != 0);
	qDebug() << "Parsed varInt: result: " << Qt::hex << result << ", numRead: " << Qt::dec << numRead;
	return numRead;
}

void MinecraftServer::resetInternalState() {
	qDebug() << "---------[DONE PARSING SOCKET DATA]------------";
	this->packetBuffer.reset();
}
