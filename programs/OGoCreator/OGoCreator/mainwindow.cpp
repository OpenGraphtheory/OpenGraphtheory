#include<QDragEnterEvent>
#include<QDragLeaveEvent>
#include<QDragMoveEvent>
#include<QDropEvent>
#include <QtWidgets/QInputDialog>

#include <QDesktopServices>
#include <QUrl>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QFileDialog>
#include <QMimeData>

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace OpenGraphtheory;
using namespace std;

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
    int newTabIdx = ui->tabWidget->addTab(gv, tabCaption);
    ui->tabWidget->setCurrentIndex(newTabIdx);
}

void MainWindow::OpenGraphFile(QString filename)
{
    ui->statusBar->showMessage("Loading " + filename + "...");
    QFileInfo fileinfo(filename);
    QString suffix = fileinfo.suffix().toLower();

    Graph* gr = new Graph;
    if(suffix == "gxl")
    {
        gr->LoadFromFile(string(filename.toUtf8().constData()));
    }
    else
    {
        (*gr) = Import::ImportFilter::Import(filename.toUtf8().constData(), suffix.toUtf8().constData());
    }

    // make sure every vertex has 2 coordinates (otherwise, the paint event would crash)
    for(Graph::VertexIterator v = gr->BeginVertices(); v != gr->EndVertices(); v++)
    {
        vector<float> Coordinates = v.GetCoordinates();
        if(Coordinates.size() < 2)
        {
            while(Coordinates.size() < 2)
                Coordinates.push_back(0);
           v.SetCoordinates(Coordinates);
        }
    }

    MakeTab(gr, fileinfo.baseName(), filename);
    ui->statusBar->showMessage("");
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dlg(this);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setDefaultSuffix("gxl");
    dlg.setFileMode(QFileDialog::ExistingFiles);

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




class ExportFilterEnumerator : public FactoryEnumerator
{
    private:
        QFileDialog* filedialog;
        QStringList stringlist;
    public:
        ExportFilterEnumerator(QFileDialog* target);
        void Enumerate(std::string name, std::string description, std::string url);
        QStringList getStringList();
};
ExportFilterEnumerator::ExportFilterEnumerator(QFileDialog* target)
{
    stringlist.append("Graph eXchange Language (*.gxl) (*.gxl)");
    filedialog = target;
}
QStringList ExportFilterEnumerator::getStringList()
{
    return stringlist;
}
void ExportFilterEnumerator::Enumerate(std::string name, std::string description, std::string)
{
    stringlist.append(QString((description+" (*." + name+") (*."+name+")").c_str()));
}




void MainWindow::on_actionSave_as_triggered()
{
    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());
    QString file = gv->getFileLocation();

    QFileDialog dlg(this);
    dlg.selectFile(file);
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setDefaultSuffix("gxl");
    ExportFilterEnumerator expfilters(&dlg);
    OpenGraphtheory::Export::ExportFilter::ExportFilterFactory.Enumerate(&expfilters);
    //dlg.setFilters(expfilters.getStringList());

    if(!dlg.exec())
        return;

    QStringList selectedfiles = dlg.selectedFiles();
    if(selectedfiles.count() == 1)
    {
        QFileInfo fileinfo(selectedfiles[0]);
        if(fileinfo.completeSuffix().toUpper() == "GXL")
        {
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), fileinfo.baseName());
            gv->setFileLocation(selectedfiles[0]);
            on_actionSave_triggered();
        }
        else
        {
            OpenGraphtheory::Export::ExportFilter::Export(*(gv->getGraph()),selectedfiles[0].toStdString(),
                                                          fileinfo.completeSuffix().toStdString(),
                                                          gv->getVertexColoring(),gv->getEdgeColoring(),72);
        }
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


// ===================================================================================================================================================


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


// ===================================================================================================================================================


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


// ===================================================================================================================================================


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


// ===================================================================================================================================================


