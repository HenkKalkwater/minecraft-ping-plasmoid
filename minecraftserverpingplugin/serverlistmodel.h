/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2020  Chris Josten <chris@netsoj.nl>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MINECRAFTSERVERPING_SERVERLISTMODEL_H
#define MINECRAFTSERVERPING_SERVERLISTMODEL_H

#include <QAbstractListModel>

#include "minecraftserver.h"

namespace MinecraftServerPing {

/**
 * @todo write docs
 */
class ServerListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        ADDRESS = Qt::UserRole,
		PORT,
		NAME,
		MOTD,
		MIN_PLAYERS,
		MAX_PLAYERS
    };
	
	int rowCount(const QModelIndex & parent) const override;
	QVariant data(const QModelIndex & index, int role) const override;
	QHash<int, QByteArray> roleNames() const override;
private:
	QList<MinecraftServer> servers;

};

}

#endif // MINECRAFTSERVERPING_SERVERLISTMODEL_H
