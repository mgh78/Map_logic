// ============================================
// mainwindow.cpp
// ============================================

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QPen>
#include <QBrush>

#include <QFile>
#include <QTextStream>
#include <QDir>

#include <cmath>
#include <cstdlib>

// ============================================
// NODE TYPE STRING
// ============================================

QString MainWindow::nodeTypeToString(NodeType type)
{
    switch(type)
    {
    case NodeType::NORMAL:
        return "NORMAL";

    case NodeType::TRAP:
        return "TRAP";

    case NodeType::EVENT:
        return "EVENT";

    case NodeType::SHOP:
        return "SHOP";

    case NodeType::START:
        return "START";

    default:
        return "UNKNOWN";
    }
}

// ============================================
// NODE COLOR
// ============================================

QColor getNodeColor(NodeType type)
{
    switch(type)
    {
    case NodeType::NORMAL:
        return QColor(0,70,255);

    case NodeType::TRAP:
        return QColor(255,0,0);

    case NodeType::EVENT:
        return QColor(0,255,0);

    case NodeType::SHOP:
        return QColor(180,0,255);

    case NodeType::START:
        return QColor(255,180,0);

    default:
        return Qt::white;
    }
}

// ============================================
// DISTANCE TO LINE
// ============================================

float distanceToSegment(
    float px,
    float py,
    float x1,
    float y1,
    float x2,
    float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    float lengthSquared =
        dx * dx + dy * dy;

    if(lengthSquared == 0)
    {
        return sqrt(
            (px - x1)*(px - x1)
            +
            (py - y1)*(py - y1)
            );
    }

    float t =
        ((px - x1) * dx
         +
         (py - y1) * dy)
        /
        lengthSquared;

    t = std::max(0.0f,
                 std::min(1.0f, t));

    float projX = x1 + t * dx;
    float projY = y1 + t * dy;

    return sqrt(
        (px - projX)*(px - projX)
        +
        (py - projY)*(py - projY)
        );
}

// ============================================
// PROJECT TO LINE
// ============================================

void projectToSegment(
    float px,
    float py,
    float x1,
    float y1,
    float x2,
    float y2,
    float& outX,
    float& outY)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    float lengthSquared =
        dx * dx + dy * dy;

    float t =
        ((px - x1) * dx
         +
         (py - y1) * dy)
        /
        lengthSquared;

    t = std::max(0.0f,
                 std::min(1.0f, t));

    outX = x1 + t * dx;
    outY = y1 + t * dy;
}

// ============================================
// CONSTRUCTOR
// ============================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(1800,1100);

    // START NODE

    nodes.push_back({

        0,

            300.0f,
            300.0f,

            NodeType::START,

        {}
    });

    // ROBOT

    robots.push_back({

        0,

            "Robot1",

            Qt::yellow,

            0,

            300.0f,
            300.0f
    });
}

// ============================================
// DESTRUCTOR
// ============================================

MainWindow::~MainWindow()
{
    delete ui;
}

// ============================================
// GET NODE
// ============================================

Node* MainWindow::getNodeById(int id)
{
    for(Node& node : nodes)
    {
        if(node.id == id)
        {
            return &node;
        }
    }

    return nullptr;
}

// ============================================
// DRAW
// ============================================

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setRenderHint(
        QPainter::Antialiasing);

    painter.fillRect(
        rect(),
        QColor(230,120,40));

    // GRID

    painter.setPen(
        QPen(QColor(255,255,255,50),1));

    for(int x=0; x<width(); x+=50)
    {
        painter.drawLine(x,0,x,height());
    }

    for(int y=0; y<height(); y+=50)
    {
        painter.drawLine(0,y,width(),y);
    }

    // CONNECTIONS

    painter.setPen(
        QPen(QColor(255,220,200),8));

    for(const Node& node : nodes)
    {
        for(int neighborId
             : node.neighbors)
        {
            Node* neighbor =
                getNodeById(neighborId);

            if(neighbor == nullptr)
            {
                continue;
            }

            painter.drawLine(
                node.x,
                node.y,
                neighbor->x,
                neighbor->y
                );
        }
    }

    // DRAW NODES

    for(const Node& node : nodes)
    {
        painter.setBrush(
            getNodeColor(node.type));

        painter.setPen(
            QPen(
                QColor(255,220,200),
                5));

        painter.drawEllipse(
            node.x - 30,
            node.y - 30,
            60,
            60);

        painter.setPen(Qt::white);

        painter.drawText(
            node.x - 8,
            node.y + 5,
            QString::number(node.id));

        painter.setPen(Qt::black);

        painter.drawText(
            node.x - 40,
            node.y + 50,
            nodeTypeToString(node.type));
    }

    // DRAW ROBOTS

    for(const Robot& robot : robots)
    {
        painter.setBrush(robot.color);

        painter.setPen(
            QPen(Qt::black,3));

        painter.drawEllipse(
            robot.x - 18,
            robot.y - 18,
            36,
            36);

        painter.drawText(
            robot.x - 25,
            robot.y - 30,
            robot.name);

        painter.drawText(
            robot.x + 25,
            robot.y,
            "Node: "
                + QString::number(
                    robot.currentNode));

        painter.drawText(
            robot.x + 25,
            robot.y + 20,
            "X:"
                + QString::number(robot.x)
                + " Y:"
                + QString::number(robot.y));
    }

    // UI TEXT

    painter.setPen(Qt::black);

    painter.drawText(20,30,
                     "Left Click Empty = Create Node");

    painter.drawText(20,60,
                     "Drag Node = Move Node");

    painter.drawText(20,90,
                     "Drag Robot = Move Robot");

    painter.drawText(20,120,
                     "C = Connect Mode");

    painter.drawText(20,150,
                     "D = Delete Mode");

    painter.drawText(20,180,
                     "R = Create Robot");

    painter.drawText(20,210,
                     "S = Save JSON");

    painter.drawText(20,240,
                     "1-5 = Change Node Type");

    if(connectMode)
    {
        painter.drawText(
            20,300,
            "CONNECT MODE ACTIVE");
    }

    if(deleteMode)
    {
        painter.drawText(
            20,330,
            "DELETE MODE ACTIVE");
    }
}

