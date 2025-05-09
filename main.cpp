#include "mainwindow.h"
#include <QApplication>
#include <QCanBus>

class SavvyCANApplication : public QApplication
{
public:
    MainWindow *mainWindow;
    
    SavvyCANApplication(int &argc, char **argv) : QApplication(argc, argv)
    {
    }

    bool event(QEvent *event) override
    {
        if (event->type() == QEvent::FileOpen)
        {
            QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);
            mainWindow->handleDroppedFile(openEvent->file());
        }

        return QApplication::event(event);
    }
};

int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
    //uncomment for verbose debug data in application output
    //qputenv("QT_FATAL_WARNINGS", "1");
    //qSetMessagePattern("Type: %{type}\nProduct Name: %{appname}\nFile: %{file}\nLine: %{line}\nMethod: %{function}\nThreadID: %{threadid}\nThreadPtr: %{qthreadptr}\nMessage: %{message}");
#endif

    //static CandleLightCanBusPlugin plugin;
    //Q_UNUSED(plugin);

    SavvyCANApplication a(argc, argv);

    a.addLibraryPath("plugins");

    // Explicitly set the plugin path
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");

    QPalette palette = a.palette();
    QColor highlightColor(80, 100, 140);  // A more subtle blue

    palette.setColor(QPalette::Highlight, highlightColor);

    // Apply the modified palette
    a.setPalette(palette);

    a.setStyleSheet(R"(
    /* Customize highlight colors */
    QWidget {
        selection-background-color: #78809c;
    }

    /* Remove the padding/highlighting around table cells */
    QTableView {
        gridline-color: #d0d0d0;
        outline: 0;  /* Removes the focus border */
    }

    QTableView::item {
        border: 0px;
        padding: 0px;
    }

    /* Remove the highlight padding */
    QTableView::item:selected {
        border: 0px;
        background-color: #78809c;
    }

    /* Remove focus border/padding */
    QTableView::item:focus {
        border: 0px;
        outline: none;
    }
)");

    //These things are used by QSettings to set up setting storage
    a.setOrganizationName("EVTV");
    a.setApplicationName("SavvyCAN");
    a.setOrganizationDomain("evtv.me");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    a.mainWindow = new MainWindow();

    QSettings settings;
    int fontSize = settings.value("Main/FontSize", 9).toUInt();
    QFont sysFont = QFont(); //get default font
    sysFont.setPointSize(fontSize);
    a.setFont(sysFont);

    a.mainWindow->show();

    int retCode = a.exec();
    
    delete a.mainWindow; a.mainWindow = NULL;
    
    return retCode;
}
