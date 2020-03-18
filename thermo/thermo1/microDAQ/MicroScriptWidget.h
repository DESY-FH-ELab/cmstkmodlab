#ifndef MICROSCRIPTWIDGET_H
#define MICROSCRIPTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPlainTextEdit>

#include "MicroScriptModel.h"

class MicroScriptEdit : public QPlainTextEdit {
public:
    MicroScriptEdit(QWidget* parent) : QPlainTextEdit(parent) { }
    virtual QSize sizeHint() const { return QSize(500, 400); }
};

class MicroMessageDisplay : public QPlainTextEdit {
public:
    MicroMessageDisplay(QWidget* parent) : QPlainTextEdit(parent) { }
    virtual QSize sizeHint() const { return QSize(500, 100); }
};

class MicroScriptWidget :
        public QWidget
{
    Q_OBJECT
public:
    explicit MicroScriptWidget(MicroScriptModel* scriptModel,
                               QWidget *parent = 0);

public slots:
    void clearMessageText();
    void appendMessageText( const QString & text );

protected:
    // Models
    MicroScriptModel* scriptModel_;

    // Widgets
    QPushButton* openScriptButton_;
    QPushButton* saveScriptButton_;
    QPushButton* saveAsScriptButton_;
    QPushButton* executeScriptButton_;
    QPushButton* abortScriptButton_;
    QWidget* buttons_;
    MicroScriptEdit* scriptEditor_;
    MicroMessageDisplay* messageDisplay_;

protected slots:
    void openScriptButtonClicked();
    void saveScriptButtonClicked();
    void saveAsScriptButtonClicked();
    void executeScriptButtonClicked();
    void abortScriptButtonClicked();

    void scriptChanged();
};

#endif // MICROSCRIPTWIDGET_H