void MinCutVertexSelectionFinished(QWidget* mainwindow, OGoGraphView* gv, vector<Graph::VertexIterator> selectedvertices, Graph* G)
{
    bool ok;
    QString text = QInputDialog::getText(mainwindow, "Minimum Cut Algorithm", "Enter a name for the Cut",
                   QLineEdit::Normal, "Cut", &ok );
    if ( !ok || text.isEmpty() )
        return;

    OpenGraphtheory::Algorithms::AlgorithmMINIMUMCUT algo;
    map<Graph::EdgeIterator, float> capacities;
    for(Graph::EdgeIterator e = G->BeginEdges(); e != G->EndEdges(); e++)
        capacities.insert(std::pair<Graph::EdgeIterator, float>(e, e.GetWeight()));
    algo.AddMinimumCut(*G, selectedvertices[0], selectedvertices[1], capacities, text.toUtf8().constData());
    gv->setEdgeColoring(text.toUtf8().constData());
    gv->resetVertexColoring();
    gv->repaint();
}

void MainWindow::on_actionMinimum_Cut_triggered()
{
    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());
    gv->selectVertices(this, 2, MinCutVertexSelectionFinished);
}


// ===================================================================================================================================================


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


// ===================================================================================================================================================


void MainWindow::on_actionMaximum_Matching_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Maximum Matching Algorithm", "Enter a name for the Maximum Matching",
                   QLineEdit::Normal, "MaximumMatching", &ok );
    if ( !ok || text.isEmpty() )
        return;

    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());

    OpenGraphtheory::Algorithms::AlgorithmMAXIMUMMATCHING algo;
    algo.AddMaximumMatching(*(gv->getGraph()), text.toUtf8().constData());

    gv->resetVertexColoring();
    gv->setEdgeColoring(text.toUtf8().constData());
}


// ===================================================================================================================================================


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


// ===================================================================================================================================================


RepaintOnChange::RepaintOnChange(OGoGraphView *target) : IntermediateStepHandler()
{
    this->target = target;
}

void RepaintOnChange::Handle(Graph*)
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


// ===================================================================================================================================================


void MainWindow::on_actionModal_Logic_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Modal-Logic Model-Checker", "Enter a modal-logic formula",
                   QLineEdit::Normal, "\\Diamond \\Box (Berlin \\vee Hamburg)", &ok );
    if ( !ok || text.isEmpty() )
        return;

    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());

    OpenGraphtheory::Logic::MLModelChecker modelchecker;
    std::list<std::string> params;
    params.push_back("mlmodel");
    modelchecker.ModelCheck(*(gv->getGraph()), text.toUtf8().constData(), params);

    gv->setVertexColoring("mlmodel");
    gv->resetEdgeColoring();
}


// ===================================================================================================================================================


void MainWindow::on_actionComputation_Tree_Logic_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Computation-Tree Logic Model-Checker", "Enter a computation-tree-logic formula",
                   QLineEdit::Normal, "A X (E X (Berlin \\vee Hamburg))", &ok );
    if ( !ok || text.isEmpty() )
        return;

    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());

    OpenGraphtheory::Logic::CTLModelChecker modelchecker;
    std::list<std::string> params;
    params.push_back("ctlmodel");
    modelchecker.ModelCheck(*(gv->getGraph()), text.toUtf8().constData(), params);

    gv->setVertexColoring("ctlmodel");
    gv->resetEdgeColoring();
}


// ===================================================================================================================================================


void MainWindow::on_actionFirst_Order_Predicate_Logic_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Computation-Tree Logic Model-Checker", "Enter a computation-tree-logic formula",
                                         QLineEdit::Normal, "\\exists x. \\forall y. (x=y \\vee E(x,y))", &ok );
    if ( !ok || text.isEmpty() )
        return;

    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());

    OpenGraphtheory::Logic::FOModelChecker modelchecker;
    std::stringstream s;
    s << text.toUtf8().constData();
    OpenGraphtheory::Logic::FOFormula* formula = static_cast<OpenGraphtheory::Logic::FOFormula*>(modelchecker.Parse(s));
    if(formula == NULL)
    {
        QMessageBox::critical(0, "FO Modelchecker", "Syntax Error");
    }
    else
    {
        std::map<std::string,OpenGraphtheory::Graph::VertexIterator> var_assignments;
        QMessageBox::information(0, "Result", formula->Interpretation(*(gv->getGraph()),var_assignments) ? "Yes" : "No");
        delete formula;
    }
}



