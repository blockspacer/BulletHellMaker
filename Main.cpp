#include "MainEditor.h"

#include <SFML/Graphics.hpp>
#include "GameInstance.h"

#include <QApplication>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <exception>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>

int main(int argc, char *argv[])
{
	boost::log::add_file_log("sample.log", boost::log::keywords::auto_flush = true, boost::log::keywords::format = "%TimeStamp% [%Uptime%] (%LineID%) <%Severity%>: %Message%");
	//boost::log::core::get()->set_filter
	//(
	//	boost::log::trivial::severity >= boost::log::trivial::info
	//);
	BOOST_LOG_TRIVIAL(info) << "aaaaaaa";

	try {
		QApplication a(argc, argv);
		//MainEditor w;
		//w.show();
		GameInstance b("test pack");
		b.loadLevel(0);
		b.start();
		return a.exec();
	} catch(std::exception e) {
		BOOST_LOG_TRIVIAL(error) << e.what();
	}
}
