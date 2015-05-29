#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QDragEnterEvent>
#include <QAction>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QtWidgets/QMessageBox>
#include <QDropEvent>
#include "../OGoWidget/ogographview.h"
#include "../../../opengt.so/Headers/opengt.h"
#include "../../../opengt.so/Headers/algorithms/clique/clique.h"
#include "../../../opengt.so/Headers/algorithms/color/color.h"
#include "../../../opengt.so/Headers/logic/ModalLogic/ml.h"
#include "../../../opengt.so/Headers/logic/ComputationTreeLogic/ctl.h"
#include "../../../opengt.so/Headers/logic/FirstOrderLogic/fo.h"
#include "../../../opengt.so/Headers/algorithms/independentset/independentset.h"
#include "../../../opengt.so/Headers/algorithms/dominatingset/dominatingset.h"
#include "../../../opengt.so/Headers/algorithms/minimumcut/minimumcut.h"
#include "../../../opengt.so/Headers/algorithms/maximummatching/maximummatching.h"
#include "../../../opengt.so/Headers/algorithms/path/path.h"
#include "../../../opengt.so/Headers/algorithms/vertexcover/vertexcover.h"
#include "../../../opengt.so/Headers/algorithms/vertexseparator/vertexseparator.h"
#include "../../../opengt.so/Headers/algorithms/vertexdisjointpaths/vertexdisjointpaths.h"
#include "../../../opengt.so/Headers/transform/springembedder3d.h"
#include "../../../opengt.so/Headers/export/export.h"
#include "../../../opengt.so/Headers/import/import.h"
#include "../../../opengt.so/Headers/generate/generate.h"


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

    void on_actionForum_triggered();

    void on_actionClique_triggered();

    void on_actionColoring_triggered();

    void on_actionPath_triggered();

    void on_actionIndependent_Set_triggered();

    void on_actionSpring_Embed_triggered();

    void on_actionDominating_Set_triggered();

    void on_actionModal_Logic_triggered();

    void on_actionComputation_Tree_Logic_triggered();

    void on_actionFirst_Order_Predicate_Logic_triggered();

    void on_actionMinimum_Cut_triggered();

    void on_actionMaximum_Matching_triggered();

    void on_actionGenerate_triggered();

    void on_actionVertex_Cover_triggered();

    void on_actionDisjoint_Paths_triggered();

    void on_actionMinimum_Separator_triggered();

private:
    Ui::MainWindow *ui;

    void dragEnterEvent(QDragEnterEvent*);
    void dragMoveEvent(QDragMoveEvent*);
    void dragLeaveEvent(QDragLeaveEvent*);
    void dropEvent(QDropEvent*);

    void MakeTab(OpenGraphtheory::Graph*, QString, QString);
public:
    void DoGenerate(QString generator);
};

class RepaintOnChange : public OpenGraphtheory::IntermediateSteps::IntermediateStepHandler
{
    private:
        OGoGraphView* target;
    public:
        RepaintOnChange(OGoGraphView* target);
        void Handle(OpenGraphtheory::Graph* G);
};

class GeneratorEnumerator : public OpenGraphtheory::FactoryEnumerator
{
    private:
        QStringList stringlist;
    public:
        void Enumerate(std::string name, std::string description, std::string url);
        QStringList getStringList();
};

class GeneratorAction : public QAction
{
    Q_OBJECT
    private:
        QString gen;
        MainWindow* target;
    public:
        GeneratorAction(MainWindow* target, QString gen);
    public slots:
        void on_GeneratorAction_triggered();
};



#endif // MAINWINDOW_H
