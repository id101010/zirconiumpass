#include "entrydialog.h"
#include "ui_entrydialog.h"
#include <QMessageBox>
#include <QTableView>
#include "plainvalue.h"
#include "cryptedvalue.h"
#include <QMenu>

EntryDialog::EntryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EntryDialog), mEntry(nullptr)
{
    ui->setupUi(this);
    ui->tableView->setModel(&mValuesTableModel);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView,&QTableView::customContextMenuRequested,this,&EntryDialog::tableContextMenuRequested);
}

Entry *EntryDialog::entry()
{
    return mEntry;
}

void EntryDialog::setEntry(Entry *entry)
{
    mEntry = entry;
}

EntryDialog::~EntryDialog()
{
    delete ui;
}

void EntryDialog::accept()
{
    if(ui->leTitle->text().trimmed().isEmpty()) {
        QMessageBox::warning(this,"Empty Title", "Title cannot be empty");
        return;
    }
    mEntry->setTitle(ui->leTitle->text());

    QDialog::accept();
}


int EntryDialog::exec()
{
    if(mEntry == nullptr) {
        mEntry = new Entry();
        this->setWindowTitle("Create new Entry");
    } else {
        ui->leTitle->setText(mEntry->title());
        this->setWindowTitle("Edit entry");
    }
    mValuesTableModel.setEntry(mEntry);
    return QDialog::exec();
}



void EntryDialog::on_pbNewPlain_clicked()
{
    PlainValue* pv = new PlainValue();
    pv->setName(QString("plainvalue%1").arg(mEntry->values().count()+1));
    pv->setValue("Url or note");
    mEntry->addValue(pv);
    ui->tableView->edit(mValuesTableModel.index(mValuesTableModel.rowCount({})-1,1));
}

void EntryDialog::on_pbNewCrypted_clicked()
{
    CryptedValue* cv = new CryptedValue();
    cv->setName(QString("password%1").arg(mEntry->values().count()+1));
    mEntry->addValue(cv);
    ui->tableView->edit(mValuesTableModel.index(mValuesTableModel.rowCount({})-1,1));
}

void EntryDialog::tableContextMenuRequested(const QPoint &pos)
{
    QModelIndex mInd =  ui->tableView->indexAt(pos);
    if(!mInd.isValid())  {
        return;
    }
    AbstractValue* selectedValue = mInd.data(Qt::UserRole).value<AbstractValue*>();
    if(selectedValue == nullptr) {
        return;
    }

    QMenu m;
    QAction* removeAction = m.addAction("Remove Value");

    QAction* selectedAction = m.exec(ui->tableView->mapToGlobal(pos));
    if(selectedAction == removeAction) {
        mEntry->removeValue(selectedValue);
    }
}