// ============================================
// MOUSE PRESS
// ============================================

void MainWindow::mousePressEvent(
    QMouseEvent *event)
{
    int mouseX = event->pos().x();
    int mouseY = event->pos().y();

    // RESET

    selectedNode = -1;
    selectedRobot = -1;

    // DELETE MODE

    if(deleteMode)
    {
        for(size_t i=0;
             i<nodes.size();
             i++)
        {
            int dx =
                mouseX - nodes[i].x;

            int dy =
                mouseY - nodes[i].y;

            if(dx*dx + dy*dy <= 30*30)
            {
                int deletedId =
                    nodes[i].id;

                nodes.erase(
                    nodes.begin()+i);

                for(Node& node : nodes)
                {
                    std::vector<int>
                        newNeighbors;

                    for(int neighbor
                         : node.neighbors)
                    {
                        if(neighbor
                            != deletedId)
                        {
                            newNeighbors
                                .push_back(
                                    neighbor);
                        }
                    }

                    node.neighbors =
                        newNeighbors;
                }

                deleteMode = false;

                update();

                return;
            }
        }
    }

    // ROBOT SELECT

    for(size_t i=0;
         i<robots.size();
         i++)
    {
        int dx =
            mouseX - robots[i].x;

        int dy =
            mouseY - robots[i].y;

        if(dx*dx + dy*dy <= 20*20)
        {
            selectedRobot =
                static_cast<int>(i);

            return;
        }
    }

    // NODE SELECT

    for(size_t i=0;
         i<nodes.size();
         i++)
    {
        int dx =
            mouseX - nodes[i].x;

        int dy =
            mouseY - nodes[i].y;

        if(dx*dx + dy*dy <= 30*30)
        {
            if(connectMode)
            {
                if(firstConnectionNode
                    == -1)
                {
                    firstConnectionNode =
                        nodes[i].id;
                }
                else
                {
                    int second =
                        nodes[i].id;

                    Node* firstNode =
                        getNodeById(
                            firstConnectionNode);

                    Node* secondNode =
                        getNodeById(
                            second);

                    if(firstNode
                        && secondNode)
                    {
                        firstNode
                            ->neighbors
                            .push_back(
                                second);

                        secondNode
                            ->neighbors
                            .push_back(
                                firstConnectionNode);
                    }

                    connectMode = false;

                    firstConnectionNode
                        = -1;
                }

                update();

                return;
            }

            selectedNode =
                static_cast<int>(i);

            return;
        }
    }

    // CREATE NODE

    int newId = 0;

    if(!nodes.empty())
    {
        newId =
            nodes.back().id + 1;
    }

    nodes.push_back({

        newId,

            static_cast<float>(mouseX),

            static_cast<float>(mouseY),

            NodeType::NORMAL,

        {}
    });

    update();
}

// ============================================
// MOUSE MOVE
// ============================================

