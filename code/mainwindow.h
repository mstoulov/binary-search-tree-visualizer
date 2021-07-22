#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItemGroup>
#include <QMouseEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSlider>
#include <QString>
#include <QColor>
#include <QPen>
#include <QFont>
#include <avl.h>
#include <rbt.h>
#include <string>
#include <set>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MyEllipse : public QGraphicsEllipseItem {
    ~MyEllipse() {}
 public:
    void setText(QGraphicsTextItem* t) { text = t; }
    QGraphicsTextItem* text;
};

class MyGraphicsView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_add_pushButton_clicked();

    void on_add_rand_pushButton_clicked();

    void on_remove_pushButton_clicked();

    void on_remove_rand_pushButton_clicked();

    void on_trees_comboBox_currentTextChanged(const QString &arg1);

    void on_scale_horizontalSlider_valueChanged(int value);

public:
    Ui::MainWindow *ui;
    MyGraphicsView* mygraphicsview;
    QPushButton* add_pushButton, *add_rand_pushButton, *remove_pushButton, *remove_rand_pushButton;
    QLineEdit* add_lineEdit, *add_rand_lineEdit, *remove_lineEdit, *remove_rand_lineEdit;
    QComboBox* trees_comboBox;
    QSlider* scale_horizontalSlider;
    int prev_scale_val;
};

class MyGraphicsView : public QGraphicsView {
    Q_OBJECT
 public:
    void add_to_trees(int num) {
        insert(rbt, num);
        insert(avl, num);
        tree_set.insert(num);
    }
    void erase_from_trees(int num) {
        erase(rbt, num);
        erase(avl, num);
        tree_set.erase(num);
    }
    void setup(QWidget* parent, MainWindow* ui_) {
        this->setParent(parent);
        scene = new QGraphicsScene;
        this->setScene(scene);
        this->setGeometry(23, 20, 1027, 802);
        scene->setSceneRect(0, 0, 1025 , 800);
        ui = ui_;
        avl = nullptr;
        rbt = nullptr;
    }
    void mousePressEvent(QMouseEvent *event) {
        MyEllipse* el = dynamic_cast<MyEllipse*>(itemAt(event->pos()));
        if (event->button() == Qt::RightButton || !el) return;
        erase_from_trees(el->text->toPlainText().toInt());
        std::cout << "yohoho\n";
        ui->scale_horizontalSlider->setValue(0);
        draw();
    }
    void draw() {
        foreach(QGraphicsItem* item, scene->items()) {
            scene->removeItem(item);
            delete item;
        }
        if (!avl) return;
        QString tree_ss = ui->trees_comboBox->currentText();
        int tree_height = tree_ss == "AVL" ? avl->height() : rbt->height();
        double el_w, el_h, l, r, y_cord, y_step;
        el_w = 1024 / (pow(2, tree_height - 1) + 5);
        el_h = el_w / 2;
        l = 0;
        r = 1024;
        y_step = 800.0 / (tree_height + 1);
        y_cord = y_step;
        el_w = 100;
        el_h = 50;
        y_step = 150;
        r = 100 * (pow(2, tree_height - 1) + 1);
        y_cord = y_step;
        scene->setSceneRect(0, 0, r + 1, y_step * (tree_height + 2));
        this->fitInView(scene->sceneRect(), Qt::AspectRatioMode::KeepAspectRatio);
        if (ui->trees_comboBox->currentText() == "AVL") {
            draw_avl(avl, el_w, el_h, l, r, y_cord, y_step);
        } else {
            draw_rbt(rbt, el_w, el_h, l, r, y_cord, y_step);
        }
    }
    void draw_rbt(RBT* p, double el_w, double el_h, double l, double r, double y_cord, double y_step) {
        MyEllipse* el = new MyEllipse;
        double x_cord = (l + r) / 2.0 - el_w / 2.0;
        el->setRect(x_cord, y_cord, el_w, el_h);
        if (p->red()) el->setPen(QPen(QColor(Qt::GlobalColor::red)));
        QString text_str = QString::number(p->elem());
        QGraphicsTextItem* text = new QGraphicsTextItem(text_str);
        int font_num = max((int)(el_w / 12.5), 1);
        double x_t_move = (5.5 - text_str.size() / 2.0) * el_w / 10;
        double y_t_move = -0.25 * el_h;
        text->setFont(QFont("kek", font_num));
        text->setPos(x_cord + x_t_move, y_cord + y_t_move + el_h / 2);
        el->setText(text);
        scene->addItem(text);
        scene->addItem(el);
        if (p->left) {
            QGraphicsLineItem* line = new QGraphicsLineItem((l + r) / 2.0, y_cord + el_h, (l + r) / 2.0 - (r - l) / 4.0, y_cord + y_step);
            draw_rbt(p->left, el_w, el_h, l, (l + r) / 2, y_cord + y_step, y_step);
            scene->addItem(line);
        }
        if (p->right) {
            QGraphicsLineItem* line = new QGraphicsLineItem((l + r) / 2.0, y_cord + el_h, (l + r) / 2.0 + (r - l) / 4.0, y_cord + y_step);
            draw_rbt(p->right, el_w, el_h, (l + r) / 2, r, y_cord + y_step, y_step);
            scene->addItem(line);
        }
        ui->prev_scale_val = 0;
    }
    void draw_avl(AVL* p, double el_w, double el_h, double l, double r, double y_cord, double y_step) {
        MyEllipse* el = new MyEllipse;
        double x_cord = (l + r) / 2.0 - el_w / 2.0;
        el->setRect(x_cord, y_cord, el_w, el_h);
        QString text_str = QString::number(p->elem());
        QGraphicsTextItem* text = new QGraphicsTextItem(text_str);
        int font_num = max((int)(el_w / 12.5), 1);
        double x_t_move = (5.5 - text_str.size() / 2.0) * el_w / 10;
        double y_t_move = -0.25 * el_h;
        text->setFont(QFont("kek", font_num));
        text->setPos(x_cord + x_t_move, y_cord + y_t_move + el_h / 2);
        el->setText(text);
        scene->addItem(text);
        scene->addItem(el);
        if (p->left) {
            QGraphicsLineItem* line = new QGraphicsLineItem((l + r) / 2.0, y_cord + el_h, (l + r) / 2.0 - (r - l) / 4.0, y_cord + y_step);
            draw_avl(p->left, el_w, el_h, l, (l + r) / 2, y_cord + y_step, y_step);
            scene->addItem(line);
        }
        if (p->right) {
            QGraphicsLineItem* line = new QGraphicsLineItem((l + r) / 2.0, y_cord + el_h, (l + r) / 2.0 + (r - l) / 4.0, y_cord + y_step);
            draw_avl(p->right, el_w, el_h, (l + r) / 2, r, y_cord + y_step, y_step);
            scene->addItem(line);
        }
    }

public:
    QGraphicsScene* scene;
    RBT* rbt;
    AVL* avl;
    std::set<int> tree_set;
    MainWindow *ui;
};



#endif // MAINWINDOW_H
