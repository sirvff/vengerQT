#ifndef DELEGATE_H
#define DELEGATE_H

#include <QMainWindow>
#include <QItemDelegate>
#include <QLineEdit>
#include <QFont>

class Delegate : public QItemDelegate
{
public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                      const QModelIndex & index) const
    {
        QLineEdit *lineEdit = new QLineEdit(parent);
        // Set validator
//        QIntValidator *validator = new QIntValidator(0, 999, lineEdit);
        QDoubleValidator *validator = new QDoubleValidator(0, 999, 2, lineEdit);
        lineEdit->setValidator(validator);
        lineEdit->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(16);
        lineEdit->setFont(font);
        return lineEdit;
    }
};

#endif // DELEGATE_H
