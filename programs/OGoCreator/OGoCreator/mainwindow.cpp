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



void GeneratorEnumerator::Enumerate(std::string name, std::string, std::string)
{
    stringlist.append(QString(name.c_str()));
}
QStringList GeneratorEnumerator::getStringList()
{
    return stringlist;
}


GeneratorAction::GeneratorAction(MainWindow* target, QString gen)
    : QAction(target)
{
    this->target = target;
    this->gen = gen;
    this->setText(gen);
}
void GeneratorAction::on_GeneratorAction_triggered()
{
    target->DoGenerate(gen);
}




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->setAcceptDrops(true);
    this->setAcceptDrops(true);


    QMenu* genmenu = ui->menuFile->findChild<QMenu*>(QString("menuGenerate"));
    genmenu->clear();

    GeneratorEnumerator gen;
    Generate::Generator::GeneratorFactory.Enumerate(&gen);
    QStringList genlist = gen.getStringList();
    for(QStringList::iterator i = genlist.begin(); i != genlist.end(); i++)
    {
        GeneratorAction* action = new GeneratorAction(this, *i);
        genmenu->addAction( action );
        QObject::connect(action, SIGNAL(triggered()),
                         action, SLOT(on_GeneratorAction_triggered()));
    }

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






class ImportExportFilterEnumerator : public FactoryEnumerator
{
    private:
        QStringList stringlist;
    public:
        ImportExportFilterEnumerator();
        void Enumerate(std::string name, std::string description, std::string url);
        QStringList getStringList();
};
ImportExportFilterEnumerator::ImportExportFilterEnumerator()
{
    stringlist.append("Graph eXchange Language (*.gxl)");
}
QStringList ImportExportFilterEnumerator::getStringList()
{
    return stringlist;
}
void ImportExportFilterEnumerator::Enumerate(std::string name, std::string description, std::string)
{
    if(name != "gxl")
        stringlist.append(QString((description+" (*."+name+")").c_str()));
}



void MainWindow::OpenGraphFile(QString filename)
{
    ui->statusBar->showMessage("Loading " + filename + "...");
    QFileInfo fileinfo(filename);
    QString suffix = fileinfo.suffix().toLower();

    Graph* gr = new Graph;
    if(suffix == "gxl")
        gr->LoadFromFile(string(filename.toUtf8().constData()));
    else
        (*gr) = Import::ImportFilter::Import(filename.toUtf8().constData(), suffix.toUtf8().constData());

    MakeTab(gr, fileinfo.baseName(), filename);
    ui->statusBar->showMessage("");
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dlg(this);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setDefaultSuffix("gxl");
    dlg.setFileMode(QFileDialog::ExistingFiles);

    ImportExportFilterEnumerator impfilters;
    OpenGraphtheory::Import::ImportFilter::ImportFilterFactory.Enumerate(&impfilters);
    dlg.setNameFilters(impfilters.getStringList());

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
    ImportExportFilterEnumerator expfilters;
    OpenGraphtheory::Export::ExportFilter::ExportFilterFactory.Enumerate(&expfilters);
    dlg.setNameFilters(expfilters.getStringList());

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
    QDesktopServices::openUrl((QUrl)("https://github.com/OpenGraphtheory/OpenGraphtheory/issues"));
}

void MainWindow::on_actionForum_triggered()
{
    QDesktopServices::openUrl((QUrl)("https://groups.google.com/forum/?hl=en#!forum/opengraphtheory"));
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


void PathVertexSelectionFinished(QWidget* mainwindow, OGoGraphView* gv, vector<Vertex*>& selectedvertices, Graph* G)
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


void MinCutVertexSelectionFinished(QWidget* mainwindow, OGoGraphView* gv, vector<Vertex*>& selectedvertices, Graph* G)
{
    bool ok;
    QString text = QInputDialog::getText(mainwindow, "Minimum Cut Algorithm", "Enter a name for the Cut",
                   QLineEdit::Normal, "Cut", &ok );
    if ( !ok || text.isEmpty() )
        return;

    OpenGraphtheory::Algorithms::AlgorithmMINIMUMCUT algo;
    OpenGraphtheory::EdgeWeighting capacities;
    for(EdgeIterator e = G->BeginEdges(); e != G->EndEdges(); e++)
        capacities.insert(std::pair<OpenGraphtheory::Edge*, float>(*e, (*e)->GetWeight()));
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


void MainWindow::on_actionVertex_Cover_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Vertex Cover Algorithm", "Enter a name for the Vertex Cover",
                   QLineEdit::Normal, "VertexCover", &ok );
    if ( !ok || text.isEmpty() )
        return;

    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());

    OpenGraphtheory::Algorithms::AlgorithmVERTEXCOVER algo;
    algo.AddVertexCover(*(gv->getGraph()), text.toUtf8().constData());

    gv->setVertexColoring(text.toUtf8().constData());
    gv->resetEdgeColoring();
}


