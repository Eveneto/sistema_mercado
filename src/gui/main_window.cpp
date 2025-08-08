#include "gui/main_window.h"
#include "database/database_manager.h"
#include "services/produto_service.h"
#include "services/venda_service.h"
#include "services/relatorio_service.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QHeaderView>

MainWindow::MainWindow(DatabaseManager* db_manager, QWidget *parent)
    : QMainWindow(parent), db_manager(db_manager), sistema_iniciado(false)
{
    // Inicializar services
    produto_service = std::make_unique<ProdutoService>(db_manager);
    venda_service = std::make_unique<VendaService>(db_manager);
    relatorio_service = std::make_unique<RelatorioService>(db_manager);
    
    setupUi();
    setupSystemTray();
    
    // Configurar timers
    timer_atualizacao = new QTimer(this);
    connect(timer_atualizacao, &QTimer::timeout, this, &MainWindow::atualizarDashboard);
    timer_atualizacao->start(60000); // Atualizar a cada minuto
    
    timer_alertas = new QTimer(this);
    connect(timer_alertas, &QTimer::timeout, this, &MainWindow::verificarAlertasEstoque);
    timer_alertas->start(300000); // Verificar alertas a cada 5 minutos
    
    // Atualizar interface inicial
    atualizarDashboard();
    atualizarStatusBar();
    
    sistema_iniciado = true;
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi() {
    setWindowTitle("Sistema Gerenciador para Supermercados");
    setMinimumSize(1200, 800);
    resize(1400, 900);
    
    setupMenus();
    setupToolBar();
    setupStatusBar();
    setupCentralWidget();
}

void MainWindow::setupMenus() {
    menu_bar = menuBar();
    
    // Menu Arquivo
    menu_arquivo = menu_bar->addMenu("&Arquivo");
    menu_arquivo->addAction("&Backup", this, &MainWindow::fazerBackup, QKeySequence("Ctrl+B"));
    menu_arquivo->addAction("&Restaurar", this, &MainWindow::restaurarBackup);
    menu_arquivo->addSeparator();
    menu_arquivo->addAction("&Sair", this, &MainWindow::sair, QKeySequence("Ctrl+Q"));
    
    // Menu Cadastros
    menu_cadastros = menu_bar->addMenu("&Cadastros");
    menu_cadastros->addAction("&Produtos", this, &MainWindow::abrirCadastroProduto, QKeySequence("Ctrl+P"));
    menu_cadastros->addAction("&Clientes", this, &MainWindow::abrirCadastroCliente, QKeySequence("Ctrl+C"));
    
    // Menu Vendas
    menu_vendas = menu_bar->addMenu("&Vendas");
    menu_vendas->addAction("&Nova Venda", this, &MainWindow::novaVenda, QKeySequence("F2"));
    menu_vendas->addAction("&Lista de Vendas", this, &MainWindow::abrirListaVendas, QKeySequence("Ctrl+L"));
    
    // Menu Relatórios
    menu_relatorios = menu_bar->addMenu("&Relatórios");
    menu_relatorios->addAction("Vendas", this, &MainWindow::abrirRelatorioVendas);
    menu_relatorios->addAction("Estoque", this, &MainWindow::abrirRelatorioEstoque);
    menu_relatorios->addAction("Produtos Mais Vendidos", this, &MainWindow::abrirRelatorioProdutosMaisVendidos);
    
    // Menu Ferramentas
    menu_ferramentas = menu_bar->addMenu("&Ferramentas");
    menu_ferramentas->addAction("Verificar Alertas", this, &MainWindow::verificarAlertasEstoque);
    
    // Menu Ajuda
    menu_ajuda = menu_bar->addMenu("&Ajuda");
    menu_ajuda->addAction("&Sobre", this, &MainWindow::sobre, QKeySequence("F1"));
}

void MainWindow::setupToolBar() {
    tool_bar = addToolBar("Principal");
    tool_bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    
    tool_bar->addAction("Nova Venda", this, &MainWindow::novaVenda);
    tool_bar->addAction("Produtos", this, &MainWindow::abrirListaProdutos);
    tool_bar->addAction("Clientes", this, &MainWindow::abrirListaClientes);
    tool_bar->addSeparator();
    tool_bar->addAction("Relatórios", this, &MainWindow::abrirRelatorioVendas);
}

void MainWindow::setupStatusBar() {
    status_bar = statusBar();
    
    status_conexao = new QLabel("Conectado");
    status_usuario = new QLabel("Sistema Mercado");
    status_data_hora = new QLabel();
    
    status_bar->addWidget(status_conexao);
    status_bar->addPermanentWidget(status_usuario);
    status_bar->addPermanentWidget(status_data_hora);
    
    // Timer para atualizar data/hora
    QTimer* timer_status = new QTimer(this);
    connect(timer_status, &QTimer::timeout, this, &MainWindow::atualizarStatusBar);
    timer_status->start(1000);
}

void MainWindow::setupCentralWidget() {
    central_widget = new QWidget;
    setCentralWidget(central_widget);
    
    tab_widget = new QTabWidget;
    
    setupDashboard();
    
    QVBoxLayout* layout = new QVBoxLayout(central_widget);
    layout->addWidget(tab_widget);
}

void MainWindow::setupDashboard() {
    dashboard_widget = new QWidget;
    tab_widget->addTab(dashboard_widget, "Dashboard");
    
    QVBoxLayout* main_layout = new QVBoxLayout(dashboard_widget);
    
    // Título
    QLabel* titulo = new QLabel("Dashboard - Sistema Mercado");
    titulo->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50; margin: 10px;");
    main_layout->addWidget(titulo);
    
    // Métricas
    QGroupBox* group_metricas = new QGroupBox("Resumo do Dia");
    QGridLayout* grid_metricas = new QGridLayout(group_metricas);
    
    lbl_vendas_hoje = new QLabel("R$ 0,00");
    lbl_vendas_mes = new QLabel("R$ 0,00");
    lbl_produtos_estoque_baixo = new QLabel("0");
    lbl_total_produtos = new QLabel("0");
    
    grid_metricas->addWidget(new QLabel("Vendas Hoje:"), 0, 0);
    grid_metricas->addWidget(lbl_vendas_hoje, 0, 1);
    grid_metricas->addWidget(new QLabel("Vendas do Mês:"), 0, 2);
    grid_metricas->addWidget(lbl_vendas_mes, 0, 3);
    grid_metricas->addWidget(new QLabel("Produtos com Estoque Baixo:"), 1, 0);
    grid_metricas->addWidget(lbl_produtos_estoque_baixo, 1, 1);
    grid_metricas->addWidget(new QLabel("Total de Produtos:"), 1, 2);
    grid_metricas->addWidget(lbl_total_produtos, 1, 3);
    
    main_layout->addWidget(group_metricas);
    
    // Ações rápidas
    QGroupBox* group_acoes = new QGroupBox("Ações Rápidas");
    QHBoxLayout* layout_acoes = new QHBoxLayout(group_acoes);
    
    btn_nova_venda = new QPushButton("Nova Venda");
    btn_cadastro_produto = new QPushButton("Cadastrar Produto");
    btn_relatorio_vendas = new QPushButton("Relatório de Vendas");
    
    btn_nova_venda->setMinimumSize(150, 50);
    btn_cadastro_produto->setMinimumSize(150, 50);
    btn_relatorio_vendas->setMinimumSize(150, 50);
    
    connect(btn_nova_venda, &QPushButton::clicked, this, &MainWindow::novaVenda);
    connect(btn_cadastro_produto, &QPushButton::clicked, this, &MainWindow::abrirCadastroProduto);
    connect(btn_relatorio_vendas, &QPushButton::clicked, this, &MainWindow::abrirRelatorioVendas);
    
    layout_acoes->addWidget(btn_nova_venda);
    layout_acoes->addWidget(btn_cadastro_produto);
    layout_acoes->addWidget(btn_relatorio_vendas);
    layout_acoes->addStretch();
    
    main_layout->addWidget(group_acoes);
    main_layout->addStretch();
}

void MainWindow::setupSystemTray() {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        system_tray = new QSystemTrayIcon(this);
        system_tray->setToolTip("Sistema Mercado");
        
        tray_menu = new QMenu(this);
        tray_menu->addAction("Mostrar", this, &QWidget::show);
        tray_menu->addAction("Nova Venda", this, &MainWindow::novaVenda);
        tray_menu->addSeparator();
        tray_menu->addAction("Sair", this, &MainWindow::sair);
        
        system_tray->setContextMenu(tray_menu);
        system_tray->show();
    }
}

