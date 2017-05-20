#include "entrydialog.h"
#include "ui_entrydialog.h"
#include <QMessageBox>
#include <QTableView>
#include "plainvalue.h"

EntryDialog::EntryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EntryDialog), mEntry(nullptr)
{
    ui->setupUi(this);
    ui->tableView->setModel(&mValuesTableModel);
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

void EntryDialog::on_pushButton_clicked()
{
    PlainValue* pv = new PlainValue();
    pv->setName("aaa");
    pv->setValue("bbbb");
    mEntry->addValue(pv);
}