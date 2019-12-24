#include "GameInstanceWindow.h"

#include <SFML/Graphics.hpp>
#include "GameInstance.h"

#include <QApplication>
#include <QFrame>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <exception>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
	boost::log::add_file_log("sample.log", boost::log::keywords::auto_flush = true, boost::log::keywords::format = "%TimeStamp% [%Uptime%] (%LineID%) <%Severity%>: %Message%");
	//boost::log::core::get()->set_filter
	//(
	//	boost::log::trivial::severity >= boost::log::trivial::info
	//);

	try {
		QApplication a(argc, argv);

		GameInstanceWindow* MainFrame = new GameInstanceWindow();

		//QFrame* MainFrame = new QFrame();
		//MainFrame->setWindowTitle("Qt SFML");
		//MainFrame->resize(400, 400);
		//MainFrame->show();

		GameInstance b("test pack", MainFrame, QPoint(0, 0), QSize(100, 100));
		b.loadLevel(0);
		b.start();

		MainFrame->getLayout()->addWidget(b.getWindow());
		MainFrame->show();

		return a.exec();
	} catch(std::exception e) {
		BOOST_LOG_TRIVIAL(error) << e.what();
	}
}
