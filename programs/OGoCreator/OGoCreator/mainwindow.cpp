#include<QDragEnterEvent>
#include<QDragLeaveEvent>
#include<QDragMoveEvent>
#include<QDropEvent>
#include <QInputDialog>

#include <QDesktopServices>
#include <QUrl>
#include <QGraphicsView>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace OpenGraphtheory;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->setAcceptDrops(true);
    this->setAcceptDrops(true);

    QStringList cmdline_args = QCoreApplication::arguments();
    for(int i = 1; i < cmdline_args.count(); i++)
        OpenGraphFile(cmdline_args[i]);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MakeTab(Graph* graph, QString tabCaption, QString filelocation)
{
    OGoGraphView *gv = new OGoGraphView(this);
    gv->setFileLocation(filelocation);
    gv->setGraph(graph);
    gv->setBackground("/home/viktor/Downloads/springfield.jpg");
    int newTabIdx = ui->tabWidget->addTab(gv, tabCaption);
    ui->tabWidget->setCurrentIndex(newTabIdx);
}

void MainWindow::OpenGraphFile(QString filename)
{
    ui->statusBar->showMessage("Loading " + filename + "...");

    Graph* gr = new Graph;
    gr->LoadFromFile(string(filename.toUtf8().constData()));

    QFileInfo fileinfo(filename);
    MakeTab(gr, fileinfo.baseName(), filename);

    ui->statusBar->showMessage("");
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dlg(this);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setDefaultSuffix("gxl");
    dlg.setFileMode(QFileDialog::ExistingFiles);
    dlg.setFilter("Graph eXchange Language Files (*.gxl)");

    if(!dlg.exec())
        return;

    QStringList files = dlg.selectedFiles();
    for(int i = 0; i < files.count(); i++)
    {
        OpenGraphFile(files[i]);
    }
}

void MainWindow::on_actionOpen_triggered(bool)
{

}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_actionNew_triggered()
{
    MakeTab(new Graph(), "New", "");
}

void MainWindow::on_actionClose_triggered()
{
    on_tabWidget_tabCloseRequested(ui->tabWidget->currentIndex());
}

void MainWindow::on_actionSave_as_triggered()
{
    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());
    QString file = gv->getFileLocation();

    QFileDialog dlg(this);
    dlg.selectFile(file);
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setDefaultSuffix("gxl");
    dlg.setFilter("Graph eXchange Language Files (*.gxl)");

    if(!dlg.exec())
        return;

    QStringList selectedfiles = dlg.selectedFiles();
    if(selectedfiles.count() == 1)
    {
        QFileInfo fileinfo(selectedfiles[0]);
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), fileinfo.baseName());

        gv->setFileLocation(selectedfiles[0]);
        on_actionSave_triggered();
    }
}

void MainWindow::on_actionSave_triggered()
{
    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());
    QString file = gv->getFileLocation();
    if(file == "")
    {
        on_actionSave_as_triggered();
        return;
    }

    gv->getGraph()->SaveToFile(string(file.toUtf8().constData()));
}

void MainWindow::on_actionBugtracker_triggered()
{
    QDesktopServices::openUrl((QUrl)("http://sourceforge.net/tracker/?group_id=340258"));
}

void MainWindow::on_actionChat_triggered()
{
    QDesktopServices::openUrl((QUrl)("http://open-graphtheory.org/tinc?key=Qx2yoLKr&channel=266178"));
}

void MainWindow::on_actionForum_triggered()
{
    QDesktopServices::openUrl((QUrl)("http://sourceforge.net/projects/opengraphtheory/forums"));
}




void MainWindow::dragEnterEvent(QDragEnterEvent* dragenterevent)
{
    dragenterevent->setAccepted(  dragenterevent->mimeData()->hasUrls() );
}

void MainWindow::dragMoveEvent(QDragMoveEvent* )
{

}

void MainWindow::dragLeaveEvent(QDragLeaveEvent* )
{

}

void MainWindow::dropEvent(QDropEvent* dropevent)
{
    if(dropevent->mimeData()->hasUrls())
        for(int i = 0; i < dropevent->mimeData()->urls().count(); i++)
            OpenGraphFile(dropevent->mimeData()->urls()[i].path());
}






void MainWindow::on_actionClique_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Clique Algorithm", "Enter a name for the Clique",
                   QLineEdit::Normal, "Clique", &ok );
    if ( !ok || text.isEmpty() )
        return;

    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());

    OpenGraphtheory::Algorithms::AlgorithmCLIQUE algo;
    algo.AddClique(*(gv->getGraph()), text.toUtf8().constData());

    gv->setVertexColoring(text.toUtf8().constData());
    gv->resetEdgeColoring();
}

void MainWindow::on_actionColoring_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Coloring Algorithm", "Enter a name for the Coloring",
                   QLineEdit::Normal, "Coloring", &ok );
    if ( !ok || text.isEmpty() )
        return;

    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());

    OpenGraphtheory::Algorithms::AlgorithmCOLORING algo;
    algo.AddColoring(*(gv->getGraph()), text.toUtf8().constData());

    gv->setVertexColoring(text.toUtf8().constData());
    gv->resetEdgeColoring();
}

void PathVertexSelectionFinished(QWidget* mainwindow, OGoGraphView* gv, vector<Graph::VertexIterator> selectedvertices, Graph* G)
{
    bool ok;
    QString text = QInputDialog::getText(mainwindow, "Routing Algorithm", "Enter a name for the Path",
                   QLineEdit::Normal, "Path", &ok );
    if ( !ok || text.isEmpty() )
        return;

    OpenGraphtheory::Algorithms::AlgorithmPATH algo;
    algo.AddPath(*G, selectedvertices[0], selectedvertices[1], text.toUtf8().constData());
    gv->setEdgeColoring(text.toUtf8().constData());
    gv->resetVertexColoring();
    gv->repaint();
}


void MainWindow::on_actionPath_triggered()
{
    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());

    gv->selectVertices(this, 2, PathVertexSelectionFinished);
}


void MainWindow::on_actionIndependent_Set_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Independent Set Algorithm", "Enter a name for the Independent Set",
                   QLineEdit::Normal, "IndependentSet", &ok );
    if ( !ok || text.isEmpty() )
        return;

    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());

    OpenGraphtheory::Algorithms::AlgorithmINDEPENDENTSET algo;
    algo.AddIndependentSet(*(gv->getGraph()), text.toUtf8().constData());

    gv->setVertexColoring(text.toUtf8().constData());
    gv->resetEdgeColoring();
}

void MainWindow::on_actionDominating_Set_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Dominating Set Algorithm", "Enter a name for the Dominating Set",
                   QLineEdit::Normal, "DominatingSet", &ok );
    if ( !ok || text.isEmpty() )
        return;

    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());

    OpenGraphtheory::Algorithms::AlgorithmDOMINATINGSET algo;
    algo.AddDominatingSet(*(gv->getGraph()), text.toUtf8().constData());

    gv->setVertexColoring(text.toUtf8().constData());
    gv->resetEdgeColoring();
}



RepaintOnChange::RepaintOnChange(OGoGraphView *target) : IntermediateStepHandler()
{
    this->target = target;
}

void RepaintOnChange::Handle(Graph* G)
{
    target->repaint();
}

void MainWindow::on_actionSpring_Embed_triggered()
{
    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());

    OpenGraphtheory::Transform::TransformerSPRINGEMBEDDER3D algo;
    RepaintOnChange *foo = new RepaintOnChange(gv);
    vector<float> params;
    //params.push_back();
    //params.push_back();
    algo.DoTransform(*(gv->getGraph()), params, foo);
    delete foo;
}


