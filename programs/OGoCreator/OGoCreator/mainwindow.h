#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include "../OGoWidget/OGoWidget/ogographview.h"
#include "../../../opengt.so/Headers/opengt.h"
#include "../../../opengt.so/Headers/algorithms/clique/clique.h"
#include "../../../opengt.so/Headers/algorithms/color/color.h"
#include "../../../opengt.so/Headers/algorithms/independentset/independentset.h"
#include "../../../opengt.so/Headers/algorithms/path/path.h"
#include "../../../opengt.so/Headers/transform/springembedder3d.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionOpen_triggered(bool checked);

    void on_actionOpen_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void OpenGraphFile(QString filename);

    void on_actionNew_triggered();

    void on_actionClose_triggered();

    void on_actionSave_as_triggered();

    void on_actionSave_triggered();

    void on_actionBugtracker_triggered();

    void on_actionChat_triggered();

    void on_actionForum_triggered();

    void on_actionClique_triggered();

    void on_actionColoring_triggered();

    void on_actionPath_triggered();

    void on_actionIndependent_Set_triggered();

    void on_actionSpring_Embed_triggered();

    private:
    Ui::MainWindow *ui;

    void dragEnterEvent(QDragEnterEvent*);
    void dragMoveEvent(QDragMoveEvent*);
    void dragLeaveEvent(QDragLeaveEvent*);
    void dropEvent(QDropEvent*);

    void MakeTab(OpenGraphtheory::Graph*, QString, QString);
};

class RepaintOnChange : public OpenGraphtheory::IntermediateSteps::IntermediateStepHandler
{
    private:
        OGoGraphView* target;
    public:
        RepaintOnChange(OGoGraphView* target);
        void Handle(Graph* G);
};


#endif // MAINWINDOW_H