void MainWindow::atualizarStatusBar() {
    status_data_hora->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"));
    
    if (db_manager && db_manager->isConectado()) {
        status_conexao->setText("Conectado");
        status_conexao->setStyleSheet("color: green;");
    } else {
        status_conexao->setText("Desconectado");
        status_conexao->setStyleSheet("color: red;");
    }
}

void MainWindow::atualizarDashboard() {
    if (!sistema_iniciado) return;
    
    // Atualizar métricas do dashboard
    // Implementar chamadas para os services para obter dados atualizados
    
    // Por enquanto, valores de exemplo
    lbl_vendas_hoje->setText("R$ 1.250,50");
    lbl_vendas_mes->setText("R$ 15.890,75");
    lbl_produtos_estoque_baixo->setText("5");
    lbl_total_produtos->setText("120");
}

// Slots vazios para implementação futura
void MainWindow::abrirCadastroProduto() {
    QMessageBox::information(this, "Info", "Funcionalidade em desenvolvimento");
}

void MainWindow::abrirListaProdutos() {
    QMessageBox::information(this, "Info", "Funcionalidade em desenvolvimento");
}

void MainWindow::editarProduto() {
    QMessageBox::information(this, "Info", "Funcionalidade em desenvolvimento");
}

void MainWindow::removerProduto() {
    QMessageBox::information(this, "Info", "Funcionalidade em desenvolvimento");
}

