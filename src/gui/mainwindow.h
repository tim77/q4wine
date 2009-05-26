/***************************************************************************
 *   Copyright (C) 2008 by Malakhov Alexey                                 *
 *   brezerk@gmail.com                                                     *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 *   In addition, as a special exception, the copyright holders give       *
 *   permission to link the code of this program with any edition of       *
 *   the Qt library by Trolltech AS, Norway (or with modified versions     *
 *   of Qt that use the same license as Qt), and distribute linked         *
 *   combinations including the two.  You must obey the GNU General        *
 *   Public License in all respects for all of the code used other than    *
 *   Qt.  If you modify this file, you may extend this exception to        *
 *   your version of the file, but you are not obligated to do so.  If     *
 *   you do not wish to do so, delete this exception statement from        *
 *   your version.                                                         *
 ***************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QObject>
#include <QSystemTrayIcon>
#include <QNetworkProxy>
#include <QWidget>
#include <QDir>
#include <QSizePolicy>
#include <QtGui>
#include <QSqlError>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlRelation>
#include <QSqlRelationalTableModel>

#include <QTimer>
#include <QTableWidget>
#include <QTabWidget>
#include <QLabel>
#include <QString>
#include <QMessageBox>
#include <QChar>
#include <QSize>
#include <QToolBar>
#include <QIcon>
#include <QTreeWidgetItem>
#include <QGroupBox>


#include <ui_MainWindow.h>

#include "config.h"

#include "wisitem.h"
#include "coremethods.h"
#include "iconsview.h"
#include "wizard.h"
#include "process.h"
#include "winebinlauncher.h"
#include "iconsettings.h"
#include "imagemanager.h"
#include "prefixsettings.h"
#include "about.h"
#include "appsettings.h"
#include "run.h"
#include "temporary/winetricks.h"
#include <stdlib.h>
#include <unistd.h>

#include <q4wine-lib/main.h>

// Experemental FreeBSD support
#ifdef _OS_FREEBSD_
	#include <kvm.h>
	#include <sys/param.h>
	#include <sys/user.h>
	#include <sys/file.h>
	#include <sys/sysctl.h>
	#include <limits.h>
	#include <paths.h>
#endif

// Experemental darwin support (need testing)
#ifdef _OS_DARWIN_
	#include <kvm.h>
	#include <sys/param.h>
	#include <sys/user.h>
	#include <sys/file.h>
	#include <sys/sysctl.h>
#endif

class QAction;
class QMenu;
class QTextEdit;

struct iconCopyBuffer {
	QString dir_id;
	QString prefix_id;
	bool move;
	QStringList names;
};


class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
	public:
		MainWindow(QWidget * parent = 0, Qt::WFlags f = 0);
		// Icon copy\cyt structure
		iconCopyBuffer iconBuffer;

	private slots:

		//Core functions
		void CoreFunction_GetProcProccessInfo(void);
		void CoreFunction_SetProcNicePriority(int priority, int pid);
		void CoreFunction_ResizeContent(int tabIndex);

		void menuMountImages_triggered ( QAction * action );

		/*
		 * Icon tray slots
		 */
		void trayIcon_Activate(QSystemTrayIcon::ActivationReason reason);

		/*
		 * Command buttons slots
		 */
		void cmdManagePrefixes_Click(void);
		void cmdCreateFake_Click(void);
		void cmdUpdateFake_Click(void);
		void cmdWinetricks_Click (void);
		void cmdTestWis_Click (void);

		/*
		 *Prefix tool bars action
		 */
		void prefixAdd_Click(void);
		void prefixDelete_Click(void);
		void prefixExport_Click(void);
		void prefixImport_Click(void);
		void prefixSettings_Click(void);

		/*
		 *Process action slots
		 */
		void processRenice_Click(void);
		void processKillSelected_Click(void);
		void processKillWine_Click(void);

		/*
		 *Icon list slots
		 */
		void lstIcons_ItemClick(QListWidgetItem * item);
		void lstIcons_ItemDoubleClick(QListWidgetItem * item);
		void lstIcons_ShowContextMenu(const QPoint);

		/*
		 *Programs tree slots
		 */
		void twPrograms_ItemClick(QTreeWidgetItem * item, int);
		void twPrograms_ShowContextMenu(const QPoint);

		/*
		 *Prefix table slots
		 */
		void tablePrefix_ShowContextMenu(const QPoint);
		void tablePrefix_UpdateContentList(const QModelIndex);

		void tableProc_ShowContextMenu(const QPoint);
		void tableProc_UpdateContentList(const QModelIndex);

		/*
		 * Context menu slots
		 */
			//Directory context
			void dirAdd_Click(void);
			void dirRename_Click(void);
			void dirDelete_Click(void);
			void dirUnmount_Click(void);
			void dirMountOther_Click(void);
			void dirConfigure_Click(void);
			void dirInstall_Click(void);
			void dirUninstall_Click(void);

			//Icon context
			void iconAdd_Click(void);
			void iconRun_Click(void);
			void iconCut_Click(void);
			void iconCopy_Click(void);
			void iconPaste_Click(void);
			void iconRename_Click(void);
			void iconOption_Click(void);
			void iconDelete_Click(void);
			void iconMount_Click(void);
			void iconMountOther_Click(void);
			void iconUnmount_Click(void);

			//Main menu slots
			void mainExit_Click(void);
			void mainPrograms_Click(void);
			void mainImageManager_Click(void);
			void mainProcess_Click(void);
			void mainSetup_Click(void);
			void mainPrefix_Click(void);
			void mainAbout_Click(void);
			void mainAboutQt_Click(void);
			void mainExportIcons_Click(void);
			void mainRun_Click(void);
			void mainOptions_Click(void);
			void mainInstall_Click(void);
	private:

                //! This is need for libq4wine-core.so import;
                typedef void *CoreLibPrototype ();
                CoreLibPrototype *CoreLibClassPointer;
                corelib *CoreLib;

		//Classes
		CoreMethods *core;
                QLibrary libq4wine;

		// Proxy
		QNetworkProxy proxy;
		// Tray icon
		QSystemTrayIcon *trayIcon;

		QMenu *trayIconMenu;
		QMenu *images;
		QMenu *menuProc;
		QMenu *menuPrefix;
		QMenu *menuDir;
		QMenu *menuDirMount;
		QMenu *menuIcon;
		QMenu *menuIconMount;

		void createTrayIcon();
		void SetMeVisible(bool visible);

		QAction *processKillSelected;
		QAction *processKillWine;
		QAction *processRefresh;
		QAction *processRenice;

		// Prefix actions for context menu
		QAction *prefixAdd;
		QAction *prefixImport;
		QAction *prefixExport;
		QAction *prefixDelete;
		QAction *prefixSettings;

		// Directories control for context menu
		QAction *dirAdd;
		QAction *dirRun;
		QAction *dirRename;
		QAction *dirDelete;
		QAction *dirMountOther;
		QAction *dirUnmount;
		QAction *dirConfigure;
		QAction *dirInstall;
		QAction *dirUninstall;

		// Icons control for context menu
		QAction *iconRun;
		QAction *iconAdd;
		QAction *iconRename;
		QAction *iconDelete;
		QAction *iconOptions;
		QAction *iconCopy;
		QAction *iconCut;
		QAction *iconPaste;
		QAction *iconMount;
		QAction *iconUnmount;
		QAction *iconMountOther;

		// Toolbars
		QToolBar *procToolBar;
		QToolBar *prefixToolBar;

		// Settings functions
		void CoreFunction_CreateMenus(void);
		void CoreFunction_CreateToolBars(void);
		void CoreFunction_SettingGet(void);
		void CoreFunction_SettingCheck(QString filePath, QString message);
		void CoreFunction_WineRunAutorunItems(void);
		void CoreFunction_WineRunProgram(QString exec, QStringList args, QString wrkdir);
		void CoreFunction_WinePrepareRunParams(ExecObject execObj);
		void CoreFunction_DatabaseUpdateConnectedItems(int currentPrefix = -1);
		void CoreFunction_ImageMount(QString image, QString mount);
		void CoreFunction_ImageUnmount(QString mount);

		//Events definition
		void resizeEvent (QResizeEvent);

		//void DeleteDirectory(QString path);

		QStringList SQL_getPrefixAndDirData(QTreeWidgetItem *treeItem);
		QStringList SQL_getPrefixAndDirInfo(QTreeWidgetItem *treeItem);
		QStringList SQL_getDirctoryInfo(QString prefix_name, QString dirname);
		bool SQL_isIconExistsByName(QString prefix_id, QString dir_id, QString name);

	protected:
		// Event filter
		bool eventFilter(QObject *obj, QEvent *event);
		bool run_timer;
		void closeEvent(QCloseEvent *event);

		//Resource\theme loader
		QIcon CoreFunction_IconLoad(QString iconName);

		QString HOME_PATH;
		QString ROOT_PATH;
		QString TEMP_PATH;
		QString PREFIX_EI_PATH;

		QString WINE_DEFAULT_PREFIX;
		QString DEFAULT_WINE_BIN, DEFAULT_WINE_SERVER, DEFAULT_WINE_LOADER, DEFAULT_WINE_LIBS;
		QString WRESTOOL_BIN, ICOTOOL_BIN;
		QString TAR_BIN, MOUNT_BIN, UMOUNT_BIN, SUDO_BIN, GUI_SUDO_BIN, NICE_BIN, RENICE_BIN, SH_BIN;
		QString CONSOLE_BIN, CONSOLE_ARGS;
		QString THEME_NAME;
		bool SHOW_TRAREY_ICON;

};

#endif


