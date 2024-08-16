#include "gui.hpp"

#include "sim.hpp"
#include "app.hpp"

namespace nGui {

	namespace nUtil {

		void fAddGuiHorLine(auto vParent, auto &vLayout) {
			auto vHSep = new QFrame(vParent);
			vLayout->addWidget(vHSep, 1);
			vHSep->setFrameShape(QFrame::HLine);
			vHSep->setLineWidth(1);
		}

	}//namespace nUtil

	//tAppWindow{

	//codetor

	tAppWindow::tAppWindow(QWidget *vParentPtr, Qt::WindowFlags vFlags)
		: QMainWindow(vParentPtr, vFlags)
		, v1stWindow{new t1stWindow(this)}
		, vSimWindow{new tSimWindow(this)}
		, vStack{new QStackedWidget(this)} {
		{//Stack

			this->setCentralWidget(this->vStack);

			this->vStack->addWidget(this->v1stWindow);
			this->vStack->addWidget(this->vSimWindow);

			this->vStack->setCurrentIndex(0);
			connect(
				static_cast<nApp::tApp *>(QApplication::instance()),
				&nApp::tApp::sRunSimCall,
				this,
				[this]() {
					this->vStack->setCurrentIndex(1);
				}
			);
		}
    if constexpr(0){//Fonts

			auto vFontFpath = "data/FiraMono-Bold.ttf";
			auto vFontIndex = QFontDatabase::addApplicationFont(vFontFpath);
			auto vFontIdentArray = QFontDatabase::applicationFontFamilies(vFontIndex);
			auto vFontIdent = vFontIdentArray.at(0);

			auto vFontEntry = QFont(vFontIdent, 16);
			vFontEntry.setBold(true);

			this->setFont(vFontEntry);
		}
		{//Window show

			this->setWindowTitle(dProjName);

			auto vRect = QGuiApplication::primaryScreen()->geometry();
			this->resize(vRect.width() / 2, vRect.height());

			this->show();
		}
	}

	tAppWindow::~tAppWindow() {
		this->disconnect();
	}

	//}tAppWindow

	//t1stWindow{

	//codetor

	t1stWindow::t1stWindow(tAppWindow *vAppWindow)
		: QWidget(vAppWindow)
		, vLayout{new QVBoxLayout(this)}
		, vPrompt{new QTextEdit("start prompt", this)}
		, vButton{new QPushButton("start", this)} {
		this->setLayout(this->vLayout);

		this->vLayout->addWidget(this->vPrompt, 4);
		this->vPrompt->setReadOnly(1);
		this->vPrompt->setText(
			"Welcome to the micro-organism simulation!\n"
			"Before we run the simulation,"
			" we want you to configure this."
			"\n"
			"The configuration is gonna start"
			" as soon as you hit the button below."
		);

		this->vLayout->addWidget(this->vButton, 1);
		this->vButton->setText("Config");
		connect(this->vButton, &QPushButton::clicked, this, [this]() {
			bool vInputFlag;
			auto vSimBeingCount = QInputDialog::getInt(
				this,
				"Starting Simulation",
				fmt::format(
					"Enter the number of micro-organisms from {} to {}.",
					this->vSimBeingMinCount,
					this->vSimBeingMaxCount
				)
					.c_str(),
				0,
				this->vSimBeingMinCount,
				this->vSimBeingMaxCount,
				1,
				&vInputFlag
			);
			if(vInputFlag) {
				auto vApp = static_cast<nApp::tApp *>(QApplication::instance());
				vApp->fRunSim(vSimBeingCount);
			}
		});
	}

	t1stWindow::~t1stWindow() {
		this->disconnect();
	}

	//}t1stWindow

	//tSimWindow{

	tSimWindow::tSimWindow(tAppWindow *vAppWindow)
		: QWidget(vAppWindow)
		, vLayout{new QVBoxLayout(this)}
		, vPrompt{new QTextEdit("key prompt", this)}
		, vOutput{new QTextEdit("key output", this)}
		, vReport{new tSimReport(this)} {
		this->setLayout(this->vLayout);

		this->vLayout->addWidget(this->vPrompt, 3);
		this->vPrompt->setReadOnly(1);
		this->vPrompt->setText(
			"Hit one of the keys:"
			"\n- 'a' to use antibiotics;"
			"\n- 'q' to quit;"
		);

		nUtil::fAddGuiHorLine(this, this->vLayout);

		this->vLayout->addWidget(this->vOutput, 2);
		this->vOutput->setReadOnly(1);

		nUtil::fAddGuiHorLine(this, this->vLayout);

		this->vLayout->addWidget(this->vReport, 8);
	}

	tSimWindow::~tSimWindow() {
		this->disconnect();
	}

	//actions

	void tSimWindow::keyPressEvent(QKeyEvent *vQKeyEvent) {
		auto vApp = static_cast<nApp::tApp *>(QApplication::instance());
		vApp->fRunEventKeyPress(vQKeyEvent);
		QWidget::keyPressEvent(vQKeyEvent);
	}

	//}tSimWindow

	//tSimReport{

	//codetor

	tSimReport::tSimReport(tSimWindow *vSimWindow)
		: QWidget(vSimWindow)
		, vLayout{new QVBoxLayout(this)}
		, vStatus{new QTextEdit(this)}
		, vScroll{new tSimReportScroll(this)} {
		this->vLayout->addWidget(this->vStatus, 1);
		this->vStatus->setReadOnly(1);

		nUtil::fAddGuiHorLine(this, this->vLayout);
		this->vLayout->addWidget(this->vScroll, 4);
	}

	tSimReport::~tSimReport() {
		this->disconnect();
	}

	//}tSimReport

	//tSimReportScroll{

	//codetor

	tSimReportScroll::tSimReportScroll(tSimReport *vSimReport)
		: QScrollArea(vSimReport), vWidget{new tSimReportScrollWidget(this)} {
		this->setWidget(this->vWidget);
		this->setWidgetResizable(1);
	}

	//}tSimReportScroll

	//tSimReportScrollWidget{

	//codetor

	tSimReportScrollWidget::
		tSimReportScrollWidget(tSimReportScroll *vSimReportScroll)
		: QWidget(vSimReportScroll), vLayout{new QVBoxLayout(this)} {
		this->setLayout(this->vLayout);
	}

	tSimReportScrollWidget::~tSimReportScrollWidget() {
		this->fClean();
    this->disconnect();
	}

	//actions

	void tSimReportScrollWidget::fClean() {
		while(auto*vLayoutItem = this->vLayout->takeAt(0)) {
      auto *vWidget = vLayoutItem->widget();
      if (vWidget) {
        vWidget->setParent(nullptr);
        delete vWidget;
      }
			delete vLayoutItem;
		}
	}

	//}tSimReportScrollWidget

}//namespace nGui