void MainWindow::novaVenda() {
    QMessageBox::information(this, "Info", "Funcionalidade em desenvolvimento");
}

void MainWindow::abrirListaVendas() {
    QMessageBox::information(this, "Info", "Funcionalidade em desenvolvimento");
}

void MainWindow::visualizarVenda() {
    QMessageBox::information(this, "Info", "Funcionalidade em desenvolvimento");
}

void MainWindow::cancelarVenda() {
    QMessageBox::information(this, "Info", "Funcionalidade em desenvolvimento");
}

void MainWindow::abrirCadastroCliente() {
    QMessageBox::information(this, "Info", "Funcionalidade em desenvolvimento");
}

void MainWindow::abrirListaClientes() {
    QMessageBox::information(this, "Info", "Funcionalidade em desenvolvimento");
}

void MainWindow::abrirRelatorioVendas() {
    QMessageBox::information(this, "Info", "Funcionalidade em desenvolvimento");
}

void MainWindow::abrirRelatorioEstoque() {
    QMessageBox::information(this, "Info", "Funcionalidade em desenvolvimento");
}

void MainWindow::abrirRelatorioProdutosMaisVendidos() {
    QMessageBox::information(this, "Info", "Funcionalidade em desenvolvimento");
}

void MainWindow::verificarAlertasEstoque() {
    QMessageBox::information(this, "Info", "Verificação de alertas em desenvolvimento");
}

void MainWindow::fazerBackup() {
    QString arquivo = QFileDialog::getSaveFileName(this, 
        "Salvar Backup", 
        "backup_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".db",
        "Arquivos de Banco (*.db)");
    
    if (!arquivo.isEmpty()) {
        if (db_manager->fazerBackup(arquivo.toStdString())) {
            QMessageBox::information(this, "Sucesso", "Backup realizado com sucesso!");
        } else {
            QMessageBox::warning(this, "Erro", "Erro ao realizar backup.");
        }
    }
}

void MainWindow::restaurarBackup() {
    QString arquivo = QFileDialog::getOpenFileName(this,
        "Restaurar Backup",
        "",
        "Arquivos de Banco (*.db)");
    
    if (!arquivo.isEmpty()) {
        int ret = QMessageBox::warning(this, "Confirmação",
            "Esta operação irá substituir todos os dados atuais. Deseja continuar?",
            QMessageBox::Yes | QMessageBox::No);
        
        if (ret == QMessageBox::Yes) {
            if (db_manager->restaurarBackup(arquivo.toStdString())) {
                QMessageBox::information(this, "Sucesso", "Backup restaurado com sucesso!");
                atualizarDashboard();
            } else {
                QMessageBox::warning(this, "Erro", "Erro ao restaurar backup.");
            }
        }
    }
}

void MainWindow::sobre() {
    QMessageBox::about(this, "Sobre",
        "<h3>Sistema Gerenciador para Supermercados</h3>"
        "<p>Versão 1.0.0</p>"
        "<p>Sistema completo para gerenciar estoque, vendas e clientes.</p>"
        "<p><b>Tecnologias:</b></p>"
        "<ul>"
        "<li>C++17</li>"
        "<li>Qt6</li>"
        "<li>SQLite</li>"
        "<li>Poco Framework</li>"
        "</ul>"
        "<p>© 2025 Sistema Mercado Inc.</p>");
}

void MainWindow::sair() {
    int ret = QMessageBox::question(this, "Confirmação",
        "Deseja realmente sair do sistema?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        QApplication::quit();
    }
}

#include "main_window.moc"
