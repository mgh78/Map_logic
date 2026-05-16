// ============================================
// mainwindow.h
// ============================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QColor>

#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// ============================================
// NODE TYPES
// ============================================

enum class NodeType {

    NORMAL,
    TRAP,
    EVENT,
    SHOP,
    START
};

// ============================================
// NODE
// ============================================

struct Node {

    int id;

    float x;
    float y;

    NodeType type;

    std::vector<int> neighbors;
};

// ============================================
// ROBOT
// ============================================

struct Robot {

    int id;

    QString name;

    QColor color;

    int currentNode;

    float x;
    float y;
};

// ============================================
// MAIN WINDOW
// ============================================

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:

    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

private:

    Ui::MainWindow *ui;

    // ============================================
    // DATA
    // ============================================

    std::vector<Node> nodes;

    std::vector<Robot> robots;

    // ============================================
    // MODES
    // ============================================

    bool connectMode = false;

    bool deleteMode = false;

    // ============================================
    // SELECTION
    // ============================================

    int selectedNode = -1;

    int selectedRobot = -1;

    int firstConnectionNode = -1;

    // ============================================
    // HELPERS
    // ============================================

    Node* getNodeById(int id);

    QString nodeTypeToString(NodeType type);
};

#endif // MAINWINDOW_H