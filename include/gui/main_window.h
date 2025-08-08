#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QGroupBox>
#include <QSplitter>
#include <QTabWidget>
#include <QTimer>
#include <QSystemTrayIcon>
#include <memory>

class DatabaseManager;
class ProdutoService;
class VendaService;
class RelatorioService;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DatabaseManager* db_manager, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Produtos
    void abrirCadastroProduto();
    void abrirListaProdutos();
    void editarProduto();
    void removerProduto();
    
    // Vendas
    void novaVenda();
    void abrirListaVendas();
    void visualizarVenda();
    void cancelarVenda();
    
    // Clientes
    void abrirCadastroCliente();
    void abrirListaClientes();
    
    // Relatórios
    void abrirRelatorioVendas();
    void abrirRelatorioEstoque();
    void abrirRelatorioProdutosMaisVendidos();
    
    // Sistema
    void verificarAlertasEstoque();
    void fazerBackup();
    void restaurarBackup();
    void sobre();
    void sair();
    
    // Interface
    void atualizarStatusBar();
    void atualizarDashboard();

private:
    // Configuração da interface
    void setupUi();
    void setupMenus();
    void setupToolBar();
    void setupStatusBar();
    void setupCentralWidget();
    void setupDashboard();
    void setupSystemTray();
    
    // Widgets principais
    QWidget* central_widget;
    QTabWidget* tab_widget;
    
    // Dashboard
    QWidget* dashboard_widget;
    QLabel* lbl_vendas_hoje;
    QLabel* lbl_vendas_mes;
    QLabel* lbl_produtos_estoque_baixo;
    QLabel* lbl_total_produtos;
    QPushButton* btn_nova_venda;
    QPushButton* btn_cadastro_produto;
    QPushButton* btn_relatorio_vendas;
    
    // Produtos
    QWidget* produtos_widget;
    QTableWidget* tabela_produtos;
    QLineEdit* filtro_produto_nome;
    QComboBox* filtro_produto_categoria;
    QPushButton* btn_adicionar_produto;
    QPushButton* btn_editar_produto;
    QPushButton* btn_remover_produto;
    
    // Vendas
    QWidget* vendas_widget;
    QTableWidget* tabela_vendas;
    QDateEdit* filtro_data_inicio;
    QDateEdit* filtro_data_fim;
    QPushButton* btn_nova_venda_tab;
    QPushButton* btn_visualizar_venda;
    QPushButton* btn_cancelar_venda;
    
    // Menus
    QMenuBar* menu_bar;
    QMenu* menu_arquivo;
    QMenu* menu_cadastros;
    QMenu* menu_vendas;
    QMenu* menu_relatorios;
    QMenu* menu_ferramentas;
    QMenu* menu_ajuda;
    
    // Toolbar
    QToolBar* tool_bar;
    
    // Status Bar
    QStatusBar* status_bar;
    QLabel* status_conexao;
    QLabel* status_usuario;
    QLabel* status_data_hora;
    
    // System Tray
    QSystemTrayIcon* system_tray;
    QMenu* tray_menu;
    
    // Timers
    QTimer* timer_atualizacao;
    QTimer* timer_alertas;
    
    // Services
    DatabaseManager* db_manager;
    std::unique_ptr<ProdutoService> produto_service;
    std::unique_ptr<VendaService> venda_service;
    std::unique_ptr<RelatorioService> relatorio_service;
    
    // Estado da aplicação
    bool sistema_iniciado;
};

#endif // MAIN_WINDOW_H
