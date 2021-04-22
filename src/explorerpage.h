#ifndef EXPLORERPAGE_H
#define EXPLORERPAGE_H

#include <QWidget>

namespace Ui {
class ExplorerPage;
}

class ExplorerPage : public QWidget
{
    Q_OBJECT

public:
    explicit ExplorerPage(QWidget *parent = nullptr);
    ~ExplorerPage();

private:
    Ui::ExplorerPage *ui;
};

#endif // EXPLORERPAGE_H
