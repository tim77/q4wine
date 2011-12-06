/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010, 2011 by Malakhov Alexey                                 *
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
 ***************************************************************************/
#include "imagemanager.h"

ImageManager::ImageManager(QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	//setModal(true);
	setupUi(this);

	// Loading libq4wine-core.so
#ifdef RELEASE
    libq4wine.setFileName(_CORELIB_PATH_);
#else
    libq4wine.setFileName("../q4wine-lib/libq4wine-core");
#endif
	if (!libq4wine.load()){
		libq4wine.load();
	}

	// Getting corelib calss pointer
	CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	CoreLib.reset((corelib *)CoreLibClassPointer(true));

	loadThemeIcons();

	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
	connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));
	connect(tableImage, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(tableImage_showContextMenu(const QPoint &)));
	connect(tableImage, SIGNAL(clicked(const QModelIndex &)), this, SLOT(update_lblPathInfo(const QModelIndex &)));

	getCDImages();

	cmdOk->setFocus(Qt::ActiveWindowFocusReason);
	return;
}

void ImageManager::update_lblPathInfo(const QModelIndex){

	if (tableImage->currentRow()<0)
		return;
	lblPathInfo->setText(tr("File path: %1").arg(db_image.getPath(tableImage->item(tableImage->currentRow(), 0)->text())));

	return;
}

void ImageManager::tableImage_showContextMenu(const QPoint){
	QMenu menu(this);
	menu.addAction(actionAdd.get());
	menu.addAction(actionRename.get());
	menu.addSeparator();
	menu.addAction(actionRefresh.get());
	menu.addSeparator();
	if (tableImage->currentRow()>=0){
		actionRemove->setEnabled(TRUE);
	} else {
		actionRemove->setEnabled(FALSE);
	}
	menu.addAction(actionRemove.get());
	menu.exec(QCursor::pos());
	return;
}


void ImageManager::getCDImages(void){
	qint64 size = 0;
	QList<QStringList> result = db_image.getFields();

	int numRows = tableImage->rowCount();
	int curRows = 0;

	for (int i = 0; i < result.size(); ++i) {
		curRows++;

		if (curRows>numRows){
			tableImage->insertRow (numRows);
			numRows = tableImage->rowCount();
		}

		QFile file (result.at(i).at(1));
		if (file.exists ()){
			size = file.size() / 1024 / 1024;
		} else {
			size = 0;
		}

		if (tableImage->item(curRows - 1, 0)){
			tableImage->item(curRows - 1, 0)->setText(result.at(i).at(0));
			tableImage->item(curRows - 1, 1)->setText(tr("%1 Mb").arg(size));
			if (!file.exists ()){
				tableImage->item(curRows - 1, 0)->setIcon (QIcon(":/data/important.png"));
			} else {
				tableImage->item(curRows - 1, 0)->setIcon (QIcon());
			}
		} else {
			std::auto_ptr<QTableWidgetItem> newItem (new QTableWidgetItem(result.at(i).at(0)));
			if (!file.exists ()){
				newItem->setIcon ( QIcon(":/data/important.png") );
			} else {
				newItem->setIcon ( QIcon() );
			}
			newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
			tableImage->setItem(curRows - 1, 0, newItem.release());
			newItem.reset(new QTableWidgetItem(tr("%1 Mb").arg(size)));
			newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
			tableImage->setItem(curRows - 1, 1, newItem.release());
		}
	}

	numRows = tableImage->rowCount();

	if (numRows > curRows)
		for (int i=curRows; i <= numRows; i++)
			tableImage->removeRow(curRows);

	tableImage->resizeRowsToContents();
	tableImage->resizeColumnsToContents();
	tableImage->horizontalHeader()->setStretchLastSection(TRUE);

	return;
}

