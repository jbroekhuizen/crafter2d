#include "layerpanel.h"
#include "ui_layerpanel.h"

#include <QButtonGroup>
#include <QContextMenuEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenu>

#include <engine/world/layer.h>

#include "newlayerdialog.h"
#include "mainwindow.h"
#include "resizelayerdialog.h"
#include "tilemapmodel.h"
#include "tileview.h"
#include "tileviewwindow.h"
#include "tileworld.h"

LayerPanel::LayerPanel(MainWindow& parent):
    DockPanel(parent),
    ui(new Ui::LayerPanel)
{
    ui->setupUi(this);

    ui->treeLayers->setModel(new TileMapModel());

    connect(ui->treeLayers->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(on_layerSelectionChanged(QItemSelection,QItemSelection)));

    QButtonGroup* pgroup = new QButtonGroup(this);
    pgroup->addButton(ui->buttonFront);
    pgroup->addButton(ui->buttonMid);
    pgroup->addButton(ui->buttonBack);

    setActiveWorld();

    createContextMenu();
}

LayerPanel::~LayerPanel()
{
    delete ui;
}

// - Get/set

void LayerPanel::setWorld(TileWorld &world)
{
    worldActivated(&world);
}

// - Overrides

void LayerPanel::worldActivated(TileWorld* pworld)
{
    TileMapModel* pmapmodel = dynamic_cast<TileMapModel*>(ui->treeLayers->model());
    Q_ASSERT(pmapmodel != NULL);
    pmapmodel->setWorld(pworld);
}

void LayerPanel::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(mpResizeAct);
    menu.exec(event->globalPos());
}

// - Operations

void LayerPanel::createContextMenu()
{
    mpResizeAct = new QAction(tr("Resize"), this);
    mpResizeAct->setStatusTip(tr("Resize the layer"));
    connect(mpResizeAct, SIGNAL(triggered()), SLOT(on_resizeact_triggered()));
}

void LayerPanel::setActiveWorld()
{
    TileView* pview = getMainWindow().getActiveView();
    if ( pview != NULL )
    {
        worldActivated(&pview->getWorld());
    }
}

void LayerPanel::setLevel(LayerLevel level)
{
    TileView* pview = getMainWindow().getActiveView();
    if ( pview != NULL )
    {
        pview->setLevel(level);
    }
}

// - Slots

void LayerPanel::on_layerSelectionChanged(const QItemSelection& selected, const QItemSelection& /* deselected */)
{
    TileView* pview = getMainWindow().getActiveView();
    if ( pview != NULL )
    {
        TileWorld& world = pview->getWorld();

        QModelIndex index;
        foreach(index, selected.indexes())
        {
            TileMap& map = world.getMap(index.row());
            pview->getWorld().setActiveMap(map);
        }
    }
}

void LayerPanel::on_buttonMoveUp_clicked()
{
    TileMapModel* pmapmodel = dynamic_cast<TileMapModel*>(ui->treeLayers->model());
    if ( pmapmodel != NULL )
    {
        QModelIndex index;
        QModelIndexList indices = ui->treeLayers->selectionModel()->selectedRows();
        foreach (index, indices)
        {
            pmapmodel->moveUp(index);
        }
    }
}

void LayerPanel::on_buttonMoveDown_clicked()
{
    TileMapModel* pmapmodel = dynamic_cast<TileMapModel*>(ui->treeLayers->model());
    if ( pmapmodel != NULL )
    {
        QModelIndex index;
        QModelIndexList indices = ui->treeLayers->selectionModel()->selectedRows();
        foreach (index, indices)
        {
            pmapmodel->moveDown(index);
        }
    }
}

void LayerPanel::on_resizeact_triggered()
{
    TileMapModel* pmapmodel = dynamic_cast<TileMapModel*>(ui->treeLayers->model());
    if ( pmapmodel != NULL )
    {
        QModelIndexList indices = ui->treeLayers->selectionModel()->selectedRows();
        Q_ASSERT(indices.size() == 1);
        QModelIndex index = indices[0];

        Q_ASSERT(pmapmodel->hasWorld());
        TileWorld* pworld = pmapmodel->getWorld();
        TileMap& map = pworld->getMap(index.row());
        Layer& layer = map.getLayer();
        QSize size(layer.getDefinition().width, layer.getDefinition().height);

        if ( ResizeLayerDialog::resize(NULL, size) )
        {
            map.resize(size);
        }
    }
}

void LayerPanel::on_buttonFront_clicked()
{
    setLevel(LayerLevel::eFront);
}

void LayerPanel::on_buttonMid_clicked()
{
    setLevel(LayerLevel::eMid);
}

void LayerPanel::on_buttonBack_clicked()
{
    setLevel(LayerLevel::eBack);
}