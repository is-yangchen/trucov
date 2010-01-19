    #ifndef SVGVIEW_H
    #define SVGVIEW_H

    #include <QWidget>
    #include <QImage>
    #ifndef QT_NO_OPENGL
    #include <QGLWidget>
    #endif

    class QPaintEvent;
    class QSvgRenderer;
    class QWheelEvent;

    class SvgRasterView : public QWidget
    {
        Q_OBJECT
    public:
        SvgRasterView(const QString &file, QWidget *parent=0);

        virtual QSize sizeHint() const;

    protected slots:
        void poluteImage();
    protected:
        virtual void paintEvent(QPaintEvent *event);
        virtual void wheelEvent(QWheelEvent *event);

    private:
        QSvgRenderer *doc;
        QImage buffer;
        bool m_dirty;
    };

    class SvgNativeView : public QWidget
    {
        Q_OBJECT
    public:
        SvgNativeView(const QString &file, QWidget *parent=0);

        virtual QSize sizeHint() const;
    protected:
        virtual void paintEvent(QPaintEvent *event);
        virtual void wheelEvent(QWheelEvent *event);

    private:
        QSvgRenderer *doc;
    };

    #ifndef QT_NO_OPENGL
    class SvgGLView : public QGLWidget
    {
        Q_OBJECT
    public:
        SvgGLView(const QString &file, QWidget *parent=0);

        virtual QSize sizeHint() const;
    protected:
        virtual void paintEvent(QPaintEvent *event);
        virtual void wheelEvent(QWheelEvent *event);

    private:
        QSvgRenderer *doc;
    };
    #endif

    #endif
