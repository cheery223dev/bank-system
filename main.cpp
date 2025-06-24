#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv); // Changed to QApplication for GUI support

    MainWindow window; // Instantiate the main window
    window.show();     // Display the main window

    return app.exec(); // Start the application event loop
}