void DisjointPathsVertexSelectionFinished(QWidget* mainwindow, OGoGraphView* gv, vector<Vertex*>& selectedvertices, Graph* G)
{
    bool ok;
    QString text = QInputDialog::getText(mainwindow, "Disjoint Paths Algorithm", "Enter a name for the Disjoint Paths",
                   QLineEdit::Normal, "DisjointPaths", &ok );
    if ( !ok || text.isEmpty() )
        return;

    OpenGraphtheory::Algorithms::AlgorithmVERTEXDISJOINTPATHS algo;
    algo.AddVertexDisjointPaths(*G, selectedvertices[0], selectedvertices[1], text.toUtf8().constData());
    gv->setEdgeColoring(text.toUtf8().constData());
    gv->resetVertexColoring();
    gv->repaint();
}
void MainWindow::on_actionDisjoint_Paths_triggered()
{
    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());
    gv->selectVertices(this, 2, DisjointPathsVertexSelectionFinished);
}


void MinimumSeparatorVertexSelectionFinished(QWidget* mainwindow, OGoGraphView* gv, vector<Vertex*>& selectedvertices, Graph* G)
{
    bool ok;
    QString text = QInputDialog::getText(mainwindow, "Minimum Vertex Separator Algorithm", "Enter a name for the Vertex Separator",
                   QLineEdit::Normal, "VertexSeparator", &ok );
    if ( !ok || text.isEmpty() )
        return;

    OpenGraphtheory::Algorithms::AlgorithmVERTEXSEPARATOR algo;
    algo.AddMinimumVertexSeparator(*G, selectedvertices[0], selectedvertices[1], text.toUtf8().constData());
    gv->setVertexColoring(text.toUtf8().constData());
    gv->resetEdgeColoring();
    gv->repaint();
}
void MainWindow::on_actionMinimum_Separator_triggered()
{
    OGoGraphView* gv = static_cast<OGoGraphView*>(ui->tabWidget->currentWidget());
    gv->selectVertices(this, 2, MinimumSeparatorVertexSelectionFinished);
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

void MainWindow::on_actionFirst_Order_Predicate_Logic_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, "First-Order Predicate Logic Model-Checker", "Enter a First-Order Predicate Logic formula",
                                         QLineEdit::Normal, "\\exists x. \\forall y. (x=y \\vee E(x,y) \\vee E(y,x))", &ok );
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
        std::map<std::string,OpenGraphtheory::Vertex*> var_assignments;
        QMessageBox::information(0, "Result", formula->Interpretation(*(gv->getGraph()),var_assignments) ? "Yes" : "No");
        delete formula;
    }
}


// ===================================================================================================================================================


void MainWindow::DoGenerate(QString generator)
{
    bool ok;
    QString text = QInputDialog::getText(this, "Generator "+generator, "Enter Parameters for " + generator + " generator",
                                         QLineEdit::Normal, "5", &ok );
    if ( !ok || text.isEmpty() )
        return;
    QStringList sparts = text.split(' ');
    std::list<int> iparts;
    for(QStringList::iterator i = sparts.begin(); i != sparts.end(); i++)
    {
        int ii = i->toInt(&ok);
        if(ok)
            iparts.push_back(ii);
    }

    Generate::Generator* gen = Generate::Generator::GeneratorFactory.Produce(generator.toStdString());
    ui->statusBar->showMessage("Generating " + generator + " " + text + "...");
    Graph* gr = new Graph();
    (*gr) = gen->Generate(iparts);

    MakeTab(gr, generator, "");
    ui->statusBar->showMessage("");
}
void MainWindow::on_actionGenerate_triggered()
{
    DoGenerate("path");
}



