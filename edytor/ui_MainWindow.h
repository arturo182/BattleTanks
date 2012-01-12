/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu 12. Jan 02:16:11 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionWczytaj;
    QAction *actionWyjdz;
    QAction *actionZapisz;
    QAction *actionNowaPlansza;
    QAction *actionDodaj;
    QAction *actionUsun;
    QAction *actionTlo;
    QAction *actionPrzybliz;
    QAction *actionOddal;
    QAction *actionPrzesuwanie;
    QAction *actionEdycjaWierzcholkow;
    QAction *actionZaznaczanie;
    QAction *actionPrzesuwanieWidoku;
    QAction *actionZapiszJako;
    QAction *actionOryginalnyRozmiar;
    QAction *actionDodajPunktRuchu;
    QAction *actionLaczeniePunktowRuchu;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menuPlik;
    QMenu *menuWidok;
    QMenu *menuNarzedzia;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_2;
    QTreeWidget *treeWidget;
    QDockWidget *dockWidget2;
    QWidget *dockWidgetContents_2;
    QGridLayout *gridLayout_3;
    QTreeWidget *treeWidget2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(734, 418);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ikona.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionWczytaj = new QAction(MainWindow);
        actionWczytaj->setObjectName(QString::fromUtf8("actionWczytaj"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/ikony/folder.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWczytaj->setIcon(icon1);
        actionWyjdz = new QAction(MainWindow);
        actionWyjdz->setObjectName(QString::fromUtf8("actionWyjdz"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/ikony/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWyjdz->setIcon(icon2);
        actionZapisz = new QAction(MainWindow);
        actionZapisz->setObjectName(QString::fromUtf8("actionZapisz"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/ikony/disk.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZapisz->setIcon(icon3);
        actionNowaPlansza = new QAction(MainWindow);
        actionNowaPlansza->setObjectName(QString::fromUtf8("actionNowaPlansza"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/ikony/page_white.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNowaPlansza->setIcon(icon4);
        actionDodaj = new QAction(MainWindow);
        actionDodaj->setObjectName(QString::fromUtf8("actionDodaj"));
        actionDodaj->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/ikony/shape_square_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDodaj->setIcon(icon5);
        actionUsun = new QAction(MainWindow);
        actionUsun->setObjectName(QString::fromUtf8("actionUsun"));
        actionUsun->setEnabled(false);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/ikony/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUsun->setIcon(icon6);
        actionTlo = new QAction(MainWindow);
        actionTlo->setObjectName(QString::fromUtf8("actionTlo"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/ikony/picture.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTlo->setIcon(icon7);
        actionPrzybliz = new QAction(MainWindow);
        actionPrzybliz->setObjectName(QString::fromUtf8("actionPrzybliz"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/ikony/zoom_in.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrzybliz->setIcon(icon8);
        actionOddal = new QAction(MainWindow);
        actionOddal->setObjectName(QString::fromUtf8("actionOddal"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/ikony/zoom_out.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOddal->setIcon(icon9);
        actionPrzesuwanie = new QAction(MainWindow);
        actionPrzesuwanie->setObjectName(QString::fromUtf8("actionPrzesuwanie"));
        actionPrzesuwanie->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/ikony/arrow_nsew.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrzesuwanie->setIcon(icon10);
        actionEdycjaWierzcholkow = new QAction(MainWindow);
        actionEdycjaWierzcholkow->setObjectName(QString::fromUtf8("actionEdycjaWierzcholkow"));
        actionEdycjaWierzcholkow->setCheckable(true);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/ikony/shape_handles.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEdycjaWierzcholkow->setIcon(icon11);
        actionZaznaczanie = new QAction(MainWindow);
        actionZaznaczanie->setObjectName(QString::fromUtf8("actionZaznaczanie"));
        actionZaznaczanie->setCheckable(true);
        actionZaznaczanie->setChecked(true);
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/ikony/cursor.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZaznaczanie->setIcon(icon12);
        actionPrzesuwanieWidoku = new QAction(MainWindow);
        actionPrzesuwanieWidoku->setObjectName(QString::fromUtf8("actionPrzesuwanieWidoku"));
        actionPrzesuwanieWidoku->setCheckable(true);
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/ikony/hand.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrzesuwanieWidoku->setIcon(icon13);
        actionZapiszJako = new QAction(MainWindow);
        actionZapiszJako->setObjectName(QString::fromUtf8("actionZapiszJako"));
        actionOryginalnyRozmiar = new QAction(MainWindow);
        actionOryginalnyRozmiar->setObjectName(QString::fromUtf8("actionOryginalnyRozmiar"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/ikony/zoom_actual.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOryginalnyRozmiar->setIcon(icon14);
        actionDodajPunktRuchu = new QAction(MainWindow);
        actionDodajPunktRuchu->setObjectName(QString::fromUtf8("actionDodajPunktRuchu"));
        actionDodajPunktRuchu->setCheckable(true);
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/ikony/flag_yellow.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDodajPunktRuchu->setIcon(icon15);
        actionLaczeniePunktowRuchu = new QAction(MainWindow);
        actionLaczeniePunktowRuchu->setObjectName(QString::fromUtf8("actionLaczeniePunktowRuchu"));
        actionLaczeniePunktowRuchu->setCheckable(true);
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/ikony/flag_connect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLaczeniePunktowRuchu->setIcon(icon16);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout->addWidget(graphicsView, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 734, 21));
        menuPlik = new QMenu(menuBar);
        menuPlik->setObjectName(QString::fromUtf8("menuPlik"));
        menuWidok = new QMenu(menuBar);
        menuWidok->setObjectName(QString::fromUtf8("menuWidok"));
        menuNarzedzia = new QMenu(menuBar);
        menuNarzedzia->setObjectName(QString::fromUtf8("menuNarzedzia"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setMovable(false);
        mainToolBar->setIconSize(QSize(16, 16));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMovable(false);
        toolBar->setIconSize(QSize(16, 16));
        MainWindow->addToolBar(Qt::LeftToolBarArea, toolBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMaximumSize(QSize(250, 524287));
        dockWidget->setFloating(false);
        dockWidget->setFeatures(QDockWidget::DockWidgetMovable);
        dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout_2 = new QGridLayout(dockWidgetContents);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        treeWidget = new QTreeWidget(dockWidgetContents);
        new QTreeWidgetItem(treeWidget);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setIndentation(16);
        treeWidget->setHeaderHidden(true);

        gridLayout_2->addWidget(treeWidget, 0, 0, 1, 1);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);
        dockWidget2 = new QDockWidget(MainWindow);
        dockWidget2->setObjectName(QString::fromUtf8("dockWidget2"));
        dockWidget2->setFeatures(QDockWidget::DockWidgetMovable);
        dockWidget2->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        gridLayout_3 = new QGridLayout(dockWidgetContents_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        treeWidget2 = new QTreeWidget(dockWidgetContents_2);
        new QTreeWidgetItem(treeWidget2);
        treeWidget2->setObjectName(QString::fromUtf8("treeWidget2"));
        treeWidget2->setIndentation(16);
        treeWidget2->setHeaderHidden(true);

        gridLayout_3->addWidget(treeWidget2, 0, 0, 1, 1);

        dockWidget2->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget2);

        menuBar->addAction(menuPlik->menuAction());
        menuBar->addAction(menuWidok->menuAction());
        menuBar->addAction(menuNarzedzia->menuAction());
        menuPlik->addAction(actionNowaPlansza);
        menuPlik->addAction(actionWczytaj);
        menuPlik->addAction(actionZapisz);
        menuPlik->addAction(actionZapiszJako);
        menuPlik->addSeparator();
        menuPlik->addAction(actionWyjdz);
        menuWidok->addAction(actionPrzybliz);
        menuWidok->addAction(actionOddal);
        menuNarzedzia->addAction(actionZaznaczanie);
        menuNarzedzia->addAction(actionPrzesuwanieWidoku);
        menuNarzedzia->addAction(actionPrzesuwanie);
        menuNarzedzia->addAction(actionUsun);
        menuNarzedzia->addSeparator();
        menuNarzedzia->addAction(actionDodaj);
        menuNarzedzia->addAction(actionEdycjaWierzcholkow);
        menuNarzedzia->addSeparator();
        menuNarzedzia->addAction(actionDodajPunktRuchu);
        menuNarzedzia->addAction(actionLaczeniePunktowRuchu);
        menuNarzedzia->addSeparator();
        menuNarzedzia->addAction(actionTlo);
        mainToolBar->addAction(actionNowaPlansza);
        mainToolBar->addAction(actionWczytaj);
        mainToolBar->addAction(actionZapisz);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionTlo);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionOryginalnyRozmiar);
        mainToolBar->addAction(actionPrzybliz);
        mainToolBar->addAction(actionOddal);
        toolBar->addAction(actionZaznaczanie);
        toolBar->addAction(actionPrzesuwanieWidoku);
        toolBar->addAction(actionPrzesuwanie);
        toolBar->addAction(actionUsun);
        toolBar->addAction(actionDodaj);
        toolBar->addAction(actionEdycjaWierzcholkow);
        toolBar->addAction(actionDodajPunktRuchu);
        toolBar->addAction(actionLaczeniePunktowRuchu);

        retranslateUi(MainWindow);
        QObject::connect(actionWczytaj, SIGNAL(triggered()), MainWindow, SLOT(wczytajPlansze()));
        QObject::connect(actionNowaPlansza, SIGNAL(triggered()), MainWindow, SLOT(nowaPlansza()));
        QObject::connect(actionWyjdz, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(actionTlo, SIGNAL(triggered()), MainWindow, SLOT(wybierzTlo()));
        QObject::connect(actionUsun, SIGNAL(triggered()), MainWindow, SLOT(usunZaznaczony()));
        QObject::connect(actionZapisz, SIGNAL(triggered()), MainWindow, SLOT(zapiszPlansze()));
        QObject::connect(actionZapiszJako, SIGNAL(triggered()), MainWindow, SLOT(zapiszPlanszeJako()));
        QObject::connect(actionPrzybliz, SIGNAL(triggered()), MainWindow, SLOT(przyblizZoom()));
        QObject::connect(actionOddal, SIGNAL(triggered()), MainWindow, SLOT(oddalZoom()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "[*]BTEdytorMap", 0, QApplication::UnicodeUTF8));
        actionWczytaj->setText(QApplication::translate("MainWindow", "Wczytaj", 0, QApplication::UnicodeUTF8));
        actionWczytaj->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionWyjdz->setText(QApplication::translate("MainWindow", "Wyjd\305\272", 0, QApplication::UnicodeUTF8));
        actionWyjdz->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionZapisz->setText(QApplication::translate("MainWindow", "Zapisz", 0, QApplication::UnicodeUTF8));
        actionZapisz->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionNowaPlansza->setText(QApplication::translate("MainWindow", "Nowa plansza", 0, QApplication::UnicodeUTF8));
        actionNowaPlansza->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionDodaj->setText(QApplication::translate("MainWindow", "Dodaj przeszkod\304\231", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionDodaj->setToolTip(QApplication::translate("MainWindow", "Dodaj przeszkod\304\231", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionDodaj->setShortcut(QApplication::translate("MainWindow", "Ctrl+4", 0, QApplication::UnicodeUTF8));
        actionUsun->setText(QApplication::translate("MainWindow", "Usu\305\204 zaznaczone elementy", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionUsun->setToolTip(QApplication::translate("MainWindow", "Usu\305\204 zaznaczone elementy", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionUsun->setShortcut(QApplication::translate("MainWindow", "Del", 0, QApplication::UnicodeUTF8));
        actionTlo->setText(QApplication::translate("MainWindow", "T\305\202o", 0, QApplication::UnicodeUTF8));
        actionPrzybliz->setText(QApplication::translate("MainWindow", "Przybli\305\274", 0, QApplication::UnicodeUTF8));
        actionPrzybliz->setShortcut(QApplication::translate("MainWindow", "Ctrl++", 0, QApplication::UnicodeUTF8));
        actionOddal->setText(QApplication::translate("MainWindow", "Oddal", 0, QApplication::UnicodeUTF8));
        actionOddal->setShortcut(QApplication::translate("MainWindow", "Ctrl+-", 0, QApplication::UnicodeUTF8));
        actionPrzesuwanie->setText(QApplication::translate("MainWindow", "Przesuwanie element\303\263w", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPrzesuwanie->setToolTip(QApplication::translate("MainWindow", "Przesuwanie", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPrzesuwanie->setShortcut(QApplication::translate("MainWindow", "Ctrl+3", 0, QApplication::UnicodeUTF8));
        actionEdycjaWierzcholkow->setText(QApplication::translate("MainWindow", "Edycja wierzcho\305\202k\303\263w", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionEdycjaWierzcholkow->setToolTip(QApplication::translate("MainWindow", "Edycja wierzcho\305\202k\303\263w", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionEdycjaWierzcholkow->setShortcut(QApplication::translate("MainWindow", "Ctrl+5", 0, QApplication::UnicodeUTF8));
        actionZaznaczanie->setText(QApplication::translate("MainWindow", "Zaznaczanie", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionZaznaczanie->setToolTip(QApplication::translate("MainWindow", "Zaznaczanie", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionZaznaczanie->setShortcut(QApplication::translate("MainWindow", "Ctrl+1", 0, QApplication::UnicodeUTF8));
        actionPrzesuwanieWidoku->setText(QApplication::translate("MainWindow", "Przesuwanie widoku", 0, QApplication::UnicodeUTF8));
        actionPrzesuwanieWidoku->setShortcut(QApplication::translate("MainWindow", "Ctrl+2", 0, QApplication::UnicodeUTF8));
        actionZapiszJako->setText(QApplication::translate("MainWindow", "Zapisz jako...", 0, QApplication::UnicodeUTF8));
        actionOryginalnyRozmiar->setText(QApplication::translate("MainWindow", "Oryginalny rozmiar", 0, QApplication::UnicodeUTF8));
        actionOryginalnyRozmiar->setShortcut(QApplication::translate("MainWindow", "Ctrl+0", 0, QApplication::UnicodeUTF8));
        actionDodajPunktRuchu->setText(QApplication::translate("MainWindow", "Dodaj punkt ruchu", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionDodajPunktRuchu->setToolTip(QApplication::translate("MainWindow", "Dodaj punkt ruchu", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionDodajPunktRuchu->setShortcut(QApplication::translate("MainWindow", "Ctrl+6", 0, QApplication::UnicodeUTF8));
        actionLaczeniePunktowRuchu->setText(QApplication::translate("MainWindow", "\305\201\304\205czenie punkt\303\263w ruchu", 0, QApplication::UnicodeUTF8));
        actionLaczeniePunktowRuchu->setShortcut(QApplication::translate("MainWindow", "Ctrl+7", 0, QApplication::UnicodeUTF8));
        menuPlik->setTitle(QApplication::translate("MainWindow", "Plik", 0, QApplication::UnicodeUTF8));
        menuWidok->setTitle(QApplication::translate("MainWindow", "Widok", 0, QApplication::UnicodeUTF8));
        menuNarzedzia->setTitle(QApplication::translate("MainWindow", "Narz\304\231dzia", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        dockWidget->setWindowTitle(QApplication::translate("MainWindow", "Drzewo element\303\263w", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("MainWindow", "Plansza", 0, QApplication::UnicodeUTF8));
        treeWidget->setSortingEnabled(__sortingEnabled);

        dockWidget2->setWindowTitle(QApplication::translate("MainWindow", "Drzewo punkt\303\263w ruchu", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = treeWidget2->headerItem();
        ___qtreewidgetitem2->setText(0, QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled1 = treeWidget2->isSortingEnabled();
        treeWidget2->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem3 = treeWidget2->topLevelItem(0);
        ___qtreewidgetitem3->setText(0, QApplication::translate("MainWindow", "Plansza", 0, QApplication::UnicodeUTF8));
        treeWidget2->setSortingEnabled(__sortingEnabled1);

    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
