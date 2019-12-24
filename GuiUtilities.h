#pragma once
#include <QWidget>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class QSFMLCanvas; }
QT_END_NAMESPACE

class QSFMLCanvas : public QWidget, public sf::RenderWindow {
public:
	QSFMLCanvas(QWidget* parent, const QPoint& position, const QSize& size, unsigned int frameTime = 0) : QWidget(parent), myInitialized(false) {
		// Setup some states to allow direct rendering into the widget
		setAttribute(Qt::WA_PaintOnScreen);
		setAttribute(Qt::WA_OpaquePaintEvent);
		setAttribute(Qt::WA_NoSystemBackground);

		// Set strong focus to enable keyboard events to be received
		setFocusPolicy(Qt::StrongFocus);

		// Setup the widget geometry
		move(position);
		resize(size);

		// Setup the timer
		myTimer.setInterval(frameTime);
	}

	virtual ~QSFMLCanvas() {

	}

protected:
	virtual void OnInit() {

	}

	virtual void OnUpdate() {

	}

	void resizeEvent(QResizeEvent* event) {
		setSize(sf::Vector2u(QWidget::width(), QWidget::height()));
		QWidget::resizeEvent(event);
	}

	virtual QPaintEngine* paintEngine() const {
		return 0;
	}

	virtual void showEvent(QShowEvent*) {
		if (!myInitialized) {
			// Under X11, we need to flush the commands sent to the server to ensure that
			// SFML will get an updated view of the windows
#ifdef Q_WS_X11
			XFlush(QX11Info::display());
#endif

			// Create the SFML window with the widget handle
			sf::RenderWindow::create((sf::WindowHandle)winId());

			// Let the derived class do its specific stuff
			OnInit();

			// Setup the timer to trigger a refresh at specified framerate
			connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
			myTimer.start();

			myInitialized = true;
		}
	}

	virtual void paintEvent(QPaintEvent*) {
		// Let the derived class do its specific stuff
		OnUpdate();

		// Display on screen
		display();
	}

	QTimer myTimer;
	bool myInitialized = false;
};