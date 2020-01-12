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

#include "serverlistmodel.h"

using namespace MinecraftServerPing;

int ServerListModel::rowCount(const QModelIndex & parent) const {
	return this->servers.size();
}

QHash<int, QByteArray> MinecraftServerPing::ServerListModel::roleNames() const {
	QHash<int, QByteArray> result;
	result[Roles::ADDRESS] = "address";
	return result;
}

QVariant MinecraftServerPing::ServerListModel::data(const QModelIndex& index, int role) const {
	switch(role) {
		case ADDRESS:
			return &this->servers[index.row()];
		default:
			return QVariant();
	}
}