void MainWindow::mouseMoveEvent(
    QMouseEvent *event)
{
    // ============================================
    // MOVE NODE
    // ============================================

    if(selectedNode != -1)
    {
        nodes[selectedNode].x =
            event->pos().x();

        nodes[selectedNode].y =
            event->pos().y();

        update();

        return;
    }

    // ============================================
    // MOVE ROBOT ONLY ON PATHS
    // ============================================

    if(selectedRobot != -1)
    {
        float mouseX =
            event->pos().x();

        float mouseY =
            event->pos().y();

        float bestDistance =
            999999;

        float bestX =
            robots[selectedRobot].x;

        float bestY =
            robots[selectedRobot].y;

        int fromNode = -1;
        int toNode = -1;

        // ========================================
        // FIND CLOSEST PATH
        // ========================================

        for(const Node& node : nodes)
        {
            for(int neighborId
                 : node.neighbors)
            {
                Node* neighbor =
                    getNodeById(
                        neighborId);

                if(neighbor == nullptr)
                {
                    continue;
                }

                float distance =
                    distanceToSegment(

                        mouseX,
                        mouseY,

                        node.x,
                        node.y,

                        neighbor->x,
                        neighbor->y
                        );

                if(distance < bestDistance)
                {
                    bestDistance =
                        distance;

                    projectToSegment(

                        mouseX,
                        mouseY,

                        node.x,
                        node.y,

                        neighbor->x,
                        neighbor->y,

                        bestX,
                        bestY
                        );

                    fromNode = node.id;
                    toNode = neighbor->id;
                }
            }
        }

        // ========================================
        // ROBOT POSITION
        // ========================================

        robots[selectedRobot].x =
            bestX;

        robots[selectedRobot].y =
            bestY;

        // ========================================
        // DIRECTION DETECTION
        // ========================================

        QString direction;

        if(fromNode != -1
            && toNode != -1)
        {
            Node* from =
                getNodeById(fromNode);

            Node* to =
                getNodeById(toNode);

            if(from && to)
            {
                float distToFrom =
                    sqrt(
                        (bestX - from->x)
                            *
                            (bestX - from->x)
                        +
                        (bestY - from->y)
                            *
                            (bestY - from->y)
                        );

                float distToTo =
                    sqrt(
                        (bestX - to->x)
                            *
                            (bestX - to->x)
                        +
                        (bestY - to->y)
                            *
                            (bestY - to->y)
                        );

                // ====================================
                // CURRENT NODE
                // ====================================

                if(distToFrom < distToTo)
                {
                    robots[selectedRobot]
                        .currentNode =
                        fromNode;

                    direction =
                        QString::number(
                            fromNode)
                        + " -> "
                        + QString::number(
                            toNode);
                }
                else
                {
                    robots[selectedRobot]
                        .currentNode =
                        toNode;

                    direction =
                        QString::number(
                            toNode)
                        + " -> "
                        + QString::number(
                            fromNode);
                }
            }
        }

        // ========================================
        // ROBOT NAME / DIRECTION
        // ========================================

        robots[selectedRobot].name =
            "Robot "
            + direction;

        update();
    }
}

// ============================================
// MOUSE RELEASE
// ============================================

void MainWindow::mouseReleaseEvent(
    QMouseEvent *event)
{
    Q_UNUSED(event);

    selectedRobot = -1;
}

// ============================================
// KEY PRESS
// ============================================

void MainWindow::keyPressEvent(
    QKeyEvent *event)
{
    // CONNECT MODE

    if(event->key() == Qt::Key_C)
    {
        connectMode = true;

        deleteMode = false;

        firstConnectionNode = -1;

        update();
    }

    // DELETE MODE

    if(event->key() == Qt::Key_D)
    {
        deleteMode = true;

        connectMode = false;

        update();
    }

    // CREATE ROBOT

    if(event->key() == Qt::Key_R)
    {
        int robotId =
            robots.size();

        robots.push_back({

            robotId,

                "Robot"
                    + QString::number(
                        robotId),

                QColor(
                    rand()%255,
                    rand()%255,
                    rand()%255),

                0,

                300.0f,
                300.0f
        });

        update();
    }

    // CHANGE NODE TYPE

    if(selectedNode != -1)
    {
        if(event->key() == Qt::Key_1)
        {
            nodes[selectedNode].type =
                NodeType::NORMAL;
        }

        if(event->key() == Qt::Key_2)
        {
            nodes[selectedNode].type =
                NodeType::TRAP;
        }

        if(event->key() == Qt::Key_3)
        {
            nodes[selectedNode].type =
                NodeType::EVENT;
        }

        if(event->key() == Qt::Key_4)
        {
            nodes[selectedNode].type =
                NodeType::SHOP;
        }

        if(event->key() == Qt::Key_5)
        {
            nodes[selectedNode].type =
                NodeType::START;
        }

        update();
    }

    // SAVE JSON

    if(event->key() == Qt::Key_S)
    {
        QString path =
            QDir::homePath()
            + "/Desktop/map.json";

        QFile file(path);

        if(file.open(
                QIODevice::WriteOnly
                |
                QIODevice::Text))
        {
            QTextStream out(&file);

            out << "{\n";

            out << "\"nodes\": [\n";

            for(size_t i=0;
                 i<nodes.size();
                 i++)
            {
                Node& node =
                    nodes[i];

                out << "{\n";

                out << "\"id\": "
                    << node.id
                    << ",\n";

                out << "\"x\": "
                    << node.x
                    << ",\n";

                out << "\"y\": "
                    << node.y
                    << ",\n";

                out << "\"type\": \""
                    << nodeTypeToString(
                           node.type)
                    << "\",\n";

                out << "\"neighbors\": [";

                for(size_t j=0;
                     j<node.neighbors.size();
                     j++)
                {
                    out << node.neighbors[j];

                    if(j !=
                        node.neighbors
                                .size()-1)
                    {
                        out << ",";
                    }
                }

                out << "]\n";

                out << "}";

                if(i !=
                    nodes.size()-1)
                {
                    out << ",";
                }

                out << "\n";
            }

            out << "]\n";

            out << "}\n";

            file.close();
        }
    }
}