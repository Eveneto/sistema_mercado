#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>
#include "gui/main_window.h"
#include "database/database_manager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Configurações da aplicação
    app.setApplicationName("Sistema Mercado");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Sistema Mercado Inc");
    app.setOrganizationDomain("sistemamercado.com");
    
    // Inicializar banco de dados
    DatabaseManager dbManager;
    if (!dbManager.inicializarBanco()) {
        QMessageBox::critical(nullptr, "Erro", 
            "Não foi possível inicializar o banco de dados.\n" +
            QString::fromStdString(dbManager.getUltimoErro()));
        return -1;
    }
    
    // Criar e mostrar janela principal
    MainWindow window(&dbManager);
    window.show();
    
    return app.exec();
}
