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

#ifndef MINECRAFTSERVERPING_H
#define MINECRAFTSERVERPING_H

#include <QDebug>
#include <QQmlExtensionPlugin>
#include <QQmlEngine>

#include "minecraftserver.h"

class MinecraftServerPingPlugin : public QQmlExtensionPlugin {
	Q_OBJECT
    Q_PLUGIN_METADATA(IID "nl.netsoj.minecraftserverping")
public:
	void registerTypes(const char *uri) override;
};
#endif
