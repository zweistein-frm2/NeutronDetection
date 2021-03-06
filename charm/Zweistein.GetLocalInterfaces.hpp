/***************************************************************************
 *   Copyright (C) 2019 by Andreas Langhoff <andreas.langhoff@frm2.tum.de> *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation;                                         *
 ***************************************************************************/
#pragma once
#include <string>
#include <boost/asio.hpp>
#include <boost/foreach.hpp>
#include <uv.h>
#include "Zweistein.Logger.hpp"

namespace Zweistein {

inline void GetLocalInterfaces(std::list<std::string>& localinterfaces) {
	char buf[512];
	uv_interface_address_t* info;
	int count, i;
	uv_interface_addresses(&info, &count);
	i = count;
	while (i--) {
		uv_interface_address_t interface = info[i];
		if (interface.is_internal) continue;
		if (interface.address.address4.sin_family == AF_INET) {
			uv_ip4_name(&interface.address.address4, buf, sizeof(buf));
			std::string address = std::string(buf);
			localinterfaces.push_back(address);
		}
		else if (interface.address.address4.sin_family == AF_INET6) {
			uv_ip6_name(&interface.address.address6, buf, sizeof(buf));
			std::string address = std::string(buf);
			//localinterfaces.push_back(address);
		}
	}
	uv_free_interface_addresses(info, count);
	

}
	inline bool  InterfaceExists(std::string proposed) {

		std::list<std::string> localinterfaces = std::list<std::string>();
		Zweistein::GetLocalInterfaces(localinterfaces);
		auto _a = std::find(localinterfaces.begin(), localinterfaces.end(), proposed);
		if (_a == localinterfaces.end()) {
			LOG_ERROR << "interfaces on this machine are:"<<std::endl;
			int i = 0;
			BOOST_FOREACH(std::string str, localinterfaces) { LOG_ERROR << str << "(" << i++ << ")  "<<std::endl; }
			//LOG_ERROR << std::endl;
			return false;
		}
		return true;
	}

	inline std::string askforInterfaceIfUnknown(std::string proposed) {

		std::list<std::string> localinterfaces = std::list<std::string>();
	    Zweistein::GetLocalInterfaces(localinterfaces);
		auto _a = std::find(localinterfaces.begin(), localinterfaces.end(), proposed);
		if (_a == localinterfaces.end()) {
			LOG_INFO << "interfaces on this machine are:"<<std::endl;
			int i = 0;
			BOOST_FOREACH(std::string str, localinterfaces) { std::cout << str << "(" << i++ << ")  "<<std::endl; }
			LOG_INFO << "Choose interface to use: (0),... "<<std::endl;
			int choice = 2;
			std::cin >> choice;
			i = 0;
			BOOST_FOREACH(std::string str, localinterfaces) { if (i++ == choice) proposed = str; }
		}
		return proposed;
	}
}

