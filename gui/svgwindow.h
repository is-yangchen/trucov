    #ifndef SVGWINDOW_H
    #define SVGWINDOW_H

    #include <QPoint>
    #include <QScrollArea>
    #include <QString>

    class QKeyEvent;
    class QMouseEvent;

    class SvgWindow : public QScrollArea
    {
    public:
        enum RendererType { Native, OpenGL, Image };

        SvgWindow();
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void openFile(const QString &file);
        void setRenderer(RendererType type = Native);

    private:
        QPoint mousePressPos;
        QPoint scrollBarValuesOnMousePress;
        QString currentPath;
        RendererType renderer;
    };

    #endif