void ImageManager::loadThemeIcons(){

	lblLogo->setPixmap(CoreLib->loadPixmap("data/iso_manager.png"));

	std::auto_ptr<QToolBar> managerToolBar (new QToolBar(tlbManager));


	actionAdd.reset(managerToolBar->addAction (CoreLib->loadIcon("data/add.png"), tr("Add image")));
	connect(actionAdd.get(), SIGNAL(triggered()), this, SLOT(actionAddImage()));

	actionRename.reset(managerToolBar->addAction (CoreLib->loadIcon("data/configure.png"), tr("Rename image")));
	connect(actionRename.get(), SIGNAL(triggered()), this, SLOT(actionRenameImage()));

	managerToolBar->addSeparator ();

	actionRemove.reset(managerToolBar->addAction (CoreLib->loadIcon("data/remove.png"), tr("Remove image")));
	connect(actionRemove.get(), SIGNAL(triggered()), this, SLOT(actionRemoveImage()));

	managerToolBar->addSeparator ();

	actionRefresh.reset(managerToolBar->addAction (CoreLib->loadIcon("data/reload.png"), tr("Refresh image list")));
	connect(actionRefresh.get(), SIGNAL(triggered()), this, SLOT(actionRefreshImageList()));

	std::auto_ptr<QBoxLayout> layout (new QBoxLayout(QBoxLayout::TopToBottom));
	layout->addWidget(managerToolBar.release());
	layout->setMargin(0);
	layout->setSpacing(0);

	tlbManager->setLayout(layout.release());

	return;
}

void ImageManager::cmdOk_Click(){
	accept();
	return;
}


void ImageManager::actionAddImage(){
	bool ok;
    QString fileName, newName, fileFilter;

#ifdef _OS_LINUX_
    fileFilter = tr("Disc image files (*.iso *.nrg *.img *.bin *.mdf)");
#endif
#ifdef _OS_FREEBSD_
    fileFilter =  tr("ISO image files (*.iso)");
#endif

#if QT_VERSION >= 0x040500
    QFileDialog::Options options;

    if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0)
        options = QFileDialog::DontUseNativeDialog;

    fileName = QFileDialog::getOpenFileName(this, tr("Open ISO image file"), QDir::homePath(), fileFilter, 0, options);
#else
    fileName = QFileDialog::getOpenFileName(this, tr("Open ISO image file"), QDir::homePath(), fileFilter);
#endif



	if(!fileName.isEmpty()){
		newName = fileName.split("/").last();
		while (db_image.isExistsByName(newName)){
			newName = QInputDialog::getText(this, tr("Sorry. It seems CD iamge already exists."), tr("Sorry. It seems CD image file already exists.<br>Please rename it, or cancel add image operation."), QLineEdit::Normal, fileName.split("/").last() , &ok);
			if (!ok){
				return;
			}
		}
		if (!db_image.addImage(newName, fileName))
			return;
		actionRefreshImageList();
	}
	return;
}

void ImageManager::actionRenameImage(){
	if (tableImage->currentRow()==-1)
		return;

	bool ok=false;
	QString newName = QInputDialog::getText(this, tr("Enter new name"), tr("Enter new name:"), QLineEdit::Normal, tableImage->item(tableImage->currentRow(), 0)->text(), &ok);
	while (db_image.isExistsByName(newName) or (newName.isEmpty())){
		if (!ok){
			return;
		}
		if (newName.isEmpty()){
			newName = QInputDialog::getText(this, tr("Sorry. CD iamge name can not be empty."), tr("Sorry. CD iamge name can not be empty.<br>Please enter new name, or cancel rename image operation."), QLineEdit::Normal, newName, &ok);
		} else {
			newName = QInputDialog::getText(this, tr("Sorry. It seems CD iamge already exists."), tr("Sorry. It seems CD image file already exists.<br>Please rename it, or cancel rename image operation."), QLineEdit::Normal, newName, &ok);
		}
	}
	if (!db_image.renameImage(newName, tableImage->item(tableImage->currentRow(), 0)->text()))
		return;
	actionRefreshImageList();
	return;
}

void ImageManager::actionRemoveImage(){
	if (tableImage->currentRow()==-1)
		return;
	if (!db_image.delImage(tableImage->item(tableImage->currentRow(), 0)->text()))
		return;
	actionRefreshImageList();
	return;
}

void ImageManager::actionRefreshImageList(){
	getCDImages();
	return;
}

void ImageManager::cmdHelp_Click(){
	CoreLib->openHelpUrl("09-image-manager.html");
}

